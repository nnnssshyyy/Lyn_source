int __cdecl RapidFire_Hook_CL_Move(int a1, char a2)
{
  int result; // eax
  int v4; // [esp+0h] [ebp-Ch]

  result = -1; /*0x1016ce84*/
  do /*0x1016ceb0*/
  {
    RapidFire_ExtraMovesRemaining = result; // Reset -1 before base CL_Move; initial CreateMove may arm N; after each move decrement counter and repeat while previous value >0. Total N+1 native calls. Same original args each call. /*0x1016ceb2*/
    if ( (_BYTE)word_103F538C == 1 ) /*0x1016cebe*/
      sub_1014B6C0(a1: a2); /*0x1016cec9*/
    __asm /*0x1016ce94*/
    {
      vmovss xmm0, [esp+0Ch+arg_0]
      vmovss [esp+0Ch+var_C], xmm0
    }
    RapidFire_Original_CL_Move(a1: v4, (unsigned __int8)a2); /*0x1016ce9f*/
    result = RapidFire_ExtraMovesRemaining - 1; /*0x1016ceab*/
  }
  while ( RapidFire_ExtraMovesRemaining > 0 ); /*0x1016ceb0*/
  return result; /*0x1016ced3*/
}