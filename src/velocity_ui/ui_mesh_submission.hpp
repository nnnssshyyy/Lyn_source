#pragma once
#include "xdraw.hpp"
#include <imgui.h>
#include <cstring>

namespace xdraw::detail {
// Bounded fallback for unusually large commands. DX9 supports ImGui VtxOffset,
// allowing each reservation to remain representable by 16-bit indices.
inline void SubmitExpanded(const draw_list& source,const draw_cmd& command,ImDrawList& destination) {
    for(std::uint32_t offset=0;offset<command.idx_count;) {
        const auto count=std::min<std::uint32_t>(command.idx_count-offset,30000);
        destination.PrimReserve(static_cast<int>(count),static_cast<int>(count));
        const auto base=destination._VtxCurrentIdx;
        for(std::uint32_t k=0;k<count;++k) {
            const auto& v=source.vertices[source.indices[command.idx_offset+offset+k]];
            destination.PrimWriteIdx(static_cast<ImDrawIdx>(base+k));
            destination.PrimWriteVtx({v.pos[0],v.pos[1]},{v.uv[0],v.uv[1]},v.col.val);
        }
        offset+=count;
    }
}
inline void SubmitIndexed(const draw_list& source,size_t command_index,ImDrawList& destination) {
    const auto& command=source.commands[command_index];
    const auto begin=command.vtx_offset;
    const auto end=command_index+1<source.commands.size()?source.commands[command_index+1].vtx_offset:
        static_cast<std::uint32_t>(source.vertices.size());
    const auto count=end-begin;
    if(!count||count>60000||count>command.idx_count) {SubmitExpanded(source,command,destination);return;}
    // XUI emits every command's vertices contiguously, before the next command.
    // Preserve that indexed mesh instead of copying a vertex per triangle index.
    static_assert(sizeof(vertex)==sizeof(ImDrawVert));
    static_assert(offsetof(vertex,pos)==offsetof(ImDrawVert,pos));
    static_assert(offsetof(vertex,uv)==offsetof(ImDrawVert,uv));
    static_assert(offsetof(vertex,col)==offsetof(ImDrawVert,col));
    destination.PrimReserve(static_cast<int>(command.idx_count),static_cast<int>(count));
    const auto base=destination._VtxCurrentIdx;
    std::memcpy(destination._VtxWritePtr,source.vertices.data()+begin,count*sizeof(vertex));
    destination._VtxWritePtr+=count;destination._VtxCurrentIdx+=count;
    for(std::uint32_t i=0;i<command.idx_count;++i) {
        const auto index=source.indices[command.idx_offset+i];
        IM_ASSERT(index>=begin&&index<end);
        destination.PrimWriteIdx(static_cast<ImDrawIdx>(base+index-begin));
    }
}
}
