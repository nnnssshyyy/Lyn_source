int sub_10001910()
{
  __int128 v1; // [esp-10h] [ebp-10h]

  HIDWORD(v1) = "C_TerrorGun_UpdateSpread"; /*0x10001915*/
  DWORD2(v1) = 0; /*0x1000191a*/
  DWORD1(v1) = "53 8B DC 83 EC ? 83 E4 ? 83 C4 ? 55 8B 6B ? 89 6C 24 ? 8B EC 83 EC ? 56 57 8B F9 E8"; /*0x1000191c*/
  LODWORD(v1) = "client.dll"; /*0x10001921*/
  return sub_101804E0(a1: v1); /*0x1000192b*/
}
