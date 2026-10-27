HMODULE __fastcall ChargerTurn_ApplySetting(_DWORD *a1)
{
  HMODULE result; // eax
  char v2; // dl
  _DWORD *v3; // esi
  int v4; // esi
  int v5; // ecx
  HMODULE v6; // ebp
  DWORD flNewProtect; // [esp+0h] [ebp-18h] BYREF

  result = (HMODULE)ChargerTurn_PatchSite; /*0x1017ccb2*/
  if ( ChargerTurn_PatchSite != nullptr ) /*0x1017ccb9*/
  {
    v2 = ChargerTurn_Enabled; /*0x1017ccbb*/
    if ( ChargerTurn_Enabled == ChargerTurn_Applied ) /*0x1017ccc8*/
      return result; /*0x1017ccc8*/
  }
  else
  {
    v3 = a1; /*0x1017ccd1*/
    result = GetModuleHandleA(lpModuleName: "client.dll"); /*0x1017ccd3*/
    if ( result == nullptr ) /*0x1017ccdb*/
      return result; /*0x1017ccdb*/
    a1 = v3; /*0x1017ccdd*/
    result += 104301; // Charger turn target is client.dll + 0x65DB4 (byte RVA; Hex-Rays HMODULE pointer arithmetic may show 104301). Verified against installed client SHA256 c0a8e1e88f7312db11bcfd4b0ec574d224a34b2d9a615d6088b28335eeb4af1f. /*0x1017ccdf*/
    ChargerTurn_PatchSite = result; /*0x1017cce4*/
    v2 = ChargerTurn_Enabled; /*0x1017cce9*/
    if ( ChargerTurn_Enabled == ChargerTurn_Applied ) /*0x1017ccf6*/
      return result; /*0x1017ccf6*/
  }
  if ( v2 != 0 ) /*0x1017ccfa*/
  {
    PatchRegistry_BackupAndFill(this: a1, lpAddress: result, a3: 0xEBu, dwSize: 1);// On toggle ON: backup original byte then fill exactly 1 byte with EB. With installed client original 74 2D (JZ +2D), EB 2D always skips the old-viewangles overwrite. No local class/carry gate in this function. /*0x1017cd04*/
  }
  else
  {
    v4 = *a1; /*0x1017cd28*/
    v5 = a1[1]; /*0x1017cd2a*/
    while ( v4 != v5 ) /*0x1017cd35*/
    {
      if ( *(HMODULE *)v4 == result && *(_DWORD *)(v4 + 8) - *(_DWORD *)(v4 + 4) == 1 ) /*0x1017cd44*/
      {
        v6 = result; /*0x1017cd54*/
        VirtualProtect(lpAddress: result, dwSize: 1u, flNewProtect: 0x40u, lpflOldProtect: &flNewProtect); /*0x1017cd56*/
        *(_BYTE *)v6 = **(_BYTE **)(v4 + 4); // On toggle OFF: restore the first matching one-byte original from patch registry. Protect results are not checked in Raze. /*0x1017cd5e*/
        VirtualProtect(lpAddress: v6, dwSize: 1u, flNewProtect, lpflOldProtect: &flNewProtect); /*0x1017cd69*/
        break; /*0x1017cd6b*/
      }
      v4 += 16; /*0x1017cd30*/
    }
  }
  result = (HMODULE)(unsigned __int8)ChargerTurn_Enabled; /*0x1017cd09*/
  ChargerTurn_Applied = ChargerTurn_Enabled; /*0x1017cd10*/
  return result; /*0x1017cd15*/
}