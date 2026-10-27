// Identified structurally as C_BasePlayer::CreateMove from prologue, weapon/vehicle view processing, cached angles and frozen flag handling. Corresponds to Valve Source SDK C_BasePlayer::CreateMove; symbols inferred.
bool __thiscall L4D2_BasePlayer_CreateMove(void *self, float inputSampleTime, void *command)
{
  unsigned int v4; // ecx
  int *v5; // eax
  unsigned int v6; // ecx
  int v7; // eax
  int v8; // eax
  int v9; // eax
  int _8; // [esp+8h] [ebp+0h]
  void *_8a; // [esp+8h] [ebp+0h]
  char *_8b; // [esp+8h] [ebp+0h]
  int _8c; // [esp+8h] [ebp+0h]
  int _8d; // [esp+8h] [ebp+0h]
  int v17; // [esp+18h] [ebp+10h]

  v4 = *((_DWORD *)self + 1282); /*0x10065cd6*/
  if ( v4 == -1 /*0x10065d07*/
    || (v5 = (int *)((char *)off_106C50A8 + 16 * (v4 & 0xFFF) + 4),
        (_UNKNOWN *)((char *)off_106C50A8 + 16 * (v4 & 0xFFF)) == (_UNKNOWN *)-4)
    || (v6 = v4 >> 12, v5[1] != v6)
    || *v5 == 0 )
  {
    if ( *(_DWORD *)(dword_107598D4 + 48) != 0 ) /*0x10065d50*/
    {
      _8a = &unk_10759FA0; /*0x10065d5d*/
      if ( ((double (__thiscall *)(int *))*(_DWORD *)(*off_106E2F04 + 40))(a1: off_106E2F04) != 0.0 ) /*0x10065d70*/
        *(_DWORD *)(v17 + 36) |= 0x20000u; /*0x10065d72*/
    }
    v9 = (*(int (__thiscall **)(void *))(*(_DWORD *)self + 980))(a1: self); /*0x10065d83*/
    if ( v9 != 0 ) /*0x10065d87*/
    {
      _8b = (char *)self + 5308; /*0x10065d96*/
      (*(void (__thiscall **)(int))(*(_DWORD *)v9 + 1056))(a1: v9); /*0x10065da2*/
    }
  }
  else
  {
    if ( v5 != nullptr && v5[1] == v6 ) /*0x10065d10*/
      v7 = *v5; /*0x10065d12*/
    else
      v7 = 0; /*0x10065d16*/
    v8 = (*(int (__thiscall **)(int))(*(_DWORD *)v7 + 376))(a1: v7); /*0x10065d22*/
    if ( v8 != 0 ) /*0x10065d26*/
    {
      (*(void (__thiscall **)(int))(*(_DWORD *)v8 + 36))(a1: v8); /*0x10065d31*/
      _8 = v17 + 12; /*0x10065d3e*/
      (*(void (__thiscall **)(int))(*(_DWORD *)dword_10735E3C + 80))(a1: dword_10735E3C); /*0x10065d42*/
    }
  }
  if ( (*((_BYTE *)self + 240) & 0x20) != 0 ) /*0x10065dab*/
  { // Raze Charger turn patches only opcode 74->EB here, leaving displacement 2D. Original JZ checks previous frozen state; EB always reaches 65DE3 and bypasses cmd old-angle copy + engine SetViewAngles while bit 20 is set. Bytes NOT patched in this analysis.
    if ( *((_BYTE *)self + 5320) != 0 ) /*0x10065db4*/
    {
      *(float *)(v17 + 12) = *((float *)self + 1327);// Frozen view overwrite: player+14BC/14C0/14C4 -> cmd+0C/10/14; followed by engine vtable slot 20 SetViewAngles. Raze makes this block unreachable while enabled. /*0x10065dbf*/
      _8c = v17 + 12; /*0x10065dc1*/
      *(float *)(v17 + 16) = *((float *)self + 1328); /*0x10065dc8*/
      *(float *)(v17 + 20) = *((float *)self + 1329); /*0x10065dd1*/
      (*(void (__thiscall **)(int))(*(_DWORD *)dword_10735E3C + 80))(a1: dword_10735E3C); /*0x10065ddf*/
    }
    else
    {
      *((_BYTE *)self + 5320) = 1; /*0x10065de3*/
    }
  }
  else
  {
    *((_BYTE *)self + 5320) = 0; /*0x10065dec*/
  }
  _8d = v17; /*0x10065df6*/
  *((float *)self + 1327) = *(float *)(v17 + 12); /*0x10065df7*/
  *((float *)self + 1328) = *(float *)(v17 + 16); /*0x10065e02*/
  *((float *)self + 1329) = *(float *)(v17 + 20); /*0x10065e0b*/
  sub_10065A90(a1: self); /*0x10065e11*/
  return true; /*0x10065e16*/
}