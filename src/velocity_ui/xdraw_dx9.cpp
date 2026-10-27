#include "xdraw.hpp"
#include "ui_mesh_submission.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <wincodec.h>
#include <d3dcompiler.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "nanosvg/include.hpp"

namespace xdraw {
using Microsoft::WRL::ComPtr;
namespace detail {
struct state {
    IDirect3DDevice9* device{}; // Owned by the existing ImGui backend.
    ComPtr<IDirect3DTexture9> white_srv;
    struct blur_level { ComPtr<IDirect3DTexture9> srv; int w{},h{}; };
    blur_level blur_chain[4];
    ComPtr<IDirect3DTexture9> scene;
    ComPtr<IDirect3DPixelShader9> down,up;
    int blur_cached_w{},blur_cached_h{};
    std::array<draw_list,3> lists;
    std::vector<std::unique_ptr<font>> fonts;
    std::vector<font*> font_stack;
    font* primary_font{};
} g;
#include "decode_utf8.inl"

// The same four-level downsample/upsample blur used by the source renderer.
constexpr char blur_shader[] = R"(
sampler2D scene : register(s0);
float4 texel : register(c0);
float4 down(float2 uv : TEXCOORD0) : COLOR0 {
    float2 d=texel.xy*0.5;
    return (tex2D(scene,uv)*4 + tex2D(scene,uv+float2(-d.x,-d.y))
        +tex2D(scene,uv+float2(d.x,-d.y))+tex2D(scene,uv+float2(-d.x,d.y))
        +tex2D(scene,uv+d))/8;
}
float4 up(float2 uv : TEXCOORD0) : COLOR0 {
    float2 d=texel.xy*0.5;
    return (tex2D(scene,uv+float2(-2*d.x,0))+tex2D(scene,uv+float2(2*d.x,0))
        +tex2D(scene,uv+float2(0,-2*d.y))+tex2D(scene,uv+float2(0,2*d.y))
        +2*(tex2D(scene,uv+float2(-d.x,-d.y))+tex2D(scene,uv+float2(d.x,-d.y))
        +tex2D(scene,uv+float2(-d.x,d.y))+tex2D(scene,uv+d)))/12;
})";
bool compile_shader(const char* entry,ComPtr<IDirect3DPixelShader9>& shader) {
    ComPtr<ID3DBlob> code,errors;
    if (FAILED(D3DCompile(blur_shader,sizeof(blur_shader)-1,nullptr,nullptr,nullptr,
                        entry,"ps_2_0",D3DCOMPILE_OPTIMIZATION_LEVEL3,0,&code,&errors))) return false;
    return SUCCEEDED(g.device->CreatePixelShader(static_cast<const DWORD*>(code->GetBufferPointer()),&shader));
}
bool create_target(ComPtr<IDirect3DTexture9>& tex,int w,int h) {
    return SUCCEEDED(g.device->CreateTexture(static_cast<UINT>(w),static_cast<UINT>(h),1,
        D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&tex,nullptr));
}
void submit(const draw_list& src,ImDrawList& dst) {
    const auto viewport=ImGui::GetIO().DisplaySize;
    for (size_t command_index=0;command_index<src.commands.size();++command_index) {
        const auto& cmd=src.commands[command_index];
        if (!cmd.idx_count || !cmd.texture) continue;
        if (cmd.has_scissor)
            dst.PushClipRect({static_cast<float>(cmd.scissor.left),static_cast<float>(cmd.scissor.top)},
                {static_cast<float>(cmd.scissor.right),static_cast<float>(cmd.scissor.bottom)},true);
        else dst.PushClipRect({0,0},viewport,false);
        dst.PushTextureID(reinterpret_cast<ImTextureID>(cmd.texture));
        SubmitIndexed(src,command_index,dst);
        dst.PopTextureID(); dst.PopClipRect();
    }
}
}

