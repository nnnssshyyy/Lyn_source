#include "chat_logic.h"
#include <cstdio>

int main() {
    using namespace chat_spy::logic;
    unsigned checks{},failures{};
    const auto expect=[&](bool ok,const char* message) {++checks;if(!ok){++failures;std::fprintf(stderr,"FAIL: %s\n",message);}};
    expect(Clean("\x03" "Player\x01 : hello\nworld") == "Player : hello world","HUD colors and newlines cannot inject extra lines");
    expect(Normalize("  PLAYER\t  says  HELLO \n")=="player says hello","ASCII case and whitespace normalization");
    const std::string unicode="José Привет 你好";
    expect(Clean(unicode)==unicode && Normalize(unicode)=="josé Привет 你好","UTF-8 bytes are preserved without locale-dependent lowercasing");
    expect(Clean("abé",3)=="ab" && Clean("abé",4)=="abé","truncation never leaves half of a UTF-8 character");
    auto pair=SplitHud("\x03" "(TEAM) A : B\x01 : message", "A : B");
    expect(pair.name=="A : B" && pair.text=="message","known player name may itself contain the separator");
    pair=SplitHud("(TEAM) Alice: hi", "Alice");
    expect(pair.name=="Alice" && pair.text=="hi","HUD prefix and compact colon format");
    expect(SplitHud("Console : hi","Alice").name.empty(),"known identities do not fall back to a guessed name");
    expect(SplitHud("Alice : hi").text=="hi" && SplitHud("server notice").name.empty(),"unknown-index HUD messages require a real separator");

    History h;
    h.Receive(10,3,"Alice","Hello",1000);
    expect(h.Advance(1149,true,0).empty() && h.Messages().empty(),"wait for HUD delivery before deciding whether an event is duplicate");
    h.Remember(10,"Alice"," hello ",1100);
    expect(h.Advance(1150,true,0).empty() && h.suppressed==1,"HUD arriving after the event suppresses the extra output");
    h.Receive(10,3,"Alice","hello",1200);
    expect(h.Advance(1350,true,0).size()==1,"one normal HUD message suppresses only one event, not repeated messages forever");
    h.Reset();h.Remember(10,"Alice","hello",1000);h.Receive(10,3,"Alice","hello",1010);
    expect(h.Advance(1160,true,0).empty() && h.suppressed==1,"HUD arriving before the event is also recognized");
    h.Reset();h.Remember(10,"Same name","hello",1000);h.Receive(11,3,"Same name","hello",1010);
    expect(h.Advance(1160,true,0).size()==1,"different userids with identical names and text stay independent");
    h.Reset();h.Remember(0,"ALICE","hello",1000);h.Receive(10,3,"Alice","hello",1010);
    expect(h.Advance(1160,true,0).empty(),"unknown-index HUD messages can match the normalized name");
    h.Reset();h.Remember(10,"Alice","hello",1000);h.Receive(10,3,"Alice","hello",2600);
    expect(h.Advance(2750,true,0).size()==1 && h.NormalCount()==0,"expired HUD text cannot suppress a later repetition");
    h.Reset();h.Remember(10,"Alice","hello",1000);h.Receive(10,3,"Alice","hello",1010);
    expect(h.Advance(1160,false,0).size()==1 && h.suppressed==0,"duplicate filter is optional");
    h.Reset();h.Receive(0,3,"Alice","hi",1000);h.Receive(1,1,"Alice","hi",1000);h.Receive(1,3,"","hi",1000);
    expect(h.Advance(1200,true,0).empty() && !h.received,"invalid identity, spectator team and empty names are ignored");
    h.Receive(1,2,"Alice","\x01\x03",1000);
    expect(!h.received,"control-only messages are ignored");
    for(int i=1;i<=300;++i) h.Remember(i,"Player","hello",1000);
    expect(h.NormalCount()==NormalLimit,"normal HUD cache never exceeds 250 entries");
    h.Reset();for(int i=1;i<=300;++i) h.Receive(i,3,"Player",std::to_string(i),1000);
    expect(h.dropped==300-HistoryLimit,"pending event queue is bounded during a message flood");
    h.Advance(1200,true,0);
    expect(h.Messages().size()==HistoryLimit && h.Messages().front().userid==173,"history keeps the most recent 128 records");
    for(int i=301;i<=350;++i) h.Receive(i,3,"Player",std::to_string(i),1300);
    h.Advance(1500,true,0);
    expect(h.Messages().size()==HistoryLimit && h.Messages().back().userid==350,"delivered history stays bounded across batches");
    h.Reset();h.Receive(10,3,"Alice","keep",1000);h.Advance(1200,true,1);
    expect(h.Messages().size()==1,"timed clearing starts independently of panel visibility");
    h.Advance(61200,true,1);
    expect(h.Messages().empty(),"periodic clear also works when only the HUD output is used");
    h.Reset();h.Receive(10,3,"Alice","keep",1000);h.Advance(1200,true,0);h.Advance(1000000,true,0);
    expect(h.Messages().size()==1,"zero interval keeps bounded history until explicitly cleared");
    h.Receive(10,3,"Alice","pending",1000001);h.Remember(10,"Alice","normal",1000001);h.Clear();
    expect(h.Messages().empty() && h.NormalCount()==0 && h.Advance(1000200,true,0).empty(),"manual clear removes pending events and duplicate cache together");
    h.Reset();expect(h.received==0 && h.suppressed==0 && h.dropped==0,"level reset clears per-session counters");
    std::printf("%u chat logic checks, %u failed\n",checks,failures);
    return failures?1:0;
}
