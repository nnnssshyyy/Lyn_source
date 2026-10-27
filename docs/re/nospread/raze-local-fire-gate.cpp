bool __userpurge sub_10023180@<al>(int a1@<ecx>, __int128 _XMM0@<xmm0>, int a3)
{
  int v7; // ecx
  bool v8; // cf
  int v11; // ecx
  bool v12; // cf
  int v14; // [esp-4h] [ebp-28h] BYREF
  int v15; // [esp-4h] [ebp-28h] BYREF
  int v16; // [esp+0h] [ebp-24h] BYREF
  int *v18; // [esp+8h] [ebp-1Ch]
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp+Ch] [ebp-18h]
  void *v20; // [esp+10h] [ebp-14h]
  int v21; // [esp+14h] [ebp-10h]

  _EDI = a1; /*0x10023189*/
  _ESI = a3; /*0x1002318b*/
  v18 = &v16; /*0x1002318e*/
  v21 = -1; /*0x10023191*/
  v20 = &loc_10026B80; /*0x1002319b*/
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList; /*0x100231a9*/
  if ( a3 == 0 ) /*0x100231b4*/
    return false; /*0x100231b4*/
  if ( dword_103EEAF0 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x100231d1*/
  {
    sub_101809C3(a1: &dword_103EEAF0); /*0x10023263*/
    if ( dword_103EEAF0 == -1 ) /*0x10023272*/
    {
      v16 = _EDI; /*0x10023278*/
      v21 = 0; /*0x1002327b*/
      dword_103EEAEC = sub_1017C730(a1: "CBasePlayer", a2: "m_nTickBase"); /*0x10023294*/
      sub_10180A43(a1: &dword_103EEAF0); /*0x1002329e*/
      _ESI = a3; /*0x100232a6*/
      _EDI = v16; /*0x100232a9*/
    }
  }
  __asm { vcvtsi2ss xmm0, xmm0, dword ptr [eax+esi] } /*0x100231dc*/
  __asm { vmulss xmm0, xmm0, dword ptr [eax+1Ch] }
  v7 = *((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex); /*0x100231fd*/
  v8 = (unsigned int)dword_103EEAF8 < *(_DWORD *)(v7 + 4); /*0x10023200*/
  if ( dword_103EEAF8 > *(_DWORD *)(v7 + 4) ) /*0x10023206*/
  {
    __asm { vmovss [ebp+var_20], xmm0 } /*0x100232b6*/
    sub_101809C3(a1: &dword_103EEAF8); /*0x100232bb*/
    __asm { vmovss xmm0, [ebp+var_20] } /*0x100232c0*/
    v8 = dword_103EEAF8 != -1; /*0x100232c8*/
    if ( dword_103EEAF8 == -1 ) /*0x100232cf*/
    {
      v16 = _EDI; /*0x100232d5*/
      v21 = 1; /*0x100232d8*/
      dword_103EEAF4 = sub_1017C730(a1: "CBaseCombatWeapon", a2: "m_flNextPrimaryAttack"); /*0x100232f1*/
      sub_10180A43(a1: &dword_103EEAF8); /*0x100232fb*/
      v8 = __CFADD__(&v14, 4); /*0x10023300*/
      _ESI = a3; /*0x10023303*/
      _EDI = v16; /*0x10023306*/
      __asm { vmovss xmm0, [ebp+var_20] } /*0x10023309*/
    }
  }
  _EAX = dword_103EEAF4; /*0x1002320c*/
  __asm { vucomiss xmm0, dword ptr [eax+edi] } /*0x10023211*/
  if ( v8 ) /*0x10023216*/
    return false; /*0x10023218*/
  v11 = *((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex); /*0x1002322e*/
  v12 = (unsigned int)dword_103EEB00 < *(_DWORD *)(v11 + 4); /*0x10023231*/
  if ( dword_103EEB00 > *(_DWORD *)(v11 + 4) ) /*0x10023237*/
  {
    __asm { vmovss [ebp+var_20], xmm0 } /*0x10023318*/
    sub_101809C3(a1: &dword_103EEB00); /*0x1002331d*/
    __asm { vmovss xmm0, [ebp+var_20] } /*0x10023322*/
    v12 = dword_103EEB00 != -1; /*0x1002332a*/
    if ( dword_103EEB00 == -1 ) /*0x10023331*/
    {
      v21 = 2; /*0x10023337*/
      dword_103EEAFC = sub_1017C730(a1: "CBaseCombatCharacter", a2: "m_flNextAttack"); /*0x10023350*/
      sub_10180A43(a1: &dword_103EEB00); /*0x1002335a*/
      v12 = __CFADD__(&v15, 4); /*0x1002335f*/
      _ESI = a3; /*0x10023362*/
      __asm { vmovss xmm0, [ebp+var_20] } /*0x10023365*/
    }
  }
  _EAX = dword_103EEAFC; /*0x1002323d*/
  __asm { vucomiss xmm0, dword ptr [eax+esi] } /*0x10023242*/
  return !v12; /*0x1002324a*/
}
