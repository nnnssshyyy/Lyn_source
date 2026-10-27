#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace player_name {
// Engine names have a byte limit. Preserve complete Unicode scalars, never a
// partial multibyte suffix, and keep control characters out of one-line labels.
inline size_t Copy(char* out,size_t capacity,const char* input,size_t bytes) noexcept {
    if(!out || !capacity) return 0;
    size_t written=0;
    if(input) for(size_t i=0;i<bytes && input[i];) {
        const auto first=static_cast<unsigned char>(input[i]);
        size_t count=first<0x80?1:first>=0xc2 && first<=0xdf?2:
            first>=0xe0 && first<=0xef?3:first>=0xf0 && first<=0xf4?4:0;
        std::uint32_t code=count==1?first:count?first&((1u<<(7-count))-1):0;
        bool valid=count!=0,incomplete=false;
        for(size_t j=1;valid && j<count;++j) {
            if(i+j>=bytes || !input[i+j]) {incomplete=true;valid=false;break;}
            const auto next=static_cast<unsigned char>(input[i+j]);
            if((next&0xc0)!=0x80) {valid=false;break;}
            code=(code<<6)|(next&0x3f);
        }
        if(incomplete) break;
        valid=valid && (count<2 || code>=0x80) && (count<3 || code>=0x800) &&
            (count<4 || code>=0x10000) && code<=0x10ffff && !(code>=0xd800 && code<=0xdfff);
        const char* value=input+i;size_t length=count;
        if(!valid) {value="\xef\xbf\xbd";length=3;count=1;}
        else if(code<0x20 || (code>=0x7f && code<=0x9f)) {value=" ";length=1;}
        if(written+length>=capacity) break;
        std::memcpy(out+written,value,length);written+=length;i+=count;
    }
    out[written]=0;return written;
}
template<size_t N,size_t M> inline size_t Copy(char (&out)[N],const char (&input)[M]) noexcept {
    return Copy(out,N,input,M);
}
}
