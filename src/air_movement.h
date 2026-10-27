#pragma once
#include <algorithm>
#include <array>
#include <cmath>

namespace air_movement {
constexpr float Pi=3.14159265358979323846f;
constexpr int Jump=2,Forward=8,Back=16,Left=512,Right=1024;
// Verified in the installed CGameMovement::AirAccelerate: only the projection
// onto wishdir is capped at 30, not the player's complete horizontal speed.
constexpr float AirCap=30.f;
inline bool MobileForAssistance(bool mobile,bool ghost,int team) noexcept {
    // Ghosts may retain incap/pin fields from their preceding life.
    return mobile || (ghost && team==3);
}
inline bool WalkPhysics(int move_type,int water) noexcept {
    // Valid ground physics applies to both living players and walking ghosts.
    // Noclip, ladders, observer cameras and swimming keep their native input.
    return move_type==2 && water>=0 && water<2;
}
inline float Normalize(float yaw) noexcept {return std::remainder(yaw,360.f);}
struct Input {
    int number{},seed{},buttons{},mouse_x{};
    float yaw{},forward{},side{};
};
struct Player {
    unsigned owner{};
    bool walk{},grounded{};
    int water{};
    float vx{},vy{};
};
struct Settings {
    bool bhop{},strafe{},directional{true};
    float interval{},air_acceleration{10.f},max_speed{220.f},friction{1.f};
};
struct Result {
    float forward{},side{},speed{},gain{},basis_yaw{};
    bool jump{},held{},hop_applied{},strafed{},restore_move{};
};
class Controller {
    struct Saved {Input input{};unsigned owner{};bool valid{},moved{},grounded{},jumped{};};
    std::array<Saved,150> inputs_{};
public:
    void Reset() noexcept {inputs_.fill({});}
    Result Run(Input command,const Player& player,const Settings& settings) noexcept {
        Result out{command.forward,command.side};out.jump=(command.buttons&Jump)!=0;
        if((!settings.bhop && !settings.strafe) || !player.owner || command.number<=0 ||
           !player.walk || player.water<0 || player.water>=2 ||
           !std::isfinite(command.yaw) || !std::isfinite(command.forward) || !std::isfinite(command.side) ||
           !std::isfinite(settings.interval) || settings.interval<.001f || settings.interval>.1f) return out;
        auto& saved=inputs_[static_cast<unsigned>(command.number)%inputs_.size()];
        if(saved.valid && saved.owner==player.owner && saved.input.number==command.number && saved.input.seed==command.seed) {
            command=saved.input;out.restore_move=saved.moved;
        } else saved={command,player.owner,true,false};
        saved.moved=false;out.forward=command.forward;out.side=command.side;
        const bool held=(command.buttons&Jump)!=0;
        out.held=held;out.basis_yaw=command.yaw;
        if(settings.bhop) {
            // Retain the original command's jump intent across reprediction,
            // clear it in the air, and re-arm on the first grounded command.
            const auto& previous=inputs_[static_cast<unsigned>(command.number-1)%inputs_.size()];
            const bool repeat_ground=previous.valid && previous.owner==player.owner &&
                previous.input.number==command.number-1 && previous.grounded && previous.jumped;
            // A landing jump can be rejected by stamina/animation cooldown.
            // Release for one grounded command so the next attempt has a new
            // input edge instead of leaving the engine's old jump bit latched.
            out.jump=held && player.grounded && !repeat_ground;out.hop_applied=true;
            saved.grounded=player.grounded;saved.jumped=out.jump;
        }
        if(!settings.strafe || !held || !std::isfinite(player.vx) || !std::isfinite(player.vy)) return out;
        const float speed=std::hypot(player.vx,player.vy);out.speed=speed;
        if(!std::isfinite(speed) || speed>100000.f || !std::isfinite(settings.air_acceleration) ||
           settings.air_acceleration<=0.f || settings.air_acceleration>10000.f ||
           !std::isfinite(settings.max_speed) || settings.max_speed<=0.f || settings.max_speed>10000.f ||
           !std::isfinite(settings.friction) || settings.friction<=0.f || settings.friction>10.f) return out;
        float forward=command.forward,side=command.side;
        // Button-only binds still express a direction, including cancelling
        // pairs. Analog movement has priority when the engine provides it.
        if(std::abs(forward)<=.1f && std::abs(side)<=.1f) {
            forward=static_cast<float>(!!(command.buttons&Forward)-!!(command.buttons&Back));
            side=static_cast<float>(!!(command.buttons&Right)-!!(command.buttons&Left));
        }
        const bool manual=std::abs(forward)>.1f || std::abs(side)>.1f;
        float desired=command.yaw;
        if(settings.directional && manual) desired-=std::atan2(side,forward)*180.f/Pi;
        // On the ground leave requested movement alone. Holding jump alone
        // can launch from rest rather than leaving auto-strafe stuck at 0 u/s.
        if(player.grounded) {
            if(speed<5.f && !manual) {out.forward=450.f;out.side=0.f;out.strafed=saved.moved=true;}
            return out;
        }
        const float wish_speed=std::min(settings.max_speed,450.f);
        const float cap=std::min(AirCap,wish_speed);
        const float acceleration=settings.air_acceleration*wish_speed*settings.interval*settings.friction;
        if(!std::isfinite(acceleration) || acceleration<=0.f) return out;
        float wish_yaw=desired;
        float vx{},vy{};
        if(speed>.001f) {
            // Maximise |v + a*w| subject to the native projection cap. Once
            // a >= cap the optimum is perpendicular, not a tiny forward turn.
            const float projection=std::clamp(cap-acceleration,0.f,speed);
            const float angle=std::acos(std::clamp(projection/speed,0.f,1.f));
            const float velocity_yaw=std::atan2(player.vy,player.vx);
            float best_error=1000.f;
            // Deterministic tie-breaking does not advance state on a replay.
            const int first=(command.number&1)?1:-1;
            for(int pass=0;pass<2;++pass) {
                const int sign=pass?-first:first;
                const float yaw=velocity_yaw+sign*angle;
                const float x=std::cos(yaw),y=std::sin(yaw);
                const float add=std::min(acceleration,std::max(0.f,cap-player.vx*x-player.vy*y));
                const float next_x=player.vx+add*x,next_y=player.vy+add*y;
                const float heading=std::atan2(next_y,next_x)*180.f/Pi;
                const float error=std::abs(Normalize(desired-heading));
                if(error+.0001f<best_error) {
                    best_error=error;wish_yaw=yaw*180.f/Pi;vx=next_x;vy=next_y;
                }
            }
        } else {
            const float add=std::min(acceleration,cap);
            vx=player.vx+add*std::cos(desired*Pi/180.f);vy=player.vy+add*std::sin(desired*Pi/180.f);
        }
        const float relative=Normalize(command.yaw-wish_yaw)*Pi/180.f;
        out.forward=450.f*std::cos(relative);out.side=450.f*std::sin(relative);
        out.gain=std::max(0.f,std::hypot(vx,vy)-speed);out.strafed=saved.moved=true;
        return out;
    }
};
}
