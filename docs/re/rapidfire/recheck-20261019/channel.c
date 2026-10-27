__int16 __stdcall RapidFire_ApplyNetChannelMode(int a1, int a2)
{
  int v2; // edi
  HMODULE ModuleHandleA; // eax
  HMODULE v4; // esi
  int v5; // eax
  int v6; // ecx
  int v7; // edi
  int v13; // [esp+0h] [ebp-1Ch]

  v2 = *(_DWORD *)(a2 + 4); /*0x10148c57*/
  ModuleHandleA = GetModuleHandleA(lpModuleName: "engine.dll"); /*0x10148c71*/
  if ( RapidFire_Enabled != 1 ) /*0x10148c7e*/
    return (__int16)ModuleHandleA; /*0x10148c7e*/
  v4 = ModuleHandleA; /*0x10148c84*/
  LOWORD(ModuleHandleA) = RapidFire_Method; /*0x10148c86*/
  if ( RapidFire_Method == 0 ) /*0x10148c8d*/
  {
    v13 = *(_DWORD *)(*((_DWORD *)v4 + 1088059) + 24); /*0x10148c9f*/
    if ( GetAsyncKeyState(vKey: RapidFire_Key) < 0 ) /*0x10148cb1*/
    {
      v5 = RapidFire_StormTicks; /*0x10148d49*/
      if ( RapidFire_StormTicks > 0 ) /*0x10148d50*/
      {
        v6 = v2 - 150 * ((v2 - 1) / 150) - 1; /*0x10148d7a*/
        v7 = v2 % 150; /*0x10148d7b*/
        if ( v7 == v6 ) // Storm branch compares cmd%150 to (cmd-1)%150; unequal for normal positive commands. Preserve as observed; do not silently replace with a recharge condition. /*0x10148d7f*/
        {
          RapidFire_TickbaseShiftRing[2 * v7] = 0; /*0x10148d81*/
          __asm /*0x10148d92*/
          {
            vmovss xmm0, ds:dword_10189E04
            vdivss xmm1, xmm0, dword ptr [edx+1Ch]
            vaddss xmm0, xmm1, xmm0
            vcvttss2si edx, xmm0
          }
          if ( v5 > _EDX ) /*0x10148da9*/
            _EDX = v5; /*0x10148da9*/
          RapidFire_ExtraMovesRemaining = _EDX; /*0x10148dac*/
        }
        else
        {
          ++dword_103F68E8; /*0x10148db4*/
        }
        if ( RapidFire_TickbaseShiftRing[2 * v6] == 0 ) /*0x10148dc2*/
        {
          *(_DWORD *)(v13 + 16) = -1; /*0x10148dcb*/
          *(_DWORD *)(v13 + 28) = 255; /*0x10148dd2*/
        }
      }
    }
    LOWORD(ModuleHandleA) = RapidFire_Method; /*0x10148cb7*/
    if ( RapidFire_Method != 1 ) /*0x10148cbf*/
      goto LABEL_10; /*0x10148cbf*/
  }
  else if ( RapidFire_Method != 1 ) /*0x10148c92*/
  {
LABEL_10:
    if ( RapidFire_Method == 2 ) /*0x10148d03*/
    {
      LOWORD(ModuleHandleA) = GetAsyncKeyState(vKey: RapidFire_Key); /*0x10148d0b*/
      if ( (__int16)ModuleHandleA < 0 ) /*0x10148d14*/
      {
        ModuleHandleA = *(HMODULE *)(*((_DWORD *)v4 + 1088059) + 24); /*0x10148d1c*/
        *((_DWORD *)ModuleHandleA + 4) = -1; // UI method 3 (index=2): netchannel+0x10=-1; netchannel+0x1C=255. Current engine SendDatagram serializes low 8 bits of +0x1C; do not infer extra damage. /*0x10148d1f*/
        *((_DWORD *)ModuleHandleA + 7) = 255; /*0x10148d26*/
        RapidFire_ForceSend = 1; /*0x10148d2d*/
      }
    }
    return (__int16)ModuleHandleA; /*0x10148d34*/
  }
  LOWORD(ModuleHandleA) = GetAsyncKeyState(vKey: RapidFire_Key); /*0x10148cc1*/
  if ( (__int16)ModuleHandleA < 0 ) /*0x10148cd0*/
  {
    ModuleHandleA = (HMODULE)(*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 296))(a1: dword_103EC770); /*0x10148cda*/
    if ( ModuleHandleA != nullptr ) /*0x10148ce2*/
    {
      LOWORD(ModuleHandleA) = RapidFire_Method2Choked;// UI method 2 (index=1): netchannel+0x1C = configured ticks. Distinct from clientstate choked_commands. /*0x10148ce4*/
      *(_DWORD *)(*(_DWORD *)(dword_103EC9A8 + 24) + 28) = RapidFire_Method2Choked; /*0x10148cf2*/
      RapidFire_ForceSend = 1; /*0x10148cf5*/
    }
  }
  goto LABEL_10; /*0x10148cf5*/
}