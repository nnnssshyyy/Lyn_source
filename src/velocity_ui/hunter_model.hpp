#pragma once
#include "xdraw.hpp"

namespace hunter_model {
struct Point { float x{},y{},z{}; };
struct Vertex { float x{},y{},z{},rhw{1}; DWORD color{}; float u{},v{}; };
struct View {
    int clip{};
    bool playing{true};
    float time{},speed{1},yaw{15},zoom{1};
    int width{},height{};
    Microsoft::WRL::ComPtr<IDirect3DTexture9> texture;
    Microsoft::WRL::ComPtr<IDirect3DSurface9> depth;
    Microsoft::WRL::ComPtr<IDirect3DVertexBuffer9> vertex_buffer;
    std::vector<Vertex> vertices;
    std::vector<Point> joints;
    std::array<float,4> bounds{},collision_bounds{};
    struct CachedFrame {
        bool valid{};
        int clip{};
        float time{},yaw{},zoom{},observed_time{},elapsed{};
        std::array<xdraw::color,7> fills{};
    } cached;
    mutable bool pick_cached{};
    mutable float pick_x{},pick_y{};
    mutable int pick_group{-1};
};
bool Render(View& view,int width,int height,const std::array<xdraw::color,7>* fills=nullptr);
int Pick(const View& view,float x,float y);
void Skeleton(const View& view,xdraw::draw_list& draw,float x,float y,float w,float h,
              xdraw::color color,float thickness);
void Invalidate(View& view);
#ifdef FREE_PREVIEW_TESTING
void ReferenceUpload(bool enabled);
struct UploadCounters {unsigned vertices{},indices{},buffered_draws{},reference_draws{},poses{},cache_hits{},picks{};};
UploadCounters Counters();
void ResetCounters();
#endif
}
