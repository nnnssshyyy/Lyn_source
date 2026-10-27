__int16 __stdcall sub_10146F50(int a1)
{
  _DWORD *v1; // eax
  _DWORD *v2; // edi
  int v3; // esi
  int (__thiscall *v4)(int); // eax
  int (__thiscall *v5)(int); // eax
  int v7; // [esp+0h] [ebp-24h] BYREF
  _DWORD *v8; // [esp+4h] [ebp-20h]
  int *v9; // [esp+8h] [ebp-1Ch]
  _DWORD v10[2]; // [esp+Ch] [ebp-18h] BYREF
  int v11; // [esp+14h] [ebp-10h]

  v9 = &v7; /*0x10146f59*/
  v11 = -1; /*0x10146f5c*/
  v1 = v10; /*0x10146f63*/
  v10[1] = &loc_101491B0; /*0x10146f66*/
  v10[0] = NtCurrentTeb()->NtTib.ExceptionList; /*0x10146f74*/
  if ( byte_103F5360 == 1 ) /*0x10146f84*/
  {
    LOWORD(v1) = GetAsyncKeyState(vKey: dword_103F5364); /*0x10146f8c*/
    if ( (__int16)v1 < 0 ) /*0x10146f95*/
    {
      v1 = (_DWORD *)sub_10179540(a1: off_1029E014); /*0x10146f9d*/
      if ( v1 != nullptr ) /*0x10146fa4*/
      {
        v2 = v1; /*0x10146fa6*/
        v3 = dword_103F536C; /*0x10146fa8*/
        if ( dword_103F6DDC > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10146fc9*/
        {
          sub_101809C3(a1: &dword_103F6DDC); /*0x10147062*/
          if ( dword_103F6DDC == -1 ) /*0x10147071*/
          {
            v8 = v2; /*0x10147077*/
            v7 = v3; /*0x1014707a*/
            v11 = 0; /*0x1014707d*/
            dword_103F6DD8 = sub_1017C730(a1: "CBasePlayer", a2: "m_lifeState"); /*0x10147096*/
            sub_10180A43(a1: &dword_103F6DDC); /*0x101470a0*/
            v3 = v7; /*0x101470a8*/
            v2 = v8; /*0x101470ab*/
          }
        }
        LOWORD(v1) = dword_103F6DD8; /*0x10146fcf*/
        if ( *((_BYTE *)v2 + dword_103F6DD8) == 0 ) /*0x10146fd8*/
        {
          v4 = *(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 296); /*0x10146fe2*/
          v11 = -1; /*0x10146fe8*/
          v1 = (_DWORD *)v4(a1: dword_103EC770); /*0x10146fef*/
          v1[2] += v3; /*0x10146ff1*/
        }
        byte_103F6908 = 1; /*0x10146ff4*/
      }
    }
  }
  if ( byte_103F5370 == 1 ) /*0x10147002*/
  {
    LOWORD(v1) = GetAsyncKeyState(vKey: dword_103F5374); /*0x1014700a*/
    if ( (__int16)v1 < 0 ) /*0x10147013*/
    {
      v5 = *(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 296); /*0x1014701d*/
      v11 = -1; /*0x10147023*/
      v1 = (_DWORD *)v5(a1: dword_103EC770); /*0x1014702a*/
      if ( v1 != nullptr ) /*0x1014702e*/
      {
        LOWORD(v1) = dword_103F537C; /*0x10147030*/
        *(_DWORD *)(*(_DWORD *)(dword_103EC9A8 + 24) + 8) += dword_103F537C; /*0x1014703e*/
      }
      else
      {
        byte_103F6908 = 1; /*0x10147043*/
      }
    }
  }
  return (__int16)v1; /*0x1014704a*/
}