char __stdcall NoSpread_ShouldRun(int a1, int a2, int a3)
{
  int v3; // esi
  char result; // al
  int v5; // ecx
  int v6; // [esp+0h] [ebp-1Ch] BYREF
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp+4h] [ebp-18h]
  void *v8; // [esp+8h] [ebp-14h]
  int v9; // [esp+Ch] [ebp-10h]

  v6 = (int)&v6; /*0x10026bbc*/
  v9 = -1; /*0x10026bbf*/
  v8 = &loc_10027070; /*0x10026bc9*/
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList; /*0x10026bd7*/
  if ( (*(_DWORD *)(a3 + 36) & 0x21) != 1 ) /*0x10026bea*/
    return 0; /*0x10026bea*/
  if ( dword_103EEF3C > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10026c07*/
  {
    sub_101809C3(a1: &dword_103EEF3C); /*0x10026c83*/
    if ( dword_103EEF3C == -1 ) /*0x10026c92*/
    {
      v9 = 0; /*0x10026c98*/
      dword_103EEF38 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_isHangingFromLedge"); /*0x10026cb1*/
      sub_10180A43(a1: &dword_103EEF3C); /*0x10026cbb*/
    }
  }
  v3 = a1; /*0x10026c09*/
  if ( *(_BYTE *)(dword_103EEF38 + a1) != 0 ) /*0x10026c15*/
    return 0; /*0x10026c15*/
  if ( dword_103EEF44 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10026c32*/
  {
    sub_101809C3(a1: &dword_103EEF44); /*0x10026ccd*/
    if ( dword_103EEF44 == -1 ) /*0x10026cdc*/
    {
      v9 = 1; /*0x10026ce2*/
      dword_103EEF40 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_isHangingFromTongue"); /*0x10026cfb*/
      sub_10180A43(a1: &dword_103EEF44); /*0x10026d05*/
      v3 = a1; /*0x10026d0d*/
    }
  }
  if ( *(_BYTE *)(dword_103EEF40 + v3) != 0 ) /*0x10026c41*/
    return 0; /*0x10026c43*/
  v5 = (*(int (__thiscall **)(int, int, struct _EXCEPTION_REGISTRATION_RECORD *, void *, int))(*(_DWORD *)a2 + 1532))( /*0x10026c6f*/
         a1: a2,
         a2: v6,
         a3: ExceptionList,
         a4: v8,
         a5: -1)
     - 1;
  result = 1; /*0x10026c75*/
  switch ( v5 ) /*0x10026c77*/
  {
    case 0: /*0x10026c77*/
    case 1: /*0x10026c77*/
    case 4: /*0x10026c77*/
    case 5: /*0x10026c77*/
    case 6: /*0x10026c77*/
    case 8: /*0x10026c77*/
    case 9: /*0x10026c77*/
    case 25: /*0x10026c77*/
    case 31: /*0x10026c77*/
    case 32: /*0x10026c77*/
    case 33: /*0x10026c77*/
    case 34: /*0x10026c77*/
    case 35: /*0x10026c77*/
    case 36: /*0x10026c77*/
      return result; // Jump table 0x10185140 indexed by weapon_id-1. Allowed IDs: 1,2,5,6,7,9,10,26,32-37. Shotguns 3/4/8/11 map to xor eax,eax at0x10026c43 (false).
    default:
      return 0;
  }
}
