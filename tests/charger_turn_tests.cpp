#include "charger_turn.h"
#include <array>
#include <cstdio>
#include <cstring>
int main(){unsigned checks{},failed{};const auto expect=[&](bool ok,const char* text){++checks;if(!ok){++failed;std::printf("FAIL: %s\n",text);}};
    using charger_turn::Eligible;
    expect(Eligible(true,true,true,true,3,true,true,false),"only a live controlled Charger can enable the turn patch");
    expect(!Eligible(false,true,true,true,3,true,true,false)&&!Eligible(true,false,true,true,3,true,true,false),"disabled option and inactive runtime restore normal behavior");
    expect(!Eligible(true,true,false,true,3,true,true,false)&&!Eligible(true,true,true,false,3,true,true,false),"outside a game or missing local entity never arms");
    expect(!Eligible(true,true,true,true,2,true,true,false)&&!Eligible(true,true,true,true,3,false,true,false),"survivors and other infected never arm");
    expect(!Eligible(true,true,true,true,3,true,false,false)&&!Eligible(true,true,true,true,3,true,true,true),"dead and ghost Chargers never arm");
    auto* page=static_cast<unsigned char*>(VirtualAlloc(nullptr,4096,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE));
    expect(page!=nullptr,"isolated native code page allocates");if(!page)return 1;
    std::array<unsigned char,80> code{};
    const unsigned char head[]{0xf6,0x86,0xf0,0,0,0,0x20,0x74,0x3f,0x80,0xbe,0xc8,0x14,0,0,0,0x74,0x2d,0xd9,0x86,0xbc,0x14,0,0,0x8d,0x47};
    std::memcpy(code.data(),head,sizeof(head));const unsigned char mark[]{0xc6,0x86,0xc8,0x14,0,0,1};std::memcpy(code.data()+63,mark,sizeof(mark));
    std::memcpy(page,code.data(),code.size());DWORD protection{};VirtualProtect(page,4096,PAGE_EXECUTE_READ,&protection);
    memory::Range image{reinterpret_cast<uintptr_t>(page),4096};charger_turn::Patch patch;
    expect(patch.Bind(image,page)&&patch.Available(),"validated block binds without any absolute game RVA");
    expect(patch.Set(true)&&patch.Active()&&page[16]==0xeb&&page[17]==0x2d,"enable changes only the opcode and preserves the branch destination");
    MEMORY_BASIC_INFORMATION info{};VirtualQuery(page,&info,sizeof(info));expect(info.Protect==PAGE_EXECUTE_READ,"native write restores executable page protection");
    expect(patch.Set(true)&&page[16]==0xeb,"repeated enabled frames do not reapply a patch");
    expect(patch.Set(false)&&!patch.Active()&&page[16]==0x74,"disable restores the original conditional jump");
    expect(!std::memcmp(page,code.data(),code.size()),"all surrounding instructions are unchanged after a cycle");
    charger_turn::Patch invalid;expect(!invalid.Bind({image.base,18},page),"truncated image cannot bind a partial pattern");
    VirtualProtect(page,4096,PAGE_READWRITE,&protection);page[63]=0x90;
    expect(!invalid.Bind(image,page),"invalid branch destination cannot bind");page[63]=0xc6;page[11]=0xc9;
    expect(!invalid.Bind(image,page),"inconsistent cached-angle and frozen-state fields cannot bind");page[11]=0xc8;
    expect(patch.Set(true),"patch can enable again after a clean restore");
    page[16]=0x90;expect(!patch.Set(false)&&page[16]==0x90&&!patch.Available(),"cleanup preserves an external opcode change and disables future writes");
    page[16]=0x74;expect(patch.Bind(image,page)&&patch.Set(true),"an explicit validated rebind recovers from a conflict");
    page[17]=0x30;expect(!patch.Set(false)&&page[17]==0x30,"cleanup also preserves an external displacement change");
    VirtualFree(page,0,MEM_RELEASE);
    std::printf("%u charger turn checks, %u failed\n",checks,failed);return failed?1:0;
}
