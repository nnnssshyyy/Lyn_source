// NoSpread path: gated by NoSpread_ShouldRun. Current switch explicitly rejects Pump/Auto/Chrome/SPAS IDs 3,4,8,11; this routine is not a full shotgun pellet collapse algorithm.
char __userpurge NoSpread_ApplyShotAngles@<al>(__int128 xmm0_0@<xmm0>, __int128 _XMM2@<xmm2>, int a1)
{
  int v3; // esi
  char *v5; // eax
  char v31; // dl
  int v32; // ecx
  float X; // [esp+0h] [ebp-6Ch]
  float Xa; // [esp+0h] [ebp-6Ch]
  int Y; // [esp+4h] [ebp-68h]
  int Ya; // [esp+4h] [ebp-68h]
  int v38; // [esp+8h] [ebp-64h]
  int v39; // [esp+8h] [ebp-64h]
  __int64 v40; // [esp+10h] [ebp-5Ch] BYREF
  int v41; // [esp+18h] [ebp-54h]
  float v43; // [esp+20h] [ebp-4Ch]
  float v44; // [esp+24h] [ebp-48h]
  float v45; // [esp+28h] [ebp-44h]
  float v46; // [esp+2Ch] [ebp-40h]

  if ( dword_103EEF34 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10026d81*/
  {
    sub_101809C3(a1: &dword_103EEF34); /*0x10027038*/
    if ( dword_103EEF34 == -1 ) /*0x10027044*/
    {
      NoSpread_CachedSharedRandomFloat = (double (__cdecl *)(_DWORD, _DWORD, _DWORD, _DWORD))NoSpread_SharedRandomFloat; /*0x1002704f*/
      sub_10180A43(a1: &dword_103EEF34); /*0x1002705b*/
    }
  }
  v3 = sub_10179540(); /*0x10026d92*/
  _EDI = sub_10179560(); /*0x10026d9f*/
  LOBYTE(v5) = v3 == 0; /*0x10026da3*/
  if ( v3 != 0 && _EDI != 0 && byte_103F4BB0 == 0 ) /*0x10026dba*/
  {
    _EBX = a1; /*0x10026dc0*/
    LOBYTE(v5) = NoSpread_ShouldRun(a1: v3, a2: _EDI, a3: a1); /*0x10026dcf*/
    if ( (_BYTE)v5 != 0 ) /*0x10026dd9*/
    {
      LOBYTE(v5) = Weapon_CanFireFromTickbase(a1: _EDI, _XMM0: xmm0_0, a3: v3); /*0x10026de4*/
      if ( (_BYTE)v5 != 0 ) /*0x10026dee*/
      {
        __asm { vmovsd xmm1, qword ptr [ebx+0Ch] } /*0x10026df4*/
        if ( NoSpread_Enabled == 1 ) /*0x10026e00*/
        {
          __asm /*0x10026e06*/
          {
            vmovss xmm0, dword ptr [edi+0D0Ch]
            vmovss [esp+6Ch+var_50], xmm0
          }
          __asm { vmovups [esp+6Ch+var_3C], xmm1 }
          NoSpread_NativeUpdateSpread(a1: _EDI);// Native CTerrorGun::UpdateSpread called with weapon in ECX. Old float weapon+0xD0C saved before call and restored after reading RNG offsets. /*0x10026e1c*/
          __asm /*0x10026e22*/
          {
            vmovss xmm1, dword ptr [edi+0D0Ch]
            vmovups [esp+6Ch+var_1C], xmm1
            vbroadcastss xmm0, ds:dword_1018513C
            vxorps xmm0, xmm1, xmm0
            vmovups [esp+6Ch+var_2C], xmm0
            vmovss [esp+6Ch+var_64], xmm1
            vmovss [esp+6Ch+Y], xmm0
          }
          v46 = NoSpread_CachedSharedRandomFloat(a1: "CTerrorGun::FireBullet HorizSpread", a2: Y, a3: v38, a4: 0);// SharedRandomFloat("CTerrorGun::FireBullet HorizSpread", -spread, spread, 0). Horizontal label is the pitch-axis offset in the installed client. /*0x10026e64*/
          __asm /*0x10026e68*/
          {
            vmovups xmm0, [esp+6Ch+var_1C]
            vmovss [esp+6Ch+var_64], xmm0
            vmovups xmm0, [esp+6Ch+var_2C]
            vmovss [esp+6Ch+Y], xmm0
          }
          v45 = NoSpread_CachedSharedRandomFloat(a1: "CTerrorGun::FireBullet VertSpread", a2: Ya, a3: v39, a4: 0);// SharedRandomFloat("CTerrorGun::FireBullet VertSpread", -spread, spread, 0). Vertical label is the yaw-axis offset in the installed client. /*0x10026e95*/
          __asm /*0x10026e99*/
          {
            vmovss xmm0, [esp+6Ch+var_40]
            vinsertps xmm0, xmm0, [esp+6Ch+var_44], 10h
            vmovups xmm1, [esp+6Ch+var_3C]
            vsubps xmm0, xmm1, xmm0
            vmovups [esp+6Ch+var_3C], xmm0
            vextractps dword ptr [esp+6Ch+var_5C+4], xmm0, 1
            vmovss xmm0, [esp+6Ch+var_50]
            vmovss dword ptr [edi+0D0Ch], xmm0
          }
          _EAX = (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 1244))(a1: v3);// Player vtable+0x4DC is also used by native FireBullet for punch angles. This NoSpread-enabled block subtracts punch after subtracting the two spread offsets. /*0x10026ed1*/
          __asm /*0x10026ed7*/
          {
            vmovsd xmm0, qword ptr [eax]
            vmovups xmm1, [esp+6Ch+var_3C]
            vsubps xmm1, xmm1, xmm0
          }
        }
        __asm { vmovd eax, xmm1 } /*0x10026ee5*/
        __asm
        {
          vxorps xmm2, xmm2, xmm2
          vxorps xmm0, xmm0, xmm0
        }
        if ( (_EAX & 0x7FFFFFFFu) <= 0x7F7FFFFF ) /*0x10026efb*/
        {
          __asm { vmovss [esp+6Ch+X], xmm1 } /*0x10026efd*/
          __asm { vmovups [esp+6Ch+var_3C], xmm1 }
          __asm
          {
            vxorps xmm2, xmm2, xmm2
            vmovups xmm1, [esp+6Ch+var_3C]
          }
          v44 = remainderf(X, Y: 360.0); /*0x10026f20*/
          __asm { vmovss xmm0, [esp+6Ch+var_48] } /*0x10026f24*/
        }
        __asm /*0x10026f2a*/
        {
          vminss xmm0, xmm0, ds:dword_10184EF4
          vmaxss xmm0, xmm0, ds:dword_10184EF8
          vmovss dword ptr [esp+6Ch+var_5C], xmm0
          vmovshdup xmm0, xmm1
          vmovd eax, xmm0
        }
        if ( (_EAX & 0x7FFFFFFFu) <= 0x7F7FFFFF ) /*0x10026f52*/
        {
          __asm { vmovss [esp+6Ch+X], xmm0 } /*0x10026f54*/
          v43 = remainderf(X: Xa, Y: 360.0); /*0x10026f67*/
          __asm { vmovss xmm2, [esp+6Ch+var_4C] } /*0x10026f6b*/
        }
        __asm { vmovss dword ptr [esp+6Ch+var_5C+4], xmm2 } /*0x10026f71*/
        v41 = 0; /*0x10026f77*/
        NoSpread_FixMovement(a1, a2: (int)&v40); /*0x10026f90*/
        __asm /*0x10026f98*/
        {
          vmovsd xmm0, [esp+6Ch+var_5C]
          vmovsd qword ptr [ebx+0Ch], xmm0
          vmovss xmm1, [esp+6Ch+var_54]
          vmovss dword ptr [ebx+14h], xmm1
        }
        if ( byte_103F4BFC == 1 ) /*0x10026fb5*/
        {
          byte_103EEAAA = 1; /*0x10026fb7*/
          __asm /*0x10026fbe*/
          {
            vmovlps qword ptr [ebx+0Ch], xmm0
            vmovss dword ptr [ebx+14h], xmm1
          }
        }
        if ( byte_103F5CDC == 0 && NoSpread_Enabled == 1 ) /*0x10026fd8*/
          byte_103EEAAA = 1; /*0x10026fda*/
        if ( (unsigned int)(dword_103F5CF8 - 2) < 2 ) /*0x10026fed*/
        {
          v5 = &byte_103EEAAA; /*0x10027020*/
LABEL_24:
          *v5 = 1; /*0x10027025*/
          return (char)v5; /*0x10027025*/
        }
        v5 = &byte_103EEAA9; /*0x10026fef*/
        if ( dword_103F5CF8 == 1 ) /*0x10026ff7*/
          goto LABEL_24; /*0x10026ff7*/
        if ( dword_103F5CF8 == 0 ) /*0x10026ffb*/
        {
          v31 = byte_103EEA61; /*0x10026ffd*/
          v32 = dword_1029E4D0; /*0x10027004*/
          __asm /*0x1002700a*/
          {
            vmovlps qword ptr [ebx+0Ch], xmm0
            vmovss dword ptr [ebx+14h], xmm1
          }
          if ( v31 != 1 || v32 == -1 ) /*0x1002701c*/
            goto LABEL_24; /*0x1002701c*/
        }
      }
    }
  }
  return (char)v5; /*0x10027028*/
}
