#include "velocity_ui/hunter_model.hpp"
#include <wincodec.h>
#include <cstdio>

namespace {
using Microsoft::WRL::ComPtr;
ComPtr<IDirect3DDevice9> test_device;
float dt=1.f/60.f;unsigned skeleton_lines{};
}
namespace xdraw {
IDirect3DDevice9* device() {return test_device.Get();}
float delta_time() {return dt;}
// Decode the actual resource so both upload paths use the same real texture.
ComPtr<IDirect3DTexture9> load_texture(std::span<const std::byte> data,int* width,int* height) {
    ComPtr<IWICImagingFactory> factory;ComPtr<IWICStream> stream;
    ComPtr<IWICBitmapDecoder> decoder;ComPtr<IWICBitmapFrameDecode> frame;ComPtr<IWICFormatConverter> format;
    if(FAILED(CoCreateInstance(CLSID_WICImagingFactory,nullptr,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&factory))) ||
       FAILED(factory->CreateStream(&stream)) ||
       FAILED(stream->InitializeFromMemory(reinterpret_cast<BYTE*>(const_cast<std::byte*>(data.data())),static_cast<DWORD>(data.size()))) ||
       FAILED(factory->CreateDecoderFromStream(stream.Get(),nullptr,WICDecodeMetadataCacheOnLoad,&decoder)) ||
       FAILED(decoder->GetFrame(0,&frame)) || FAILED(factory->CreateFormatConverter(&format)) ||
       FAILED(format->Initialize(frame.Get(),GUID_WICPixelFormat32bppBGRA,WICBitmapDitherTypeNone,nullptr,0,WICBitmapPaletteTypeCustom))) return {};
    UINT w{},h{};if(FAILED(format->GetSize(&w,&h))) return {};
    ComPtr<IDirect3DTexture9> texture;
    if(FAILED(test_device->CreateTexture(w,h,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&texture,nullptr))) return {};
    D3DLOCKED_RECT lock{};if(FAILED(texture->LockRect(0,&lock,nullptr,0))) return {};
    const auto result=format->CopyPixels(nullptr,static_cast<UINT>(lock.Pitch),static_cast<UINT>(lock.Pitch)*h,static_cast<BYTE*>(lock.pBits));
    texture->UnlockRect(0);if(FAILED(result)) return {};
    if(width) *width=static_cast<int>(w);if(height) *height=static_cast<int>(h);return texture;
}
void draw_list::line(float,float,float,float,color,float,bool) {++skeleton_lines;}
}

std::vector<std::uint32_t> Pixels(const hunter_model::View& view) {
    ComPtr<IDirect3DSurface9> gpu,cpu;
    if(FAILED(view.texture->GetSurfaceLevel(0,&gpu)) ||
       FAILED(test_device->CreateOffscreenPlainSurface(view.width,view.height,D3DFMT_A8R8G8B8,D3DPOOL_SYSTEMMEM,&cpu,nullptr)) ||
       FAILED(test_device->GetRenderTargetData(gpu.Get(),cpu.Get()))) return {};
    D3DLOCKED_RECT lock{};if(FAILED(cpu->LockRect(&lock,nullptr,D3DLOCK_READONLY))) return {};
    std::vector<std::uint32_t> out(static_cast<size_t>(view.width)*view.height);
    for(int y=0;y<view.height;++y) std::memcpy(out.data()+static_cast<size_t>(y)*view.width,
        static_cast<const BYTE*>(lock.pBits)+static_cast<size_t>(y)*lock.Pitch,view.width*4);
    cpu->UnlockRect();return out;
}

