#pragma once
#include "game.h"
#include "aim_logic.h"

namespace aimbot {
// Follow measured angular motion without smoothing it a second time. This is
// not velocity extrapolation: only consecutive observed aim directions qualify.
class NormalTracking {
    unsigned local_{},weapon_{},target_{};
    int number_{},hitbox_{-1},sample_{};
    float interval_{};
    game::Vec3 goal_{},input_{},output_{};
public:
    void Reset() noexcept {*this={};}
    game::Vec3 Update(unsigned local,unsigned weapon,unsigned target,int number,int hitbox,int sample,
                      game::Vec3 input,game::Vec3 goal,float smoothing,float interval) noexcept {
        const bool identity=local && local==local_ && weapon==weapon_ && target==target_ &&
            hitbox==hitbox_ && sample==sample_ && interval==interval_;
        if(identity && number==number_ && goal.x==goal_.x && goal.y==goal_.y &&
           ((input.x==input_.x && input.y==input_.y) || (input.x==output_.x && input.y==output_.y))) return output_;
        const float fraction=logic::SmoothFraction(smoothing,interval);
        game::Vec3 base=input;
        if(identity && number==number_+1) {
            const game::Vec3 motion{game::Normalize(goal.x-goal_.x),game::Normalize(goal.y-goal_.y),0};
            // Large discontinuities are reacquisition/teleport, not steady motion.
            if(game::Length(motion)<=5.f) {
                const auto follow=[](float motion,float error) {
                    return motion*error>0.f?std::copysign(std::min(std::abs(motion),std::abs(error)),motion):0.f;
                };
                base.x+=follow(motion.x,game::Normalize(goal.x-base.x));
                base.y+=follow(motion.y,game::Normalize(goal.y-base.y));
            }
        }
        game::Vec3 output{std::clamp(base.x+game::Normalize(goal.x-base.x)*fraction,-89.f,89.f),
                          game::Normalize(base.y+game::Normalize(goal.y-base.y)*fraction),0};
        local_=local;weapon_=weapon;target_=target;number_=number;hitbox_=hitbox;sample_=sample;
        interval_=interval;goal_=goal;input_=input;output_=output;
        return output;
    }
};
}
