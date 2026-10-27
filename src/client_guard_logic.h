#pragma once
#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <vector>

namespace client_guard {
inline constexpr size_t RuleLimit=64,TextLimit=255,CommandLimit=4095;
struct Rule {std::string name,value;bool operator==(const Rule&) const=default;};
struct Rules {
    std::vector<Rule> queries,settings;
    std::vector<std::string> commands,messages;
    bool operator==(const Rules&) const=default;
};
inline char Lower(char c) noexcept {return c>='A'&&c<='Z'?static_cast<char>(c+('a'-'A')):c;}
// Bounded glob matching; no recursion and no signed-char locale conversion.
inline bool Match(std::string_view text,std::string_view pattern) noexcept {
    if(text.size()>CommandLimit||pattern.size()>TextLimit)return false;
    size_t t{},p{},star=std::string_view::npos,retry{};
    while(t<text.size()) {
        if(p<pattern.size()&&(pattern[p]=='?'||Lower(pattern[p])==Lower(text[t]))) {++p;++t;}
        else if(p<pattern.size()&&pattern[p]=='*') {star=p++;retry=t;}
        else if(star!=std::string_view::npos) {p=star+1;t=++retry;}
        else return false;
    }
    while(p<pattern.size()&&pattern[p]=='*')++p;
    return p==pattern.size();
}
inline Rules DefaultRules() {
    return {{
        {"c_thirdpersonshoulder","0"},{"mat_queue_mode","-1"},{"mat_hdr_level","2"},
        {"mat_postprocess_enable","1"},{"r_drawothermodels","1"},{"cl_drawshadowtexture","0"},
        {"mat_fullbright","0"},{"c_thirdpersonshoulderoffset","0"},{"c_thirdpersonshoulderheight","0"},
        {"cam_idealdist","0"},{"sv_cheats","0"},{"mat_wireframe","0"},{"snd_show","0"},
        {"snd_visualize","0"},{"mat_proxy","0"},{"r_drawmodelstatsoverlay","0"},
        {"r_shadowwireframe","0"},{"r_showenvcubemap","0"},{"r_drawrenderboxes","0"},
        {"r_modelwireframedecal","0"},{"sv_pure","2"},{"sv_consistency","1"},
        {"r_flashlightfov","53"},{"r_flashlightconstant","0"},{"cl_max_shadow_renderable_dist","3000"}
    },{{"sv_consistency","0"},{"sv_pure","0"}},
      {"bind","exec","cl_consistencycheck"},{"Shake","Fade","BlurFade"}};
}
inline bool ValidName(std::string_view name) noexcept {
    return !name.empty()&&name.size()<=TextLimit&&std::all_of(name.begin(),name.end(),[](unsigned char c){
        return c>32&&c<127&&c!=';'&&c!='"'&&c!='\\';
    });
}
inline bool ValidValue(std::string_view value) noexcept {
    return value.size()<=TextLimit&&std::all_of(value.begin(),value.end(),[](unsigned char c){return c>=32&&c!=127;});
}
inline bool Valid(const Rules& r) noexcept {
    for(const auto* list:{&r.queries,&r.settings}) {
        if(list->size()>RuleLimit)return false;
        for(const auto& rule:*list)if(!ValidName(rule.name)||!ValidValue(rule.value))return false;
    }
    for(const auto* list:{&r.commands,&r.messages}) {
        if(list->size()>RuleLimit)return false;
        for(const auto& name:*list)if(!ValidName(name))return false;
    }
    return true;
}
inline const Rule* Find(const std::vector<Rule>& rules,std::string_view name) noexcept {
    for(const auto& r:rules)if(Match(name,r.name))return &r;
    return nullptr;
}
inline bool Listed(const std::vector<std::string>& rules,std::string_view name) noexcept {
    for(const auto& r:rules)if(Match(name,r))return true;
    return false;
}
// Source batches can contain several commands. Inspect every command boundary,
// while preserving semicolons inside quoted arguments and // comments.
inline bool BlockCommand(std::string_view batch,const std::vector<std::string>& rules) noexcept {
    if(batch.size()>CommandLimit)return false;
    bool quote{},comment{},first=true;size_t token=std::string_view::npos;
    const auto inspect=[&](size_t end){return token!=std::string_view::npos&&Listed(rules,batch.substr(token,end-token));};
    for(size_t i=0;i<=batch.size();++i) {
        const char c=i<batch.size()?batch[i]:'\n';
        if(comment) {if(c!='\n'&&c!='\r')continue;comment=false;}
        if(!quote&&(c==';'||c=='\n'||c=='\r')) {
            if(inspect(i))return true;
            token=std::string_view::npos;first=true;continue;
        }
        if(!quote&&c=='/'&&i+1<batch.size()&&batch[i+1]=='/') {
            if(inspect(i))return true;
            token=std::string_view::npos;first=false;comment=true;continue;
        }
        if(c=='"') {
            if(first&&!quote&&token==std::string_view::npos)token=i+1;
            else if(first&&quote) {if(inspect(i))return true;token=std::string_view::npos;first=false;}
            quote=!quote;continue;
        }
        if(first&&!quote&&(c==' '||c=='\t')) {
            if(token!=std::string_view::npos) {if(inspect(i))return true;token=std::string_view::npos;first=false;}
        } else if(first&&token==std::string_view::npos)token=i;
    }
    return false;
}
inline bool NullSound(std::string_view sample) noexcept {
    const auto slash=sample.find_last_of("/\\");
    const auto base=sample.substr(slash==std::string_view::npos?0:slash+1);
    return Match(base,"null.wav")||Match(base,"null.mp3")||Match(base,"null");
}
inline bool Heartbeat(std::string_view sample) noexcept {
    return sample.size()<=CommandLimit&&Match(sample,"*heartbeatloop*");
}
}
