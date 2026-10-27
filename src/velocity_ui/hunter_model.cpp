#include "hunter_model.hpp"
#include "../log.h"
#include <stdexcept>

extern "C" IMAGE_DOS_HEADER __ImageBase;
namespace hunter_model {
namespace {
using Microsoft::WRL::ComPtr;
struct Vec { float x{},y{},z{}; };
struct Quat { float x{},y{},z{},w{1}; };
struct Matrix { float a[3][4]{}; };
struct Bone { int parent; unsigned visible; Matrix inverse; };
struct SourceVertex { Vec position,normal; float u,v,weight[3]; unsigned bone[3]; };
struct Pose { Vec position; Quat rotation; };
struct Clip { float fps; unsigned frames; std::vector<Pose> poses; };
struct Asset {
    bool attempted{},ready{};
    std::vector<Bone> bones;
    std::vector<SourceVertex> vertices;
    std::array<std::vector<unsigned>,7> indices;
    std::vector<Clip> clips;
    ComPtr<IDirect3DTexture9> diffuse;
    ComPtr<IDirect3DIndexBuffer9> index_buffer;
    std::array<UINT,7> index_offsets{};
    ComPtr<IDirect3DStateBlock9> render_state;
    DWORD render_targets{};
} asset;
#ifdef FREE_PREVIEW_TESTING
bool reference_upload{};
UploadCounters upload_counters{};
#endif
static_assert(sizeof(Bone)==56 && sizeof(SourceVertex)==56 && sizeof(Pose)==28);

std::span<const std::byte> Resource(unsigned id) {
    const auto module=reinterpret_cast<HMODULE>(&__ImageBase);
    const auto res=FindResourceW(module,MAKEINTRESOURCEW(id),RT_RCDATA);
    const auto handle=res?LoadResource(module,res):nullptr;
    const auto ptr=handle?static_cast<const std::byte*>(LockResource(handle)):nullptr;
    return ptr?std::span<const std::byte>{ptr,SizeofResource(module,res)}:std::span<const std::byte>{};
}
struct Reader {
    std::span<const std::byte> data;
    template<class T> T Read() {
        if(data.size()<sizeof(T)) throw std::runtime_error("truncated preview asset");
        T value; std::memcpy(&value,data.data(),sizeof(value)); data=data.subspan(sizeof(value)); return value;
    }
    template<class T> std::vector<T> Array(size_t count) {
        if(count>data.size()/sizeof(T)) throw std::runtime_error("invalid preview array");
        std::vector<T> out(count); std::memcpy(out.data(),data.data(),count*sizeof(T));
        data=data.subspan(count*sizeof(T)); return out;
    }
};
bool Finite(Vec p) { return std::isfinite(p.x)&&std::isfinite(p.y)&&std::isfinite(p.z); }
bool Load() {
    if(asset.attempted) return asset.ready;
    asset.attempted=true;
    try {
        Reader r{Resource(202)};
        if(r.Read<unsigned>()!=0x31525048) return false;
        const auto nb=r.Read<unsigned>(),nv=r.Read<unsigned>(),ni=r.Read<unsigned>(),nc=r.Read<unsigned>();
        if(!nb||nb>128||!nv||nv>32768||ni>196608||nc!=3) return false;
        asset.bones=r.Array<Bone>(nb); asset.vertices=r.Array<SourceVertex>(nv);
        for(unsigned i=0;i<nb;++i) {
            const auto& b=asset.bones[i];
            if(b.parent<-1||b.parent>=static_cast<int>(i)) return false;
            for(const auto& row:b.inverse.a) for(float x:row) if(!std::isfinite(x)) return false;
        }
        for(const auto& v:asset.vertices) {
            if(!Finite(v.position)||!Finite(v.normal)||!std::isfinite(v.u)||!std::isfinite(v.v)) return false;
            float sum=0;
            for(int i=0;i<3;++i) {
                if(v.bone[i]>=nb||!std::isfinite(v.weight[i])||v.weight[i]<0||v.weight[i]>1.001f) return false;
                sum+=v.weight[i];
            }
            if(std::abs(sum-1.f)>.01f) return false;
        }
        unsigned total=0;
        for(auto& indices:asset.indices) {
            const auto count=r.Read<unsigned>();
            if(count>ni-total||count%3) return false;
            indices=r.Array<unsigned>(count); total+=count;
            for(auto i:indices) if(i>=nv) return false;
        }
        if(total!=ni) return false;
        for(unsigned i=0;i<nc;++i) {
            Clip clip{r.Read<float>(),r.Read<unsigned>(),{}};
            if(!std::isfinite(clip.fps)||clip.fps<=0||clip.fps>120||clip.frames<2||clip.frames>1024) return false;
            clip.poses=r.Array<Pose>(static_cast<size_t>(clip.frames)*nb);
            for(const auto& p:clip.poses) {
                const auto& q=p.rotation;
                const float len=q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w;
                if(!Finite(p.position)||!std::isfinite(len)||len<.9f||len>1.1f) return false;
            }
            asset.clips.push_back(std::move(clip));
        }
        if(!r.data.empty()) return false;
        asset.diffuse=xdraw::load_texture(Resource(203));
        asset.ready=asset.diffuse!=nullptr;
        app::Log("Hunter 3D: %u vertices, %u bones, %u original animation clips",nv,nb,nc);
    } catch(...) { app::Log("Hunter 3D: resource unavailable"); }
    return asset.ready;
}
Vec Transform(const Matrix& m,Vec p,bool normal=false) {
    return {m.a[0][0]*p.x+m.a[0][1]*p.y+m.a[0][2]*p.z+(normal?0:m.a[0][3]),
            m.a[1][0]*p.x+m.a[1][1]*p.y+m.a[1][2]*p.z+(normal?0:m.a[1][3]),
            m.a[2][0]*p.x+m.a[2][1]*p.y+m.a[2][2]*p.z+(normal?0:m.a[2][3])};
}
Matrix Multiply(const Matrix& a,const Matrix& b) {
    Matrix m;
    for(int i=0;i<3;++i) {
        for(int j=0;j<4;++j) for(int k=0;k<3;++k) m.a[i][j]+=a.a[i][k]*b.a[k][j];
        m.a[i][3]+=a.a[i][3];
    }
    return m;
}
Matrix Interpolate(const Pose& a,const Pose& b,float t) {
    const auto& p=a.rotation; const auto& q=b.rotation;
    const float sign=p.x*q.x+p.y*q.y+p.z*q.z+p.w*q.w<0?-1.f:1.f;
    float x=p.x*(1-t)+q.x*t*sign,y=p.y*(1-t)+q.y*t*sign;
    float z=p.z*(1-t)+q.z*t*sign,w=p.w*(1-t)+q.w*t*sign;
    const float inv=1.f/std::sqrt(x*x+y*y+z*z+w*w); x*=inv;y*=inv;z*=inv;w*=inv;
    return {{{1-2*(y*y+z*z),2*(x*y-z*w),2*(x*z+y*w),a.position.x*(1-t)+b.position.x*t},
             {2*(x*y+z*w),1-2*(x*x+z*z),2*(y*z-x*w),a.position.y*(1-t)+b.position.y*t},
             {2*(x*z-y*w),2*(y*z+x*w),1-2*(x*x+y*y),a.position.z*(1-t)+b.position.z*t}}};
}
void PoseModel(View& view) {
#ifdef FREE_PREVIEW_TESTING
    ++upload_counters.poses;
#endif
    view.pick_cached=false;
    const auto& clip=asset.clips[std::clamp(view.clip,0,2)];
    const float frame=std::fmod(std::max(view.time,0.f)*clip.fps,static_cast<float>(clip.frames-1));
    const auto f=static_cast<unsigned>(frame); const float fraction=frame-static_cast<float>(f);
    const auto n=asset.bones.size();
    std::array<Matrix,128> world{},skin{};
    view.joints.resize(n); view.vertices.resize(asset.vertices.size());
    // Root motion is already extracted in the original animation. Keep its sway.
    const float yaw=view.yaw*std::numbers::pi_v<float>/180.f,c=std::cos(yaw),s=std::sin(yaw);
    const float scale=std::min(view.height/82.f,view.width/76.f)*view.zoom;
    auto project=[&](Vec p) -> Point {
        const float depth=c*p.x+s*p.y;
        return {view.width*.5f+(s*p.x-c*p.y)*scale,
                view.height*.53f-(p.z-36.f)*scale,.5f-depth/512.f};
    };
    // Preview the standard standing hull independently of the animated mesh.
    // Camera rotation and zoom still affect both box modes.
    view.collision_bounds={1e9f,1e9f,-1e9f,-1e9f};
    for(unsigned corner=0;corner<8;++corner) {
        const auto p=project({corner&1?16.f:-16.f,corner&2?16.f:-16.f,corner&4?72.f:0.f});
        view.collision_bounds[0]=std::min(view.collision_bounds[0],p.x);
        view.collision_bounds[1]=std::min(view.collision_bounds[1],p.y);
        view.collision_bounds[2]=std::max(view.collision_bounds[2],p.x);
        view.collision_bounds[3]=std::max(view.collision_bounds[3],p.y);
    }
    for(size_t i=0;i<n;++i) {
        const auto local=Interpolate(clip.poses[f*n+i],clip.poses[(f+1)*n+i],fraction);
        const auto parent=asset.bones[i].parent;
        world[i]=parent<0?local:Multiply(world[parent],local);
        skin[i]=Multiply(world[i],asset.bones[i].inverse);
        view.joints[i]=project({world[i].a[0][3],world[i].a[1][3],world[i].a[2][3]});
    }
    view.bounds={1e9f,1e9f,-1e9f,-1e9f};
    for(size_t i=0;i<asset.vertices.size();++i) {
        const auto& src=asset.vertices[i]; Vec pos{},norm{};
        for(int k=0;k<3;++k) {
            if(src.weight[k]==0.f) continue;
            const auto p=Transform(skin[src.bone[k]],src.position);
            const auto nrm=Transform(skin[src.bone[k]],src.normal,true); const auto weight=src.weight[k];
            pos.x+=p.x*weight;pos.y+=p.y*weight;pos.z+=p.z*weight;
            norm.x+=nrm.x*weight;norm.y+=nrm.y*weight;norm.z+=nrm.z*weight;
        }
        const auto p=project(pos);
        // Soft studio light follows the viewer; texture stays readable from every side.
        const float light=std::clamp(.66f+.40f*(.7f*(norm.x*c+norm.y*s)+.55f*norm.z),.36f,1.f);
        const auto l=static_cast<unsigned>(light*255);
        view.vertices[i]={p.x-.5f,p.y-.5f,p.z,1,D3DCOLOR_ARGB(255,l,l,l),src.u,src.v};
        view.bounds[0]=std::min(view.bounds[0],p.x);view.bounds[1]=std::min(view.bounds[1],p.y);
        view.bounds[2]=std::max(view.bounds[2],p.x);view.bounds[3]=std::max(view.bounds[3],p.y);
    }
}
// State blocks do not capture render targets. Save every MRT slot explicitly.
struct Restore {
    IDirect3DDevice9* d;
    ComPtr<IDirect3DStateBlock9> state;
    std::array<ComPtr<IDirect3DSurface9>,4> targets;
    ComPtr<IDirect3DSurface9> depth;
    D3DVIEWPORT9 viewport{};
    DWORD count{};
    bool captured{};
    explicit Restore(IDirect3DDevice9* device):d(device) {
        if(!asset.render_targets) {
            D3DCAPS9 caps{};if(FAILED(d->GetDeviceCaps(&caps))) return;
            asset.render_targets=std::clamp<DWORD>(caps.NumSimultaneousRTs,1,4);
        }
        if(!asset.render_state && FAILED(d->CreateStateBlock(D3DSBT_ALL,&asset.render_state))) return;
        state=asset.render_state;
        if(FAILED(state->Capture())||FAILED(d->GetViewport(&viewport))) return;
        count=asset.render_targets;
        for(DWORD i=0;i<count;++i) d->GetRenderTarget(i,&targets[i]);
        d->GetDepthStencilSurface(&depth);
        captured=targets[0]!=nullptr;
    }
    ~Restore() {
        if(!captured) return;
        d->SetTexture(0,nullptr); d->SetDepthStencilSurface(nullptr);
        for(DWORD i=1;i<count;++i) d->SetRenderTarget(i,nullptr);
        d->SetRenderTarget(0,targets[0].Get());
        for(DWORD i=1;i<count;++i) d->SetRenderTarget(i,targets[i].Get());
        d->SetDepthStencilSurface(depth.Get()); state->Apply(); d->SetViewport(&viewport);
    }
};
bool UploadMesh(IDirect3DDevice9* d,View& view) {
#ifdef FREE_PREVIEW_TESTING
    if(reference_upload) return false;
#endif
    if(!asset.index_buffer) {
        UINT count=0;
        for(size_t g=0;g<7;++g) {asset.index_offsets[g]=count;count+=static_cast<UINT>(asset.indices[g].size());}
        if(!count || FAILED(d->CreateIndexBuffer(count*sizeof(unsigned),D3DUSAGE_WRITEONLY,D3DFMT_INDEX32,
                                                 D3DPOOL_DEFAULT,&asset.index_buffer,nullptr))) return false;
        void* data{};
        if(FAILED(asset.index_buffer->Lock(0,0,&data,0))) {asset.index_buffer.Reset();return false;}
        for(size_t g=0;g<7;++g) std::memcpy(static_cast<unsigned*>(data)+asset.index_offsets[g],
                                          asset.indices[g].data(),asset.indices[g].size()*sizeof(unsigned));
        if(FAILED(asset.index_buffer->Unlock())) {asset.index_buffer.Reset();return false;}
#ifdef FREE_PREVIEW_TESTING
        ++upload_counters.indices;
#endif
    }
    const auto bytes=static_cast<UINT>(view.vertices.size()*sizeof(Vertex));
    if(!view.vertex_buffer && FAILED(d->CreateVertexBuffer(bytes,D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,
        D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1,D3DPOOL_DEFAULT,&view.vertex_buffer,nullptr))) return false;
    void* data{};
    if(FAILED(view.vertex_buffer->Lock(0,bytes,&data,D3DLOCK_DISCARD))) return false;
    std::memcpy(data,view.vertices.data(),bytes);
    const bool unlocked=SUCCEEDED(view.vertex_buffer->Unlock());
#ifdef FREE_PREVIEW_TESTING
    if(unlocked) ++upload_counters.vertices;
#endif
    return unlocked;
}
}
void Invalidate(View& view) {
    view.texture.Reset();view.depth.Reset();view.vertex_buffer.Reset();view.width=view.height=0;
    view.cached={};view.pick_cached=false;
    asset.index_buffer.Reset();asset.render_state.Reset();asset.render_targets=0;
}
#ifdef FREE_PREVIEW_TESTING
void ReferenceUpload(bool enabled) {reference_upload=enabled;}
UploadCounters Counters() {return upload_counters;}
void ResetCounters() {upload_counters={};}
#endif
bool Render(View& view,int width,int height,const std::array<xdraw::color,7>* fills) {
    auto* d=xdraw::device();
    if(!d||!Load()) return false;
    width=std::clamp(width,128,768);height=std::clamp(height,128,768);
    if(!view.texture||view.width!=width||view.height!=height) {
        Invalidate(view);
        if(FAILED(d->CreateTexture(width,height,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&view.texture,nullptr))||
           FAILED(d->CreateDepthStencilSurface(width,height,D3DFMT_D16,D3DMULTISAMPLE_NONE,0,TRUE,&view.depth,nullptr))) {
            Invalidate(view);return false;
        }
        view.width=width;view.height=height;
    }
    auto& cached=view.cached;
    const bool seek=cached.valid&&view.time!=cached.observed_time;
    const float dt=xdraw::delta_time();
    if(view.playing)view.time+=dt*view.speed;
    cached.observed_time=view.time;
    cached.elapsed=std::min(1.f,cached.elapsed+dt);
    const std::array<xdraw::color,7> colors=fills?*fills:std::array<xdraw::color,7>{};
    const bool changed=!cached.valid||seek||cached.clip!=view.clip||cached.yaw!=view.yaw||cached.zoom!=view.zoom||
        !std::equal(colors.begin(),colors.end(),cached.fills.begin(),[](auto a,auto b){return a.val==b.val;});
    // Keep the animation clock at display rate, but reuse the texture between
    // 60 Hz poses. Rotation, zoom, seeks and priority edits refresh immediately.
    if(!changed&&(view.time==cached.time||cached.elapsed+1.e-6f<1.f/60.f)) {
#ifdef FREE_PREVIEW_TESTING
        ++upload_counters.cache_hits;
#endif
        return true;
    }
    cached.valid=false;
    PoseModel(view);
    // Seven zone draws share one vertex upload; static indices are uploaded
    // once per device/reset. UP remains a fallback if a buffer cannot be made.
    const bool buffered=UploadMesh(d,view);
    Restore restore(d); if(!restore.captured) return false;
    ComPtr<IDirect3DSurface9> target;
    if(FAILED(view.texture->GetSurfaceLevel(0,&target))) return false;
    d->SetTexture(0,nullptr);d->SetDepthStencilSurface(nullptr);
    for(DWORD i=1;i<restore.count;++i) d->SetRenderTarget(i,nullptr);
    if(FAILED(d->SetRenderTarget(0,target.Get()))||FAILED(d->SetDepthStencilSurface(view.depth.Get()))) return false;
    const D3DVIEWPORT9 vp{0,0,static_cast<DWORD>(width),static_cast<DWORD>(height),0,1};
    d->SetViewport(&vp);
    d->SetRenderState(D3DRS_SCISSORTESTENABLE,FALSE);
    if(FAILED(d->Clear(0,nullptr,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,0,1,0))) return false;
    d->SetVertexShader(nullptr);d->SetPixelShader(nullptr);
    d->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1);
    if(buffered) {d->SetStreamSource(0,view.vertex_buffer.Get(),0,sizeof(Vertex));d->SetIndices(asset.index_buffer.Get());}
    d->SetStreamSourceFreq(0,1);
    d->SetRenderState(D3DRS_ZENABLE,TRUE);d->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
    d->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
    d->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);d->SetRenderState(D3DRS_LIGHTING,FALSE);
    d->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);d->SetRenderState(D3DRS_FOGENABLE,FALSE);
    d->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);d->SetRenderState(D3DRS_WRAP0,0);
    d->SetRenderState(D3DRS_STENCILENABLE,FALSE);d->SetRenderState(D3DRS_CLIPPLANEENABLE,0);
    d->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);d->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
    d->SetRenderState(D3DRS_SRGBWRITEENABLE,FALSE);d->SetRenderState(D3DRS_COLORWRITEENABLE,15);
    d->SetRenderState(D3DRS_DEPTHBIAS,0);d->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS,0);
    d->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);d->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
    d->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);d->SetSamplerState(0,D3DSAMP_SRGBTEXTURE,FALSE);
    d->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);d->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
    d->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,0);
    d->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
    d->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
    d->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
    d->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);d->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
    for(size_t group=0;group<7;++group) {
        const auto& indices=asset.indices[group]; if(indices.empty()) continue;
        const bool fill=fills&&(*fills)[group].a;
        d->SetTexture(0,fill?nullptr:asset.diffuse.Get());
        d->SetTextureStageState(0,D3DTSS_COLOROP,fill?D3DTOP_SELECTARG1:D3DTOP_MODULATE);
        d->SetTextureStageState(0,D3DTSS_COLORARG1,fill?D3DTA_TFACTOR:D3DTA_TEXTURE);
        d->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
        if(fill) {
            const auto col=(*fills)[group]; d->SetRenderState(D3DRS_TEXTUREFACTOR,D3DCOLOR_ARGB(255,col.r,col.g,col.b));
        }
        const HRESULT drawn=buffered?
            d->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,static_cast<UINT>(view.vertices.size()),
                                     asset.index_offsets[group],static_cast<UINT>(indices.size()/3)):
            d->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,static_cast<UINT>(view.vertices.size()),
                                     static_cast<UINT>(indices.size()/3),indices.data(),D3DFMT_INDEX32,view.vertices.data(),sizeof(Vertex));
        if(FAILED(drawn)) return false;
