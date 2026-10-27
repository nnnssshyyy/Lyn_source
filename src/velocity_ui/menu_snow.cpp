#include "menu_snow.hpp"
#include "xdraw.hpp"

namespace menu_effects {
TriangleSnow::TriangleSnow() {
    std::uint32_t seed=0x73a4b21u;
    const auto random=[&]() {seed=seed*1664525u+1013904223u;return static_cast<float>(seed>>8)/16777216.f;};
    for(auto& p:particles_)p={random(),random(),random()*6.2831853f,18.f+random()*26.f,
                             2.2f+random()*2.2f,(random()-.5f)*1.2f};
}
void TriangleSnow::advance(float width,float height,float dt) {
    if(!std::isfinite(dt)||!std::isfinite(width)||!std::isfinite(height)||width<=0.f||height<=0.f)return;
    dt=std::clamp(dt,0.f,.05f);
    for(auto& p:particles_) {
        p.x+=std::sin(p.angle)*dt*6.f/width;
        p.y+=p.speed*dt/height;
        p.x-=std::floor(p.x);p.y-=std::floor(p.y);
        p.angle=std::remainder(p.angle+p.spin*dt,6.2831853f);
    }
}
void TriangleSnow::draw(xdraw::draw_list& dl,float x,float y,float width,float height,float dt,float opacity,
                        const std::array<float,4>* exclusion) {
    if(!std::isfinite(opacity)||!std::isfinite(x)||!std::isfinite(y)||!std::isfinite(width)||!std::isfinite(height)||
       opacity<=0.f||width<=16.f||height<=16.f)return;
    opacity=std::clamp(opacity,0.f,1.f);
    advance(width,height,dt);
    dl.push_clip(x+8.f,y+8.f,width-16.f,height-16.f);
    for(const auto& p:particles_) {
        const float cx=x+8.f+p.x*(width-16.f),cy=y+8.f+p.y*(height-16.f);
        const float reach=p.size*1.65f+1.f;
        // Exclude the whole particle, including its halo, before it touches the
        // menu. Translucent panels therefore never contain falling particles.
        if(exclusion&&cx+reach>=(*exclusion)[0]&&cx-reach<=(*exclusion)[2]&&
           cy+reach>=(*exclusion)[1]&&cy-reach<=(*exclusion)[3])continue;
        const float c=std::cos(p.angle),s=std::sin(p.angle);
        const auto triangle=[&](float radius,xdraw::color color) {
            const float ax=cx-s*radius,ay=cy+c*radius;
            const float bx=cx+(-.8660254f*c+.5f*s)*radius,by=cy+(-.8660254f*s-.5f*c)*radius;
            const float ex=cx+(.8660254f*c+.5f*s)*radius,ey=cy+(.8660254f*s-.5f*c)*radius;
            dl.triangle_filled(ax,ay,bx,by,ex,ey,color,false);
        };
        triangle(p.size*1.65f,{220,232,255,static_cast<std::uint8_t>(18.f*opacity)});
        triangle(p.size,{228,239,255,static_cast<std::uint8_t>((82.f+p.size*13.f)*opacity)});
    }
    dl.pop_clip();
}

namespace {constexpr float drift_x=-.56f,drift_y=.8284926f;}
CometField::CometField() {
    std::uint32_t seed=0x73a4b21u;
    const auto random=[&]() {seed=seed*1664525u+1013904223u;return static_cast<float>(seed>>8)/16777216.f;};
    for(auto& p:particles_)p={random(),random(),68.f+random()*64.f,32.f+random()*38.f,
                             .65f+random()*.45f,random()*6.2831853f};
    for(auto& star:stars_)star={random(),random(),28.f+random()*58.f};
}
void CometField::advance(float width,float height,float dt) {
    if(!std::isfinite(dt)||!std::isfinite(width)||!std::isfinite(height)||width<=0.f||height<=0.f)return;
    dt=std::clamp(dt,0.f,.05f);
    for(auto& p:particles_) {
        p.x+=drift_x*p.speed*dt/width;
        p.y+=drift_y*p.speed*dt/height;
        p.x-=std::floor(p.x);p.y-=std::floor(p.y);
        p.phase=std::remainder(p.phase+dt*.35f,6.2831853f);
    }
}
void CometField::draw(xdraw::draw_list& dl,float x,float y,float width,float height,float dt,float opacity,
                        const std::array<float,4>* exclusion) {
    if(!std::isfinite(opacity)||!std::isfinite(x)||!std::isfinite(y)||!std::isfinite(width)||!std::isfinite(height)||
       opacity<=0.f||width<=16.f||height<=16.f)return;
    opacity=std::clamp(opacity,0.f,1.f);
    advance(width,height,dt);
    const auto excluded=[&](float left,float top,float right,float bottom) {
        return exclusion&&right>=(*exclusion)[0]&&left<=(*exclusion)[2]&&
                          bottom>=(*exclusion)[1]&&top<=(*exclusion)[3];
    };
    dl.push_clip(x+8.f,y+8.f,width-16.f,height-16.f);
    for(const auto& star:stars_) {
        const float sx=std::round(x+8.f+star.x*(width-16.f)),sy=std::round(y+8.f+star.y*(height-16.f));
        if(!excluded(sx-1,sy-1,sx+2,sy+2))
            dl.rect_filled(sx,sy,1.f,1.f,{157,188,231,static_cast<std::uint8_t>(star.brightness*opacity)});
    }
    for(const auto& p:particles_) {
        const float cx=x+8.f+p.x*(width-16.f),cy=y+8.f+p.y*(height-16.f);
        const float tx=cx-drift_x*p.length,ty=cy-drift_y*p.length;
        const float reach=p.size*2.5f+1.f;
        // The tail and halo must both clear the controls, even at scaled menu sizes.
        if(excluded(std::min(cx,tx)-reach,std::min(cy,ty)-reach,
                    std::max(cx,tx)+reach,std::max(cy,ty)+reach))continue;
        const float fade=std::clamp(std::min({p.x,1.f-p.x,p.y,1.f-p.y})*14.f,0.f,1.f);
        const float alpha=opacity*fade*(.82f+.18f*std::sin(p.phase));
        if(alpha<=.005f)continue;
        dl.ensure_cmd(nullptr);
        // Two tapered quads give a fading trail without per-comet buffers or textures.
        const auto trail=[&](float half,xdraw::color tint) {
            const float nx=drift_y,ny=-drift_x;
            const auto a=dl.emit_vtx(tx+nx*.05f,ty+ny*.05f,0,0,tint.alpha(0));
            const auto b=dl.emit_vtx(cx+nx*half,cy+ny*half,1,0,tint);
            const auto c=dl.emit_vtx(cx-nx*half,cy-ny*half,1,1,tint);
            const auto d=dl.emit_vtx(tx-nx*.05f,ty-ny*.05f,0,1,tint.alpha(0));
            dl.emit_quad(a,b,c,d);
        };
        trail(p.size*2.f,{74,128,204,static_cast<std::uint8_t>(32.f*alpha)});
        trail(p.size*.6f,{178,212,255,static_cast<std::uint8_t>(155.f*alpha)});
        dl.circle_filled(cx,cy,p.size*2.5f,{117,169,242,static_cast<std::uint8_t>(20.f*alpha)},8,false);
        dl.rect_filled(cx-p.size*.5f,cy-p.size*.5f,p.size,p.size,
                       {224,240,255,static_cast<std::uint8_t>(220.f*alpha)});
    }
    dl.pop_clip();
}
}
