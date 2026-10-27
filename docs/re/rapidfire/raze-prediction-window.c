int __cdecl sub_1016CE80(int a1, unsigned __int8 a2)
{
  int result; // eax
  int v4; // [esp+0h] [ebp-Ch]

  result = -1; /*0x1016ce84*/
  do /*0x1016ceb0*/
  {
    dword_103F6DBC = result; /*0x1016ceb2*/
    if ( (_BYTE)word_103F538C == 1 ) /*0x1016cebe*/
      sub_1014B6C0(a1: a2); /*0x1016cec9*/
    __asm /*0x1016ce94*/
    {
      vmovss xmm0, [esp+0Ch+arg_0]
      vmovss [esp+0Ch+var_C], xmm0
    }
    dword_103F7458(a1: v4, a2); /*0x1016ce9f*/
    result = dword_103F6DBC - 1; /*0x1016ceab*/
  }
  while ( dword_103F6DBC > 0 ); /*0x1016ceb0*/
  return result; /*0x1016ced3*/
}