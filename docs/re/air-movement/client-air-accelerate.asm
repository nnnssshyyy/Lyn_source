100FDD00: push ebp
100FDD01: mov ebp, esp
100FDD03: movss xmm0, dword ptr [ebp + 0xc]
100FDD08: push esi
100FDD09: mov esi, dword ptr [ecx + 4]
100FDD0C: cmp byte ptr [esi + 0x1380], 0
100FDD13: movaps xmm1, xmm0
100FDD16: jne 0x100fde09
100FDD1C: movss xmm2, dword ptr [esi + 0x13b8]
100FDD24: xorps xmm3, xmm3
100FDD27: ucomiss xmm2, xmm3
100FDD2A: lahf 
100FDD2B: test ah, 0x44
100FDD2E: jp 0x100fde09
100FDD34: movss xmm2, dword ptr [0x1054c188]
100FDD3C: comiss xmm0, xmm2
100FDD3F: jbe 0x100fdd44
100FDD41: movaps xmm1, xmm2
100FDD44: mov eax, dword ptr [ecx + 8]
100FDD47: mov edx, dword ptr [ebp + 8]
100FDD4A: movss xmm2, dword ptr [eax + 0x44]
100FDD4F: mulss xmm2, dword ptr [edx + 4]
100FDD54: movss xmm4, dword ptr [edx]
100FDD58: mulss xmm4, dword ptr [eax + 0x40]
100FDD5D: addss xmm2, xmm4
100FDD61: movss xmm4, dword ptr [eax + 0x48]
100FDD66: mulss xmm4, dword ptr [edx + 8]
100FDD6B: addss xmm2, xmm4
100FDD6F: subss xmm1, xmm2
100FDD73: comiss xmm3, xmm1
100FDD76: jae 0x100fde09
100FDD7C: mulss xmm0, dword ptr [ebp + 0x10]
100FDD81: mov eax, dword ptr [0x106c49a8]
100FDD86: mulss xmm0, dword ptr [eax + 0x10]
100FDD8B: mulss xmm0, dword ptr [esi + 0x13c4]
100FDD93: comiss xmm0, xmm1
100FDD96: jbe 0x100fdd9b
100FDD98: movaps xmm0, xmm1
100FDD9B: mov eax, dword ptr [ecx + 8]
100FDD9E: movss xmm1, dword ptr [edx]
100FDDA2: mulss xmm1, xmm0
100FDDA6: addss xmm1, dword ptr [eax + 0x40]
100FDDAB: movss dword ptr [eax + 0x40], xmm1
100FDDB0: movss xmm1, dword ptr [edx]
100FDDB4: mulss xmm1, xmm0
100FDDB8: addss xmm1, dword ptr [eax + 0x68]
100FDDBD: movss dword ptr [eax + 0x68], xmm1
100FDDC2: movss xmm1, dword ptr [edx + 4]
100FDDC7: mulss xmm1, xmm0
100FDDCB: addss xmm1, dword ptr [eax + 0x44]
100FDDD0: movss dword ptr [eax + 0x44], xmm1
100FDDD5: movss xmm1, dword ptr [edx + 4]
100FDDDA: mulss xmm1, xmm0
100FDDDE: addss xmm1, dword ptr [eax + 0x6c]
100FDDE3: movss dword ptr [eax + 0x6c], xmm1
100FDDE8: movaps xmm1, xmm0
100FDDEB: mulss xmm1, dword ptr [edx + 8]
100FDDF0: addss xmm1, dword ptr [eax + 0x48]
100FDDF5: movss dword ptr [eax + 0x48], xmm1
100FDDFA: mulss xmm0, dword ptr [edx + 8]
100FDDFF: addss xmm0, dword ptr [eax + 0x70]
100FDE04: movss dword ptr [eax + 0x70], xmm0
100FDE09: pop esi
100FDE0A: pop ebp
100FDE0B: ret 0xc
