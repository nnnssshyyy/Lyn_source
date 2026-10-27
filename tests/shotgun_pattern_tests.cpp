#include "shotgun_pattern.h"
#include <bcrypt.h>
#include <cstdio>
#include <cstring>
#include <limits>

namespace {
unsigned checks{},failures{};
void Expect(bool pass,const char* message){++checks;if(!pass){++failures;std::printf("FAIL %s\n",message);}}
int PseudoRandom(int command) {
    BCRYPT_ALG_HANDLE algorithm{};BCRYPT_HASH_HANDLE hash{};unsigned char digest[16]{};int seed=-1;
    if(BCryptOpenAlgorithmProvider(&algorithm,BCRYPT_MD5_ALGORITHM,nullptr,0)>=0 &&
       BCryptCreateHash(algorithm,&hash,nullptr,0,nullptr,0,0)>=0 &&
       BCryptHashData(hash,reinterpret_cast<PUCHAR>(&command),4,0)>=0 &&
       BCryptFinishHash(hash,digest,sizeof(digest),0)>=0){std::memcpy(&seed,digest+6,4);seed&=0x7fffffff;}
    if(hash)BCryptDestroyHash(hash);if(algorithm)BCryptCloseAlgorithmProvider(algorithm,0);return seed;
}
}
int main() {
    using namespace shotgun_pattern;
    for(const auto& c:Candidates){
        Expect(c.number>0 && c.seed==PseudoRandom(c.number),"wire command reproduces exactly the selected prediction seed");
        for(int pellets=8;pellets<=11;++pellets)
            Expect(std::abs(MaxRadius(c.seed,pellets)-c.radius[pellets-8])<1e-6f,"every preset covers every pellet, including last pellet");
    }
    Expect(Pellets(6)==10 && Pellets(7)==8 && Pellets(8)==11 && Pellets(9)==9,"four vanilla shotguns have distinct pellet counts");
    Expect(Pellets(0)==0 && Pellets(-1)==0 && Pellets(49)==0,"other weapons cannot select a shotgun pattern");
    for(int pellets=8;pellets<=11;++pellets){
        float previous=MaxRadius(123456,pellets);
        for(int percent=0;percent<=100;++percent){
            const auto* selected=Select(123456,pellets,percent*.01f);
            const float radius=selected?selected->radius[pellets-8]:MaxRadius(123456,pellets);
            Expect(radius<=previous+1e-6f,"increasing Accuracy cannot widen the selected pattern");previous=radius;
        }
        Expect(previous<.22f,"100 percent covers all pellets within 22 percent of the native radial limit");
        Expect(previous>0.f,"100 percent is never claimed to eliminate residual scatter");
        const auto* best=Select(123456,pellets,1.f);
        Expect(best && Select(best->seed,pellets,1.f)==nullptr,"an already tighter native seed is preserved");
    }
    Expect(MaxRadius(494641349,10)>.9f && MaxRadius(494641349,11)>.9f,"reference constant leaves outer pellets on Pump and Auto");
    Expect(!Select(123456,7,1) && !Select(123456,12,1) && !Select(-1,10,1),"unsupported counts and missing seeds fail closed");
    Expect(!Select(123456,10,0) && !Select(123456,10,-1) && !Select(123456,10,1.1f) &&
           !Select(123456,10,std::numeric_limits<float>::quiet_NaN()),"zero and invalid intensity preserve native commands");
    History history;game::Command shot{},next{};
    shot.number=159;shot.tick=1000;shot.seed=Candidates.back().seed;shot.angles={11.f,22.f,0};shot.buttons=1;
    shot.tail[5]=73;next.number=160;next.tick=1001;next.seed=PseudoRandom(160);next.buttons=32;
    const auto untouched=shot;
    Expect(history.Store(&shot,shot,Candidates.back().number),"record a seeded shot only after its command write");
    auto to=shot,from=next;
    Expect(history.Translate(&shot,to) && to.number==Candidates.back().number,"shot serializes with chosen positive wire number");
    to.number=shot.number;
    Expect(std::memcmp(&to,&untouched,sizeof(to))==0 && std::memcmp(&shot,&untouched,sizeof(shot))==0,"translation preserves local number and every non-number field");
    Expect(!history.Translate(&next,from),"ordinary following command retains its own native number");
    from=shot;
    Expect(history.Translate(&shot,from) && from.number==Candidates.back().number,"delta baseline translates after bind release");
    to=shot;
    Expect(history.Translate(&shot,to),"packet backup reuses the identical chosen number");
    game::Command other=shot;to=other;
    Expect(!history.Translate(&other,to),"same identity in a different input slot or buffer cannot match");
    to=shot;to.seed^=1;Expect(!history.Translate(&shot,to),"different prediction seed cannot match old record");
    to=shot;to.number+=150;Expect(!history.Translate(&shot,to),"ring reuse cannot match an old command number");
    game::Command replacement=shot;replacement.number+=150;
    Expect(history.Store(&shot,replacement,4),"ring slot accepts a later command");to=shot;
    Expect(!history.Translate(&shot,to),"overwritten history cannot affect an older command");
    Expect(!history.Store(nullptr,shot,4) && !history.Store(&shot,shot,0),"invalid stores are rejected");
    history.Store(&shot,shot,5);history.Clear();to=shot;
    Expect(!history.Translate(&shot,to),"map change clears every wire mapping");
    std::printf("%u shotgun pattern checks, %u failed\n",checks,failures);return failures?1:0;
}
