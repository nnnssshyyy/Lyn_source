_DWORD *__userpurge NoSpread_PredictionBegin@<eax>(int a1@<ecx>, __int128 _XMM1@<xmm1>, int a3)
{
  _DWORD *result; // eax
  _DWORD *v5; // esi
  void (__thiscall *v6)(int, _DWORD *); // eax
  int v7; // ebx
  int v8; // eax
  _BYTE *v12; // ecx
  int v13; // eax
  void (__thiscall *v19)(int, _DWORD); // eax
  int v20; // [esp+0h] [ebp-2Ch] BYREF
  char v21; // [esp+6h] [ebp-26h]
  char v22; // [esp+7h] [ebp-25h]
  int v23; // [esp+8h] [ebp-24h]
  _DWORD *v24; // [esp+Ch] [ebp-20h]
  int *v25; // [esp+10h] [ebp-1Ch]
  _DWORD v26[2]; // [esp+14h] [ebp-18h] BYREF
  int v27; // [esp+1Ch] [ebp-10h]

  _EDI = a1; /*0x1002d2b9*/
  v25 = &v20; /*0x1002d2bb*/
  v27 = -1; /*0x1002d2be*/
  result = v26; /*0x1002d2c5*/
  v26[1] = &loc_1002D8A0; /*0x1002d2c8*/
  v26[0] = NtCurrentTeb()->NtTib.ExceptionList; /*0x1002d2d6*/
  if ( dword_103EF050 != 0 )
  {
    result = (_DWORD *)sub_10179540(); /*0x1002d2f2*/
    if ( result != nullptr )
    {
      v5 = result; /*0x1002d2ff*/
      if ( dword_103EF058 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1002d31c*/
      {
        sub_101809C3(a1: &dword_103EF058); /*0x1002d52c*/
        if ( dword_103EF058 == -1 ) /*0x1002d53b*/
        {
          v23 = _EDI; /*0x1002d541*/
          v24 = v5; /*0x1002d544*/
          v27 = 0; /*0x1002d547*/
          dword_103EF054 = sub_1017C730(a1: "CBasePlayer", a2: "m_fFlags"); /*0x1002d560*/
          sub_10180A43(a1: &dword_103EF058); /*0x1002d56a*/
          v5 = v24; /*0x1002d572*/
          _EDI = v23; /*0x1002d575*/
        }
      }
      *(_DWORD *)(_EDI + 180) = *(_DWORD *)((char *)v5 + dword_103EF054); /*0x1002d32a*/
      v6 = *(void (__thiscall **)(int, _DWORD *))(*(_DWORD *)dword_103EF050 + 4); /*0x1002d338*/
      v27 = -1; /*0x1002d33b*/
      v6(a1: dword_103EF050, a2: v5); /*0x1002d343*/
      if ( dword_103EF060 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1002d360*/
      {
        sub_101809C3(a1: &dword_103EF060); /*0x1002d582*/
        if ( dword_103EF060 == -1 ) /*0x1002d591*/
        {
          v23 = _EDI; /*0x1002d597*/
          v24 = v5; /*0x1002d59a*/
          v27 = 1; /*0x1002d59d*/
          dword_103EF05C = sub_1017C730(a1: "CBasePlayer", a2: "m_hConstraintEntity") - 8; /*0x1002d5b9*/
          sub_10180A43(a1: &dword_103EF060); /*0x1002d5c3*/
          v5 = v24; /*0x1002d5cb*/
          _EDI = v23; /*0x1002d5ce*/
        }
      }
      v7 = a3; /*0x1002d366*/
      *(_DWORD *)((char *)v5 + dword_103EF05C) = a3; /*0x1002d36e*/
      v8 = *(_DWORD *)(a3 + 4); /*0x1002d371*/
      v27 = -1; /*0x1002d374*/
      *(_DWORD *)Prediction_SeedPointer = sub_1017C4A0(a1: v8) & 0x7FFFFFFF;// Set global prediction seed = MD5_PseudoRandom(original command_number) &0x7FFFFFFF. No fixed shotgun command-number replacement in this path. /*0x1002d38f*/
      _EAX = dword_103EC8D0; /*0x1002d391*/
      __asm /*0x1002d396*/
      {
        vmovss xmm0, dword ptr [eax+0Ch]
        vmovss dword ptr [edi+0A8h], xmm0
        vmovss xmm0, dword ptr [eax+10h]
        vmovss dword ptr [edi+0ACh], xmm0
      }
      *(_DWORD *)(_EDI + 176) = *(_DWORD *)(_EAX + 24); /*0x1002d3b3*/
      if ( dword_103EEAF0 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1002d3d4*/
      {
        sub_101809C3(a1: &dword_103EEAF0); /*0x1002d5db*/
        if ( dword_103EEAF0 == -1 ) /*0x1002d5ea*/
        {
          v23 = _EDI; /*0x1002d5f0*/
          v24 = v5; /*0x1002d5f3*/
          v27 = 2; /*0x1002d5f6*/
          dword_103EEAEC = sub_1017C730(a1: "CBasePlayer", a2: "m_nTickBase"); /*0x1002d60f*/
          sub_10180A43(a1: &dword_103EEAF0); /*0x1002d619*/
          v5 = v24; /*0x1002d621*/
          _EDI = v23; /*0x1002d624*/
          v7 = a3; /*0x1002d627*/
        }
      }
      v20 = *(_DWORD *)((char *)v5 + dword_103EEAEC); /*0x1002d3e2*/
      v12 = (_BYTE *)dword_103EC918; /*0x1002d3e5*/
      v22 = *(_BYTE *)(dword_103EC918 + 8); /*0x1002d3ef*/
      v21 = *(_BYTE *)(dword_103EC918 + 24); /*0x1002d3f6*/
      if ( dword_103EEF90 != 0 && *(_BYTE *)(dword_103EEF90 + 60) != 1 ) /*0x1002d406*/
      {
        v13 = dword_103EEF8C + 1; /*0x1002d43e*/
      }
      else
      {
        if ( dword_103EEAF0 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1002d423*/
        {
          sub_101809C3(a1: &dword_103EEAF0); /*0x1002d684*/
          if ( dword_103EEAF0 == -1 ) /*0x1002d693*/
          {
            v23 = _EDI; /*0x1002d699*/
            v24 = v5; /*0x1002d69c*/
            v27 = 3; /*0x1002d69f*/
            dword_103EEAEC = sub_1017C730(a1: "CBasePlayer", a2: "m_nTickBase"); /*0x1002d6b8*/
            sub_10180A43(a1: &dword_103EEAF0); /*0x1002d6c2*/
            v5 = v24; /*0x1002d6ca*/
            _EDI = v23; /*0x1002d6cd*/
            v7 = a3; /*0x1002d6d0*/
          }
        }
        v13 = *(_DWORD *)((char *)v5 + dword_103EEAEC); /*0x1002d42e*/
        v12 = (_BYTE *)dword_103EC918; /*0x1002d431*/
      }
      v27 = -1; /*0x1002d43f*/
      dword_103EEF8C = v13; /*0x1002d446*/
      dword_103EEF90 = v7; /*0x1002d44b*/
      _EDX = dword_103EC8D0; /*0x1002d451*/
      __asm
      { // Prediction context: current time=tick*interval; frame_time=interval or0 when prediction paused. NoSpread executes before PredictionEnd restores these globals.
        vmovss xmm0, dword ptr [edx+1Ch]; Prediction context: current time=tick*interval; frame_time=interval or0 when prediction paused. NoSpread executes before PredictionEnd restores these globals.
        vcvtsi2ss xmm1, xmm1, eax
        vmulss xmm1, xmm0, xmm1
        vmovss dword ptr [edx+0Ch], xmm1
      }
      __asm { vxorps xmm1, xmm1, xmm1 }
      if ( v12[10] == 0 ) /*0x1002d471*/
        __asm { vmovaps xmm1, xmm0 } /*0x1002d473*/
      __asm { vmovss dword ptr [edx+10h], xmm1 } /*0x1002d477*/
      *(_DWORD *)(_EDX + 24) = v13; /*0x1002d47c*/
      v12[24] = 0; /*0x1002d47f*/
      v12[8] = 1; /*0x1002d483*/
      (*(void (__thiscall **)(_BYTE *, int))(*(_DWORD *)v12 + 52))(a1: v12, a2: v7 + 12); /*0x1002d48d*/
      (*(void (__thiscall **)(int, _DWORD *, int, int, int))(*(_DWORD *)dword_103EC918 + 76))( /*0x1002d4a1*/
        a1: dword_103EC918,
        a2: v5,
        a3: v7,
        a4: dword_103EF050,
        a5: _EDI);
      (*(void (__thiscall **)(int, _DWORD *, int))(*(_DWORD *)dword_103EC930 + 4))(a1: dword_103EC930, a2: v5, a3: _EDI); /*0x1002d4ae*/
      (*(void (__thiscall **)(int, _DWORD *, int, int))(*(_DWORD *)dword_103EC918 + 80))( /*0x1002d4bc*/
        a1: dword_103EC918,
        a2: v5,
        a3: v7,
        a4: _EDI);
      if ( dword_103EEAF0 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1002d4da*/
      {
        sub_101809C3(a1: &dword_103EEAF0); /*0x1002d634*/
        if ( dword_103EEAF0 == -1 ) /*0x1002d643*/
        {
          v24 = v5; /*0x1002d649*/
          v27 = 4; /*0x1002d64c*/
          dword_103EEAEC = sub_1017C730(a1: "CBasePlayer", a2: "m_nTickBase"); /*0x1002d665*/
          sub_10180A43(a1: &dword_103EEAF0); /*0x1002d66f*/
          v5 = v24; /*0x1002d677*/
        }
      }
      *(_DWORD *)((char *)v5 + dword_103EEAEC) = v20; /*0x1002d4e8*/
      v19 = *(void (__thiscall **)(int, _DWORD))(*(_DWORD *)dword_103EF050 + 4); /*0x1002d4f3*/
      v27 = -1; /*0x1002d4f6*/
      v19(a1: dword_103EF050, a2: 0); /*0x1002d4ff*/
      result = (_DWORD *)dword_103EC918; /*0x1002d501*/
      *(_BYTE *)(dword_103EC918 + 8) = v22; /*0x1002d50a*/
      *((_BYTE *)result + 24) = v21; /*0x1002d511*/
    }
  }
  return result; /*0x1002d514*/
}
