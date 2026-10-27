int __stdcall RapidFire_ArmExtraMoves(int a1, int a2)
{
  int result; // eax
  int v5; // ebx
  int v6; // ecx
  int v7; // esi
  int v8; // ecx
  int v17; // esi
  int v18; // ecx
  int v23; // ecx
  int v28; // ecx
  int v29; // ecx
  int v34; // ecx
  int v35; // eax
  int v36; // ecx
  int v41; // ecx
  int v42; // ecx
  int v47; // ecx
  int v48; // [esp+0h] [ebp-24h] BYREF
  int v49; // [esp+4h] [ebp-20h]
  int *v50; // [esp+8h] [ebp-1Ch]
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp+Ch] [ebp-18h]
  void *v52; // [esp+10h] [ebp-14h]
  int v53; // [esp+14h] [ebp-10h]

  _EDI = a1; /*0x101486ec*/
  v50 = &v48; /*0x101486ef*/
  v53 = -1; /*0x101486f2*/
  v52 = &loc_101491F0; /*0x101486fc*/
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList; /*0x1014870a*/
  result = 150 * (*(_DWORD *)(a2 + 4) / 150); /*0x10148729*/
  v5 = *(_DWORD *)(a2 + 4) % 150; /*0x1014872f*/
  if ( byte_103F5380 != 1 ) /*0x10148738*/
    goto LABEL_6; /*0x10148738*/
  if ( GetAsyncKeyState(vKey: dword_103F5384) >= 0 ) /*0x10148749*/
  {
    result = *(_DWORD *)(a1 + 16); /*0x1014874f*/
    v6 = 1; /*0x10148755*/
    if ( result >= 2 ) /*0x1014875a*/
      v6 = *(_DWORD *)(a1 + 16); /*0x1014875a*/
    *(_DWORD *)(a1 + 16) = v6 - 1; /*0x1014875e*/
LABEL_6:
    if ( HIBYTE(word_103F53E0) != 1 ) /*0x10148768*/
      goto LABEL_16; /*0x10148768*/
    goto LABEL_7; /*0x10148768*/
  }
  _EAX = dword_103EC8D0; /*0x101487e8*/
  __asm /*0x101487ed*/
  {
    vmovss xmm0, dword ptr [eax+1Ch]
    vmovss dword ptr [edi+10h], xmm0
  }
  *(_DWORD *)(a1 + 20) = *(_DWORD *)(a2 + 4); /*0x101487fa*/
  __asm /*0x10148802*/
  {
    vmovss xmm0, ds:dword_101852F4
    vdivss xmm0, xmm0, dword ptr [eax+1Ch]
    vcvttss2si eax, xmm0
  }
  result = _EAX & ~(_EAX >> 31); /*0x10148818*/
  __asm { vcvtsi2ss xmm0, xmm1, eax } /*0x1014881d*/
  RapidFire_TickbaseShiftRing[2 * v5] = result; /*0x10148821*/
  __asm /*0x10148828*/
  {
    vaddss xmm0, xmm0, ds:dword_10184184
    vmulss xmm0, xmm0, dword ptr [edi+10h]
    vmovss dword ptr [edi+10h], xmm0
  }
  RapidFire_ForceSend = 1; /*0x1014883a*/
  if ( HIBYTE(word_103F53E0) != 1 ) /*0x10148848*/
    goto LABEL_16; /*0x10148848*/
