void __userpurge sub_101472B0(int a1@<ecx>, __int128 _XMM0@<xmm0>, int a3, int a4)
{
  int v6; // esi
  int v7; // edi
  int v8; // eax
  int v9; // ecx
  int v10; // esi
  int v11; // edi
  int (__thiscall *v17)(int); // eax
  int (__thiscall *v18)(int); // eax
  int (__thiscall *v19)(int); // eax
  int v20; // eax
  int v21; // esi
  int v22; // eax
  int v23; // esi
  SHORT AsyncKeyState; // ax
  char v25; // cl
  void (__thiscall *v27)(int, __int64 *); // eax
  char v33; // cf
  char v34; // zf
  int v122; // eax
  int v123; // esi
  int v124; // edi
  int (__thiscall *v125)(int, _DWORD, _DWORD, _DWORD, _DWORD); // eax
  int v126; // eax
  int v127; // edi
  int (__thiscall *v128)(int); // eax
  int v129; // eax
  __int128 v131; // [esp+0h] [ebp-8Ch] BYREF
  float v133; // [esp+20h] [ebp-6Ch] BYREF
  float v134; // [esp+24h] [ebp-68h]
  float v135; // [esp+28h] [ebp-64h]
  float v136; // [esp+2Ch] [ebp-60h] BYREF
  float v137; // [esp+30h] [ebp-5Ch]
  float v138; // [esp+34h] [ebp-58h]
  __int128 v139; // [esp+38h] [ebp-54h]
  __int64 v140; // [esp+48h] [ebp-44h] BYREF
  int v141; // [esp+50h] [ebp-3Ch]
  __int128 v142; // [esp+54h] [ebp-38h]
  float *v143; // [esp+64h] [ebp-28h]
  float *v144; // [esp+68h] [ebp-24h]
  float v145; // [esp+6Ch] [ebp-20h]
  __int128 *v146; // [esp+70h] [ebp-1Ch]
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp+74h] [ebp-18h]
  void *v148; // [esp+78h] [ebp-14h]
  int v149; // [esp+7Ch] [ebp-10h]

  LODWORD(v142) = a1; /*0x101472bc*/
  _EDX = a3; /*0x101472bf*/
  v146 = &v131; /*0x101472c2*/
  v149 = -1; /*0x101472c5*/
  v148 = &loc_101491C0; /*0x101472cf*/
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList; /*0x101472dd*/
  if ( HIBYTE(word_103F538C) == 1 ) /*0x101472ed*/
  {
    if ( dword_103F6DBC == -1 ) /*0x101472fa*/
    {
      _EAX = dword_103EC8D0; /*0x1014730b*/
      __asm /*0x10147310*/
      {
        vmovss xmm0, dword ptr [eax+1Ch]
        vmovss dword ptr [edx+10h], xmm0
      }
      *(_DWORD *)(a3 + 20) = *(_DWORD *)(a4 + 4); /*0x10147320*/
      if ( *(_BYTE *)(a3 + 327) != 0 ) /*0x1014732a*/
        goto LABEL_17; /*0x1014732a*/
    }
    else if ( *(_BYTE *)(a3 + 327) != 0 ) /*0x10147303*/
    {
      goto LABEL_17; /*0x10147303*/
    }
    LODWORD(v139) = *(_DWORD *)(a3 + 20); /*0x10147338*/
    v6 = (unsigned __int64)(458129845LL * (int)v139) >> 32; /*0x1014733d*/
    v7 = *(_DWORD *)(a4 + 4); /*0x10147342*/
    v8 = (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 296))(a1: dword_103EC770); /*0x10147353*/
    if ( *(_DWORD *)(a3 + 228) != 3 && (*(_DWORD *)(a3 + 7076) == *(_DWORD *)(a3 + 376) || *(_DWORD *)(a3 + 8076) != -1) ) /*0x1014737e*/
    {
      v9 = dword_103F5390; /*0x10147384*/
      if ( dword_103F5390 > 0 ) /*0x1014738c*/
      {
        v10 = v139 - 150 * (((unsigned int)v6 >> 31) + (v6 >> 4)); /*0x101473a5*/
        v11 = v7 % 150; /*0x101473b7*/
        if ( v11 == v10 ) /*0x101473bb*/
        {
          dword_103F690C[2 * v11] = 0; /*0x101473bd*/
          __asm /*0x101473ce*/
          {
            vmovss xmm0, ds:dword_10191CD0
            vdivss xmm0, xmm0, dword ptr [edx+1Ch]
            vaddss xmm0, xmm0, ds:dword_10184EDC
            vcvttss2si edx, xmm0
          }
          if ( v9 > _EDX ) /*0x101473e9*/
            _EDX = v9; /*0x101473e9*/
          dword_103F6DBC = _EDX; /*0x101473ec*/
        }
        else
        {
          ++dword_103F68E0; /*0x101473f4*/
        }
        if ( dword_103F690C[2 * v10] == 0 ) /*0x10147402*/
        {
          *(_DWORD *)(v8 + 16) = -1; /*0x10147404*/
          *(_DWORD *)(v8 + 28) = 255; /*0x1014740b*/
        }
      }
    }
  }
