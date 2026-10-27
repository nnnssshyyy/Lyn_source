int __thiscall NoSpread_PredictionEnd(_DWORD *this)
{
  int result; // eax
  int v3; // edi
  _DWORD v7[4]; // [esp+0h] [ebp-20h] BYREF
  int v8; // [esp+10h] [ebp-10h]

  _ESI = this; /*0x1002d789*/
  v7[1] = v7; /*0x1002d78b*/
  v8 = -1; /*0x1002d78e*/
  v7[3] = &loc_1002D8B0; /*0x1002d798*/
  v7[2] = NtCurrentTeb()->NtTib.ExceptionList; /*0x1002d7a6*/
  result = sub_10179540(); /*0x1002d7b5*/
  if ( result != 0 ) /*0x1002d7bc*/
  {
    v3 = result; /*0x1002d7be*/
    __asm { vmovss xmm0, dword ptr [esi+0A8h] } /*0x1002d7c0*/
    _EAX = dword_103EC8D0; /*0x1002d7c8*/
    __asm /*0x1002d7cd*/
    {
      vmovss dword ptr [eax+0Ch], xmm0
      vmovss xmm0, dword ptr [esi+0ACh]
      vmovss dword ptr [eax+10h], xmm0
    }
    *(_DWORD *)(_EAX + 24) = _ESI[44]; /*0x1002d7e5*/
    if ( dword_103EF060 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1002d803*/
    {
      sub_101809C3(a1: &dword_103EF060); /*0x1002d832*/
      if ( dword_103EF060 == -1 ) /*0x1002d841*/
      {
        v7[0] = v3; /*0x1002d843*/
        v8 = 0; /*0x1002d846*/
        dword_103EF05C = sub_1017C730(a1: "CBasePlayer", a2: "m_hConstraintEntity") - 8; /*0x1002d862*/
        sub_10180A43(a1: &dword_103EF060); /*0x1002d86c*/
        v3 = v7[0]; /*0x1002d874*/
      }
    }
    *(_DWORD *)(dword_103EF05C + v3) = 0; /*0x1002d80a*/
    result = dword_103ECBD4; /*0x1002d811*/
    *(_DWORD *)dword_103ECBD4 = -1; /*0x1002d816*/
  }
  return result; /*0x1002d81c*/
}
