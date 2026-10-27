int __fastcall sub_1016FF00(int a1, int a2, int a3, int a4)
{
  int v7; // eax
  int v8; // eax
  int v9; // esi
  int v10; // eax
  int v11; // eax
  int v12; // esi
  int v13; // eax
  int v14; // ecx
  int v15; // edx
  char v16; // cl
  char v19; // al
  char v20; // cl
  char v21; // al
  bool v22; // zf
  int v23; // ecx
  int v28; // ecx
  int v29; // eax
  int v33; // [esp+0h] [ebp-50h]
  unsigned int v34; // [esp+8h] [ebp-48h] BYREF
  int v35; // [esp+Ch] [ebp-44h]
  int v37; // [esp+20h] [ebp-30h]
  int v38; // [esp+24h] [ebp-2Ch]
  int v39; // [esp+28h] [ebp-28h]
  char *v40; // [esp+2Ch] [ebp-24h]
  int v41; // [esp+30h] [ebp-20h]
  int *v42; // [esp+34h] [ebp-1Ch]
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp+38h] [ebp-18h]
  void *v44; // [esp+3Ch] [ebp-14h]
  int v45; // [esp+40h] [ebp-10h]
  char ***vars0; // [esp+50h] [ebp+0h]

  _EBX = a4; /*0x1016ff0d*/
  v42 = (int *)&v34; /*0x1016ff10*/
  v45 = -1; /*0x1016ff13*/
  v44 = &loc_10176A00; /*0x1016ff1d*/
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList; /*0x1016ff2b*/
  byte_103EEAA9 = 0; /*0x1016ff34*/
  byte_103EEAAA = 0; /*0x1016ff3b*/
  byte_103EEA61 = 0; /*0x1016ff42*/
  v39 = sub_10179540(a1: off_1029E014); /*0x1016ff54*/
  if ( a4 == 0 ) /*0x1016ff59*/
    goto LABEL_110; /*0x1016ff59*/
  v7 = *(_DWORD *)(a4 + 4); /*0x1016ff5f*/
  if ( v7 == 0 ) /*0x1016ff64*/
    goto LABEL_110; /*0x1016ff64*/
  v38 = a1; /*0x1016ff6a*/
  v37 = a2; /*0x1016ff6d*/
  v34 = v7 - 150 * (((unsigned int)((unsigned __int64)(458129845LL * v7) >> 32) >> 31) + v7 / 0x96u); /*0x1016ff8b*/
  dword_103F690C[2 * v34] = 0; /*0x1016ff8e*/
  v8 = sub_10179540(a1: off_1029E014); /*0x1016ff9f*/
  if ( v8 != 0 && dword_103F6DBC == -1 ) /*0x1016ffaf*/
  {
    v9 = v8; /*0x1016ffb9*/
    sub_101471E0(a1: v8, a2: a4); /*0x1016ffbb*/
    sub_101486E0(a1: v9, a2: a4); /*0x1016ffc8*/
  }
  v35 = sub_10179540(a1: off_1029E014); /*0x1016ffd8*/
  sub_10179560(a1: off_1029E014); /*0x1016ffe1*/
  (*(void (__thiscall **)(int, _DWORD, bool, _DWORD, int))(*(_DWORD *)dword_103EC918 + 12))( /*0x10170015*/
    a1: dword_103EC918,
    a2: *(_DWORD *)(dword_103EC9A8 + 212),
    a3: *(_DWORD *)(dword_103EC9A8 + 212) > 0,
    a4: *(_DWORD *)(dword_103EC9A8 + 19028),
    a5: *(_DWORD *)(dword_103EC9A8 + 19020) + *(_DWORD *)(dword_103EC9A8 + 19024));
  if ( byte_103EEA60 == 1 ) /*0x1017001f*/
  {
    _EBX = a4; /*0x10170021*/
    v10 = *(_DWORD *)(a4 + 36); /*0x10170024*/
    a2 = v37; /*0x10170029*/
    a1 = v38; /*0x1017002c*/
    if ( (v10 & 2) != 0 ) /*0x1017002f*/
      *(_DWORD *)(a4 + 36) = v10 & 0xFFFFFFFD; /*0x10170038*/
    goto LABEL_110; /*0x1017003b*/
  }
  v40 = **vars0; /*0x10170047*/
  v11 = sub_10179540(a1: off_1029E014); /*0x10170050*/
  _EBX = a4; /*0x10170057*/
  if ( v11 != 0 ) /*0x1017005a*/
  {
    v12 = v11; /*0x1017005c*/
    if ( sub_10179560(a1: off_1029E014) != 0 ) /*0x1017006b*/
    {
      byte_103EEAA8 = sub_10023180(a1: v12); /*0x10170077*/
      byte_103F74EC = sub_10170B70(a1: v12); /*0x10170084*/
    }
  }
  if ( dword_103F73EC > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x101700a4*/
  {
    sub_101809C3(a1: &dword_103F73EC); /*0x1017075e*/
    if ( dword_103F73EC == -1 ) /*0x1017076d*/
    {
      byte_103F73E9 = byte_103EEAA8; /*0x1017077a*/
      sub_10180A43(a1: &dword_103F73EC); /*0x10170784*/
    }
  }
  a2 = v37; /*0x101700b7*/
  a1 = v38; /*0x101700ba*/
  if ( byte_103EEAA8 == byte_103F73E9 ) /*0x101700bd*/
  {
    if ( byte_103EEAA8 != 0 ) /*0x101700c1*/
      ++dword_103F74F0; /*0x101700c3*/
    else
      dword_103F74F0 = 0; /*0x101700dc*/
  }
  else
  {
    dword_103F74F0 = 0; /*0x101700cb*/
    byte_103F73E9 = byte_103EEAA8; /*0x101700d5*/
  }
  v40 -= 29; /*0x101700e6*/
  sub_101462D0(a1: a4); /*0x101700f1*/
  sub_101472B0(a1: v39, a2: a4); /*0x10170100*/
  sub_101493C0(a1: off_102A3C78); /*0x1017010b*/
  sub_1002D2B0(a1: a4); /*0x10170117*/
  if ( byte_103F5341 == 1 && byte_103F5340 == 1 ) /*0x1017012c*/
  {
    v13 = sub_10179540(a1: off_1029E014); /*0x10170134*/
    if ( v13 != 0 ) /*0x1017013b*/
    {
      v41 = v13; /*0x1017013d*/
      if ( dword_103EF058 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1017015b*/
      {
        sub_101809C3(a1: &dword_103EF058); /*0x101709e3*/
        if ( dword_103EF058 == -1 ) /*0x101709f2*/
        {
          v45 = 0; /*0x101709f8*/
          dword_103EF054 = sub_1017C730(a1: "CBasePlayer", a2: "m_fFlags"); /*0x10170a11*/
          sub_10180A43(a1: &dword_103EF058); /*0x10170a1b*/
          a2 = v37; /*0x10170a23*/
          a1 = v38; /*0x10170a26*/
          _EBX = a4; /*0x10170a29*/
        }
      }
      if ( (*(_BYTE *)(dword_103EF054 + v41) & 1) != 0 && (LOBYTE(off_103EBE18[0][5].m256_f32[5]) & 1) == 0 ) /*0x1017017b*/
      {
        *v40 = 0; /*0x10170180*/
        v45 = -1; /*0x10170183*/
      }
    }
  }
  v45 = -1; /*0x10170190*/
  sub_101652D0(a1: _EBX); /*0x10170198*/
  sub_10020070(a1: _EBX); /*0x101701a4*/
  sub_10148F80(a1: _EBX, a2: v40); /*0x101701b3*/
  sub_10146F50(a1: _EBX); /*0x101701bf*/
  sub_10026D60(a1: _EBX); /*0x101701cb*/
  sub_1002D780(a1: off_103EBE18[0]); /*0x101701d6*/
  if ( dword_103F73F4 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x101701f6*/
  {
    sub_101809C3(a1: &dword_103F73F4); /*0x10170796*/
    if ( dword_103F73F4 == -1 ) /*0x101707a5*/
    {
      dword_103F73F0 = dword_103EEA5C; /*0x101707b0*/
      sub_10180A43(a1: &dword_103F73F4); /*0x101707ba*/
    }
  }
  if ( dword_103EEA5C == dword_103F73F0 ) /*0x10170207*/
  {
    v14 = dword_103F74F4 + 1; /*0x1017020f*/
  }
  else
  {
    dword_103F73F0 = dword_103EEA5C; /*0x10170212*/
    v14 = 0; /*0x10170217*/
  }
  v15 = 0; /*0x10170219*/
  if ( dword_103EEA5C >= 2 ) /*0x1017021e*/
    v15 = v14; /*0x1017021e*/
  dword_103F74F4 = v15; /*0x10170221*/
  if ( byte_103F5CDC != 0 || (v16 = 1, byte_103F4BF2 == 0) ) /*0x1017023b*/
    v16 = byte_103F4BFC & (byte_103F5CDC ^ 1); /*0x10170245*/
  __asm /*0x10170247*/
  {
    vmovss xmm0, dword ptr [ebx+1Ch]
    vmovups xmm1, xmmword ptr [ebx+0Ch]
  }
  if ( v39 != 0 && v16 != 0 ) /*0x1017025d*/
  {
    if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1017027e*/
    {
      __asm /*0x1017091f*/
      {
        vmovss [ebp+var_20], xmm0
        vmovups [ebp+var_40], xmm1
      }
      sub_101809C3(a1: &dword_103EEAD8); /*0x10170929*/
      __asm /*0x1017092e*/
      {
        vmovups xmm1, [ebp+var_40]
        vmovss xmm0, [ebp+var_20]
      }
      if ( dword_103EEAD8 == -1 ) /*0x10170942*/
      {
        v45 = 1; /*0x10170948*/
        dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x10170961*/
        sub_10180A43(a1: &dword_103EEAD8); /*0x1017096b*/
        a2 = v37; /*0x10170973*/
        a1 = v38; /*0x10170976*/
        _EBX = a4; /*0x10170979*/
        __asm /*0x1017097c*/
        {
          vmovss xmm0, [ebp+var_20]
          vmovups xmm1, [ebp+var_40]
        }
      }
    }
    if ( *(_DWORD *)(dword_103EEAD4 + v39) != 3 ) /*0x10170290*/
    {
      __asm /*0x10170292*/
      {
        vmovups [ebp+var_40], xmm1
        vmovss [ebp+var_20], xmm0
      }
      v45 = -1; /*0x101702a2*/
      if ( sub_10179560(a1: off_1029E014) != 0 /*0x101702cf*/
        && (unsigned __int8)sub_10023180(a1: v39) != 0
        && (*(_DWORD *)(_EBX + 36) & 1) != 0
        && byte_103EEAAA == 1 )
      {
        *v40 = 0; /*0x101702d4*/
        __asm /*0x101702d7*/
        {
          vmovss xmm0, [ebp+var_20]
          vmovups xmm1, [ebp+var_40]
        }
      }
      else
      {
        *v40 = 1; /*0x101702e6*/
        __asm /*0x101702e9*/
        {
          vmovss xmm0, [ebp+var_20]
          vmovss dword ptr [ebx+1Ch], xmm0
          vmovups xmm1, [ebp+var_40]
          vmovups xmmword ptr [ebx+0Ch], xmm1
        }
      }
    }
  }
  if ( byte_103EEAA9 == 1 && dword_103F5CF8 == 0 && (v19 = 1, v20 = 1, byte_103F5CDC != 0) /*0x1017032a*/
    || (v19 = 0, v20 = 0, byte_103F73F8 == 1) )
  {
    *v40 = v19; /*0x1017032f*/
    byte_103F73F8 = v20; /*0x10170331*/
  }
  if ( dword_103F5CF8 == 3 && v39 != 0 && (byte_103F5CDC & 1) != 0 ) /*0x10170355*/
  {
    if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10170376*/
    {
      __asm /*0x1017083d*/
      {
        vmovss [ebp+var_20], xmm0
        vmovups [ebp+var_40], xmm1
      }
      sub_101809C3(a1: &dword_103EEAD8); /*0x10170847*/
      __asm /*0x1017084c*/
      {
        vmovups xmm1, [ebp+var_40]
        vmovss xmm0, [ebp+var_20]
      }
      if ( dword_103EEAD8 == -1 ) /*0x10170860*/
      {
        v45 = 2; /*0x10170866*/
        dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x1017087f*/
        sub_10180A43(a1: &dword_103EEAD8); /*0x10170889*/
        a2 = v37; /*0x10170891*/
        a1 = v38; /*0x10170894*/
        _EBX = a4; /*0x10170897*/
        __asm /*0x1017089a*/
        {
          vmovss xmm0, [ebp+var_20]
          vmovups xmm1, [ebp+var_40]
        }
      }
    }
    if ( *(_DWORD *)(dword_103EEAD4 + v39) != 3 ) /*0x10170388*/
    {
      __asm /*0x1017038a*/
      {
        vmovups [ebp+var_40], xmm1
        vmovss [ebp+var_20], xmm0
      }
      v45 = -1; /*0x1017039a*/
      if ( sub_10179560(a1: off_1029E014) != 0 /*0x101703c7*/
        && (unsigned __int8)sub_10023180(a1: v39) != 0
        && (*(_DWORD *)(_EBX + 36) & 1) != 0
        && byte_103EEAAA == 1 )
      {
        *v40 = 0; /*0x101703cc*/
        __asm /*0x101703cf*/
        {
          vmovss xmm0, [ebp+var_20]
          vmovups xmm1, [ebp+var_40]
        }
      }
      else
      {
        *v40 = 1; /*0x101703de*/
        __asm /*0x101703e1*/
        {
          vmovss xmm0, [ebp+var_20]
          vmovss dword ptr [ebx+1Ch], xmm0
          vmovups xmm1, [ebp+var_40]
          vmovups xmmword ptr [ebx+0Ch], xmm1
        }
      }
    }
  }
  if ( dword_103F5CF8 == 2 && v39 != 0 && (byte_103F5CDC & 1) != 0 ) /*0x10170413*/
  {
    if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10170434*/
    {
      __asm /*0x101708ae*/
      {
        vmovss [ebp+var_20], xmm0
        vmovups [ebp+var_40], xmm1
      }
      sub_101809C3(a1: &dword_103EEAD8); /*0x101708b8*/
      __asm /*0x101708bd*/
      {
        vmovups xmm1, [ebp+var_40]
        vmovss xmm0, [ebp+var_20]
      }
      if ( dword_103EEAD8 == -1 ) /*0x101708d1*/
      {
        v45 = 3; /*0x101708d7*/
        dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x101708f0*/
        sub_10180A43(a1: &dword_103EEAD8); /*0x101708fa*/
        a2 = v37; /*0x10170902*/
        a1 = v38; /*0x10170905*/
        _EBX = a4; /*0x10170908*/
        __asm /*0x1017090b*/
        {
          vmovss xmm0, [ebp+var_20]
          vmovups xmm1, [ebp+var_40]
        }
      }
    }
    if ( *(_DWORD *)(dword_103EEAD4 + v39) != 3 ) /*0x10170446*/
    {
      __asm /*0x10170448*/
      {
        vmovups [ebp+var_40], xmm1
        vmovss [ebp+var_20], xmm0
      }
      v45 = -1; /*0x10170458*/
      if ( sub_10179560(a1: off_1029E014) != 0 /*0x10170485*/
        && (unsigned __int8)sub_10023180(a1: v39) != 0
        && (*(_DWORD *)(_EBX + 36) & 1) != 0
        && byte_103EEAAA == 1 )
      {
        *v40 = 0; /*0x1017048a*/
        __asm /*0x1017048d*/
        {
          vmovss xmm0, [ebp+var_20]
          vmovups xmm1, [ebp+var_40]
        }
      }
      else
      {
        *v40 = 1; /*0x1017049c*/
        __asm /*0x1017049f*/
        {
          vmovss xmm0, [ebp+var_20]
          vmovss dword ptr [ebx+1Ch], xmm0
          vmovups xmm1, [ebp+var_40]
          vmovups xmmword ptr [ebx+0Ch], xmm1
        }
      }
    }
  }
  if ( byte_103EEAA9 == 1 && dword_103F5CF8 == 1 && v39 != 0 && (byte_103F5CDC & 1) != 0 ) /*0x101704d2*/
  {
    if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x101704ef*/
    {
      __asm /*0x101707cc*/
      {
        vmovss [ebp+var_20], xmm0
        vmovups [ebp+var_40], xmm1
      }
      sub_101809C3(a1: &dword_103EEAD8); /*0x101707d6*/
      __asm /*0x101707db*/
      {
        vmovups xmm1, [ebp+var_40]
        vmovss xmm0, [ebp+var_20]
      }
      if ( dword_103EEAD8 == -1 ) /*0x101707ef*/
      {
        v45 = 4; /*0x101707f5*/
        dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x1017080e*/
        sub_10180A43(a1: &dword_103EEAD8); /*0x10170818*/
        a2 = v37; /*0x10170820*/
        a1 = v38; /*0x10170823*/
        _EBX = a4; /*0x10170826*/
        __asm /*0x10170829*/
        {
          vmovss xmm0, [ebp+var_20]
          vmovups xmm1, [ebp+var_40]
        }
      }
    }
    if ( *(_DWORD *)(dword_103EEAD4 + v39) != 3 ) /*0x10170501*/
    {
      *v40 = 0; /*0x10170523*/
      v21 = 1; /*0x10170526*/
      goto LABEL_80; /*0x10170526*/
    }
  }
  if ( byte_103F73F9 == 1 ) /*0x1017050a*/
  {
    *v40 = 1; /*0x1017050f*/
    __asm /*0x10170512*/
    {
      vmovss dword ptr [ebx+1Ch], xmm0
      vmovups xmmword ptr [ebx+0Ch], xmm1
    }
    v21 = 0; /*0x1017051c*/
LABEL_80:
    byte_103F73F9 = v21; /*0x10170528*/
  }
  v45 = -1; /*0x1017052d*/
  if ( byte_103F5D43 == 1 ) /*0x1017053d*/
  {
    v41 = sub_10179540(a1: off_1029E014); /*0x1017054e*/
    if ( v41 != 0 ) /*0x10170553*/
    {
      if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10170574*/
      {
        sub_101809C3(a1: &dword_103EEAD8); /*0x10170990*/
        if ( dword_103EEAD8 == -1 ) /*0x1017099f*/
        {
          v45 = 5; /*0x101709a5*/
          dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x101709be*/
          sub_10180A43(a1: &dword_103EEAD8); /*0x101709c8*/
          a2 = v37; /*0x101709d0*/
          a1 = v38; /*0x101709d3*/
          _EBX = a4; /*0x101709d6*/
        }
      }
      if ( *(_DWORD *)(dword_103EEAD4 + v41) == 3 ) /*0x10170586*/
      {
        if ( dword_103EEA68 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x101705a7*/
        {
          sub_101809C3(a1: &dword_103EEA68); /*0x10170a36*/
          if ( dword_103EEA68 == -1 ) /*0x10170a45*/
          {
            v45 = 6; /*0x10170a4b*/
            dword_103EEA64 = sub_1017C730(a1: "CBasePlayer", a2: "deadflag"); /*0x10170a64*/
            sub_10180A43(a1: &dword_103EEA68); /*0x10170a6e*/
            a2 = v37; /*0x10170a76*/
            a1 = v38; /*0x10170a79*/
            _EBX = a4; /*0x10170a7c*/
          }
        }
        v22 = *(_DWORD *)(dword_103EEA64 + v35) == 0; /*0x101705b5*/
        v45 = -1; /*0x101705b9*/
        if ( v22 ) /*0x101705c0*/
        {
          if ( (*(_DWORD *)sub_10035DA0(a1: v35) == 4 || *(_DWORD *)sub_10035DA0(a1: v41) == 2) && dword_103F6DBC == -1 ) /*0x101705eb*/
          {
            if ( GetAsyncKeyState(vKey: dword_103F5D44) < 0 ) /*0x10170600*/
            {
              __asm /*0x1017061e*/
              {
                vmovss xmm0, ds:dword_10191CD0
                vdivss xmm0, xmm0, dword ptr [eax+1Ch]
                vaddss xmm0, xmm0, ds:dword_10184EDC
                vcvttss2si eax, xmm0
              }
              v28 = 150; /*0x1017063c*/
              if ( _EAX >= 151 ) /*0x10170641*/
                v28 = _EAX; /*0x10170641*/
              dword_103F6DBC = v28; /*0x10170644*/
              dword_103F690C[2 * v34] = v28; /*0x1017064d*/
              v29 = *(_DWORD *)(v41 + 16) + 1; /*0x1017065a*/
              if ( v29 >= v28 ) /*0x1017065d*/
                v29 = v28; /*0x1017065d*/
              *(_DWORD *)(v41 + 16) = v29; /*0x10170660*/
              byte_103F6908 = 1; /*0x10170663*/
              *(_DWORD *)(_EBX + 36) &= 0xFFFFF9E7; /*0x1017066a*/
              __asm /*0x10170671*/
              {
                vxorps xmm0, xmm0, xmm0
                vmovsd qword ptr [ebx+18h], xmm0
              }
            }
            else
            {
              v23 = 1; /*0x1017060b*/
              if ( *(int *)(v41 + 16) >= 2 ) /*0x10170610*/
                v23 = *(_DWORD *)(v41 + 16); /*0x10170610*/
              *(_DWORD *)(v41 + 16) = v23 - 1; /*0x10170614*/
            }
          }
          if ( (*(_DWORD *)sub_10035DA0(a1: v35) == 4 || *(_DWORD *)sub_10035DA0(a1: v41) == 2) /*0x101706b5*/
            && GetAsyncKeyState(vKey: dword_103F5D44) < 0
            && (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 296))(a1: dword_103EC770) != 0 )
          {
            *(_DWORD *)(*(_DWORD *)(dword_103EC9A8 + 24) + 28) = 200; /*0x101706bf*/
            byte_103F6908 = 1; /*0x101706c6*/
            *(_DWORD *)(_EBX + 36) &= 0xFFFFF9E7; /*0x101706cd*/
            __asm /*0x101706d4*/
            {
              vxorps xmm0, xmm0, xmm0
              vmovsd qword ptr [ebx+18h], xmm0
            }
          }
        }
      }
    }
  }
  v45 = -1; /*0x101706e3*/
  sub_10148C40(a1: v39, a2: _EBX); /*0x101706ee*/
  if ( byte_103F6908 == 1 && byte_103EEAAA == 0 ) /*0x10170703*/
    *v40 = 1; /*0x10170708*/
  if ( byte_103EEAA9 != 0 || byte_103EEAAA != 0 ) /*0x1017071b*/
    return 0; /*0x10170728*/
LABEL_110:
  __asm { vmovss xmm0, [ebp+arg_0] } /*0x1017072a*/
  __asm { vmovss [esp+50h+var_50], xmm0 }
  return dword_103F74E8(a1, a2, a3: v33, a4: _EBX); /*0x1017071d*/
}