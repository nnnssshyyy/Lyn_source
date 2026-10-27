#include "player_name.h"
#include "esp_font.h"
#include <imgui_internal.h>
#include <cstdio>
#include <cstring>
#include <string>

int main(int argc,char** argv) {
    unsigned checks=0,failed=0;
    const auto expect=[&](bool pass,const char* text) {
        ++checks;if(!pass) {++failed;std::fprintf(stderr,"FAIL: %s\n",text);}
    };
    char name[64]{};
    player_name::Copy(name,"♡ 𝓐lex ★ ⠿ Ω Ж");
    expect(std::strcmp(name,"♡ 𝓐lex ★ ⠿ Ω Ж")==0,"styled letters, symbols and scripts are preserved");
    char tiny[5]{};
    player_name::Copy(tiny,"𝓐x");expect(std::strcmp(tiny,"𝓐")==0,"four-byte scalar fits exactly before terminator");
    char smaller[4]{};
    player_name::Copy(smaller,"𝓐");expect(!smaller[0],"a four-byte scalar is never split by destination limit");
    const char partial[]{'A',char(0xf0),char(0x9d),char(0x93)};
    player_name::Copy(name,partial);expect(std::strcmp(name,"A")==0,"engine byte-boundary truncation drops only partial suffix");
    player_name::Copy(name,"A\nB\tC");expect(std::strcmp(name,"A B C")==0,"controls cannot create multiple ESP rows");
    const char malformed[]{'a',char(0xff),'b',0};
    player_name::Copy(name,malformed);expect(std::strcmp(name,"a\xef\xbf\xbd" "b")==0,"malformed byte does not consume the next valid letter");
    const char overlong[]{char(0xc0),char(0xaf),0};player_name::Copy(name,overlong);
    expect(std::strcmp(name,"\xef\xbf\xbd\xef\xbf\xbd")==0,"overlong encoding is rejected");
    player_name::Copy(name,"é");expect(std::strcmp(name,"é")==0,"combining accents remain intact");
    expect(player_name::Copy(nullptr,0,"a",1)==0,"zero capacity is safe");
    ImGui::CreateContext();auto& io=ImGui::GetIO();io.IniFilename=nullptr;
    expect(sizeof(ImWchar)==4,"font atlas supports supplementary Unicode planes");
    expect(esp::font::Initialize() && io.Fonts->Build(),"production ESP font atlas builds");
    for(int size=esp::font::Minimum;size<=esp::font::Maximum;++size) {
        auto* font=esp::font::family.sizes[size-esp::font::Minimum];
        for(const ImWchar code:{0x00e9u,0x03a9u,0x0416u,0x2661u,0x2605u,0x283fu,0x30c4u,0x1d4d0u,0x1f600u})
            expect(font && font->FindGlyphNoFallback(code),"every native ESP size contains real name glyphs");
        const auto dimensions=font->CalcTextSizeA(static_cast<float>(size),10000.f,0.f,"♡ 𝓐lex ★ ⠿ Ω Ж");
        expect(dimensions.x>size*5 && dimensions.y>0,"Unicode labels have measurable layout bounds");
    }
    expect(io.Fonts->TexWidth<=4096 && io.Fonts->TexHeight<=4096,"name atlas stays within a 4096-square DX9 texture");
    std::printf("Atlas: %d x %d\n",io.Fonts->TexWidth,io.Fonts->TexHeight);
    // An optional image of the actual baked glyphs for visual inspection.
    if(argc>1) {
        const int width=820,height=340;std::string rgb(width*height*3,char(22));
        auto* font=esp::font::family.sizes[8];float pen=24;int line=24;
        for(const auto* sample:{"ESP Unicode / 18 px","♡ 𝓐lex ★ ⠿ Ω Ж","𝕯𝖊𝖛𝖎𝖑  ♛  ☠  ツ","Ångel  François  Игрок  Ωmega","☯  ♫  ✿  ★  →  ∞  ⣿  😀"}) {
            pen=24;
            for(const char* cursor=sample;*cursor;) {
                unsigned int code{};const int consumed=ImTextCharFromUtf8(&code,cursor,nullptr);if(!consumed)break;cursor+=consumed;
                const auto* glyph=font->FindGlyph(code);if(!glyph)continue;
                const int sx=static_cast<int>(glyph->U0*io.Fonts->TexWidth+.5f),sy=static_cast<int>(glyph->V0*io.Fonts->TexHeight+.5f);
                const int w=static_cast<int>((glyph->U1-glyph->U0)*io.Fonts->TexWidth+.5f),h=static_cast<int>((glyph->V1-glyph->V0)*io.Fonts->TexHeight+.5f);
                for(int y=0;y<h;++y)for(int x=0;x<w;++x) {
                    const int dx=static_cast<int>(pen+glyph->X0)+x,dy=line+static_cast<int>(glyph->Y0)+y;
                    if(dx<0||dy<0||dx>=width||dy>=height)continue;
                    const auto a=io.Fonts->TexPixelsAlpha8[(sy+y)*io.Fonts->TexWidth+sx+x];
                    for(int c=0;c<3;++c)rgb[(dy*width+dx)*3+c]=static_cast<char>(22+(230-22)*a/255);
                }
                pen+=glyph->AdvanceX;
            }
            line+=55;
        }
        FILE* file{};fopen_s(&file,argv[1],"wb");
        if(file){std::fprintf(file,"P6\n%d %d\n255\n",width,height);std::fwrite(rgb.data(),1,rgb.size(),file);std::fclose(file);}
    }
    ImGui::DestroyContext();esp::font::family={};
    std::printf("%u player-name/font checks, %u failed\n",checks,failed);return failed?1:0;
}
