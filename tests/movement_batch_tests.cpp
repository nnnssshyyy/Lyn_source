#include "movement_batch.h"
#include <cstdio>
#include <limits>
#include <vector>

int main() {
    unsigned checks{},failures{};
    const auto expect=[&](bool ok,const char* name) {++checks;if(!ok) {++failures;std::fprintf(stderr,"FAIL: %s\n",name);}};
    movement_batch::SpeedBudget budget;
    expect(budget.Next(3.f,false,1)==0,"disabled speedhack never generates extra ticks");
    expect(budget.Next(1.f,true,1)==0,"1x keeps normal command rate");
    int total=0;for(int i=0;i<100;++i) total+=1+budget.Next(3.f,true,1);
    expect(total==300,"3x generates exactly three player commands per original tick");
    budget.Reset();total=0;for(int i=0;i<8;++i) total+=1+budget.Next(2.5f,true,1);
    expect(total==20,"fractional multipliers preserve their average command rate");
    budget.Reset();budget.Next(1.5f,true,1);budget.Next(3.f,false,1);
    expect(budget.Next(1.5f,true,1)==0,"releasing the key discards fractional credit");
    budget.Reset();budget.Next(1.5f,true,1);
    expect(budget.Next(1.5f,true,0x1001)==0,"respawned entity handles cannot inherit speed credit");
    expect(budget.Next(3.f,true,0)==0 && budget.owner==0,"missing local player clears the speed budget");
    expect(budget.Next(std::numeric_limits<float>::quiet_NaN(),true,1)==0 &&
           budget.Next(std::numeric_limits<float>::infinity(),true,1)==0,"invalid multipliers generate no commands");
    expect(budget.Next(1000.f,true,1)==14,"speed multiplier is bounded to the 15-command limit");
    budget.Reset();expect(budget.Next(15.f,true,1)==14,"15x generates fourteen extra player commands");
    budget.Reset();total=0;for(int i=0;i<8;++i) total+=1+budget.Next(14.5f,true,1);
    expect(total==116,"high fractional multipliers stay accurate at the wire limit");
    expect(movement_batch::Combined(7,14)==14 && movement_batch::Combined(2,3)==3,
           "speed and rapidfire share commands instead of multiplying their budgets");
    expect(movement_batch::Combined(100,100)==100,"rapidfire retains its native movement replay count above fourteen");
    expect(movement_batch::Combined(100,0)==14&&movement_batch::Combined(7,-1)==7,
           "speedhack alone retains its fourteen-extra limit");
    expect(movement_batch::Combined(2,800)==800&&movement_batch::Combined(100,801)==800&&
           movement_batch::Combined(-1,-1)==0,"rapidfire reaches the configured ceiling without adding speedhack extras");
    expect(!movement_batch::RapidTick(1,0) && movement_batch::RapidTick(1,2) && !movement_batch::RapidTick(3,2),
           "movement-only extras cannot force rapidfire attacks");
    std::vector<bool> finals;std::vector<int> indices;
    movement_batch::RunExtras(3,true,[&](int index,bool final) {indices.push_back(index);finals.push_back(final);return true;});
    expect(indices==std::vector<int>({1,2,3}) && finals==std::vector<bool>({false,false,true}),
           "extra ticks have distinct indices and flush only on the final tick");
    finals.clear();
    movement_batch::RunExtras(3,false,[&](int,bool final) {finals.push_back(final);return true;});
    expect(finals==std::vector<bool>({false,false,false}),"non-final engine ticks never force a packet flush");
    int calls=0;
    movement_batch::RunExtras(7,true,[&](int,bool) {++calls;return false;});
    expect(calls==1,"loss of focus can cancel the remainder of a batch");
    calls=0;movement_batch::RunExtras(100,true,[&](int,bool) {++calls;return true;});
    expect(calls==14,"extra tick executor independently enforces the batch cap");
    calls=0;movement_batch::RunExtras(-1,true,[&](int,bool) {++calls;return true;});
    expect(calls==0,"unarmed batches execute no extra ticks");
    std::printf("%u movement batch checks, %u failed\n",checks,failures);
    return failures?1:0;
}