int main() {
    CoInitializeEx(nullptr,COINIT_MULTITHREADED);
    const auto instance=GetModuleHandleW(nullptr);
    WNDCLASSW cls{};cls.hInstance=instance;cls.lpfnWndProc=DefWindowProcW;cls.lpszClassName=L"LynPreviewRegression";
    RegisterClassW(&cls);
    // No ShowWindow: this test renders to offscreen targets only.
    const auto window=CreateWindowExW(0,cls.lpszClassName,L"",WS_POPUP,0,0,64,64,nullptr,nullptr,instance,nullptr);
    ComPtr<IDirect3D9> d3d;d3d.Attach(Direct3DCreate9(D3D_SDK_VERSION));
    D3DPRESENT_PARAMETERS pp{};pp.Windowed=TRUE;pp.hDeviceWindow=window;pp.SwapEffect=D3DSWAPEFFECT_DISCARD;
    pp.BackBufferWidth=pp.BackBufferHeight=64;pp.BackBufferFormat=D3DFMT_X8R8G8B8;
    if(!window || !d3d || FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,window,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,&pp,&test_device))) {
        if(window) DestroyWindow(window);std::puts("SKIP: Direct3D9 device unavailable");return 77;
    }
    unsigned checks{},failed{};
    const auto expect=[&](bool ok,const char* name){++checks;if(!ok){++failed;std::fprintf(stderr,"FAIL: %s\n",name);}};
    hunter_model::View reference{},buffered{};
    reference.playing=buffered.playing=false;
    const auto render=[&](hunter_model::View& view,bool old,const std::array<xdraw::color,7>* fills) {
        hunter_model::ReferenceUpload(old);
        if(FAILED(test_device->BeginScene())) return false;
        const bool ok=hunter_model::Render(view,320,400,fills);test_device->EndScene();return ok;
    };
    std::vector<std::uint32_t> first_pose;
    unsigned group_draws{};
    for(int clip=0;clip<3;++clip) {
        reference.clip=buffered.clip=clip;reference.time=buffered.time=.25f;
        reference.yaw=buffered.yaw=15.f+clip*30.f;
        hunter_model::ResetCounters();
        expect(render(reference,true,nullptr),"reference preview renders");
        const auto old=hunter_model::Counters();group_draws=old.reference_draws;
        expect(render(buffered,false,nullptr) && buffered.vertex_buffer,"buffered preview renders with a GPU vertex buffer");
        const auto counters=hunter_model::Counters();
        expect(counters.vertices==1 && counters.buffered_draws==group_draws && group_draws>1,"each preview uploads vertices once across all anatomical draws");
        const auto a=Pixels(reference),b=Pixels(buffered);
        expect(!a.empty() && a==b,"buffered and UP paths produce identical pixels for the original animation");
        expect(std::any_of(b.begin(),b.end(),[](auto p){return (p>>24)!=0;}),"comparison contains visible model pixels");
        if(!clip) first_pose=b;else expect(b!=first_pose,"walk/run poses and rotation remain animated");
    }
    std::array<xdraw::color,7> fills{};
    for(int i=0;i<7;++i) fills[i]={static_cast<std::uint8_t>(20+i*30),80,180,255};
    expect(render(reference,true,&fills) && render(buffered,false,&fills) && Pixels(reference)==Pixels(buffered),"priority fills preserve exact colors and geometry");
    hunter_model::ResetCounters();render(buffered,false,nullptr);render(buffered,false,nullptr);
    expect(hunter_model::Counters().indices==0 && hunter_model::Counters().vertices==1&&hunter_model::Counters().cache_hits==1,
        "paused frames reuse their texture after clearing priority fills");
    const auto paused_pixels=Pixels(buffered);hunter_model::ResetCounters();
    for(int i=0;i<20;++i)render(buffered,false,nullptr);
    expect(hunter_model::Counters().poses==0&&hunter_model::Counters().vertices==0&&hunter_model::Counters().cache_hits==20&&Pixels(buffered)==paused_pixels,
        "a paused preview needs no skinning, uploads or redraws and retains identical pixels");
    buffered.yaw+=10.f;render(buffered,false,nullptr);
    expect(hunter_model::Counters().poses==1&&Pixels(buffered)!=paused_pixels,"rotation refreshes a paused preview immediately");
    buffered.zoom=.9f;render(buffered,false,nullptr);
    expect(hunter_model::Counters().poses==2,"zoom refreshes a paused preview immediately");
    const auto standard_box=buffered.collision_bounds;
    expect(standard_box[2]>standard_box[0]&&standard_box[3]>standard_box[1],"standard preview box has a valid projected hull");
    buffered.time+=.1f;render(buffered,false,nullptr);
    expect(hunter_model::Counters().poses==3,"seeking a paused animation refreshes the cached pose");
    expect(buffered.collision_bounds==standard_box,"standard preview box remains independent of the animated pose");
    dt=1.f/240.f;buffered.playing=true;hunter_model::ResetCounters();const float high_hz_start=buffered.time;
    for(int i=0;i<240;++i)render(buffered,false,nullptr);
    expect(hunter_model::Counters().poses==60&&hunter_model::Counters().cache_hits==180,
        "240 Hz menus update animation textures at 60 Hz while reusing intervening frames");
    expect(std::abs(buffered.time-high_hz_start-1.f)<.0001f,"refresh limiting preserves one second of animation time at 240 Hz");
    dt=1.f/60.f;
    buffered.playing=true;buffered.speed=2.f;const float before=buffered.time;render(buffered,false,nullptr);
    expect(std::abs(buffered.time-before-dt*2.f)<.00001f,"playback advances at the original delta time and speed");
    const auto saved=test_device->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);DWORD after{};
    render(buffered,false,nullptr);test_device->GetRenderState(D3DRS_ALPHABLENDENABLE,&after);
    expect(SUCCEEDED(saved) && after==TRUE,"cached state blocks restore the caller's render state");
    {
        ComPtr<IDirect3DVertexBuffer9> native_vertices,restored_vertices;
        ComPtr<IDirect3DIndexBuffer9> native_indices,restored_indices;
        const bool made=SUCCEEDED(test_device->CreateVertexBuffer(128,D3DUSAGE_WRITEONLY,0,D3DPOOL_DEFAULT,&native_vertices,nullptr)) &&
            SUCCEEDED(test_device->CreateIndexBuffer(12,D3DUSAGE_WRITEONLY,D3DFMT_INDEX32,D3DPOOL_DEFAULT,&native_indices,nullptr));
        expect(made,"caller buffers are available for state regression");
        if(made) {
            test_device->SetStreamSource(0,native_vertices.Get(),0,sizeof(hunter_model::Vertex));
            test_device->SetIndices(native_indices.Get());render(buffered,false,nullptr);
            UINT offset{},stride{};test_device->GetStreamSource(0,&restored_vertices,&offset,&stride);
            test_device->GetIndices(&restored_indices);
            expect(restored_vertices.Get()==native_vertices.Get() && stride==sizeof(hunter_model::Vertex) &&
                restored_indices.Get()==native_indices.Get(),"preview restores the caller's vertex/index bindings");
            test_device->SetStreamSource(0,nullptr,0,0);test_device->SetIndices(nullptr);
        }
    }
    xdraw::draw_list lines;hunter_model::Skeleton(buffered,lines,0,0,320,400,{255,255,255},1);
    expect(skeleton_lines>0,"the animated skeleton remains available");
    expect(hunter_model::Pick(buffered,-1000,-1000)==-1,"out-of-bounds picking exits without altering hit zones");
    const float pick_x=(buffered.bounds[0]+buffered.bounds[2])*.5f,pick_y=(buffered.bounds[1]+buffered.bounds[3])*.5f;
    hunter_model::ResetCounters();const int picked=hunter_model::Pick(buffered,pick_x,pick_y);
    expect(hunter_model::Pick(buffered,pick_x,pick_y)==picked&&hunter_model::Counters().picks==1,
        "stationary picking reuses the result for an unchanged preview pose");
    render(buffered,false,nullptr);hunter_model::Pick(buffered,pick_x,pick_y);
    expect(hunter_model::Counters().picks==2,"a fresh animation pose invalidates cached picking");
    hunter_model::Invalidate(reference);hunter_model::Invalidate(buffered);hunter_model::ResetCounters();
    expect(!buffered.texture && !buffered.vertex_buffer && render(buffered,false,nullptr),"device-object invalidation recreates render targets and mesh buffers");
    expect(hunter_model::Counters().indices==1,"the static index buffer is recreated after invalidation");
    hunter_model::Invalidate(reference);hunter_model::Invalidate(buffered);
    const auto before_reset=buffered.time;
    expect(SUCCEEDED(test_device->Reset(&pp)),"native DX9 reset is not blocked by cached default-pool resources");
    expect(render(buffered,false,nullptr) && std::abs(buffered.time-before_reset-dt*2.f)<.00001f,
        "animation continues with its original clock after a native device reset");
    hunter_model::Invalidate(reference);hunter_model::Invalidate(buffered);
    std::printf("%u DX9 preview checks, %u failed; %u anatomical draws share one vertex upload\n",checks,failed,group_draws);
    test_device.Reset();DestroyWindow(window);return failed?1:0;
}
