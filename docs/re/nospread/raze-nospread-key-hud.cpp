void sub_1015B4D0()
{
  _BYTE *v0; // esi
  char v1; // al
  int v9; // ecx
  int v10; // eax
  int v11; // esi
  int v12; // edi
  int v13; // ebx
  char *v14; // esi
  int v15; // ebx
  int v16; // ebp
  char v17; // [esp+0h] [ebp-4Ch]
  int v18; // [esp+4h] [ebp-48h]
  _DWORD v19[2]; // [esp+Ch] [ebp-40h] BYREF
  int v20[5]; // [esp+14h] [ebp-38h] BYREF
  char v21[4]; // [esp+28h] [ebp-24h]
  int v22; // [esp+2Ch] [ebp-20h]
  int v23; // [esp+30h] [ebp-1Ch] BYREF
  int v24; // [esp+34h] [ebp-18h] BYREF

  if ( byte_103F5AA8 != 1 /*0x1015b508*/
    || byte_103F4BA8 == 1
    && (*(unsigned __int8 (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 356))(a1: dword_103EC770) != 0 )
  {
    return; /*0x1015b508*/
  }
  v0 = off_1029E02C; /*0x1015b50e*/
  v1 = *(_BYTE *)off_1029E02C; /*0x1015b514*/
  if ( *(_BYTE *)off_1029E02C == 0 ) /*0x1015b519*/
  {
    if ( (*(unsigned __int8 (__thiscall **)(int))(*(_DWORD *)dword_103EC8A0 + 8))(a1: dword_103EC8A0) != 0 ) /*0x1015b528*/
      return; /*0x1015b528*/
    v0 = off_1029E02C; /*0x1015b52e*/
    v1 = *(_BYTE *)off_1029E02C; /*0x1015b534*/
  }
  __asm /*0x1015b537*/
  {
    vmovss xmm0, ds:dword_10184380
    vmulss xmm1, xmm0, dword ptr xmmword_103F5A3C
    vcvttss2si ecx, xmm1
    vmulss xmm1, xmm0, dword ptr xmmword_103F5A3C+4
  }
  LOBYTE(v24) = _ECX; /*0x1015b553*/
  __asm { vcvttss2si ecx, xmm1 } /*0x1015b557*/
  BYTE1(v24) = _ECX; /*0x1015b55b*/
  __asm /*0x1015b55f*/
  {
    vmulss xmm0, xmm0, dword ptr xmmword_103F5A3C+8
    vcvttss2si ecx, xmm0
  }
  BYTE2(v24) = _ECX; /*0x1015b56b*/
  HIBYTE(v24) = -1; /*0x1015b56f*/
  v23 = -15461376; /*0x1015b574*/
  if ( (v1 & 1) != 0 ) /*0x1015b57e*/
  {
    v9 = *((_DWORD *)off_1029E4E0 + 256); /*0x1015b58a*/
    v10 = *((_DWORD *)off_1029E4E0 + 257); /*0x1015b590*/
    if ( byte_103F7048 != 0 || v0[1] == 0 ) /*0x1015b5a5*/
    {
      v11 = v9 - qword_103F5AAC; /*0x1015b5bd*/
      v12 = v10 - HIDWORD(qword_103F5AAC); /*0x1015b5d3*/
      v13 = *((_DWORD *)off_1029E4E0 + 1); /*0x1015b5ec*/
      if ( v9 > (int)qword_103F5AAC /*0x1015b5f2*/
        && v10 > SHIDWORD(qword_103F5AAC)
        && v9 < (int)qword_103F5AAC + 200
        && v10 < HIDWORD(qword_103F5AAC) + 15
        && v13 == 1 )
      {
        dword_103F704C = v9 - qword_103F5AAC; /*0x1015b5fb*/
        dword_103F7050 = v10 - HIDWORD(qword_103F5AAC); /*0x1015b601*/
        byte_103F7048 = 1; /*0x1015b607*/
LABEL_12:
        LODWORD(qword_103F5AAC) = v9 - v11; /*0x1015b616*/
        HIDWORD(qword_103F5AAC) = v10 - v12; /*0x1015b620*/
        goto LABEL_17; /*0x1015b625*/
      }
      if ( (unsigned int)(v13 - 1) >= 2 ) /*0x1015b62b*/
      {
        byte_103F7048 = 0; /*0x1015b64a*/
        goto LABEL_17; /*0x1015b64a*/
      }
      if ( byte_103F7048 != 0 ) /*0x1015b63a*/
      {
        v11 = dword_103F704C; /*0x1015b63c*/
        v12 = dword_103F7050; /*0x1015b642*/
        goto LABEL_12; /*0x1015b648*/
      }
    }
  }
LABEL_17:
  v22 = v23; /*0x1015b651*/
  sub_10177780(a1: qword_103F5AAC, a2: HIDWORD(qword_103F5AAC), a3: 200, a4: 15, a5: v23); /*0x1015b673*/
  v18 = *(_DWORD *)ArgList; /*0x1015b67d*/
  if ( dword_103EF180 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1015b69c*/
  {
    sub_101809C3(a1: &dword_103EF180); /*0x1015b887*/
    if ( dword_103EF180 == -1 ) /*0x1015b896*/
    {
      sub_10180BFB(Function: sub_10045670); /*0x1015b8a1*/
      sub_10180A43(a1: &dword_103EF180); /*0x1015b8ae*/
    }
  }
  v14 = (char *)sub_10041820(Str: "Key Binds"); /*0x1015b6b1*/
  *(_DWORD *)v21 = -986896; /*0x1015b6b3*/
  v15 = HIDWORD(qword_103F5AAC) + 7; /*0x1015b6c1*/
  v16 = qword_103F5AAC + 100; /*0x1015b6ca*/
  sub_1017B960(a1: v20); /*0x1015b6d8*/
  sub_10177250(a1: v18, a2: (int)v20, a3: v16, a4: v15, a5: v21[0], a6: 24, Format: v14, ArgList: v17); /*0x1015b6ec*/
  v19[1] = v24; /*0x1015b6fe*/
  sub_101777E0(a1: qword_103F5AAC, a2: HIDWORD(qword_103F5AAC), a3: 200, a4: 15, a5: v24); /*0x1015b71a*/
  v19[0] = HIDWORD(qword_103F5AAC) + 14; /*0x1015b727*/
  v20[0] = (int)v19; /*0x1015b72f*/
  v20[1] = (int)&v23; /*0x1015b737*/
  v20[2] = (int)&v24; /*0x1015b73f*/
  sub_1015B8C0(a1: (unsigned __int8)byte_103F5ADC, a2: (unsigned __int8)byte_103F5CDC, a3: dword_103F5AE0); /*0x1015b764*/
  sub_1015B8C0(a1: (unsigned __int8)byte_103F5AE4, a2: (unsigned __int8)byte_103F4BFC, a3: dword_103F5AE8); /*0x1015b789*/
  sub_1015B8C0(a1: (unsigned __int8)byte_103F5AEC, a2: (unsigned __int8)byte_103F4BF2, a3: dword_103F5AF0); /*0x1015b7ae*/
  sub_1015B8C0(a1: (unsigned __int8)byte_103F5AF4, a2: (unsigned __int8)word_103F4BDD, a3: dword_103F5AF8); /*0x1015b7d3*/
  sub_1015B8C0(a1: (unsigned __int8)byte_103F5AFC, a2: HIBYTE(word_103F4BDD), a3: dword_103F5B00); /*0x1015b7f8*/
  sub_1015B8C0(a1: (unsigned __int8)byte_103F5B04, a2: (unsigned __int8)byte_103F4BDF, a3: dword_103F5B08); /*0x1015b81d*/
  sub_1015B8C0(a1: (unsigned __int8)byte_103F5B0C, a2: (unsigned __int8)byte_103F5340, a3: dword_103F5B10); /*0x1015b842*/
  sub_1015B8C0(a1: (unsigned __int8)byte_103F5B14, a2: (unsigned __int8)byte_103F5350, a3: dword_103F5B18); /*0x1015b867*/
}