#ifdef FREE_PREVIEW_TESTING
        if(buffered) ++upload_counters.buffered_draws;else ++upload_counters.reference_draws;
#endif
    }
    cached.valid=true;cached.clip=view.clip;cached.time=view.time;cached.yaw=view.yaw;cached.zoom=view.zoom;
    cached.fills=colors;cached.elapsed=0.f;
    return true;
}
int Pick(const View& view,float x,float y) {
    if(!asset.ready||view.vertices.size()!=asset.vertices.size()) return -1;
    if(x<view.bounds[0]||x>view.bounds[2]||y<view.bounds[1]||y>view.bounds[3]) return -1;
    if(view.pick_cached&&view.pick_x==x&&view.pick_y==y)return view.pick_group;
#ifdef FREE_PREVIEW_TESTING
    ++upload_counters.picks;
#endif
    int found=-1;float closest=2;
    for(int group=0;group<7;++group) {
        const auto& ids=asset.indices[group];
        for(size_t i=0;i<ids.size();i+=3) {
            const auto& a=view.vertices[ids[i]];const auto& b=view.vertices[ids[i+1]];const auto& c=view.vertices[ids[i+2]];
            const float den=(b.y-c.y)*(a.x-c.x)+(c.x-b.x)*(a.y-c.y);
            if(std::abs(den)<.0001f) continue;
            const float u=((b.y-c.y)*(x-c.x)+(c.x-b.x)*(y-c.y))/den;
            const float v=((c.y-a.y)*(x-c.x)+(a.x-c.x)*(y-c.y))/den;
            if(u<0||v<0||u+v>1) continue;
            const float z=u*a.z+v*b.z+(1-u-v)*c.z;
            if(z>=0&&z<=1&&z<closest) {closest=z;found=group;}
        }
    }
    view.pick_cached=true;view.pick_x=x;view.pick_y=y;view.pick_group=found;
    return found;
}
void Skeleton(const View& view,xdraw::draw_list& draw,float x,float y,float w,float h,xdraw::color color,float thickness) {
    if(!asset.ready||view.joints.size()!=asset.bones.size()||!view.width||!view.height) return;
    for(size_t i=0;i<view.joints.size();++i) {
        const auto& bone=asset.bones[i];if(!bone.visible||bone.parent<0) continue;
        const auto a=view.joints[i],b=view.joints[bone.parent];
        const float ax=x+a.x*w/view.width,ay=y+a.y*h/view.height,bx=x+b.x*w/view.width,by=y+b.y*h/view.height;
        draw.line(ax,ay,bx,by,{0,0,0,220},thickness+1.f);
        draw.line(ax,ay,bx,by,color,thickness);
    }
}
}
