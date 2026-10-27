int sub_10179560()
{
  int v0; // esi
  int v1; // eax
  int v2; // eax

  v0 = dword_103EC7B8; /*0x10179561*/
  v1 = (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 48))(a1: dword_103EC770); /*0x1017956f*/
  v2 = (*(int (__thiscall **)(int, int))(*(_DWORD *)v0 + 12))(a1: v0, a2: v1); /*0x10179577*/
  if ( v2 != 0 ) /*0x1017957c*/
    return (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 980))(a1: v2); /*0x10179583*/
  else
    return 0; /*0x10179589*/
}