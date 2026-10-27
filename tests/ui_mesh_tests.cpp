#include "velocity_ui/ui_mesh_submission.hpp"
#include <imgui_internal.h>
#include <chrono>
#include <cstdio>

namespace {
unsigned checks{},failures{};
void Expect(bool ok,const char* label) {++checks;if(!ok){++failures;std::fprintf(stderr,"FAIL: %s\n",label);}}
void Command(xdraw::draw_list& mesh,unsigned quads) {
    xdraw::draw_cmd command{};command.idx_offset=static_cast<unsigned>(mesh.indices.size());
    command.vtx_offset=static_cast<unsigned>(mesh.vertices.size());command.idx_count=quads*6;
    mesh.commands.push_back(command);
    for(unsigned i=0;i<quads;++i) {
        const auto base=static_cast<unsigned>(mesh.vertices.size());
        for(unsigned j=0;j<4;++j)mesh.vertices.push_back({{float(i%128)+float(j&1),float(i/128)+float(j>>1)},
            {float(j&1),float(j>>1)},{static_cast<std::uint8_t>(i),static_cast<std::uint8_t>(j),120,180}});
        for(unsigned index:{0,1,2,0,2,3})mesh.indices.push_back(base+index);
    }
}
void Reset(ImDrawList& out) {out._ResetForNewFrame();out.PushClipRect({0,0},{1280,800});}
void Submit(const xdraw::draw_list& source,ImDrawList& out,bool indexed) {
    Reset(out);
    for(size_t i=0;i<source.commands.size();++i) {
        if(indexed)xdraw::detail::SubmitIndexed(source,i,out);
        else xdraw::detail::SubmitExpanded(source,source.commands[i],out);
    }
}
std::vector<ImDrawVert> Expand(const ImDrawList& list) {
    std::vector<ImDrawVert> vertices;
    for(const auto& command:list.CmdBuffer)for(unsigned i=0;i<command.ElemCount;++i) {
        const auto index=command.VtxOffset+list.IdxBuffer[command.IdxOffset+i];
        if(index>=static_cast<unsigned>(list.VtxBuffer.Size))return {};
        vertices.push_back(list.VtxBuffer[static_cast<int>(index)]);
    }
    return vertices;
}
bool Equivalent(const ImDrawList& a,const ImDrawList& b) {
    const auto x=Expand(a),y=Expand(b);
    return !x.empty()&&x.size()==y.size()&&std::memcmp(x.data(),y.data(),x.size()*sizeof(ImDrawVert))==0;
}
}
int main() {
    ImGui::CreateContext();auto& io=ImGui::GetIO();io.IniFilename=nullptr;io.DisplaySize={1280,800};io.DeltaTime=1.f/60.f;
    io.BackendFlags|=ImGuiBackendFlags_RendererHasVtxOffset;io.Fonts->AddFontDefault();
    unsigned char* pixels{};int w{},h{};io.Fonts->GetTexDataAsRGBA32(&pixels,&w,&h);ImGui::NewFrame();
    ImDrawList old(ImGui::GetDrawListSharedData()),indexed(ImGui::GetDrawListSharedData());
    xdraw::draw_list mesh;Command(mesh,2000);Command(mesh,4000);
    Submit(mesh,old,false);Submit(mesh,indexed,true);
    Expect(Equivalent(old,indexed),"indexed commands preserve every triangle, UV, color and order including nonzero vertex offsets");
    Expect(indexed.VtxBuffer.Size==24000&&old.VtxBuffer.Size==36000,"shared quad vertices are uploaded once instead of per index");
    Expect(indexed.IdxBuffer.Size==old.IdxBuffer.Size,"optimization preserves the original index count");
    Command(mesh,20000);Submit(mesh,old,false);Submit(mesh,indexed,true);
    Expect(Equivalent(old,indexed),"large command fallback preserves geometry across 16-bit vertex-offset boundaries");
    Expect(indexed.CmdBuffer.Size>1,"large meshes split commands without overflowing 16-bit indices");
    Command(mesh,10);Submit(mesh,old,false);Submit(mesh,indexed,true);
    Expect(Equivalent(old,indexed),"a small indexed command still works after a large fallback mesh");
    xdraw::draw_list benchmark;Command(benchmark,4000);Command(benchmark,4000);
    const auto measure=[&](bool compact) {
        const auto begin=std::chrono::steady_clock::now();
        for(int i=0;i<500;++i)Submit(benchmark,compact?indexed:old,compact);
        return std::chrono::duration<double,std::micro>(std::chrono::steady_clock::now()-begin).count()/500.;
    };
    const double expanded_us=measure(false),indexed_us=measure(true);
    Expect(Equivalent(old,indexed),"benchmark paths retain identical rendered geometry");
    std::printf("UI mesh (8000 quads): %d -> %d vertices, %.3f -> %.3f us/submission\n",
        old.VtxBuffer.Size,indexed.VtxBuffer.Size,expanded_us,indexed_us);
    ImGui::EndFrame();ImGui::DestroyContext();std::printf("%u UI mesh checks, %u failed\n",checks,failures);return failures?1:0;
}
