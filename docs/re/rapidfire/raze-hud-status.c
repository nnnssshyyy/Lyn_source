char __cdecl sub_100E8940(char a1, int vKey)
{
  char result; // al
  unsigned __int16 AsyncKeyState; // ax
  int v4; // ebx
  const char *v5; // edi
  __int128 v6; // [esp-10h] [ebp-1Ch]

  result = vKey; /*0x100e8943*/
  if ( vKey != 0 ) /*0x100e8949*/
  {
    AsyncKeyState = GetAsyncKeyState(vKey); /*0x100e8956*/
    v4 = AsyncKeyState >> 15; /*0x100e895b*/
    result = a1 & ((AsyncKeyState & 0x8000u) != 0); /*0x100e8964*/
    if ( result == 1 && byte_103F600C == 0 ) /*0x100e8977*/
    {
      byte_103F5CDC = 0; /*0x100e897d*/
      word_103F4BDD = 0; /*0x100e8984*/
      byte_103F4BDF = 0; /*0x100e898d*/
      byte_103F50F1 = 0; /*0x100e8994*/
      word_103F5100 = 0; /*0x100e899b*/
      byte_103F5144 = 0; /*0x100e89a4*/
      byte_103F515C = 0; /*0x100e89ab*/
      byte_103F516C = 0; /*0x100e89b2*/
      byte_103F5210 = 0; /*0x100e89b9*/
      byte_103F52A4 = 0; /*0x100e89c0*/
      byte_103F52E4 = 0; /*0x100e89c7*/
      byte_103F532C = 0; /*0x100e89ce*/
      byte_103F5340 = 0; /*0x100e89d5*/
      byte_103F5348 = 0; /*0x100e89dc*/
      byte_103F5350 = 0; /*0x100e89e3*/
      byte_103F5360 = 0; /*0x100e89ea*/
      byte_103F5370 = 0; /*0x100e89f1*/
      byte_103F5380 = 0; /*0x100e89f8*/
      word_103F538C = 0; /*0x100e89ff*/
      word_103F5394 = 0; /*0x100e8a08*/
      byte_103F5397 = 0; /*0x100e8a11*/
      word_103F53A4 = 0; /*0x100e8a18*/
      byte_103F53B0 = 0; /*0x100e8a21*/
      byte_103F53BC = 0; /*0x100e8a28*/
      byte_103F53D4 = 0; /*0x100e8a2f*/
      word_103F53E0 = 0; /*0x100e8a36*/
      byte_103F53F0 = 0; /*0x100e8a3f*/
      byte_103F5404 = 0; /*0x100e8a46*/
      byte_103F5424 = 0; /*0x100e8a4d*/
      byte_103F546C = 0; /*0x100e8a54*/
      byte_103F5608 = 0; /*0x100e8a5b*/
      byte_103F5654 = 0; /*0x100e8a62*/
      byte_103F5664 = 0; /*0x100e8a69*/
      byte_103F569A = 0; /*0x100e8a70*/
      byte_103F59B8 = 0; /*0x100e8a77*/
      byte_103F5A60 = 0; /*0x100e8a7e*/
      ChatSpy_Enabled = 0; /*0x100e8a85*/
      byte_103F5AA8 = 0; /*0x100e8a8c*/
      byte_103F5ADC = 0; /*0x100e8a93*/
      byte_103F5AE4 = 0; /*0x100e8a9a*/
      byte_103F5AEC = 0; /*0x100e8aa1*/
      byte_103F5AF4 = 0; /*0x100e8aa8*/
      byte_103F5AFC = 0; /*0x100e8aaf*/
      byte_103F5B04 = 0; /*0x100e8ab6*/
      byte_103F5B0C = 0; /*0x100e8abd*/
      byte_103F5B14 = 0; /*0x100e8ac4*/
      word_103F50FC = 0; /*0x100e8acb*/
      dword_103F50F8 = 0; /*0x100e8ad4*/
      dword_103F53C8 = 0; /*0x100e8ade*/
      *(int *)((char *)&dword_103F53C8 + 3) = 0; /*0x100e8ae8*/
      if ( dword_103EF180 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x100e8b13*/
      {
        sub_101809C3(a1: &dword_103EF180); /*0x100e8b4e*/
        if ( dword_103EF180 == -1 ) /*0x100e8b5d*/
        {
          sub_10180BFB(Function: sub_10045670); /*0x100e8b64*/
          sub_10180A43(a1: &dword_103EF180); /*0x100e8b71*/
        }
      }
      v5 = (const char *)sub_10041820(Str: "All functions have been deactivated."); /*0x100e8b24*/
      *((_QWORD *)&v6 + 1) = __PAIR64__(strlen(Str: v5), (unsigned int)v5); /*0x100e8b32*/
      DWORD1(v6) = 8; /*0x100e8b33*/
      LODWORD(v6) = "RazeHack"; /*0x100e8b35*/
      result = sub_1014C0A0(a1: v6); /*0x100e8b3a*/
    }
    byte_103F600C = v4; /*0x100e8b3f*/
  }
  return result; /*0x100e8b45*/
}