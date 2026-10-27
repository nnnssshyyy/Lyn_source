#pragma once
#include <algorithm>
#include <cstdint>
#include <deque>
#include <string>
#include <string_view>
#include <vector>

namespace chat_spy::logic {
inline constexpr size_t HistoryLimit=128,NormalLimit=250,TextLimit=511,NameLimit=95;
inline constexpr std::uint64_t GraceMs=150,MatchMs=1500;
inline bool Space(unsigned char c) { return c==' ' || (c>='\t' && c<='\r'); }
inline std::string Clean(std::string_view value,size_t limit=TextLimit) {
    std::string out;out.reserve(std::min(value.size(),limit));
    for(unsigned char c:value) {
        if(!c) break;
        if(c>=1 && c<=7) continue; // Source HUD colors, never UTF-8 bytes.
        if(c<32 || c==127) {if(Space(c)) c=' ';else continue;}
        if(out.size()==limit) break;
        out.push_back(static_cast<char>(c));
    }
    // Do not leave the trailing half of a UTF-8 character after truncation.
    if(!out.empty()) {
        size_t first=out.size()-1;
        while(first && (static_cast<unsigned char>(out[first])&0xc0)==0x80) --first;
        const auto lead=static_cast<unsigned char>(out[first]);
        const size_t bytes=lead<0x80?1:(lead&0xe0)==0xc0?2:(lead&0xf0)==0xe0?3:(lead&0xf8)==0xf0?4:1;
        if(out.size()-first<bytes) out.resize(first);
    }
    while(!out.empty() && Space(static_cast<unsigned char>(out.back()))) out.pop_back();
    const auto start=out.find_first_not_of(" \t\r\n");
    if(start==std::string::npos) return {};
    return start?out.substr(start):out;
}
inline std::string Normalize(std::string_view value) {
    const auto clean=Clean(value);std::string out;bool gap=false;
    for(unsigned char c:clean) {
        if(Space(c)) {gap=!out.empty();continue;}
        if(gap) out+=' ';
        gap=false;
        out+=static_cast<char>(c>='A' && c<='Z'?c+('a'-'A'):c);
    }
    return out;
}
struct Pair {std::string name,text;};
inline Pair SplitHud(std::string_view line,std::string_view known_name={}) {
    const auto clean=Clean(line,2047),name=Clean(known_name,NameLimit);
    if(!name.empty()) {
        // A name may itself contain " : ". Match the complete known name.
        size_t pos=0;
        while((pos=clean.find(name,pos))!=std::string::npos) {
            const size_t end=pos+name.size();
            for(const std::string_view separator:{std::string_view(" : "),std::string_view(": ")})
                if(clean.compare(end,separator.size(),separator)==0)
                    return {name,Clean(std::string_view(clean).substr(end+separator.size()))};
            ++pos;
        }
        return {}; // No guessed split when an engine identity is available.
    }
    const auto pos=clean.find(" : ");
    if(pos==std::string::npos) return {};
    return {Clean(std::string_view(clean).substr(0,pos),NameLimit),Clean(std::string_view(clean).substr(pos+3))};
}
struct Message {
    int userid{},team{};std::uint64_t time{},serial{};
    std::string name,text;
};
struct Normal {int userid{};std::uint64_t time{};std::string name,text;};
class History {
    std::deque<Message> waiting_,messages_;
    std::deque<Normal> normal_;
    std::uint64_t serial_{},clear_started_{};
    int clear_minutes_{};
    bool Match(const Message& m) {
        const auto name=Normalize(m.name),text=Normalize(m.text);
        const auto it=std::find_if(normal_.begin(),normal_.end(),[&](const Normal& n) {
            const auto distance=m.time>=n.time?m.time-n.time:n.time-m.time;
            return distance<=MatchMs && (n.userid?m.userid==n.userid:name==n.name) && text==n.text;
        });
        if(it==normal_.end()) return false;
        normal_.erase(it);return true; // One HUD message consumes one event.
    }
public:
    unsigned received{},suppressed{},dropped{};
    const std::deque<Message>& Messages() const {return messages_;}
    size_t NormalCount() const {return normal_.size();}
    void Clear() {waiting_.clear();messages_.clear();normal_.clear();clear_started_=0;clear_minutes_=0;}
    void Reset() {Clear();received=suppressed=dropped=0;}
    void Remember(int userid,std::string_view name,std::string_view text,std::uint64_t now) {
        const auto n=Normalize(name),t=Normalize(text);
        if(n.empty() || t.empty()) return;
        normal_.push_back({userid,now,n,t});
        while(normal_.size()>NormalLimit) normal_.pop_front();
    }
    void Receive(int userid,int team,std::string_view name,std::string_view text,std::uint64_t now) {
        auto n=Clean(name,NameLimit),t=Clean(text);
        if(userid<=0 || (team!=2 && team!=3) || n.empty() || t.empty()) return;
        ++received;
        if(waiting_.size()==HistoryLimit) {waiting_.pop_front();++dropped;}
        waiting_.push_back({userid,team,now,++serial_,std::move(n),std::move(t)});
    }
    std::vector<Message> Advance(std::uint64_t now,bool deduplicate,int clear_minutes) {
        clear_minutes=std::clamp(clear_minutes,0,240);
        if(clear_minutes!=clear_minutes_ || !clear_started_) {clear_minutes_=clear_minutes;clear_started_=now;}
        if(clear_minutes && now>=clear_started_ && now-clear_started_>=std::uint64_t(clear_minutes)*60000) {
            Clear();clear_started_=now;clear_minutes_=clear_minutes;
        }
        std::vector<Message> ready;
        while(!waiting_.empty()) {
            const auto& front=waiting_.front();
            if(now<front.time || now-front.time<GraceMs) break;
            auto message=std::move(waiting_.front());waiting_.pop_front();
            if(deduplicate && Match(message)) {++suppressed;continue;}
            ready.push_back(message);messages_.push_back(std::move(message));
            if(messages_.size()>HistoryLimit) messages_.pop_front();
        }
        std::erase_if(normal_,[now](const Normal& n){return now>=n.time && now-n.time>MatchMs+GraceMs;});
        return ready;
    }
};
}