LABEL_7:
  result = sub_10179540(); /*0x1014876e*/
  if ( result == 0 ) /*0x1014877b*/
    goto LABEL_16; /*0x1014877b*/
  v7 = result; /*0x10148781*/
  if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1014879e*/
  {
    sub_101809C3(a1: &dword_103EEAD8); /*0x10148b4e*/
    if ( dword_103EEAD8 == -1 ) /*0x10148b5d*/
    {
      v49 = v7; /*0x10148b63*/
      v48 = v5; /*0x10148b66*/
      v53 = 0; /*0x10148b69*/
      dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x10148b82*/
      sub_10180A43(a1: &dword_103EEAD8); /*0x10148b8c*/
      _EDI = a1; /*0x10148b94*/
      v5 = v48; /*0x10148b97*/
      v7 = v49; /*0x10148b9a*/
    }
  }
  result = dword_103EEAD4; /*0x101487a4*/
  if ( *(_DWORD *)(dword_103EEAD4 + v7) != 3 ) /*0x101487ad*/
  {
LABEL_16:
    if ( byte_103F53F0 != 1 ) /*0x10148855*/
      goto LABEL_39; /*0x10148855*/
    goto LABEL_17; /*0x10148855*/
  }
  if ( GetAsyncKeyState(vKey: dword_103F53E4) >= 0 ) /*0x101487c2*/
  {
    result = *(_DWORD *)(v7 + 16); /*0x101487c8*/
    v8 = 1; /*0x101487ce*/
    if ( result >= 2 ) /*0x101487d3*/
      v8 = *(_DWORD *)(v7 + 16); /*0x101487d3*/
    *(_DWORD *)(v7 + 16) = v8 - 1; /*0x101487d7*/
    if ( byte_103F53F0 != 1 ) /*0x101487e1*/
      goto LABEL_39; /*0x101487e1*/
LABEL_17:
    v53 = -1; /*0x1014885b*/
    result = sub_10179540(); /*0x10148868*/
    if ( result != 0 ) /*0x1014886f*/
    {
      v17 = result; /*0x10148875*/
      if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10148892*/
      {
        sub_101809C3(a1: &dword_103EEAD8); /*0x10148ba7*/
        if ( dword_103EEAD8 == -1 ) /*0x10148bb6*/
        {
          v49 = v17; /*0x10148bbc*/
          v48 = v5; /*0x10148bbf*/
          v53 = 1; /*0x10148bc2*/
          dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x10148bdb*/
          sub_10180A43(a1: &dword_103EEAD8); /*0x10148be5*/
          _EDI = a1; /*0x10148bed*/
          v5 = v48; /*0x10148bf0*/
          v17 = v49; /*0x10148bf3*/
        }
      }
      result = dword_103EEAD4; /*0x10148898*/
      if ( *(_DWORD *)(dword_103EEAD4 + v17) == 2 ) /*0x101488a1*/
      {
        if ( GetAsyncKeyState(vKey: dword_103F53F8) < 0 ) /*0x101488b6*/
        {
          __asm /*0x10148932*/
          {
            vmovss xmm0, ds:dword_10191CD0
            vdivss xmm0, xmm0, dword ptr [eax+1Ch]
            vaddss xmm0, xmm0, ds:dword_10184EDC
            vcvttss2si eax, xmm0
          }
          if ( dword_103F5400 > _EAX ) /*0x10148953*/
            _EAX = dword_103F5400; /*0x10148953*/
          v28 = 0; /*0x10148956*/
          if ( _EAX >= 0 ) /*0x1014895a*/
            v28 = _EAX; /*0x1014895a*/
          RapidFire_ExtraMovesRemaining = _EAX; /*0x1014895d*/
          RapidFire_TickbaseShiftRing[2 * v5] = v28; /*0x10148962*/
          result = *(_DWORD *)(v17 + 16) + 1; /*0x1014896c*/
          if ( result >= v28 ) /*0x1014896f*/
            result = v28; /*0x1014896f*/
          *(_DWORD *)(v17 + 16) = result; /*0x10148972*/
          RapidFire_ForceSend = 1; /*0x10148975*/
        }
        else
        {
          result = *(_DWORD *)(v17 + 16); /*0x101488b8*/
          v18 = 1; /*0x101488be*/
          if ( result >= 2 ) /*0x101488c3*/
            v18 = *(_DWORD *)(v17 + 16); /*0x101488c3*/
          *(_DWORD *)(v17 + 16) = v18 - 1; /*0x101488c7*/
        }
      }
    }
    goto LABEL_39; /*0x101488ca*/
  }
  __asm /*0x101488d4*/
  {
    vmovss xmm0, ds:dword_10191CD0
    vdivss xmm0, xmm0, dword ptr [eax+1Ch]
    vaddss xmm0, xmm0, ds:dword_10184EDC
    vcvttss2si eax, xmm0
  }
  if ( dword_103F53EC > _EAX ) /*0x101488f5*/
    _EAX = dword_103F53EC; /*0x101488f5*/
  v23 = 0; /*0x101488f8*/
  if ( _EAX >= 0 ) /*0x101488fc*/
    v23 = _EAX; /*0x101488fc*/
  RapidFire_ExtraMovesRemaining = _EAX; /*0x101488ff*/
  RapidFire_TickbaseShiftRing[2 * v5] = v23; /*0x10148904*/
  result = *(_DWORD *)(v7 + 16) + 1; /*0x1014890e*/
  if ( result >= v23 ) /*0x10148911*/
    result = v23; /*0x10148911*/
  *(_DWORD *)(v7 + 16) = result; /*0x10148914*/
  RapidFire_ForceSend = 1; /*0x10148917*/
  if ( byte_103F53F0 == 1 ) /*0x10148925*/
    goto LABEL_17; /*0x10148925*/