LABEL_17:
  _EDI = (_DWORD *)a4; /*0x10147412*/
  if ( HIBYTE(word_103F5394) == 1 && (*(_BYTE *)(a4 + 4) & 1) != 0 ) /*0x10147422*/
    *(_BYTE *)(a4 + 39) |= 4u; /*0x10147424*/
  if ( byte_103F53D4 == 1 ) /*0x1014742f*/
  {
    sub_10147F90(a1: a4); /*0x10147452*/
    _EBX = v142; /*0x1014745e*/
    if ( byte_103F53CE != 1 ) /*0x10147461*/
    {
LABEL_22:
      if ( byte_103F4BF5 != 1 ) /*0x10147444*/
        goto LABEL_23; /*0x10147444*/
      goto LABEL_27; /*0x10147444*/
    }
  }
  else
  {
    _EBX = v142; /*0x10147438*/
    if ( byte_103F53CE != 1 ) /*0x1014743b*/
      goto LABEL_22; /*0x1014743b*/
  }
  sub_10148280(a1: a4); /*0x10147464*/
  if ( byte_103F4BF5 != 1 ) /*0x10147470*/
  {
LABEL_23:
    if ( byte_103F53B0 != 1 ) /*0x1014744d*/
      goto LABEL_31; /*0x1014744d*/
    goto LABEL_28; /*0x1014744d*/
  }
LABEL_27:
  sub_10148590(a1: a4); /*0x10147472*/
  if ( byte_103F53B0 == 1 ) /*0x10147481*/
  {
LABEL_28:
    if ( GetAsyncKeyState(vKey: dword_103F53B4) < 0 /*0x101474a4*/
      && (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 296))(a1: dword_103EC770) != 0 )
    {
      *(_DWORD *)(a4 + 8) = 0xFFFFFF; /*0x101474a6*/
    }
  }
