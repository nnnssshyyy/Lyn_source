char __fastcall sub_1016CF10(int a1, int a2, int a3, int a4, int a5)
{
  int v7; // edi
  bool v8; // cl
  int v10; // eax
  int v11; // esi
  int v12; // ebx
  int v13; // esi
  int v14; // ecx
  void (__thiscall *v21)(int, int, int); // eax
  int v26; // [esp+10h] [ebp-34h]
  int v27; // [esp+14h] [ebp-30h]
  int v28; // [esp+14h] [ebp-30h]
  int v29; // [esp+18h] [ebp-2Ch] BYREF
  int v30; // [esp+1Ch] [ebp-28h]
  int v31; // [esp+20h] [ebp-24h]
  int v32; // [esp+24h] [ebp-20h]
  int *v33; // [esp+28h] [ebp-1Ch]
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp+2Ch] [ebp-18h]
  void *v35; // [esp+30h] [ebp-14h]
  int v36; // [esp+34h] [ebp-10h]

  v7 = a3; /*0x1016cf1d*/
  v33 = &v29; /*0x1016cf20*/
  v36 = -1; /*0x1016cf23*/
  v35 = &loc_101769A0; /*0x1016cf2d*/
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList; /*0x1016cf3b*/
  if ( byte_103EEA60 != 1 /*0x1016cf64*/
    || (_EAX = sub_10179540(a1: off_1029E014),
        v8 = _EAX != 0,
        LOBYTE(_EAX) = a3 == _EAX,
        ((unsigned __int8)_EAX & v8) == 0) )
  {
    v29 = a1; /*0x1016cf6a*/
    v32 = a2; /*0x1016cf6d*/
    v10 = sub_10179540(a1: off_1029E014); /*0x1016cf76*/
    if ( v10 != 0 ) /*0x1016cf7d*/
    {
      v11 = v10; /*0x1016cf83*/
      v12 = dword_103F690C[2 * (*(_DWORD *)(a4 + 4) % 150)]; /*0x1016cfa6*/
      if ( dword_103EEAF0 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1016cfc8*/
      {
        sub_101809C3(a1: &dword_103EEAF0); /*0x1016d143*/
        if ( dword_103EEAF0 == -1 ) /*0x1016d152*/
        {
          v30 = v12; /*0x1016d158*/
          v31 = v11; /*0x1016d15b*/
          v36 = 0; /*0x1016d15e*/
          dword_103EEAEC = sub_1017C730(a1: "CBasePlayer", a2: "m_nTickBase"); /*0x1016d177*/
          sub_10180A43(a1: &dword_103EEAF0); /*0x1016d181*/
          v7 = a3; /*0x1016d189*/
          v11 = v31; /*0x1016d18c*/
          v12 = v30; /*0x1016d18f*/
        }
      }
      *(_DWORD *)(dword_103EEAEC + v11) -= v12; /*0x1016cfd3*/
      if ( v7 == v11 ) /*0x1016cfd8*/
      {
        if ( dword_103EEAF0 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1016cff5*/
        {
          sub_101809C3(a1: &dword_103EEAF0); /*0x1016d19c*/
          if ( dword_103EEAF0 == -1 ) /*0x1016d1ab*/
          {
            v31 = v11; /*0x1016d1b1*/
            v36 = 1; /*0x1016d1b4*/
            dword_103EEAEC = sub_1017C730(a1: "CBasePlayer", a2: "m_nTickBase"); /*0x1016d1cd*/
            sub_10180A43(a1: &dword_103EEAF0); /*0x1016d1d7*/
            v7 = a3; /*0x1016d1df*/
            v11 = v31; /*0x1016d1e2*/
          }
        }
        dword_103F7460 = *(_DWORD *)(dword_103EEAEC + v11); /*0x1016d003*/
        v36 = -1; /*0x1016d008*/
      }
    }
    v36 = -1; /*0x1016d012*/
    dword_103F745C(a1: v29, a2: v32, a3: v7, a4, a5); /*0x1016d024*/
    dword_103EF050 = a5; /*0x1016d02a*/
    _EAX = sub_10179540(a1: off_1029E014); /*0x1016d036*/
    if ( _EAX != 0 && byte_103F5D42 == 1 && byte_103EEA60 == 0 ) /*0x1016d057*/
    {
      v13 = _EAX; /*0x1016d05d*/
      if ( v7 == _EAX ) /*0x1016d061*/
      {
        LOBYTE(_EAX) = a4; /*0x1016d067*/
        if ( *(_BYTE *)(a4 + 60) == 0 ) /*0x1016d06e*/
        {
          if ( dword_103EF190 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1016d08f*/
          {
            sub_101809C3(a1: &dword_103EF190); /*0x1016d1ef*/
            if ( dword_103EF190 == -1 ) /*0x1016d1fe*/
            {
              v32 = v13; /*0x1016d204*/
              v36 = 2; /*0x1016d207*/
              dword_103EF18C = sub_1017C730(a1: "CTerrorPlayer", a2: "m_iThrowGrenadeCounter") - 24; /*0x1016d223*/
              sub_10180A43(a1: &dword_103EF190); /*0x1016d22d*/
              v7 = a3; /*0x1016d235*/
              v13 = v32; /*0x1016d238*/
            }
          }
          LOBYTE(_EAX) = dword_103EF18C; /*0x1016d095*/
          v14 = *(_DWORD *)(dword_103EF18C + v7); /*0x1016d09a*/
          if ( v14 != 0 ) /*0x1016d09f*/
          {
            _EAX = dword_103EC8D0; /*0x1016d0a5*/
            __asm /*0x1016d0aa*/
            {
              vmovss xmm0, dword ptr [eax+10h]
              vmovss [ebp+var_20], xmm0
            }
            __asm { vxorps xmm0, xmm0, xmm0 }
            if ( *(_BYTE *)(dword_103EC918 + 10) == 0 ) /*0x1016d0c2*/
            {
              __asm { vmovss xmm0, dword ptr [eax+1Ch] } /*0x1016d0c4*/
              v36 = -1; /*0x1016d0c9*/
            }
            __asm { vmovss dword ptr [eax+10h], xmm0 } /*0x1016d0d0*/
            _EAX = a4; /*0x1016d0d5*/
            __asm /*0x1016d0d8*/
            {
              vmovss xmm0, dword ptr [eax+0Ch]
              vmovss xmm1, dword ptr [eax+10h]
            }
            v21 = *(void (__thiscall **)(int, int, int))(*(_DWORD *)v14 + 12); /*0x1016d0e4*/
            v36 = -1; /*0x1016d0e7*/
            __asm /*0x1016d0f1*/
            {
              vmovss [esp+34h+var_30], xmm0
              vmovss [esp+34h+var_34], xmm1
            }
            v21(a1: v14, a2: v26, a3: v27); /*0x1016d0fc*/
            _EAX = dword_103EC8D0; /*0x1016d0fe*/
            __asm { vmovss xmm0, dword ptr [eax+10h] } /*0x1016d103*/
            __asm { vmovss [esp+30h+var_30], xmm0 }
            (*(void (__thiscall **)(int, int))(*(_DWORD *)v13 + 800))(a1: v13, a2: v28); /*0x1016d114*/
            _EAX = dword_103EC8D0; /*0x1016d11c*/
            __asm /*0x1016d121*/
            {
              vmovss xmm0, [ebp+var_20]
              vmovss dword ptr [eax+10h], xmm0
            }
          }
        }
      }
    }
  }
  return _EAX; /*0x1016d12b*/
}