LABEL_39:
  if ( RapidFire_Enabled != 1 ) /*0x10148983*/
    return result; /*0x10148983*/
  result = RapidFire_Method; /*0x10148989*/
  if ( RapidFire_Method == 0 )
  {
    if ( GetAsyncKeyState(vKey: RapidFire_Key) < 0 )
    { // Rapidfire method=0: N=max(config StormSpeed, int(0.06f/interval+0.5f)); sets shared CL_Move counter and command_number%150 tickbase correction. No NextPrimaryAttack write in this block.
      __asm
      {
        vmovss xmm0, ds:dword_10191CD0; Rapidfire method=0: N=max(config StormSpeed, int(0.06f/interval+0.5f)); sets shared CL_Move counter and command_number%150 tickbase correction. No NextPrimaryAttack write in this block.
        vdivss xmm0, xmm0, dword ptr [eax+1Ch]
        vaddss xmm0, xmm0, ds:dword_10184EDC
        vcvttss2si eax, xmm0
      }
      if ( RapidFire_StormSpeed > _EAX ) /*0x10148a02*/
        _EAX = RapidFire_StormSpeed; /*0x10148a02*/
      v34 = 0; /*0x10148a05*/
      if ( _EAX >= 0 ) /*0x10148a09*/
        v34 = _EAX; /*0x10148a09*/
      RapidFire_ExtraMovesRemaining = _EAX; /*0x10148a0c*/
      RapidFire_TickbaseShiftRing[2 * v5] = v34; /*0x10148a11*/
      v35 = *(_DWORD *)(_EDI + 16) + 1; /*0x10148a1b*/
      if ( v35 >= v34 ) /*0x10148a1e*/
        v35 = v34; /*0x10148a1e*/
      *(_DWORD *)(_EDI + 16) = v35; /*0x10148a21*/
      RapidFire_ForceSend = 1; /*0x10148a24*/
      result = RapidFire_Method; /*0x10148a2b*/
      if ( RapidFire_Method == 1 ) /*0x10148a33*/
        goto LABEL_56; /*0x10148a33*/
    }
    else
    {
      v29 = 1; /*0x101489c4*/
      if ( *(int *)(_EDI + 16) >= 2 ) /*0x101489c9*/
        v29 = *(_DWORD *)(_EDI + 16); /*0x101489c9*/
      *(_DWORD *)(_EDI + 16) = v29 - 1; /*0x101489cd*/
      result = RapidFire_Method; /*0x101489d0*/
      if ( RapidFire_Method == 1 ) /*0x101489d8*/
        goto LABEL_56; /*0x101489d8*/
    }
LABEL_42:
    if ( RapidFire_Method != 2 ) /*0x101489a2*/
      return result; /*0x101489a2*/
    goto LABEL_68; /*0x101489a2*/
  }
  if ( RapidFire_Method != 1 ) /*0x10148995*/
    goto LABEL_42; /*0x10148995*/