LABEL_31:
  if ( BYTE1(dword_103F53C8) == 1 ) /*0x101474b4*/
  {
    if ( dword_103F6DE4 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x101474d1*/
    {
      sub_101809C3(a1: &dword_103F6DE4); /*0x10147bad*/
      if ( dword_103F6DE4 == -1 ) /*0x10147bbc*/
      {
        v149 = 0; /*0x10147bc2*/
        dword_103F6DE0 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_jockeyAttacker"); /*0x10147bdb*/
        sub_10180A43(a1: &dword_103F6DE4); /*0x10147be5*/
        _EDI = (_DWORD *)a4; /*0x10147bed*/
        _EBX = v142; /*0x10147bf0*/
      }
    }
    if ( *(_DWORD *)(dword_103F6DE0 + a3) != -1 ) /*0x101474e3*/
    {
      v17 = *(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 296); /*0x101474ed*/
      v149 = -1; /*0x101474f3*/
      if ( v17(a1: dword_103EC770) != 0 ) /*0x101474fe*/
        _EDI[2] = 0xFFFFFF; /*0x10147500*/
    }
  }
  if ( BYTE2(dword_103F53C8) == 1 ) /*0x1014750e*/
  {
    if ( dword_103F6DEC > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1014752b*/
    {
      sub_101809C3(a1: &dword_103F6DEC); /*0x10147bfd*/
      if ( dword_103F6DEC == -1 ) /*0x10147c0c*/
      {
        v149 = 1; /*0x10147c12*/
        dword_103F6DE8 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_tongueOwner"); /*0x10147c2b*/
        sub_10180A43(a1: &dword_103F6DEC); /*0x10147c35*/
        _EDI = (_DWORD *)a4; /*0x10147c3d*/
        _EBX = v142; /*0x10147c40*/
      }
    }
    LODWORD(v140) = *(_DWORD *)(dword_103F6DE8 + a3); /*0x1014753c*/
    v149 = -1; /*0x1014753f*/
    if ( sub_10144C10(a1: &v140) != 0 /*0x10147562*/
      && (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 296))(a1: dword_103EC770) != 0 )
    {
      *(_DWORD *)(*(_DWORD *)(dword_103EC9A8 + 24) + 8) += 500; /*0x1014756c*/
    }
  }
  if ( byte_103F53BC == 1 && GetAsyncKeyState(vKey: dword_103F53C0) < 0 ) /*0x1014758b*/
  {
    v18 = *(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 296); /*0x10147595*/
    v149 = -1; /*0x1014759b*/
    if ( v18(a1: dword_103EC770) != 0 ) /*0x101475a6*/
      _EDI[4] = 2096152002; /*0x101475a8*/
  }
  if ( HIBYTE(word_103F53E0) == 1 && GetAsyncKeyState(vKey: dword_103F53E4) < 0 ) /*0x101475c7*/
  {
    if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x101475e4*/
    {
      sub_101809C3(a1: &dword_103EEAD8); /*0x10147cf3*/
      if ( dword_103EEAD8 == -1 ) /*0x10147d02*/
      {
        v149 = 2; /*0x10147d08*/
        dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x10147d21*/
        sub_10180A43(a1: &dword_103EEAD8); /*0x10147d2b*/
        _EDI = (_DWORD *)a4; /*0x10147d33*/
        _EBX = v142; /*0x10147d36*/
      }
    }
    if ( *(_DWORD *)(dword_103EEAD4 + a3) == 3 ) /*0x101475f6*/
    {
      *(_DWORD *)(*(_DWORD *)(dword_103EC9A8 + 24) + 8) += dword_103F53E8; /*0x10147606*/
      byte_103F6908 = 1; /*0x10147609*/
    }
  }
  if ( byte_103F53F0 == 1 && GetAsyncKeyState(vKey: dword_103F53F4) < 0 ) /*0x10147628*/
  {
    if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10147645*/
    {
      sub_101809C3(a1: &dword_103EEAD8); /*0x10147d43*/
      if ( dword_103EEAD8 == -1 ) /*0x10147d52*/
      {
        v149 = 3; /*0x10147d58*/
        dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x10147d71*/
        sub_10180A43(a1: &dword_103EEAD8); /*0x10147d7b*/
        _EDI = (_DWORD *)a4; /*0x10147d83*/
        _EBX = v142; /*0x10147d86*/
      }
    }
    if ( *(_DWORD *)(dword_103EEAD4 + a3) == 2 ) /*0x10147657*/
    {
      v19 = *(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 296); /*0x10147661*/
      v149 = -1; /*0x10147667*/
      if ( v19(a1: dword_103EC770) != 0 ) /*0x10147672*/
      {
        *(_DWORD *)(*(_DWORD *)(dword_103EC9A8 + 24) + 8) += dword_103F53FC; /*0x10147682*/
        byte_103F6908 = 1; /*0x10147685*/
      }
    }
  }
  if ( (_BYTE)word_103F53E0 == 1 ) /*0x10147693*/
  {
    v149 = -1; /*0x10147695*/
    sub_10146B10(a1: _EDI); /*0x1014769d*/
  }
  if ( HIBYTE(dword_103F53C8) == 1 ) /*0x101476a9*/
  {
    if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x101476ca*/
    {
      sub_101809C3(a1: &dword_103EEAD8); /*0x10147c4d*/
      if ( dword_103EEAD8 == -1 ) /*0x10147c5c*/
      {
        v149 = 4; /*0x10147c62*/
        dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x10147c7b*/
        sub_10180A43(a1: &dword_103EEAD8); /*0x10147c85*/
        _EDI = (_DWORD *)a4; /*0x10147c8d*/
        _EBX = v142; /*0x10147c90*/
      }
    }
    v149 = -1; /*0x101476d6*/
    v20 = sub_10179540(a1: off_1029E014); /*0x101476dd*/
    if ( v20 != 0 ) /*0x101476e4*/
    {
      v21 = v20; /*0x101476ea*/
      if ( dword_103EEF7C > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10147707*/
      {
        sub_101809C3(a1: &dword_103EEF7C); /*0x10147c9d*/
        if ( dword_103EEF7C == -1 ) /*0x10147cac*/
        {
          LODWORD(v139) = v21; /*0x10147cb2*/
          v149 = 5; /*0x10147cb5*/
          dword_103EEF78 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_zombieClass"); /*0x10147cce*/
          sub_10180A43(a1: &dword_103EEF7C); /*0x10147cd8*/
          _EDI = (_DWORD *)a4; /*0x10147ce0*/
          _EBX = v142; /*0x10147ce3*/
          v21 = v139; /*0x10147ce6*/
        }
      }
      if ( *(_DWORD *)(dword_103EEF78 + v21) == 6 ) /*0x10147716*/
      {
        if ( dword_103F6DF4 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10147733*/
        {
          sub_101809C3(a1: &dword_103F6DF4); /*0x10147d93*/
          if ( dword_103F6DF4 == -1 ) /*0x10147da2*/
          {
            LODWORD(v139) = v21; /*0x10147da8*/
            v149 = 6; /*0x10147dab*/
            dword_103F6DF0 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_carryVictim"); /*0x10147dc4*/
            sub_10180A43(a1: &dword_103F6DF4); /*0x10147dce*/
            _EDI = (_DWORD *)a4; /*0x10147dd6*/
            _EBX = v142; /*0x10147dd9*/
            v21 = v139; /*0x10147ddc*/
          }
        }
        v149 = -1; /*0x1014773f*/
        v22 = sub_10144C10(a1: dword_103F6DF0 + v21); /*0x10147748*/
        if ( v22 == 0 ) /*0x1014774f*/
          goto LABEL_68; /*0x1014774f*/
        if ( byte_103F68E4 == 0 ) /*0x10147758*/
        {
          v23 = v22; /*0x10147762*/
          if ( (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 296))(a1: dword_103EC770) != 0 ) /*0x1014776c*/
          {
            v22 = v23; /*0x1014776e*/
            _EDI[5] = 2096152002; /*0x10147770*/
LABEL_68:
            byte_103F68E4 = v22 != 0; /*0x10147777*/
          }
        }
      }
    }
  }
  AsyncKeyState = GetAsyncKeyState(vKey: dword_103F5398); /*0x10147780*/
  if ( AsyncKeyState < 0 ) /*0x1014778f*/
  {
    v25 = byte_103F5397; /*0x1014779a*/
    if ( byte_103F68E5 == 0 ) /*0x101477a8*/
    {
      v25 = byte_103F5397 ^ 1; /*0x101477aa*/
      byte_103F5397 ^= 1u; /*0x101477ad*/
    }
  }
  else
  {
    v25 = byte_103F5397; /*0x10147791*/
  }
  byte_103F68E5 = AsyncKeyState < 0; /*0x101477b6*/
  if ( (v25 & 1) != 0 ) /*0x101477c0*/
  {
    __asm /*0x101477cc*/
    {
      vxorps xmm0, xmm0, xmm0
      vmovsd [ebp+var_44], xmm0
    }
    v141 = 0; /*0x101477d5*/
    v27 = *(void (__thiscall **)(int, __int64 *))(*(_DWORD *)dword_103EC770 + 76); /*0x101477de*/
    v149 = -1; /*0x101477e1*/
    v27(a1: dword_103EC770, a2: &v140); /*0x101477ec*/
    __asm /*0x101477ee*/
    {
      vmovss xmm1, dword ptr [ebx]
      vbroadcastss xmm0, ds:dword_10184EE0
      vandps xmm2, xmm1, xmm0
      vmovss xmm1, ds:dword_10184EE4
      vucomiss xmm1, xmm2
    }
    if ( !(v33 | v34) ) /*0x1014780b*/
    {
      __asm /*0x1014780d*/
      {
        vmovss xmm2, dword ptr [ebx+4]
        vandps xmm2, xmm2, xmm0
        vucomiss xmm1, xmm2
      }
      __asm
      {
        vmovss xmm2, dword ptr [ebx+8]
        vandps xmm0, xmm2, xmm0
        vucomiss xmm1, xmm0
      }
      _EAX = (*(int (__thiscall **)(int))(*(_DWORD *)a3 + 44))(a1: a3); /*0x10147830*/
      __asm /*0x10147833*/
      {
        vmovss xmm0, dword ptr [eax]
        vmovss dword ptr [ebx], xmm0
        vmovss xmm0, dword ptr [eax+4]
        vmovss dword ptr [ebx+4], xmm0
        vmovss xmm0, dword ptr [eax+8]
        vaddss xmm0, xmm0, ds:dword_10187424
        vmovss dword ptr [ebx+8], xmm0
      }
    }
    __asm /*0x10147857*/
    {
      vmovss xmm0, ds:dword_10184F10
      vmulss xmm1, xmm0, dword ptr [ebp+var_44]
    }
    __asm { vmovss [ebp+var_20], xmm1 }
    v137 = cos(v145); /*0x10147880*/
    v134 = sin(v145); /*0x10147882*/
    __asm { vmulss xmm1, xmm0, dword ptr [ebp+var_44+4] } /*0x10147884*/
    __asm { vmovss [ebp+var_20], xmm1 }
    v138 = cos(v145); /*0x101478a5*/
    v135 = sin(v145); /*0x101478a7*/
    __asm { vmulss xmm0, xmm0, [ebp+var_3C] } /*0x101478a9*/
    v143 = &v136; /*0x101478b1*/
    v144 = &v133; /*0x101478b7*/
    __asm { vmovss [ebp+var_20], xmm0 } /*0x101478ba*/
    _EAX = &v133; /*0x101478c7*/
    v136 = cos(v145); /*0x101478ca*/
    v133 = sin(v145); /*0x101478cc*/
    __asm /*0x101478ce*/
    {
      vmovss xmm1, [ebp+var_68]
      vbroadcastss xmm3, ds:dword_1018513C
      vmovss xmm2, [ebp+var_6C]
    }
    _EDX = _EDI[9]; /*0x101478e1*/
    __asm { vxorps xmm0, xmm0, xmm0 } /*0x101478e4*/
    LOBYTE(_EAX) = (_EDX & 8) == 0; /*0x101478eb*/
    if ( (_EDX & 8) != 0 ) /*0x101478ee*/
      __asm { vsubss xmm0, xmm0, xmm1 } /*0x101478f0*/
    __asm /*0x101478f4*/
    {
      vxorps xmm4, xmm2, xmm3
      vbroadcastss xmm5, [ebp+var_5C]
    }
    LOBYTE(_ECX) = (_EDX & 0x10) == 0; /*0x10147901*/
    __asm { vmovups [ebp+var_38], xmm2 } /*0x10147904*/
    if ( (_EDX & 0x10) != 0 ) /*0x10147909*/
      __asm { vaddss xmm0, xmm1, xmm0 } /*0x1014790b*/
    __asm /*0x1014790f*/
    {
      vmulss xmm6, xmm5, xmm4
      vmovd xmm2, edx
      vpbroadcastd xmm2, xmm2
      vpand xmm2, xmm2, ds:xmmword_10199FF0
      vxorps xmm3, xmm3, xmm3
      vpcmpeqd xmm2, xmm2, xmm3
      vpextrb edx, xmm2, 0Ch
    }
    if ( (_EDX & 1) == 0 ) /*0x10147935*/
      __asm { vaddss xmm0, xmm6, xmm0 } /*0x10147937*/
    __asm /*0x1014793b*/
    {
      vmovss xmm7, [ebp+var_60]
      vpextrb edx, xmm2, 8
    }
    if ( (_EDX & 1) == 0 ) /*0x10147949*/
      __asm { vsubss xmm0, xmm0, xmm6 } /*0x1014794b*/
    __asm /*0x1014794f*/
    {
      vmulss xmm3, xmm5, xmm7
      vpextrb edx, xmm2, 4
    }
    __asm { vmovups [ebp+var_8C], xmm5 }
    if ( (_EDX & 1) == 0 ) /*0x10147964*/
      __asm { vaddss xmm0, xmm3, xmm0 } /*0x10147966*/
    __asm /*0x1014796a*/
    {
      vmovups [ebp+var_54], xmm4
      vmulss xmm6, xmm1, xmm4
      vbroadcastss xmm4, ds:dword_1018513C
      vxorps xmm5, xmm7, xmm4
      vmulss xmm1, xmm1, xmm7
      vmovups [ebp+var_7C], xmm1
      vmovss xmm1, [ebp+var_58]
      vmovss xmm4, [ebp+var_64]
      vmovd edx, xmm2
    }
    if ( (_EDX & 1) == 0 ) /*0x1014799a*/
      __asm { vsubss xmm0, xmm0, xmm3 } /*0x1014799c*/
    __asm /*0x101479a0*/
    {
      vinsertps xmm3, xmm7, xmm5, 1Ch
      vinsertps xmm5, xmm1, xmm4, 10h
      vshufps xmm7, xmm5, xmm5, 0E1h
      vmulps xmm3, xmm7, xmm3
      vbroadcastss xmm6, xmm6
      vfmadd213ps xmm6, xmm5, xmm3
      vinsertps xmm1, xmm4, xmm1, 10h
      vmovups xmm3, [ebp+var_38]
      vinsertps xmm3, xmm3, dword ptr [ebp+var_54], 10h
      vmulps xmm4, xmm5, [ebp+var_8C]
      vmulps xmm3, xmm1, xmm3
      vbroadcastss xmm1, dword ptr [ebp+var_7C]
      vfmadd213ps xmm1, xmm5, xmm3
      vxorps xmm3, xmm3, xmm3
      vaddps xmm3, xmm4, xmm3
      vmovd xmm5, eax
      vpbroadcastb xmm5, xmm5
      vpslld xmm5, xmm5, 1Fh
      vpsrad xmm5, xmm5, 1Fh
      vpandn xmm3, xmm5, xmm3
      vsubps xmm4, xmm3, xmm4
      vmovd xmm5, ecx
      vpbroadcastb xmm5, xmm5
      vpslld xmm5, xmm5, 1Fh
      vblendvps xmm3, xmm4, xmm3, xmm5
      vaddps xmm4, xmm6, xmm3
      vpshufd xmm5, xmm2, 0FFh
      vblendvps xmm3, xmm4, xmm3, xmm5
      vsubps xmm4, xmm3, xmm6
      vpshufd xmm5, xmm2, 0FAh
      vblendvps xmm3, xmm4, xmm3, xmm5
      vaddps xmm4, xmm1, xmm3
      vpshufd xmm5, xmm2, 55h ; 'U'
      vblendvps xmm3, xmm4, xmm3, xmm5
      vsubps xmm1, xmm3, xmm1
      vpbroadcastd xmm2, xmm2
      vblendvps xmm1, xmm1, xmm3, xmm2
      vbroadcastss xmm2, dword_103F53A0
      vmulss xmm0, xmm2, xmm0
      vmulps xmm1, xmm2, xmm1
      vmovsd xmm2, qword ptr [ebx]
      vaddps xmm1, xmm2, xmm1
      vmovlps qword ptr [ebx], xmm1
      vaddss xmm0, xmm0, dword ptr [ebx+8]
      vmovss dword ptr [ebx+8], xmm0
      vmovsd xmm0, [ebp+var_44]
      vmovsd qword ptr [edi+0Ch], xmm0
      vmovss xmm0, [ebp+var_3C]
      vmovss dword ptr [edi+14h], xmm0
    }
    _XMM0 = ((__m128 (__thiscall *)(int, __int64 *))*(_DWORD *)(*(_DWORD *)dword_103EC770 + 80))( /*0x10147a9f*/
              a1: dword_103EC770,
              a2: &v140);
    __asm /*0x10147aa2*/
    {
      vxorps xmm0, xmm0, xmm0
      vmovups xmmword ptr [edi+18h], xmm0
    }
    if ( byte_103F5360 == 1 && GetAsyncKeyState(vKey: dword_103F5364) < 0 ) /*0x10147ac7*/
    {
      v122 = sub_10179540(a1: off_1029E014); /*0x10147ad3*/
      if ( v122 != 0 ) /*0x10147ada*/
      {
        v123 = v122; /*0x10147ae0*/
        v124 = dword_103F536C; /*0x10147ae2*/
        if ( dword_103F6DDC > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10147b03*/
        {
          sub_101809C3(a1: &dword_103F6DDC); /*0x10147de9*/
          if ( dword_103F6DDC == -1 ) /*0x10147df8*/
          {
            LODWORD(v139) = v124; /*0x10147dfe*/
            LODWORD(v142) = v123; /*0x10147e01*/
            v149 = 7; /*0x10147e04*/
            dword_103F6DD8 = sub_1017C730(a1: "CBasePlayer", a2: "m_lifeState"); /*0x10147e1d*/
            sub_10180A43(a1: &dword_103F6DDC); /*0x10147e27*/
            v123 = v142; /*0x10147e2f*/
            v124 = v139; /*0x10147e32*/
          }
        }
        if ( *(_BYTE *)(dword_103F6DD8 + v123) == 0 ) /*0x10147b12*/
        {
          v125 = *(int (__thiscall **)(int, _DWORD, _DWORD, _DWORD, _DWORD))(*(_DWORD *)dword_103EC770 + 296); /*0x10147b1c*/
          v149 = -1; /*0x10147b22*/
          v126 = v125(a1: dword_103EC770, a2: v131, a3: DWORD1(v131), a4: DWORD2(v131), a5: HIDWORD(v131)); /*0x10147b29*/
          *(_DWORD *)(v126 + 8) += v124; /*0x10147b2b*/
        }
        v127 = dword_103F536C; /*0x10147b2e*/
        if ( dword_103F6DDC > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10147b4f*/
        {
          sub_101809C3(a1: &dword_103F6DDC); /*0x10147e3f*/
          if ( dword_103F6DDC == -1 ) /*0x10147e4e*/
          {
            LODWORD(v139) = v127; /*0x10147e54*/
            LODWORD(v142) = v123; /*0x10147e57*/
            v149 = 7; /*0x10147e5a*/
            dword_103F6DD8 = sub_1017C730(a1: "CBasePlayer", a2: "m_lifeState"); /*0x10147e73*/
            sub_10180A43(a1: &dword_103F6DDC); /*0x10147e7d*/
            v123 = v142; /*0x10147e85*/
            v127 = v139; /*0x10147e88*/
          }
        }
        if ( *(_BYTE *)(dword_103F6DD8 + v123) == 0 ) /*0x10147b5e*/
        {
          v128 = *(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 296); /*0x10147b68*/
          v149 = -1; /*0x10147b6e*/
          v129 = v128(a1: dword_103EC770); /*0x10147b75*/
          *(_DWORD *)(v129 + 8) += v127; /*0x10147b77*/
        }
        byte_103EEAA9 = 1; /*0x10147b7a*/
      }
    }
  }
  else
  {
    __asm /*0x10147b83*/
    {
      vxorps xmm0, xmm0, xmm0
      vmovsd qword ptr [ebx], xmm0
    }
    *(_DWORD *)(_EBX + 8) = 0; /*0x10147b8b*/
  }
}