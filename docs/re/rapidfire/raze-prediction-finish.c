void __stdcall sub_101471E0(int a1, int a2)
{
  int v4; // esi
  int v5; // ecx
  int v10; // esi
  int v13; // ecx

  if ( byte_103F5350 == 1 ) /*0x101471e7*/
  {
    _EDI = a1; /*0x101471f4*/
    v4 = *(_DWORD *)(a2 + 4); /*0x101471f8*/
    if ( GetAsyncKeyState(vKey: dword_103F5354) < 0 ) /*0x1014720a*/
    {
      _ECX = dword_103EC8D0; /*0x1014723c*/
      __asm /*0x10147242*/
      {
        vmovss xmm0, dword ptr [ecx+1Ch]
        vmovss dword ptr [edi+10h], xmm0
      }
      *(_DWORD *)(a1 + 20) = *(_DWORD *)(a2 + 4); /*0x1014724f*/
      __asm /*0x10147258*/
      {
        vxorps xmm0, xmm0, xmm0
        vdivss xmm1, xmm0, dword ptr [ecx+1Ch]
      }
      v10 = v4 % 150; /*0x10147261*/
      __asm /*0x10147263*/
      {
        vaddss xmm0, xmm1, xmm0
        vcvttss2si eax, xmm0
      }
      if ( dword_103F535C > _EAX ) /*0x10147273*/
        _EAX = dword_103F535C; /*0x10147273*/
      v13 = 0; /*0x10147276*/
      if ( _EAX >= 0 ) /*0x1014727a*/
        v13 = _EAX; /*0x1014727a*/
      __asm { vcvtsi2ss xmm0, xmm2, ecx } /*0x1014727d*/
      dword_103F6DBC = _EAX; /*0x10147281*/
      dword_103F690C[2 * v10] = v13; /*0x10147286*/
      __asm /*0x1014728d*/
      {
        vaddss xmm0, xmm0, ds:dword_10184184
        vmulss xmm0, xmm0, dword ptr [edi+10h]
        vmovss dword ptr [edi+10h], xmm0
      }
      byte_103F6908 = 1; /*0x1014729f*/
    }
    else
    {
      v5 = 1; /*0x10147212*/
      if ( *(int *)(a1 + 16) >= 2 ) /*0x10147217*/
        v5 = *(_DWORD *)(a1 + 16); /*0x10147217*/
      *(_DWORD *)(a1 + 16) = v5 - 1; /*0x1014721b*/
    }
  }
}