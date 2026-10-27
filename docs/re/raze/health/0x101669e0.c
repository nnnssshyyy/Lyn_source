_BYTE *__stdcall sub_101669E0(_BYTE *a1, char *a2, char *a3)
{
  _BYTE *v3; // edi
  char *v4; // ebx
  char *v5; // esi
  int v14; // eax
  char v26; // al
  int v27; // eax
  int v28; // esi
  int v29; // eax
  unsigned __int8 (__thiscall *v30)(char *); // eax
  int (__thiscall *v35)(char *); // eax
  int v36; // eax
  void *v50; // ecx
  void *v51; // ecx
  void *v52; // ecx
  int v53; // esi
  int v54; // eax
  __int128 v56; // [esp+0h] [ebp-D8h] BYREF
  __int16 v59; // [esp+30h] [ebp-A8h] BYREF
  char v60; // [esp+32h] [ebp-A6h]
  void *v61[4]; // [esp+34h] [ebp-A4h]
  int v62; // [esp+44h] [ebp-94h]
  unsigned int v63; // [esp+48h] [ebp-90h]
  void *v64[4]; // [esp+4Ch] [ebp-8Ch]
  int v65; // [esp+5Ch] [ebp-7Ch]
  unsigned int v66; // [esp+60h] [ebp-78h]
  void *Block[4]; // [esp+64h] [ebp-74h]
  int v68; // [esp+74h] [ebp-64h]
  unsigned int v69; // [esp+78h] [ebp-60h]
  int v84; // [esp+B4h] [ebp-24h]
  __int128 *v86; // [esp+BCh] [ebp-1Ch]
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp+C0h] [ebp-18h]
  void *v88; // [esp+C4h] [ebp-14h]
  int v89; // [esp+C8h] [ebp-10h]

  v3 = a1; /*0x101669ec*/
  v4 = a3; /*0x101669ef*/
  v5 = a2; /*0x101669f2*/
  v86 = &v56; /*0x101669f5*/
  v89 = -1; /*0x101669f8*/
  v88 = &loc_1016BAC0; /*0x10166a02*/
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList; /*0x10166a10*/
  if ( a3 == nullptr || a2 == nullptr ) /*0x10166a1b*/
    goto LABEL_53; /*0x10166a25*/
  __asm /*0x10166a2b*/
  {
    vmovss xmm1, ds:dword_10184380
    vmulss xmm0, xmm1, dword ptr xmmword_103F5034
    vmovss [ebp+var_3C], xmm0
    vmulss xmm0, xmm1, dword ptr xmmword_103F5034+4
    vmovss [ebp+var_40], xmm0
    vmulss xmm0, xmm1, dword ptr xmmword_103F5034+8
    vmovss [ebp+var_44], xmm0
    vmulss xmm0, xmm1, dword ptr xmmword_103F4FF0
    vmovss [ebp+var_30], xmm0
    vmulss xmm0, xmm1, dword ptr xmmword_103F4FF0+4
    vmovss [ebp+var_34], xmm0
    vmulss xmm0, xmm1, dword ptr xmmword_103F4FF0+8
    vmovss [ebp+var_38], xmm0
    vmulss xmm0, xmm1, dword ptr qword_103F4C74
    vmovss [ebp+var_50], xmm0
  }
  v14 = *(_DWORD *)a3; /*0x10166a8e*/
  __asm /*0x10166a90*/
  {
    vmulss xmm0, xmm1, dword ptr qword_103F4C74+4
    vmovss [ebp+var_48], xmm0
    vmulss xmm0, xmm1, dword_103F4C7C
    vmovss [ebp+var_4C], xmm0
    vmovss xmm0, dword_103F5668
    vmovss [ebp+var_28], xmm0
    vmovss xmm0, dword_103F5678
    vmovss [ebp+var_20], xmm0
    vmovss xmm0, dword_103F5688
    vmovss [ebp+var_2C], xmm0
    vmovsd xmm0, qword_103F566C
    vmovups [ebp+var_C8], xmm0
    vmovsd xmm0, qword_103F567C
    vmovups [ebp+var_B8], xmm0
    vmovsd xmm0, qword_103F568C
    vmovups [ebp+var_D8], xmm0
    vmulss xmm0, xmm1, dword_103F4FD8
    vmovss [ebp+var_54], xmm0
    vmulss xmm0, xmm1, dword_103F4FDC
    vmovss [ebp+var_58], xmm0
    vmulss xmm0, xmm1, dword_103F4FE0
    vmovss [ebp+var_5C], xmm0
  }
  v26 = (*(int (__thiscall **)(char *))(v14 + 572))(a1: a3); /*0x10166b2a*/
  if ( a3 == a2 || v26 == 0 ) /*0x10166b3a*/
  {
LABEL_36:
    if ( v4 == v5 || (v89 = -1, (char *)sub_10035F90(this: v4) == v5) ) /*0x10166e14*/
    {
      __asm /*0x10166e86*/
      {
        vcvttss2si eax, [ebp+var_54]
        vcvttss2si ecx, [ebp+var_58]
        vcvttss2si edx, [ebp+var_5C]
      }
      goto LABEL_45; /*0x10166e90*/
    }
    if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10166e31*/
    {
      sub_101809C3(a1: &dword_103EEAD8); /*0x10166f53*/
      if ( dword_103EEAD8 == -1 ) /*0x10166f62*/
      {
        v89 = 3; /*0x10166f68*/
        dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x10166f81*/
        sub_10180A43(a1: &dword_103EEAD8); /*0x10166f8b*/
        v3 = a1; /*0x10166f93*/
        v4 = a3; /*0x10166f96*/
      }
    }
    if ( *(_DWORD *)&v4[dword_103EEAD4] == 2 ) /*0x10166e40*/
    {
      if ( dword_103F4FEC == 0 ) /*0x10166e4d*/
      {
        __asm /*0x10166eb2*/
        {
          vcvttss2si eax, [ebp+var_30]
          vcvttss2si ecx, [ebp+var_34]
          vcvttss2si edx, [ebp+var_38]
        }
        goto LABEL_45; /*0x10166ec1*/
      }
      if ( dword_103F7384 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10166e6a*/
      {
        sub_101809C3(a1: &dword_103F7384); /*0x101670a4*/
        if ( dword_103F7384 == -1 ) /*0x101670b3*/
        {
          v89 = 4; /*0x101670b9*/
          dword_103F7380 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_isIncapacitated"); /*0x101670d2*/
          sub_10180A43(a1: &dword_103F7384); /*0x101670dc*/
          v3 = a1; /*0x101670e4*/
          v4 = a3; /*0x101670e7*/
        }
      }
      if ( v4[dword_103F7380] == 1 ) /*0x10166e79*/
      {
        *(_DWORD *)v3 = -65536; /*0x10166e7b*/
        return v3; /*0x10166e81*/
      }
      if ( dword_103F4FEC == 1 ) /*0x10166eca*/
      {
        v89 = -1; /*0x10166ecc*/
        v53 = *(_DWORD *)sub_10035E70(this: v4); /*0x10166eda*/
        v54 = (*(int (__thiscall **)(char *))(*(_DWORD *)v4 + 468))(a1: v4); /*0x10166ee0*/
        sub_101671B0(a1: v3, a2: v54, a3: v53); /*0x10166ee9*/
        return v3; /*0x10166eee*/
      }
    }
    if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10166f0b*/
    {
      sub_101809C3(a1: &dword_103EEAD8); /*0x10167052*/
      if ( dword_103EEAD8 == -1 ) /*0x10167061*/
      {
        v89 = 5; /*0x10167067*/
        dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x10167080*/
        sub_10180A43(a1: &dword_103EEAD8); /*0x1016708a*/
        v3 = a1; /*0x10167092*/
        v4 = a3; /*0x10167095*/
      }
    }
    if ( *(_DWORD *)&v4[dword_103EEAD4] == 3 ) /*0x10166f1a*/
    {
      __asm /*0x10166f1c*/
      {
        vcvttss2si eax, [ebp+var_3C]
        vcvttss2si ecx, [ebp+var_40]
        vcvttss2si edx, [ebp+var_44]
      }
      goto LABEL_45; /*0x10166f2b*/
    }
LABEL_53:
    *(_DWORD *)v3 = -1; /*0x10166f30*/
    return v3; /*0x10166f30*/
  }
  if ( (unsigned __int8)sub_10166490(a1: a3) == 0 ) /*0x10166b4b*/
    goto LABEL_12; /*0x10166b4b*/
  v27 = dword_103EEAD8; /*0x10166b4d*/
  if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10166b68*/
  {
    sub_101809C3(a1: &dword_103EEAD8); /*0x10166fa3*/
    v27 = dword_103EEAD8; /*0x10166fab*/
    if ( dword_103EEAD8 == -1 ) /*0x10166fb3*/
    {
      v89 = 0; /*0x10166fb9*/
      dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x10166fd2*/
      sub_10180A43(a1: &dword_103EEAD8); /*0x10166fdc*/
      v27 = dword_103EEAD8; /*0x10166fe4*/
      v3 = a1; /*0x10166fe9*/
      v4 = a3; /*0x10166fec*/
      v5 = a2; /*0x10166fef*/
    }
  }
  v28 = *(_DWORD *)&v5[dword_103EEAD4]; /*0x10166b74*/
  if ( v27 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10166b8d*/
  {
    sub_101809C3(a1: &dword_103EEAD8); /*0x10166ffc*/
    if ( dword_103EEAD8 == -1 ) /*0x1016700b*/
    {
      v84 = v28; /*0x10167011*/
      v89 = 1; /*0x10167014*/
      dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x1016702d*/
      sub_10180A43(a1: &dword_103EEAD8); /*0x10167037*/
      v3 = a1; /*0x1016703f*/
      v4 = a3; /*0x10167042*/
      v28 = v84; /*0x10167045*/
    }
  }
  if ( v28 == 3 ) /*0x10166b96*/
    goto LABEL_46; /*0x10166b96*/
  if ( v28 != 2 ) /*0x10166b9f*/
    goto LABEL_12; /*0x10166b9f*/
  v29 = *(_DWORD *)&v4[dword_103EEAD4]; /*0x10166ba6*/
  if ( v29 == 3 ) /*0x10166bac*/
  {
LABEL_46:
    *(_DWORD *)v3 = dword_102A4424; /*0x10166eab*/
    return v3; /*0x10166ead*/
  }
  if ( v29 != 2 ) /*0x10166bb5*/
  {
LABEL_12:
    v30 = *(unsigned __int8 (__thiscall **)(char *))(*(_DWORD *)v4 + 960); /*0x10166bc3*/
    v89 = -1; /*0x10166bcb*/
    if ( v30(a1: v4) != 0 ) /*0x10166bd8*/
    {
      __asm /*0x10166bda*/
      {
        vcvttss2si eax, [ebp+var_50]
        vcvttss2si ecx, [ebp+var_48]
        vcvttss2si edx, [ebp+var_4C]
      }
LABEL_45:
      *v3 = _EAX; /*0x10166e95*/
      v3[1] = _ECX; /*0x10166e97*/
      v3[2] = _EDX; /*0x10166e9a*/
      v3[3] = -1; /*0x10166e9d*/
      return v3; /*0x10166ea1*/
    }
    v59 = 0; /*0x10166bee*/
    v60 = 0; /*0x10166bf7*/
    __asm /*0x10166bfe*/
    {
      vxorps xmm0, xmm0, xmm0
      vmovups xmmword ptr [ebp+var_A4], xmm0
    }
    v62 = 0; /*0x10166c0a*/
    v63 = 15; /*0x10166c14*/
    __asm { vmovups xmmword ptr [ebp+var_8C], xmm0 } /*0x10166c1e*/
    v65 = 0; /*0x10166c26*/
    v66 = 15; /*0x10166c2d*/
    __asm { vmovups xmmword ptr [ebp+Block], xmm0 } /*0x10166c34*/
    v68 = 0; /*0x10166c39*/
    v69 = 15; /*0x10166c40*/
    v35 = *(int (__thiscall **)(char *))(*((_DWORD *)v4 + 2) + 32); /*0x10166c4d*/
    v89 = 2; /*0x10166c50*/
    v36 = v35(a1: v4 + 8); /*0x10166c57*/
    sub_10152FA0(a1: v36, a2: &v59); /*0x10166c61*/
    v3 = a1; /*0x10166c70*/
    if ( byte_103F569A == 1 && byte_103F569C == 1 ) /*0x10166c80*/
    {
      if ( HIBYTE(v59) != 0 ) /*0x10166c8d*/
        goto LABEL_19; /*0x10166c8d*/
      __asm /*0x10166c96*/
      {
        vmovss xmm0, [ebp+var_2C]
        vmovss [ebp+var_20], xmm0
        vmovups xmm0, [ebp+var_D8]
        vmovups [ebp+var_B8], xmm0
      }
      if ( v60 != 0 ) /*0x10166cb0*/
        goto LABEL_19; /*0x10166cb0*/
      __asm /*0x10166cb9*/
      {
        vmovss xmm0, [ebp+var_28]
        vmovss [ebp+var_20], xmm0
        vmovups xmm0, [ebp+var_C8]
        vmovups [ebp+var_B8], xmm0
      }
      if ( (_BYTE)v59 != 0 ) /*0x10166cd3*/
      {
LABEL_19:
        __asm /*0x10166cd5*/
        {
          vmovups xmm2, [ebp+var_B8]
          vmovshdup xmm0, xmm2
          vmovss xmm1, ds:dword_10184380
          vmulss xmm0, xmm0, xmm1
          vcvttss2si eax, xmm0
          vmulss xmm0, xmm2, xmm1
          vcvttss2si ecx, xmm0
          vmulss xmm0, xmm1, [ebp+var_20]
          vcvttss2si edx, xmm0
        }
        *a1 = _EDX; /*0x10166d02*/
        a1[1] = _ECX; /*0x10166d04*/
        a1[2] = _EAX; /*0x10166d07*/
        a1[3] = -1; /*0x10166d0a*/
        sub_100F8C70(a1: &v59); /*0x10166d14*/
        return v3; /*0x10166d19*/
      }
    }
    v4 = a3; /*0x10166d24*/
    if ( v69 >= 0x10 ) /*0x10166d27*/
    {
      v50 = Block[0]; /*0x10166d29*/
      if ( v69 + 1 >= 0x1000 ) /*0x10166d35*/
      {
        if ( (unsigned int)Block[0] - *((_DWORD *)Block[0] - 1) - 4 >= 0x20 ) /*0x10166d42*/
          goto LABEL_63; /*0x10166d42*/
        v50 = *((void **)Block[0] - 1); /*0x10166d4d*/
      }
      sub_10180976(Block: v50); /*0x10166d51*/
    }
    v68 = 0; /*0x10166d59*/
    v69 = 15; /*0x10166d60*/
    LOBYTE(Block[0]) = 0; /*0x10166d67*/
    if ( v66 >= 0x10 ) /*0x10166d71*/
    {
      v51 = v64[0]; /*0x10166d73*/
      if ( v66 + 1 >= 0x1000 ) /*0x10166d82*/
      {
        if ( (unsigned int)v64[0] - *((_DWORD *)v64[0] - 1) - 4 >= 0x20 ) /*0x10166d8f*/
          goto LABEL_63; /*0x10166d8f*/
        v51 = *((void **)v64[0] - 1); /*0x10166d9a*/
      }
      sub_10180976(Block: v51); /*0x10166d9e*/
    }
    v65 = 0; /*0x10166da6*/
    v66 = 15; /*0x10166dad*/
    LOBYTE(v64[0]) = 0; /*0x10166db4*/
    if ( v63 < 0x10 ) /*0x10166dc4*/
    {
LABEL_35:
      v5 = a2; /*0x10166df9*/
      goto LABEL_36; /*0x10166df9*/
    }
    v52 = v61[0]; /*0x10166dc6*/
    if ( v63 + 1 < 0x1000 ) /*0x10166dd5*/
    {
LABEL_34:
      sub_10180976(Block: v52); /*0x10166def*/
      goto LABEL_35; /*0x10166df1*/
    }
    if ( (unsigned int)v61[0] - *((_DWORD *)v61[0] - 1) - 4 < 0x20 ) /*0x10166de2*/
    {
      v52 = *((void **)v61[0] - 1); /*0x10166ded*/
      goto LABEL_34; /*0x10166ded*/
    }
LABEL_63:
    BUG(); /*0x1016709d*/
  }
  *(_DWORD *)v3 = dword_102A4420; /*0x10166bbc*/
  return v3; /*0x10166f36*/
}