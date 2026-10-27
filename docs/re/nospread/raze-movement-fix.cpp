void __stdcall sub_10027EA0(int a1, int a2)
{
  double v27; // st7
  double v40; // [esp+0h] [ebp-54h]
  double v41; // [esp+0h] [ebp-54h]
  double v42; // [esp+0h] [ebp-54h]
  double v43; // [esp+0h] [ebp-54h]
  double v44; // [esp+0h] [ebp-54h]
  double v45; // [esp+8h] [ebp-4Ch]
  double v46; // [esp+8h] [ebp-4Ch]
  float v50; // [esp+24h] [ebp-30h]
  float v51; // [esp+28h] [ebp-2Ch]

  _EDI = a2; /*0x10027ea5*/
  _ESI = a1; /*0x10027ea9*/
  __asm /*0x10027ead*/
  {
    vmovss xmm1, dword ptr [esi+18h]
    vmovss xmm2, dword ptr [esi+1Ch]
    vxorps xmm0, xmm0, xmm0
    vucomiss xmm2, xmm0
  }
  __asm
  {
    vcvtss2sd xmm0, xmm1, xmm1
    vmovss dword ptr [esp+54h+var_40], xmm1
    vcvtss2sd xmm1, xmm2, xmm2
    vmovss xmm3, dword ptr [esi+20h]
    vmovups [esp+54h+var_18], xmm3
    vmovsd [esp+54h+var_4C], xmm0
    vmovsd [esp+54h+var_54], xmm1
    vmovss [esp+54h+var_44], xmm2
  }
  v51 = atan2(Y: v40, X: v45); /*0x10027f16*/
  __asm /*0x10027f1a*/
  {
    vmovss xmm0, [esp+54h+var_2C]
    vmulss xmm0, xmm0, ds:dword_10184F14
    vaddss xmm1, xmm0, ds:dword_10184F54
    vcmpltss xmm2, xmm0, ds:dword_10184F30
    vblendvps xmm0, xmm0, xmm1, xmm2
    vmovups [esp+54h+var_28], xmm0
    vmovss xmm0, [esp+54h+var_44]
    vmulss xmm0, xmm0, xmm0
    vmovss xmm1, dword ptr [esp+54h+var_40]
    vfmadd231ss xmm0, xmm1, xmm1
    vcvtss2sd xmm0, xmm0, xmm0
    vmovsd [esp+54h+var_40], xmm0
    vsqrtsd xmm0, xmm0, xmm0
    vcvtsd2ss xmm0, xmm0, xmm0
    vbroadcastss xmm1, ds:dword_1018513C
    vxorps xmm1, xmm1, [esp+54h+var_18]
    vcvtss2sd xmm0, xmm0, xmm0
    vcvtss2sd xmm1, xmm1, xmm1
    vmovsd [esp+54h+var_4C], xmm0
    vmovsd [esp+54h+var_54], xmm1
  }
  v27 = atan2(Y: v41, X: v46); /*0x10027f8e*/
  __asm /*0x10027f95*/
  {
    vmovsd xmm1, [esp+54h+var_40]
    vmovsd [esp+54h+var_54], xmm1
  }
  sub_10028030(a1: v42); /*0x10027fa0*/
  v50 = v27; /*0x10027fa8*/
  __asm /*0x10027fac*/
  {
    vmovss xmm0, [esp+54h+var_30]
    vmovss dword ptr [esp+54h+var_18], xmm0
    vmovss xmm0, dword ptr [edi+4]
    vsubss xmm0, xmm0, dword ptr [esi+10h]
    vaddss xmm0, xmm0, dword ptr [esp+54h+var_28]
    vmulss xmm0, xmm0, ds:dword_10184F10
    vcvtss2sd xmm0, xmm0, xmm0
    vmovsd qword ptr [esp+54h+var_28], xmm0
    vmovsd [esp+54h+var_54], xmm0
  }
  cos(X: v43); /*0x10027fdf*/
  __asm /*0x10027fe8*/
  {
    vmovss xmm0, dword ptr [esp+54h+var_18]
    vmulss xmm0, xmm0, [esp+54h+var_34]
    vmovss dword ptr [esi+18h], xmm0
    vmovsd xmm0, qword ptr [esp+54h+var_28]
    vmovsd [esp+54h+var_54], xmm0
  }
  sin(X: v44); /*0x10028004*/
  __asm /*0x1002800d*/
  {
    vmovss xmm0, dword ptr [esp+54h+var_18]
    vmulss xmm0, xmm0, [esp+54h+var_38]
    vmovss dword ptr [esi+1Ch], xmm0
  }
}