ComPtr<IDirect3DTexture9> create_srv_from_rgba(const std::uint8_t* pixels,int w,int h) {
    ComPtr<IDirect3DTexture9> tex;
    if (!pixels || !detail::g.device || w<=0 || h<=0 || w>8192 || h>8192) return tex;
    if (FAILED(detail::g.device->CreateTexture(static_cast<UINT>(w),static_cast<UINT>(h),1,0,
        D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&tex,nullptr))) return {};
    D3DLOCKED_RECT lock{};
    if (FAILED(tex->LockRect(0,&lock,nullptr,0))) return {};
    for (int y=0;y<h;++y) {
        auto* dst=static_cast<uint8_t*>(lock.pBits)+y*lock.Pitch;
        const auto* src=pixels+static_cast<size_t>(y)*w*4;
        for(int x=0;x<w;++x) {
            dst[x*4]=src[x*4+2]; dst[x*4+1]=src[x*4+1];
            dst[x*4+2]=src[x*4]; dst[x*4+3]=src[x*4+3];
        }
    }
    tex->UnlockRect(0);
    return tex;
}
ComPtr<IDirect3DTexture9> load_texture(std::span<const std::byte> data,int* w,int* h) {
    if (data.empty() || data.size()>UINT_MAX) return {};
    const HRESULT apartment=CoInitializeEx(nullptr,COINIT_MULTITHREADED);
    struct Apartment { bool release; ~Apartment(){ if(release) CoUninitialize(); } } cleanup{SUCCEEDED(apartment)};
    ComPtr<IWICImagingFactory> factory;
    if (FAILED(CoCreateInstance(CLSID_WICImagingFactory,nullptr,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&factory)))) return {};
    ComPtr<IWICStream> stream;
    if (FAILED(factory->CreateStream(&stream)) || FAILED(stream->InitializeFromMemory(
        reinterpret_cast<BYTE*>(const_cast<std::byte*>(data.data())),static_cast<DWORD>(data.size())))) return {};
    ComPtr<IWICBitmapDecoder> decoder;
    if (FAILED(factory->CreateDecoderFromStream(stream.Get(),nullptr,WICDecodeMetadataCacheOnDemand,&decoder))) return {};
    ComPtr<IWICBitmapFrameDecode> frame;
    ComPtr<IWICFormatConverter> converter;
    UINT width{},height{};
    if (FAILED(decoder->GetFrame(0,&frame)) || FAILED(frame->GetSize(&width,&height)) ||
        !width || !height || width>8192 || height>8192 || FAILED(factory->CreateFormatConverter(&converter))) return {};
    if (FAILED(converter->Initialize(frame.Get(),GUID_WICPixelFormat32bppRGBA,WICBitmapDitherTypeNone,
        nullptr,0,WICBitmapPaletteTypeCustom))) return {};
    std::vector<BYTE> pixels(static_cast<size_t>(width)*height*4);
    if (FAILED(converter->CopyPixels(nullptr,width*4,static_cast<UINT>(pixels.size()),pixels.data()))) return {};
    auto texture=create_srv_from_rgba(pixels.data(),static_cast<int>(width),static_cast<int>(height));
    if (texture) { if(w) *w=static_cast<int>(width); if(h) *h=static_cast<int>(height); }
    return texture;
}
ComPtr<IDirect3DTexture9> load_svg(std::span<const std::byte> data,float scale,int* w,int* h) {
    if(data.empty() || !(scale>0)) return {};
    std::string svg(reinterpret_cast<const char*>(data.data()),data.size());
    auto* image=nsvgParse(svg.data(),"px",96);
    if(!image) return {};
    struct Image { NSVGimage* value; ~Image(){nsvgDelete(value);} } image_owner{image};
    const int width=static_cast<int>(image->width*scale),height=static_cast<int>(image->height*scale);
    if(width<=0 || height<=0 || width>8192 || height>8192) return {};
    auto* raster=nsvgCreateRasterizer();
    if(!raster) return {};
    std::vector<BYTE> pixels(static_cast<size_t>(width)*height*4);
    nsvgRasterize(raster,image,0,0,scale,pixels.data(),width,height,width*4);
    nsvgDeleteRasterizer(raster);
    auto texture=create_srv_from_rgba(pixels.data(),width,height);
    if(texture) {if(w) *w=width; if(h) *h=height;}
    return texture;
}
ComPtr<IDirect3DTexture9> load_svg(const char* data,float scale,int* w,int* h) {
    return data ? load_svg({reinterpret_cast<const std::byte*>(data),std::strlen(data)},scale,w,h) : ComPtr<IDirect3DTexture9>{};
}
bool initialize(IDirect3DDevice9* device) {
    if (!device) return false;
    detail::g.device=device;
    const uint8_t white[]{255,255,255,255};
    detail::g.white_srv=create_srv_from_rgba(white,1,1);
    return detail::g.white_srv!=nullptr;
}
void invalidate_device_objects() {
    detail::g.scene.Reset();
    for(auto& level:detail::g.blur_chain) level={};
    detail::g.blur_cached_w=detail::g.blur_cached_h=0;
}
void capture_blur() {
    auto& g=detail::g;
    if(!g.device) return;
    ComPtr<IDirect3DSurface9> target;
    if(FAILED(g.device->GetRenderTarget(0,&target))) return;
    D3DSURFACE_DESC desc{};
    if(FAILED(target->GetDesc(&desc)) || !desc.Width || !desc.Height) return;
    if(!g.down && !detail::compile_shader("down",g.down)) return;
    if(!g.up && !detail::compile_shader("up",g.up)) return;
    if(g.blur_cached_w!=static_cast<int>(desc.Width) || g.blur_cached_h!=static_cast<int>(desc.Height)) {
        invalidate_device_objects();
        int w=static_cast<int>(desc.Width),h=static_cast<int>(desc.Height);
        if(!detail::create_target(g.scene,w,h)) return;
        for(auto& level:g.blur_chain) {
            w=std::max(1,w/2); h=std::max(1,h/2); level.w=w; level.h=h;
            if(!detail::create_target(level.srv,w,h)) {invalidate_device_objects(); return;}
        }
        g.blur_cached_w=static_cast<int>(desc.Width); g.blur_cached_h=static_cast<int>(desc.Height);
    }
    ComPtr<IDirect3DSurface9> scene_surface;
    if(FAILED(g.scene->GetSurfaceLevel(0,&scene_surface)) ||
       FAILED(g.device->StretchRect(target.Get(),nullptr,scene_surface.Get(),nullptr,D3DTEXF_NONE))) return;
    ComPtr<IDirect3DStateBlock9> state;
    if(FAILED(g.device->CreateStateBlock(D3DSBT_ALL,&state)) || FAILED(state->Capture())) return;
    ComPtr<IDirect3DSurface9> depth;
    g.device->GetDepthStencilSurface(&depth);
    D3DVIEWPORT9 viewport{}; g.device->GetViewport(&viewport);
    std::array<ComPtr<IDirect3DSurface9>,3> additional_targets;
    D3DCAPS9 caps{}; g.device->GetDeviceCaps(&caps);
    const DWORD target_count=std::min<DWORD>(caps.NumSimultaneousRTs,4);
    for(DWORD i=1;i<target_count;++i) g.device->GetRenderTarget(i,&additional_targets[i-1]);
    if(FAILED(g.device->BeginScene())) return;
    g.device->SetDepthStencilSurface(nullptr);
    for(DWORD i=1;i<target_count;++i) g.device->SetRenderTarget(i,nullptr);
    g.device->SetVertexShader(nullptr);
    g.device->SetFVF(D3DFVF_XYZRHW|D3DFVF_TEX1);
    g.device->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
    g.device->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,0);
    g.device->SetRenderState(D3DRS_ZENABLE,FALSE);
    g.device->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
    g.device->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
    g.device->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
    g.device->SetRenderState(D3DRS_SCISSORTESTENABLE,FALSE);
    g.device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
    g.device->SetRenderState(D3DRS_STENCILENABLE,FALSE);
    g.device->SetRenderState(D3DRS_FOGENABLE,FALSE);
    g.device->SetRenderState(D3DRS_LIGHTING,FALSE);
    g.device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
    g.device->SetRenderState(D3DRS_COLORWRITEENABLE,15);
    g.device->SetRenderState(D3DRS_SRGBWRITEENABLE,FALSE);
    g.device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
    g.device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
    g.device->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
    g.device->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
    g.device->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
    g.device->SetSamplerState(0,D3DSAMP_SRGBTEXTURE,FALSE);
    const auto pass=[&](IDirect3DTexture9* source,int sw,int sh,const detail::state::blur_level& destination,
                        IDirect3DPixelShader9* shader) -> bool {
        ComPtr<IDirect3DSurface9> surface;
        if(FAILED(destination.srv->GetSurfaceLevel(0,&surface))) return false;
        g.device->SetTexture(0,nullptr);
        if(FAILED(g.device->SetRenderTarget(0,surface.Get()))) return false;
        D3DVIEWPORT9 vp{0,0,static_cast<DWORD>(destination.w),static_cast<DWORD>(destination.h),0,1};
        g.device->SetViewport(&vp);
        g.device->SetPixelShader(shader);
        const float texel[]{1.f/sw,1.f/sh,0,0};
        g.device->SetPixelShaderConstantF(0,texel,1);
        g.device->SetTexture(0,source);
        struct Quad { float x,y,z,rhw,u,v; };
        const float w=static_cast<float>(destination.w)-.5f,h=static_cast<float>(destination.h)-.5f;
        const Quad quad[]{{-.5f,-.5f,0,1,0,0},{w,-.5f,0,1,1,0},{-.5f,h,0,1,0,1},{w,h,0,1,1,1}};
        return SUCCEEDED(g.device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,quad,sizeof(Quad)));
    };
    bool success=true;
    for(int i=0;i<4 && success;++i) {
        auto* src=i ? g.blur_chain[i-1].srv.Get() : g.scene.Get();
        success=pass(src,i ? g.blur_chain[i-1].w : g.blur_cached_w,
                     i ? g.blur_chain[i-1].h : g.blur_cached_h,g.blur_chain[i],g.down.Get());
    }
    for(int i=3;i>0 && success;--i)
        success=pass(g.blur_chain[i].srv.Get(),g.blur_chain[i].w,g.blur_chain[i].h,g.blur_chain[i-1],g.up.Get());
    g.device->SetTexture(0,nullptr);
    g.device->SetRenderTarget(0,target.Get());
    for(DWORD i=1;i<target_count;++i) g.device->SetRenderTarget(i,additional_targets[i-1].Get());
    g.device->SetDepthStencilSurface(depth.Get());
    state->Apply(); g.device->SetViewport(&viewport);
    g.device->EndScene();
    if(!success) invalidate_device_objects();
}
void begin_frame(bool) { for(auto& list:detail::g.lists) list.clear(); }
void end_frame() {
    auto* destination=ImGui::GetForegroundDrawList();
    for(const auto& list:detail::g.lists) detail::submit(list,*destination);
}
draw_list& get(layer l) { return detail::g.lists[static_cast<size_t>(l)]; }
draw_list& get_glow(layer l) { return get(l); }
std::pair<int,int> viewport_size() {
    const auto s=ImGui::GetIO().DisplaySize; return {static_cast<int>(s.x/interface_scale),static_cast<int>(s.y/interface_scale)};
}
std::pair<float,float> measure_text(std::string_view text,font* f) {
    if(!f) f=current_font(); f=interface_font(f);
    if(!f)return {};auto result=f->measure(text);return {result.first/f->density,result.second/f->density};
}
font* interface_font(font* source) {
    if(source&&interface_scale>1.01f) {
        const int index=interface_scale<1.375f?0:1;
        if(source->interface_variants[index])return source->interface_variants[index];
    }
    return source;
}
float delta_time() { return std::clamp(ImGui::GetIO().DeltaTime,0.f,.1f); }
float framerate() { return ImGui::GetIO().Framerate; }
IDirect3DDevice9* device() { return detail::g.device; }

#include "font.inl"
#include "load_font.inl"
#include "font_stack.inl"
#include "draw_primitives.inl"
} // namespace xdraw
