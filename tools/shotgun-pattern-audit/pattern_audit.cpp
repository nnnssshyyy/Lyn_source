#include <windows.h>
#include <bcrypt.h>
#include <array>
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <string_view>
#include <climits>
#include <cstdlib>
#include "../../src/shotgun_pattern.h"

static uint32_t crc(uint32_t seed,int extra) {
    uint32_t c=~0u;
    const auto byte=[&](unsigned char b){c^=b;for(int j=0;j<8;++j)c=(c>>1)^(0xedb88320u&(0u-(c&1)));};
    for(int i=0;i<4;++i)byte(static_cast<unsigned char>(seed>>(8*i)));
    for(int i=0;i<4;++i)byte(static_cast<unsigned char>(static_cast<uint32_t>(extra)>>(8*i)));
    for(unsigned char b:std::string_view("CTerrorPlayer::FireBullet Spread"))byte(b);
    return ~c;
}
static float uniform(uint32_t seed) {
    const int signed_seed=static_cast<int>(seed);
    int state=signed_seed==INT_MIN?1:std::max(1,std::abs(signed_seed));
    std::array<int,32> table{};
    const auto step=[&](){const int k=state/127773;state=16807*(state-k*127773)-2836*k;if(state<0)state+=2147483647;};
    for(int j=39;j>=0;--j){step();if(j<32)table[j]=state;}
    const int iy=table[0];step();
    return std::min((1.f/2147483647.f)*table[iy/67108864],1.f-1.2e-7f);
}
int main(int argc,char** argv) {
    const wchar_t* bin=L"C:\\Program Files (x86)\\Steam\\steamapps\\common\\Left 4 Dead 2\\bin";
    SetDllDirectoryW(bin);
    auto dll=LoadLibraryW(L"C:\\Program Files (x86)\\Steam\\steamapps\\common\\Left 4 Dead 2\\bin\\vstdlib.dll");
    if(!dll){std::printf("vstdlib load failed %lu\n",GetLastError());return 1;}
    using Construct=void(__thiscall*)(void*);using Set=void(__thiscall*)(void*,int);using Float=float(__thiscall*)(void*,float,float);
    const auto ctor=reinterpret_cast<Construct>(GetProcAddress(dll,"??0CUniformRandomStream@@QAE@XZ"));
    const auto set=reinterpret_cast<Set>(GetProcAddress(dll,"?SetSeed@CUniformRandomStream@@UAEXH@Z"));
    const auto next=reinterpret_cast<Float>(GetProcAddress(dll,"?RandomFloat@CUniformRandomStream@@UAEMMM@Z"));
    if(!ctor||!set||!next)return 2;
    alignas(16) std::array<unsigned char,512> stream{};ctor(stream.data());
    int verified=0;
    for(uint32_t seed: {0u,1u,494641349u,0xffffffffu,0x80000000u,123456u})
        for(int i=1;i<=32;++i){const auto hashed=crc(seed,i);set(stream.data(),static_cast<int>(hashed));const float a=next(stream.data(),-1.f,1.f),b=uniform(hashed)*2.f-1.f;if(std::abs(a-b)>1e-6f){std::printf("mismatch %u %d %.9g %.9g\n",seed,i,a,b);return 3;}++verified;}
    std::printf("Native RNG matches: %d\n",verified);std::fflush(stdout);
    for(const auto& c:shotgun_pattern::Candidates){
        float radius{};
        for(int i=2;i<=11;++i){set(stream.data(),static_cast<int>(crc(c.seed,i)));radius=std::max(radius,std::abs(next(stream.data(),-1.f,1.f)));if(i>=8){if(std::abs(radius-c.radius[i-8])>1e-6f)return 5;++verified;}}
    }
    std::printf("Native RNG and full preset checks: %d\n",verified);std::fflush(stdout);
    BCRYPT_ALG_HANDLE algorithm{};BCRYPT_HASH_HANDLE hash{};
    if(BCryptOpenAlgorithmProvider(&algorithm,BCRYPT_MD5_ALGORITHM,nullptr,0)<0 || BCryptCreateHash(algorithm,&hash,nullptr,0,nullptr,0,BCRYPT_HASH_REUSABLE_FLAG)<0)return 4;
    const int limit=argc>1?std::atoi(argv[1]):1000000;
    std::array<float,4> best{1.f,1.f,1.f,1.f};std::array<uint32_t,12> salt{};
    for(int i=2;i<=11;++i)salt[i]=crc(0,i)^crc(0,0);
    for(int command=1;command<=limit;++command){
        unsigned char digest[16]{};uint32_t seed{};
        BCryptHashData(hash,reinterpret_cast<PUCHAR>(&command),4,0);BCryptFinishHash(hash,digest,16,0);std::memcpy(&seed,digest+6,4);seed&=0x7fffffff;
        const auto zero=crc(seed,0);float radius=0.f;
        for(int i=2;i<=11;++i){
            radius=std::max(radius,std::abs(uniform(zero^salt[i])*2.f-1.f));
            if(i>=8&&radius<best[i-8]){best[i-8]=radius;std::printf("record pellets=%d command=%d seed=%u max_radius=%.9g\n",i,command,seed,radius);std::fflush(stdout);}
            if(i<8&&radius>=*std::max_element(best.begin(),best.end()))break;
        }
    }
    BCryptDestroyHash(hash);BCryptCloseAlgorithmProvider(algorithm,0);
    std::printf("Searched %d positive commands\n",limit);return 0;
}
