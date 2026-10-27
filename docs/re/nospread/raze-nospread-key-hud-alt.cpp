void sub_1015BBF0()
{
  int v0; // esi
  int v5; // edi
  __int128 v17; // [esp+0h] [ebp-64h]
  __int128 v18; // [esp+10h] [ebp-54h]
  int v19; // [esp+20h] [ebp-44h] BYREF
  int v20[2]; // [esp+24h] [ebp-40h] BYREF
  int v21; // [esp+2Ch] [ebp-38h] BYREF
  int v22[4]; // [esp+30h] [ebp-34h] BYREF
  int v23; // [esp+40h] [ebp-24h] BYREF
  __int64 v24; // [esp+44h] [ebp-20h] BYREF
  int v25; // [esp+4Ch] [ebp-18h] BYREF

  if ( byte_103F5396 == 1 /*0x1015bc52*/
    && byte_103F5AA8 == 1
    && (byte_103F4BA8 != 1
     || (*(unsigned __int8 (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 356))(a1: dword_103EC770) == 0)
    && (*(_BYTE *)off_1029E02C != 0
     || (*(unsigned __int8 (__thiscall **)(int))(*(_DWORD *)dword_103EC8A0 + 8))(a1: dword_103EC8A0) == 0) )
  {
    v0 = sub_10079720(); /*0x1015bc5d*/
    v25 = v0; /*0x1015bc5f*/
    __asm /*0x1015bc63*/
    {
      vmovsd xmm0, qword_103F5AAC
      vcvtdq2ps xmm0, xmm0
      vmovups [esp+64h+var_54], xmm0
      vmovlps [esp+64h+var_20], xmm0
      vmovss xmm0, dword ptr xmmword_103F5A3C+8
      vmovsd xmm1, qword ptr xmmword_103F5A3C
      vmovsd qword ptr [esp+64h+var_34], xmm1
      vmovss [esp+64h+var_34+8], xmm0
    }
    v22[3] = 1065353216; /*0x1015bc97*/
    v5 = sub_100711F0(a1: v22); /*0x1015bcac*/
    v23 = v5; /*0x1015bcae*/
    __asm /*0x1015bcb2*/
    {
      vmovaps xmm0, ds:xmmword_10184200
      vmovups xmmword ptr [esp+64h+var_34], xmm0
    }
    v21 = sub_100711F0(a1: v22); /*0x1015bcc9*/
    __asm /*0x1015bccd*/
    {
      vmovddup xmm0, ds:qword_101861A0
      vmovups [esp+64h+var_64], xmm0
      vaddps xmm0, xmm0, [esp+64h+var_54]
      vmovlps qword ptr [esp+64h+var_34], xmm0
    }
    sub_100997B0(a1: &v24, a2: v22, a3: v21, a4: 0, a5: 0); /*0x1015bcf3*/
    __asm /*0x1015bcf8*/
    {
      vmovsd xmm0, [esp+64h+var_20]
      vaddps xmm0, xmm0, [esp+64h+var_64]
      vmovlps qword ptr [esp+64h+var_34], xmm0
    }
    sub_100996C0(a1: &v24, a2: v22, a3: v5, a4: 0, a5: 0, a6: 1065353216); /*0x1015bd17*/
    sub_100731A0(a1: (int)v20, a2: (char *)"Binds", a3: 0, a4: 0, a5: -1082130432); /*0x1015bd2f*/
    __asm /*0x1015bd37*/
    {
      vmovsd xmm0, [esp+64h+var_20]
      vaddps xmm0, xmm0, ds:xmmword_1019AC70
      vmovsd xmm1, qword ptr [esp+64h+var_40]
      vbroadcastss xmm2, ds:dword_10185558
      vmulps xmm1, xmm1, xmm2
      vaddps xmm0, xmm0, xmm1
      vmovlps qword ptr [esp+64h+var_34], xmm0
    }
    sub_1009AF80(a1: v0, a2: (int)v22, a3: -1, a4: (char *)"Binds", a5: 0); /*0x1015bd6e*/
    v19 = HIDWORD(qword_103F5AAC) + 14; /*0x1015bd7b*/
    v22[0] = (int)&v19; /*0x1015bd83*/
    v22[1] = (int)&v25; /*0x1015bd8b*/
    v22[2] = (int)&v21; /*0x1015bd93*/
    v22[3] = (int)&v23; /*0x1015bd9b*/
    sub_1015BEF0(a1: (unsigned __int8)byte_103F5ADC, a2: (unsigned __int8)byte_103F5CDC, a3: dword_103F5AE0); /*0x1015bdc0*/
    sub_1015BEF0(a1: (unsigned __int8)byte_103F5AE4, a2: (unsigned __int8)byte_103F4BFC, a3: dword_103F5AE8); /*0x1015bde5*/
    sub_1015BEF0(a1: (unsigned __int8)byte_103F5AEC, a2: (unsigned __int8)byte_103F4BF2, a3: dword_103F5AF0); /*0x1015be0a*/
    sub_1015BEF0(a1: (unsigned __int8)byte_103F5AF4, a2: (unsigned __int8)word_103F4BDD, a3: dword_103F5AF8); /*0x1015be2f*/
    sub_1015BEF0(a1: (unsigned __int8)byte_103F5AFC, a2: HIBYTE(word_103F4BDD), a3: dword_103F5B00); /*0x1015be54*/
    sub_1015BEF0(a1: (unsigned __int8)byte_103F5B04, a2: (unsigned __int8)byte_103F4BDF, a3: dword_103F5B08); /*0x1015be79*/
    sub_1015BEF0(a1: (unsigned __int8)byte_103F5B0C, a2: (unsigned __int8)byte_103F5340, a3: dword_103F5B10); /*0x1015be9e*/
    sub_1015BEF0(a1: (unsigned __int8)byte_103F5B14, a2: (unsigned __int8)byte_103F5350, a3: dword_103F5B18); /*0x1015bec3*/
    if ( *(_BYTE *)off_1029E02C == 1 ) /*0x1015bed3*/
      sub_1015BAE0( /*0x1015bed5*/
        a1: v17,
        a2: DWORD1(v17),
        a3: DWORD2(v17),
        a4: HIDWORD(v17),
        a5: v18,
        a6: DWORD1(v18),
        a7: DWORD2(v18),
        a8: HIDWORD(v18));
  }
}
