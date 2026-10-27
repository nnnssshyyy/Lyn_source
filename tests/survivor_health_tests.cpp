#include "survivor_health.h"
#include <cmath>
#include <cstdio>
#include <limits>

int main() {
    unsigned failed{};
    const auto expect=[&](bool ok,const char* name) {
        if(!ok) {++failed; std::fprintf(stderr,"FAIL: %s\n",name);}
    };

    using survivor_health::Calculate;
    using survivor_health::Sample;
    {
        const auto hp=Calculate(Sample{50,100,300,false,0.f,0.f,100.f,.27f});
        expect(hp.current==50 && hp.maximum==100 && hp.temporary==0,"permanent health without pills");
    }
    {
        const auto fresh=Calculate(Sample{20,100,300,false,50.f,100.f,100.f,.27f});
        const auto decayed=Calculate(Sample{20,100,300,false,50.f,100.f,200.f,.27f});
        expect(fresh.current==70 && fresh.temporary==50,"pills add to permanent health");
        expect(decayed.current==43 && decayed.temporary==23,"temporary health decays with game time");
    }
    {
        const auto capped=Calculate(Sample{90,100,300,false,50.f,10.f,10.f,.27f});
        expect(capped.current==140 && capped.maximum==100,"reported health retains the game's unclamped total");
    }
    {
        const auto from_buffer=Calculate(Sample{1,100,300,true,275.f,10.f,10.f,.27f});
        const auto bleeding=Calculate(Sample{1,100,300,true,275.f,10.f,20.f,.27f});
        const auto from_health=Calculate(Sample{245,100,300,true,0.f,0.f,200.f,.27f});
        expect(from_buffer.current==276 && from_buffer.maximum==300,"downed buffer adds to base HP");
        expect(bleeding.current==273 && bleeding.maximum==300,"downed buffer follows client health decay");
        expect(from_health.current==245 && from_health.maximum==300,"downed permanent pool is respected");
    }
    {
        const auto no_buffer=Calculate(Sample{1,100,300,true,std::numeric_limits<float>::quiet_NaN(),0.f,10.f,.27f});
        expect(no_buffer.current==1 && no_buffer.maximum==300,"invalid buffer cannot invent health");
    }
    std::printf("survivor health: %s\n",failed?"FAIL":"PASS");
    return failed?1:0;
}
