_BYTE *__stdcall sub_1016A520(_BYTE *a1, int a2, int a3)
{
  _BYTE *v3; // edi
  int v4; // ebx
  char v9; // al
  int v13; // esi
  int (__thiscall *v14)(int); // eax
  int v15; // eax
  int v17; // ecx
  int v18; // ebx
  int v19; // eax
  char v20; // cl
  int v21; // [esp+0h] [ebp-28h] BYREF
  int *v24; // [esp+Ch] [ebp-1Ch]
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp+10h] [ebp-18h]
  void *v26; // [esp+14h] [ebp-14h]
  int v27; // [esp+18h] [ebp-10h]

  v3 = a1; /*0x1016a529*/
  v4 = a3; /*0x1016a52c*/
  v24 = &v21; /*0x1016a532*/
  v27 = -1; /*0x1016a535*/
  v26 = &loc_1016BB30; /*0x1016a53f*/
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList; /*0x1016a54d*/
  if ( a3 != 0 && a2 != 0 ) /*0x1016a558*/
  {
    __asm /*0x1016a568*/
    {
      vmovss xmm0, ds:dword_10184380
      vmulss xmm1, xmm0, dword ptr qword_103F4C74
      vmovss [ebp+var_20], xmm1
      vmulss xmm1, xmm0, dword ptr qword_103F4C74+4
      vmovss [ebp+var_24], xmm1
      vmulss xmm0, xmm0, dword_103F4C7C
      vmovss [ebp+var_28], xmm0
    }
    v9 = (*(int (__thiscall **)(int))(*(_DWORD *)a3 + 572))(a1: a3); /*0x1016a59b*/
    if ( a3 != a2 && v9 != 0 && (*(unsigned __int8 (__thiscall **)(int))(*(_DWORD *)a3 + 960))(a1: a3) != 0 ) /*0x1016a5b5*/
    {
      __asm /*0x1016a5b7*/
      {
        vcvttss2si eax, [ebp+var_20]
        vcvttss2si ecx, [ebp+var_24]
        vcvttss2si edx, [ebp+var_28]
      }
      *a1 = _EAX; /*0x1016a5c6*/
      a1[1] = _ECX; /*0x1016a5c8*/
      a1[2] = _EDX; /*0x1016a5cb*/
      a1[3] = -1; /*0x1016a5ce*/
      return v3; /*0x1016a5d2*/
    }
    if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1016a5ef*/
    {
      sub_101809C3(a1: &dword_103EEAD8); /*0x1016a6b6*/
      if ( dword_103EEAD8 == -1 ) /*0x1016a6c5*/
      {
        v27 = 0; /*0x1016a6cb*/
        dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x1016a6e4*/
        sub_10180A43(a1: &dword_103EEAD8); /*0x1016a6ee*/
        v3 = a1; /*0x1016a6f6*/
        v4 = a3; /*0x1016a6f9*/
      }
    }
    if ( *(_DWORD *)(dword_103EEAD4 + v4) == 3 ) /*0x1016a5fe*/
    {
      if ( dword_103EF0D4 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1016a61b*/
      {
        sub_101809C3(a1: &dword_103EF0D4); /*0x1016a706*/
        if ( dword_103EF0D4 == -1 ) /*0x1016a715*/
        {
          v27 = 1; /*0x1016a71b*/
          dword_103EF0D0 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_iMaxHealth"); /*0x1016a734*/
          sub_10180A43(a1: &dword_103EF0D4); /*0x1016a73e*/
          v3 = a1; /*0x1016a746*/
          v4 = a3; /*0x1016a749*/
        }
      }
      v13 = *(_DWORD *)(dword_103EF0D0 + v4); /*0x1016a626*/
      v14 = *(int (__thiscall **)(int))(*(_DWORD *)v4 + 468); /*0x1016a62b*/
      v27 = -1; /*0x1016a631*/
      v15 = v14(a1: v4); /*0x1016a63a*/
      if ( v15 <= v13 ) /*0x1016a63e*/
      {
        v17 = v15 & ~(v15 >> 31); /*0x1016a669*/
        LOBYTE(v18) = -46; /*0x1016a682*/
        if ( 510 * (v13 - v17) / v13 < 210 ) /*0x1016a687*/
          v18 = 510 * (v13 - v17) / v13; /*0x1016a687*/
        v19 = 510 * v17 / v13; /*0x1016a695*/
        v20 = -26; /*0x1016a69c*/
        if ( v19 < 230 ) /*0x1016a6a1*/
          v20 = v19; /*0x1016a6a1*/
        *v3 = v18; /*0x1016a6a4*/
        v3[1] = v20; /*0x1016a6a6*/
        *((_WORD *)v3 + 1) = -206; /*0x1016a6a9*/
      }
      else
      {
        *(_DWORD *)v3 = dword_1029E0DC; /*0x1016a645*/
      }
      return v3; /*0x1016a64f*/
    }
  }
  *(_DWORD *)v3 = -1; /*0x1016a649*/
  return v3; /*0x1016a649*/
}