LABEL_56:
  if ( GetAsyncKeyState(vKey: RapidFire_Key) >= 0 ) /*0x10148a48*/
  {
    result = *(_DWORD *)(_EDI + 16); /*0x10148a4a*/
    v36 = 1; /*0x10148a50*/
    if ( result >= 2 ) /*0x10148a55*/
      v36 = *(_DWORD *)(_EDI + 16); /*0x10148a55*/
    *(_DWORD *)(_EDI + 16) = v36 - 1; /*0x10148a59*/
    if ( RapidFire_Method != 2 ) /*0x10148a63*/
      return result; /*0x10148a63*/
LABEL_68:
    if ( GetAsyncKeyState(vKey: RapidFire_Key) < 0 ) /*0x10148ad1*/
    {
      __asm /*0x10148aec*/
      {
        vmovss xmm0, ds:dword_10191CD0
        vdivss xmm0, xmm0, dword ptr [eax+1Ch]
        vaddss xmm0, xmm0, ds:dword_10184EDC
        vcvttss2si eax, xmm0
      }
      if ( RapidFire_Method3Speed > _EAX ) /*0x10148b0d*/
        _EAX = RapidFire_Method3Speed; /*0x10148b0d*/
      v47 = 0; /*0x10148b10*/
      if ( _EAX >= 0 ) /*0x10148b14*/
        v47 = _EAX; /*0x10148b14*/
      RapidFire_ExtraMovesRemaining = _EAX; /*0x10148b17*/
      RapidFire_TickbaseShiftRing[2 * v5] = v47; /*0x10148b1c*/
      result = *(_DWORD *)(_EDI + 16) + 1; /*0x10148b26*/
      if ( result >= v47 ) /*0x10148b29*/
        result = v47; /*0x10148b29*/
      *(_DWORD *)(_EDI + 16) = result; /*0x10148b2c*/
      RapidFire_ForceSend = 1; /*0x10148b2f*/
    }
    else
    {
      result = *(_DWORD *)(_EDI + 16); /*0x10148ad3*/
      v42 = 1; /*0x10148ad9*/
      if ( result >= 2 ) /*0x10148ade*/
        v42 = *(_DWORD *)(_EDI + 16); /*0x10148ade*/
      *(_DWORD *)(_EDI + 16) = v42 - 1; /*0x10148ae2*/
    }
    return result; /*0x10148ae5*/
  }
  __asm /*0x10148a6f*/
  {
    vmovss xmm0, ds:dword_10191CD0
    vdivss xmm0, xmm0, dword ptr [eax+1Ch]
    vaddss xmm0, xmm0, ds:dword_10184EDC
    vcvttss2si eax, xmm0
  }
  if ( RapidFire_Method2Speed > _EAX ) /*0x10148a90*/
    _EAX = RapidFire_Method2Speed; /*0x10148a90*/
  v41 = 0; /*0x10148a93*/
  if ( _EAX >= 0 ) /*0x10148a97*/
    v41 = _EAX; /*0x10148a97*/
  RapidFire_ExtraMovesRemaining = _EAX; /*0x10148a9a*/
  RapidFire_TickbaseShiftRing[2 * v5] = v41; /*0x10148a9f*/
  result = *(_DWORD *)(_EDI + 16) + 1; /*0x10148aa9*/
  if ( result >= v41 ) /*0x10148aac*/
    result = v41; /*0x10148aac*/
  *(_DWORD *)(_EDI + 16) = result; /*0x10148aaf*/
  RapidFire_ForceSend = 1; /*0x10148ab2*/
  if ( RapidFire_Method == 2 ) /*0x10148ac0*/
    goto LABEL_68; /*0x10148ac0*/
  return result; /*0x10148b36*/
}