; NoSpread_ApplyShotAngles @ 0x10026d60; complete 162 instructions
10026d60  push ebx; NoSpread path: gated by NoSpread_ShouldRun. Current switch explicitly rejects Pump/Auto/Chrome/SPAS IDs 3,4,8,11; this routine is not a full shotgun pellet collapse algorithm.
; NoSpread path: gated by NoSpread_ShouldRun. Current switch explicitly rejects Pump/Auto/Chrome/SPAS IDs 3,4,8,11; this routine is not a full shotgun pellet collapse algorithm.
10026d61  push edi
10026d62  push esi
10026d63  sub esp, 60h
10026d66  mov eax, dword_103EEF34
10026d6b  mov ecx, TlsIndex
10026d71  mov edx, large fs:2Ch
10026d78  mov ecx, [edx+ecx*4]
10026d7b  cmp eax, [ecx+4]
10026d81  jg loc_10027031
10026d87  mov ecx, off_1029E014
10026d8d  call sub_10179540
10026d92  mov esi, eax
10026d94  mov ecx, off_1029E014
10026d9a  call sub_10179560
10026d9f  mov edi, eax
10026da1  test esi, esi
10026da3  setz al
10026da6  test edi, edi
10026da8  setz cl
10026dab  or cl, al
10026dad  jnz loc_10027028
10026db3  cmp byte_103F4BB0, 0
10026dba  jnz loc_10027028
10026dc0  mov ebx, [esp+6Ch+arg_0]
10026dc4  mov [esp+6Ch+var_64], ebx
10026dc8  mov [esp+6Ch+Y], edi
10026dcc  mov [esp+6Ch+X], esi
10026dcf  call NoSpread_ShouldRun
10026dd4  sub esp, 0Ch
10026dd7  test al, al
10026dd9  jz loc_10027028
10026ddf  mov [esp+6Ch+X], esi
10026de2  mov ecx, edi
10026de4  call Weapon_CanFireFromTickbase
10026de9  sub esp, 4
10026dec  test al, al
10026dee  jz loc_10027028
10026df4  vmovsd xmm1, qword ptr [ebx+0Ch]
10026df9  cmp NoSpread_Enabled, 1
10026e00  jnz loc_10026EE5
10026e06  vmovss xmm0, dword ptr [edi+0D0Ch]
10026e0e  vmovss [esp+6Ch+var_50], xmm0
10026e14  mov ecx, edi
10026e16  vmovups [esp+6Ch+var_3C], xmm1
10026e1c  call NoSpread_NativeUpdateSpread; Native CTerrorGun::UpdateSpread called with weapon in ECX. Old float weapon+0xD0C saved before call and restored after reading RNG offsets.
; Native CTerrorGun::UpdateSpread called with weapon in ECX. Old float weapon+0xD0C saved before call and restored after reading RNG offsets.
10026e22  vmovss xmm1, dword ptr [edi+0D0Ch]
10026e2a  vmovups [esp+6Ch+var_1C], xmm1
10026e30  vbroadcastss xmm0, ds:dword_1018513C
10026e39  vxorps xmm0, xmm1, xmm0
10026e3d  vmovups [esp+6Ch+var_2C], xmm0
10026e43  vmovss [esp+6Ch+var_64], xmm1
10026e49  vmovss [esp+6Ch+Y], xmm0
10026e4f  mov [esp+6Ch+var_60], 0
10026e57  mov [esp+6Ch+X], offset aCterrorgunFire; "CTerrorGun::FireBullet HorizSpread"
10026e5e  call NoSpread_CachedSharedRandomFloat
10026e64  fstp [esp+6Ch+var_40]; SharedRandomFloat("CTerrorGun::FireBullet HorizSpread", -spread, spread, 0). Horizontal label is the pitch-axis offset in the installed client.
; SharedRandomFloat("CTerrorGun::FireBullet HorizSpread", -spread, spread, 0). Horizontal label is the pitch-axis offset in the installed client.
10026e68  vmovups xmm0, [esp+6Ch+var_1C]
10026e6e  vmovss [esp+6Ch+var_64], xmm0
10026e74  vmovups xmm0, [esp+6Ch+var_2C]
10026e7a  vmovss [esp+6Ch+Y], xmm0
10026e80  mov [esp+6Ch+var_60], 0
10026e88  mov [esp+6Ch+X], offset aCterrorgunFire_0; "CTerrorGun::FireBullet VertSpread"
10026e8f  call NoSpread_CachedSharedRandomFloat
10026e95  fstp [esp+6Ch+var_44]; SharedRandomFloat("CTerrorGun::FireBullet VertSpread", -spread, spread, 0). Vertical label is the yaw-axis offset in the installed client.
; SharedRandomFloat("CTerrorGun::FireBullet VertSpread", -spread, spread, 0). Vertical label is the yaw-axis offset in the installed client.
10026e99  vmovss xmm0, [esp+6Ch+var_40]
10026e9f  vinsertps xmm0, xmm0, [esp+6Ch+var_44], 10h
10026ea7  vmovups xmm1, [esp+6Ch+var_3C]
10026ead  vsubps xmm0, xmm1, xmm0
10026eb1  vmovups [esp+6Ch+var_3C], xmm0
10026eb7  vextractps dword ptr [esp+6Ch+var_5C+4], xmm0, 1
10026ebf  vmovss xmm0, [esp+6Ch+var_50]
10026ec5  vmovss dword ptr [edi+0D0Ch], xmm0
10026ecd  mov eax, [esi]
10026ecf  mov ecx, esi
10026ed1  call dword ptr [eax+4DCh]; Player vtable+0x4DC is also used by native FireBullet for punch angles. This NoSpread-enabled block subtracts punch after subtracting the two spread offsets.
; Player vtable+0x4DC is also used by native FireBullet for punch angles. This NoSpread-enabled block subtracts punch after subtracting the two spread offsets.
10026ed7  vmovsd xmm0, qword ptr [eax]
10026edb  vmovups xmm1, [esp+6Ch+var_3C]
10026ee1  vsubps xmm1, xmm1, xmm0
10026ee5  vmovd eax, xmm1
10026ee9  and eax, 7FFFFFFFh
10026eee  vxorps xmm2, xmm2, xmm2
10026ef2  vxorps xmm0, xmm0, xmm0
10026ef6  cmp eax, 7F7FFFFFh
10026efb  jg short loc_10026F2A
10026efd  vmovss [esp+6Ch+X], xmm1
10026f02  mov [esp+6Ch+Y], 43B40000h; Y
; Y
10026f0a  vmovups [esp+6Ch+var_3C], xmm1
10026f10  call ds:remainderf; api-ms-win-crt-math-l1-1-0
10026f16  vxorps xmm2, xmm2, xmm2
10026f1a  vmovups xmm1, [esp+6Ch+var_3C]
10026f20  fstp [esp+6Ch+var_48]
10026f24  vmovss xmm0, [esp+6Ch+var_48]
10026f2a  vminss xmm0, xmm0, ds:dword_10184EF4
10026f32  vmaxss xmm0, xmm0, ds:dword_10184EF8
10026f3a  vmovss dword ptr [esp+6Ch+var_5C], xmm0
10026f40  vmovshdup xmm0, xmm1
10026f44  vmovd eax, xmm0
10026f48  and eax, 7FFFFFFFh
10026f4d  cmp eax, 7F7FFFFFh
10026f52  jg short loc_10026F71
10026f54  vmovss [esp+6Ch+X], xmm0
10026f59  mov [esp+6Ch+Y], 43B40000h; Y
; Y
10026f61  call ds:remainderf; api-ms-win-crt-math-l1-1-0
10026f67  fstp [esp+6Ch+var_4C]
10026f6b  vmovss xmm2, [esp+6Ch+var_4C]
10026f71  vmovss dword ptr [esp+6Ch+var_5C+4], xmm2
10026f77  mov [esp+6Ch+var_54], 0
10026f7f  mov ecx, off_1029E4D4
10026f85  lea eax, [esp+6Ch+var_5C]
10026f89  mov [esp+6Ch+Y], eax
10026f8d  mov [esp+6Ch+X], ebx
10026f90  call NoSpread_FixMovement
10026f95  sub esp, 8
10026f98  vmovsd xmm0, [esp+6Ch+var_5C]
10026f9e  vmovsd qword ptr [ebx+0Ch], xmm0
10026fa3  vmovss xmm1, [esp+6Ch+var_54]
10026fa9  vmovss dword ptr [ebx+14h], xmm1
10026fae  cmp byte_103F4BFC, 1
10026fb5  jnz short loc_10026FC8
10026fb7  mov byte_103EEAAA, 1
10026fbe  vmovlps qword ptr [ebx+0Ch], xmm0
10026fc3  vmovss dword ptr [ebx+14h], xmm1
10026fc8  cmp byte_103F5CDC, 0
10026fcf  jnz short loc_10026FE1
10026fd1  cmp NoSpread_Enabled, 1
10026fd8  jnz short loc_10026FE1
10026fda  mov byte_103EEAAA, 1
10026fe1  mov ecx, dword_103F5CF8
10026fe7  lea eax, [ecx-2]
10026fea  cmp eax, 2
10026fed  jb short loc_10027020
10026fef  mov eax, offset byte_103EEAA9
10026ff4  cmp ecx, 1
10026ff7  jz short loc_10027025
10026ff9  test ecx, ecx
10026ffb  jnz short loc_10027028
10026ffd  movzx edx, byte_103EEA61
10027004  mov ecx, dword_1029E4D0
1002700a  vmovlps qword ptr [ebx+0Ch], xmm0
1002700f  vmovss dword ptr [ebx+14h], xmm1
10027014  cmp dl, 1
10027017  jnz short loc_10027025
10027019  cmp ecx, 0FFFFFFFFh
1002701c  jnz short loc_10027028
1002701e  jmp short loc_10027025
10027020  mov eax, offset byte_103EEAAA
10027025  mov byte ptr [eax], 1
10027028  add esp, 60h
1002702b  pop esi
1002702c  pop edi
1002702d  pop ebx
1002702e  retn 4
10027031  mov [esp+6Ch+X], offset dword_103EEF34
10027038  call sub_101809C3
1002703d  cmp dword_103EEF34, 0FFFFFFFFh
10027044  jnz loc_10026D87
1002704a  mov eax, NoSpread_SharedRandomFloat
1002704f  mov NoSpread_CachedSharedRandomFloat, eax
10027054  mov [esp+6Ch+X], offset dword_103EEF34
1002705b  call sub_10180A43
10027060  jmp loc_10026D87

; NoSpread_ShouldRun @ 0x10026bb0; complete 106 instructions
10026bb0  push ebp
10026bb1  mov ebp, esp
10026bb3  push ebx
10026bb4  push edi
10026bb5  push esi
10026bb6  sub esp, 10h
10026bb9  mov eax, [ebp+arg_8]
10026bbc  mov [ebp+var_1C], esp
10026bbf  mov [ebp+var_10], 0FFFFFFFFh
10026bc6  lea ecx, [ebp+var_18]
10026bc9  mov [ebp+var_14], offset loc_10027070
10026bd0  mov edx, large fs:0
10026bd7  mov [ebp+var_18], edx
10026bda  mov large fs:0, ecx
10026be1  mov eax, [eax+24h]
10026be4  and eax, 21h
10026be7  cmp eax, 1
10026bea  jnz short def_10026C77; jumptable 10026C77 default case, cases 3,4,8,11-25,27-31
10026bec  mov eax, dword_103EEF3C
10026bf1  mov ecx, TlsIndex
10026bf7  mov edx, large fs:2Ch
10026bfe  mov ecx, [edx+ecx*4]
10026c01  cmp eax, [ecx+4]
10026c07  jg short loc_10026C7E
10026c09  mov esi, [ebp+arg_0]
10026c0c  mov eax, dword_103EEF38
10026c11  cmp byte ptr [eax+esi], 0
10026c15  jnz short def_10026C77; jumptable 10026C77 default case, cases 3,4,8,11-25,27-31
10026c17  mov eax, dword_103EEF44
10026c1c  mov ecx, TlsIndex
10026c22  mov edx, large fs:2Ch
10026c29  mov ecx, [edx+ecx*4]
10026c2c  cmp eax, [ecx+4]
10026c32  jg loc_10026CC8
10026c38  mov eax, dword_103EEF40
10026c3d  cmp byte ptr [eax+esi], 0
10026c41  jz short loc_10026C59
10026c43  xor eax, eax; jumptable 10026C77 default case, cases 3,4,8,11-25,27-31
; jumptable 10026C77 default case, cases 3,4,8,11-25,27-31
10026c45  mov ecx, [ebp+var_18]; jumptable 10026C77 cases 1,2,5-7,9,10,26,32-37
; jumptable 10026C77 cases 1,2,5-7,9,10,26,32-37
10026c48  mov large fs:0, ecx
10026c4f  add esp, 10h
10026c52  pop esi
10026c53  pop edi
10026c54  pop ebx
10026c55  pop ebp
10026c56  retn 0Ch
10026c59  mov ecx, [ebp+arg_4]
10026c5c  mov eax, [ecx]
10026c5e  mov eax, [eax+5FCh]
10026c64  mov [ebp+var_10], 0FFFFFFFFh
10026c6b  call eax
10026c6d  mov ecx, eax
10026c6f  dec ecx; switch 37 cases
; switch 37 cases
10026c70  cmp ecx, 24h
10026c73  ja short def_10026C77; jumptable 10026C77 default case, cases 3,4,8,11-25,27-31
10026c75  mov al, 1
10026c77  jmp ds:jpt_10026C77[ecx*4]; Jump table 0x10185140 indexed by weapon_id-1. Allowed IDs: 1,2,5,6,7,9,10,26,32-37. Shotguns 3/4/8/11 map to xor eax,eax at0x10026c43 (false).
; Jump table 0x10185140 indexed by weapon_id-1. Allowed IDs: 1,2,5,6,7,9,10,26,32-37. Shotguns 3/4/8/11 map to xor eax,eax at0x10026c43 (false).
10026c7e  push offset dword_103EEF3C
10026c83  call sub_101809C3
10026c88  add esp, 4
10026c8b  cmp dword_103EEF3C, 0FFFFFFFFh
10026c92  jnz loc_10026C09
10026c98  mov [ebp+var_10], 0
10026c9f  push offset aMIshangingfrom; "m_isHangingFromLedge"
10026ca4  push offset aCterrorplayer; "CTerrorPlayer"
10026ca9  call sub_1017C730
10026cae  add esp, 8
10026cb1  mov dword_103EEF38, eax
10026cb6  push offset dword_103EEF3C
10026cbb  call sub_10180A43
10026cc0  add esp, 4
10026cc3  jmp loc_10026C09
10026cc8  push offset dword_103EEF44
10026ccd  call sub_101809C3
10026cd2  add esp, 4
10026cd5  cmp dword_103EEF44, 0FFFFFFFFh
10026cdc  jnz loc_10026C38
10026ce2  mov [ebp+var_10], 1
10026ce9  push offset aMIshangingfrom_0; "m_isHangingFromTongue"
10026cee  push offset aCterrorplayer; "CTerrorPlayer"
10026cf3  call sub_1017C730
10026cf8  add esp, 8
10026cfb  mov dword_103EEF40, eax
10026d00  push offset dword_103EEF44
10026d05  call sub_10180A43
10026d0a  add esp, 4
10026d0d  mov esi, [ebp+arg_0]
10026d10  jmp loc_10026C38
10026d20  push ebp
10026d21  sub esp, 8
10026d24  add ebp, 0Ch
10026d27  push offset dword_103EEF3C
10026d2c  call sub_10180A15
10026d31  add esp, 0Ch
10026d34  pop ebp
10026d35  retn
10026d40  push ebp
10026d41  sub esp, 8
10026d44  add ebp, 0Ch
10026d47  push offset dword_103EEF44
10026d4c  call sub_10180A15
10026d51  add esp, 0Ch
10026d54  pop ebp
10026d55  retn
10027070  mov eax, offset stru_10296B70
10027075  jmp __CxxFrameHandler3

; NoSpread_PredictionBegin @ 0x1002d2b0; complete 295 instructions
1002d2b0  push ebp
1002d2b1  mov ebp, esp
1002d2b3  push ebx
1002d2b4  push edi
1002d2b5  push esi
1002d2b6  sub esp, 20h
1002d2b9  mov edi, ecx
1002d2bb  mov [ebp+var_1C], esp
1002d2be  mov [ebp+var_10], 0FFFFFFFFh
1002d2c5  lea eax, [ebp+var_18]
1002d2c8  mov [ebp+var_14], offset loc_1002D8A0
1002d2cf  mov ecx, large fs:0
1002d2d6  mov [ebp+var_18], ecx
1002d2d9  mov large fs:0, eax
1002d2df  cmp dword_103EF050, 0
1002d2e6  jz loc_1002D514
1002d2ec  mov ecx, off_1029E014
1002d2f2  call sub_10179540
1002d2f7  test eax, eax
1002d2f9  jz loc_1002D514
1002d2ff  mov esi, eax
1002d301  mov eax, dword_103EF058
1002d306  mov ecx, TlsIndex
1002d30c  mov edx, large fs:2Ch
1002d313  mov ecx, [edx+ecx*4]
1002d316  cmp eax, [ecx+4]
1002d31c  jg loc_1002D527
1002d322  mov eax, dword_103EF054
1002d327  mov eax, [eax+esi]
1002d32a  mov [edi+0B4h], eax
1002d330  mov ecx, dword_103EF050
1002d336  mov eax, [ecx]
1002d338  mov eax, [eax+4]
1002d33b  mov [ebp+var_10], 0FFFFFFFFh
1002d342  push esi
1002d343  call eax
1002d345  mov eax, dword_103EF060
1002d34a  mov ecx, TlsIndex
1002d350  mov edx, large fs:2Ch
1002d357  mov ecx, [edx+ecx*4]
1002d35a  cmp eax, [ecx+4]
1002d360  jg loc_1002D57D
1002d366  mov ebx, [ebp+arg_0]
1002d369  mov eax, dword_103EF05C
1002d36e  mov [eax+esi], ebx
1002d371  mov eax, [ebx+4]
1002d374  mov [ebp+var_10], 0FFFFFFFFh
1002d37b  push eax
1002d37c  call sub_1017C4A0
1002d381  add esp, 4
1002d384  and eax, 7FFFFFFFh
1002d389  mov ecx, Prediction_SeedPointer
1002d38f  mov [ecx], eax; Set global prediction seed = MD5_PseudoRandom(original command_number) &0x7FFFFFFF. No fixed shotgun command-number replacement in this path.
; Set global prediction seed = MD5_PseudoRandom(original command_number) &0x7FFFFFFF. No fixed shotgun command-number replacement in this path.
1002d391  mov eax, dword_103EC8D0
1002d396  vmovss xmm0, dword ptr [eax+0Ch]
1002d39b  vmovss dword ptr [edi+0A8h], xmm0
1002d3a3  vmovss xmm0, dword ptr [eax+10h]
1002d3a8  vmovss dword ptr [edi+0ACh], xmm0
1002d3b0  mov eax, [eax+18h]
1002d3b3  mov [edi+0B0h], eax
1002d3b9  mov eax, dword_103EEAF0
1002d3be  mov ecx, TlsIndex
1002d3c4  mov edx, large fs:2Ch
1002d3cb  mov ecx, [edx+ecx*4]
1002d3ce  cmp eax, [ecx+4]
1002d3d4  jg loc_1002D5D6
1002d3da  mov eax, dword_103EEAEC
1002d3df  mov eax, [eax+esi]
1002d3e2  mov [ebp+var_2C], eax
1002d3e5  mov ecx, dword_103EC918
1002d3eb  movzx eax, byte ptr [ecx+8]
1002d3ef  mov [ebp+var_25], al
1002d3f2  movzx eax, byte ptr [ecx+18h]
1002d3f6  mov [ebp+var_26], al
1002d3f9  mov eax, dword_103EEF90
1002d3fe  test eax, eax
1002d400  jz short loc_1002D408
1002d402  cmp byte ptr [eax+3Ch], 1
1002d406  jnz short loc_1002D439
1002d408  mov eax, dword_103EEAF0
1002d40d  mov ecx, TlsIndex
1002d413  mov edx, large fs:2Ch
1002d41a  mov ecx, [edx+ecx*4]
1002d41d  cmp eax, [ecx+4]
1002d423  jg loc_1002D67F
1002d429  mov eax, dword_103EEAEC
1002d42e  mov eax, [eax+esi]
1002d431  mov ecx, dword_103EC918
1002d437  jmp short loc_1002D43F
1002d439  mov eax, dword_103EEF8C
1002d43e  inc eax
1002d43f  mov [ebp+var_10], 0FFFFFFFFh
1002d446  mov dword_103EEF8C, eax
1002d44b  mov dword_103EEF90, ebx
1002d451  mov edx, dword_103EC8D0
1002d457  vmovss xmm0, dword ptr [edx+1Ch]; Prediction context: current time=tick*interval; frame_time=interval or0 when prediction paused. NoSpread executes before PredictionEnd restores these globals.
; Prediction context: current time=tick*interval; frame_time=interval or0 when prediction paused. NoSpread executes before PredictionEnd restores these globals.
1002d45c  vcvtsi2ss xmm1, xmm1, eax
1002d460  vmulss xmm1, xmm0, xmm1
1002d464  vmovss dword ptr [edx+0Ch], xmm1
1002d469  cmp byte ptr [ecx+0Ah], 0
1002d46d  vxorps xmm1, xmm1, xmm1
1002d471  jnz short loc_1002D477
1002d473  vmovaps xmm1, xmm0
1002d477  vmovss dword ptr [edx+10h], xmm1
1002d47c  mov [edx+18h], eax
1002d47f  mov byte ptr [ecx+18h], 0
1002d483  mov byte ptr [ecx+8], 1
1002d487  lea eax, [ebx+0Ch]
1002d48a  mov edx, [ecx]
1002d48c  push eax
1002d48d  call dword ptr [edx+34h]
1002d490  mov ecx, dword_103EC918
1002d496  mov eax, [ecx]
1002d498  push edi
1002d499  push dword_103EF050
1002d49f  push ebx
1002d4a0  push esi
1002d4a1  call dword ptr [eax+4Ch]
1002d4a4  mov ecx, dword_103EC930
1002d4aa  mov eax, [ecx]
1002d4ac  push edi
1002d4ad  push esi
1002d4ae  call dword ptr [eax+4]
1002d4b1  mov ecx, dword_103EC918
1002d4b7  mov eax, [ecx]
1002d4b9  push edi
1002d4ba  push ebx
1002d4bb  push esi
1002d4bc  call dword ptr [eax+50h]
1002d4bf  mov eax, dword_103EEAF0
1002d4c4  mov ecx, TlsIndex
1002d4ca  mov edx, large fs:2Ch
1002d4d1  mov ecx, [edx+ecx*4]
1002d4d4  cmp eax, [ecx+4]
1002d4da  jg loc_1002D62F
1002d4e0  mov eax, dword_103EEAEC
1002d4e5  mov ecx, [ebp+var_2C]
1002d4e8  mov [eax+esi], ecx
1002d4eb  mov ecx, dword_103EF050
1002d4f1  mov eax, [ecx]
1002d4f3  mov eax, [eax+4]
1002d4f6  mov [ebp+var_10], 0FFFFFFFFh
1002d4fd  push 0
1002d4ff  call eax
1002d501  mov eax, dword_103EC918
1002d506  movzx ecx, [ebp+var_25]
1002d50a  mov [eax+8], cl
1002d50d  movzx ecx, [ebp+var_26]
1002d511  mov [eax+18h], cl
1002d514  mov eax, [ebp+var_18]
1002d517  mov large fs:0, eax
1002d51d  add esp, 20h
1002d520  pop esi
1002d521  pop edi
1002d522  pop ebx
1002d523  pop ebp
1002d524  retn 4
1002d527  push offset dword_103EF058
1002d52c  call sub_101809C3
1002d531  add esp, 4
1002d534  cmp dword_103EF058, 0FFFFFFFFh
1002d53b  jnz loc_1002D322
1002d541  mov [ebp+var_24], edi
1002d544  mov [ebp+var_20], esi
1002d547  mov [ebp+var_10], 0
1002d54e  push offset aMFflags; "m_fFlags"
1002d553  push offset aCbaseplayer; "CBasePlayer"
1002d558  call sub_1017C730
1002d55d  add esp, 8
1002d560  mov dword_103EF054, eax
1002d565  push offset dword_103EF058
1002d56a  call sub_10180A43
1002d56f  add esp, 4
1002d572  mov esi, [ebp+var_20]
1002d575  mov edi, [ebp+var_24]
1002d578  jmp loc_1002D322
1002d57d  push offset dword_103EF060
1002d582  call sub_101809C3
1002d587  add esp, 4
1002d58a  cmp dword_103EF060, 0FFFFFFFFh
1002d591  jnz loc_1002D366
1002d597  mov [ebp+var_24], edi
1002d59a  mov [ebp+var_20], esi
1002d59d  mov [ebp+var_10], 1
1002d5a4  push offset aMHconstrainten; "m_hConstraintEntity"
1002d5a9  push offset aCbaseplayer; "CBasePlayer"
1002d5ae  call sub_1017C730
1002d5b3  add esp, 8
1002d5b6  add eax, 0FFFFFFF8h
1002d5b9  mov dword_103EF05C, eax
1002d5be  push offset dword_103EF060
1002d5c3  call sub_10180A43
1002d5c8  add esp, 4
1002d5cb  mov esi, [ebp+var_20]
1002d5ce  mov edi, [ebp+var_24]
1002d5d1  jmp loc_1002D366
1002d5d6  push offset dword_103EEAF0
1002d5db  call sub_101809C3
1002d5e0  add esp, 4
1002d5e3  cmp dword_103EEAF0, 0FFFFFFFFh
1002d5ea  jnz loc_1002D3DA
1002d5f0  mov [ebp+var_24], edi
1002d5f3  mov [ebp+var_20], esi
1002d5f6  mov [ebp+var_10], 2
1002d5fd  push offset aMNtickbase; "m_nTickBase"
1002d602  push offset aCbaseplayer; "CBasePlayer"
1002d607  call sub_1017C730
1002d60c  add esp, 8
1002d60f  mov dword_103EEAEC, eax
1002d614  push offset dword_103EEAF0
1002d619  call sub_10180A43
1002d61e  add esp, 4
1002d621  mov esi, [ebp+var_20]
1002d624  mov edi, [ebp+var_24]
1002d627  mov ebx, [ebp+arg_0]
1002d62a  jmp loc_1002D3DA
1002d62f  push offset dword_103EEAF0
1002d634  call sub_101809C3
1002d639  add esp, 4
1002d63c  cmp dword_103EEAF0, 0FFFFFFFFh
1002d643  jnz loc_1002D4E0
1002d649  mov [ebp+var_20], esi
1002d64c  mov [ebp+var_10], 4
1002d653  push offset aMNtickbase; "m_nTickBase"
1002d658  push offset aCbaseplayer; "CBasePlayer"
1002d65d  call sub_1017C730
1002d662  add esp, 8
1002d665  mov dword_103EEAEC, eax
1002d66a  push offset dword_103EEAF0
1002d66f  call sub_10180A43
1002d674  add esp, 4
1002d677  mov esi, [ebp+var_20]
1002d67a  jmp loc_1002D4E0
1002d67f  push offset dword_103EEAF0
1002d684  call sub_101809C3
1002d689  add esp, 4
1002d68c  cmp dword_103EEAF0, 0FFFFFFFFh
1002d693  jnz loc_1002D429
1002d699  mov [ebp+var_24], edi
1002d69c  mov [ebp+var_20], esi
1002d69f  mov [ebp+var_10], 3
1002d6a6  push offset aMNtickbase; "m_nTickBase"
1002d6ab  push offset aCbaseplayer; "CBasePlayer"
1002d6b0  call sub_1017C730
1002d6b5  add esp, 8
1002d6b8  mov dword_103EEAEC, eax
1002d6bd  push offset dword_103EEAF0
1002d6c2  call sub_10180A43
1002d6c7  add esp, 4
1002d6ca  mov esi, [ebp+var_20]
1002d6cd  mov edi, [ebp+var_24]
1002d6d0  mov ebx, [ebp+arg_0]
1002d6d3  jmp loc_1002D429
1002d6e0  push ebp
1002d6e1  sub esp, 10h
1002d6e4  add ebp, 0Ch
1002d6e7  push offset dword_103EF058
1002d6ec  call sub_10180A15
1002d6f1  add esp, 14h
1002d6f4  pop ebp
1002d6f5  retn
1002d700  push ebp
1002d701  sub esp, 10h
1002d704  add ebp, 0Ch
1002d707  push offset dword_103EF060
1002d70c  call sub_10180A15
1002d711  add esp, 14h
1002d714  pop ebp
1002d715  retn
1002d720  push ebp
1002d721  sub esp, 10h
1002d724  add ebp, 0Ch
1002d727  push offset dword_103EEAF0
1002d72c  call sub_10180A15
1002d731  add esp, 14h
1002d734  pop ebp
1002d735  retn
1002d740  push ebp
1002d741  sub esp, 10h
1002d744  add ebp, 0Ch
1002d747  push offset dword_103EEAF0
1002d74c  call sub_10180A15
1002d751  add esp, 14h
1002d754  pop ebp
1002d755  retn
1002d760  push ebp
1002d761  sub esp, 10h
1002d764  add ebp, 0Ch
1002d767  push offset dword_103EEAF0
1002d76c  call sub_10180A15
1002d771  add esp, 14h
1002d774  pop ebp
1002d775  retn
1002d8a0  mov eax, offset stru_10296DEC
1002d8a5  jmp __CxxFrameHandler3

; NoSpread_PredictionEnd @ 0x1002d780; complete 72 instructions
1002d780  push ebp
1002d781  mov ebp, esp
1002d783  push ebx
1002d784  push edi
1002d785  push esi
1002d786  sub esp, 14h
1002d789  mov esi, ecx
1002d78b  mov [ebp+var_1C], esp
1002d78e  mov [ebp+var_10], 0FFFFFFFFh
1002d795  lea eax, [ebp+var_18]
1002d798  mov [ebp+var_14], offset loc_1002D8B0
1002d79f  mov ecx, large fs:0
1002d7a6  mov [ebp+var_18], ecx
1002d7a9  mov large fs:0, eax
1002d7af  mov ecx, off_1029E014
1002d7b5  call sub_10179540
1002d7ba  test eax, eax
1002d7bc  jz short loc_1002D81C
1002d7be  mov edi, eax
1002d7c0  vmovss xmm0, dword ptr [esi+0A8h]
1002d7c8  mov eax, dword_103EC8D0
1002d7cd  vmovss dword ptr [eax+0Ch], xmm0
1002d7d2  vmovss xmm0, dword ptr [esi+0ACh]
1002d7da  vmovss dword ptr [eax+10h], xmm0
1002d7df  mov ecx, [esi+0B0h]
1002d7e5  mov [eax+18h], ecx
1002d7e8  mov eax, dword_103EF060
1002d7ed  mov ecx, TlsIndex
1002d7f3  mov edx, large fs:2Ch
1002d7fa  mov ecx, [edx+ecx*4]
1002d7fd  cmp eax, [ecx+4]
1002d803  jg short loc_1002D82D
1002d805  mov eax, dword_103EF05C
1002d80a  mov dword ptr [eax+edi], 0
1002d811  mov eax, Prediction_SeedPointer
1002d816  mov dword ptr [eax], 0FFFFFFFFh
1002d81c  mov eax, [ebp+var_18]
1002d81f  mov large fs:0, eax
1002d825  add esp, 14h
1002d828  pop esi
1002d829  pop edi
1002d82a  pop ebx
1002d82b  pop ebp
1002d82c  retn
1002d82d  push offset dword_103EF060
1002d832  call sub_101809C3
1002d837  add esp, 4
1002d83a  cmp dword_103EF060, 0FFFFFFFFh
1002d841  jnz short loc_1002D805
1002d843  mov [ebp+var_20], edi
1002d846  mov [ebp+var_10], 0
1002d84d  push offset aMHconstrainten; "m_hConstraintEntity"
1002d852  push offset aCbaseplayer; "CBasePlayer"
1002d857  call sub_1017C730
1002d85c  add esp, 8
1002d85f  add eax, 0FFFFFFF8h
1002d862  mov dword_103EF05C, eax
1002d867  push offset dword_103EF060
1002d86c  call sub_10180A43
1002d871  add esp, 4
1002d874  mov edi, [ebp+var_20]
1002d877  jmp short loc_1002D805
1002d880  push ebp
1002d881  sub esp, 8
1002d884  add ebp, 0Ch
1002d887  push offset dword_103EF060
1002d88c  call sub_10180A15
1002d891  add esp, 0Ch
1002d894  pop ebp
1002d895  retn
1002d8b0  mov eax, offset stru_10296E38
1002d8b5  jmp __CxxFrameHandler3

; NoSpread_FixMovement @ 0x10027ea0; complete 82 instructions
10027ea0  push edi
10027ea1  push esi
10027ea2  sub esp, 4Ch
10027ea5  mov edi, [esp+54h+arg_4]
10027ea9  mov esi, [esp+54h+arg_0]
10027ead  vmovss xmm1, dword ptr [esi+18h]
10027eb2  vmovss xmm2, dword ptr [esi+1Ch]
10027eb7  vxorps xmm0, xmm0, xmm0
10027ebb  vucomiss xmm2, xmm0
10027ebf  jnz short loc_10027EE7
10027ec1  jp short loc_10027EE7
10027ec3  vucomiss xmm1, xmm0
10027ec7  jnz short loc_10027EE7
10027ec9  jp short loc_10027EE7
10027ecb  vmulss xmm0, xmm2, xmm2
10027ecf  vfmadd213ss xmm1, xmm1, xmm0
10027ed4  vcvtss2sd xmm1, xmm1, xmm1
10027ed8  vxorps xmm0, xmm0, xmm0
10027edc  vmovups [esp+54h+var_28], xmm0
10027ee2  jmp loc_10027F9B
10027ee7  vcvtss2sd xmm0, xmm1, xmm1
10027eeb  vmovss dword ptr [esp+54h+var_40], xmm1
10027ef1  vcvtss2sd xmm1, xmm2, xmm2
10027ef5  vmovss xmm3, dword ptr [esi+20h]
10027efa  vmovups [esp+54h+var_18], xmm3
10027f00  vmovsd [esp+54h+var_4C], xmm0
10027f06  vmovsd [esp+54h+var_54], xmm1
10027f0b  vmovss [esp+54h+var_44], xmm2
10027f11  call atan2
10027f16  fstp [esp+54h+var_2C]
10027f1a  vmovss xmm0, [esp+54h+var_2C]
10027f20  vmulss xmm0, xmm0, ds:dword_10184F14
10027f28  vaddss xmm1, xmm0, ds:dword_10184F54
10027f30  vcmpltss xmm2, xmm0, ds:dword_10184F30
10027f39  vblendvps xmm0, xmm0, xmm1, xmm2
10027f3f  vmovups [esp+54h+var_28], xmm0
10027f45  vmovss xmm0, [esp+54h+var_44]
10027f4b  vmulss xmm0, xmm0, xmm0
10027f4f  vmovss xmm1, dword ptr [esp+54h+var_40]
10027f55  vfmadd231ss xmm0, xmm1, xmm1
10027f5a  vcvtss2sd xmm0, xmm0, xmm0
10027f5e  vmovsd [esp+54h+var_40], xmm0
10027f64  vsqrtsd xmm0, xmm0, xmm0
10027f68  vcvtsd2ss xmm0, xmm0, xmm0
10027f6c  vbroadcastss xmm1, ds:dword_1018513C
10027f75  vxorps xmm1, xmm1, [esp+54h+var_18]
10027f7b  vcvtss2sd xmm0, xmm0, xmm0
10027f7f  vcvtss2sd xmm1, xmm1, xmm1
10027f83  vmovsd [esp+54h+var_4C], xmm0
10027f89  vmovsd [esp+54h+var_54], xmm1
10027f8e  call atan2
10027f93  fstp st
10027f95  vmovsd xmm1, [esp+54h+var_40]
10027f9b  vmovsd [esp+54h+var_54], xmm1
10027fa0  call sub_10028030
10027fa5  sub esp, 8
10027fa8  fstp [esp+54h+var_30]
10027fac  vmovss xmm0, [esp+54h+var_30]
10027fb2  vmovss dword ptr [esp+54h+var_18], xmm0
10027fb8  vmovss xmm0, dword ptr [edi+4]
10027fbd  vsubss xmm0, xmm0, dword ptr [esi+10h]
10027fc2  vaddss xmm0, xmm0, dword ptr [esp+54h+var_28]
10027fc8  vmulss xmm0, xmm0, ds:dword_10184F10
10027fd0  vcvtss2sd xmm0, xmm0, xmm0
10027fd4  vmovsd qword ptr [esp+54h+var_28], xmm0
10027fda  vmovsd [esp+54h+var_54], xmm0
10027fdf  call cos
10027fe4  fstp [esp+54h+var_34]
10027fe8  vmovss xmm0, dword ptr [esp+54h+var_18]
10027fee  vmulss xmm0, xmm0, [esp+54h+var_34]
10027ff4  vmovss dword ptr [esi+18h], xmm0
10027ff9  vmovsd xmm0, qword ptr [esp+54h+var_28]
10027fff  vmovsd [esp+54h+var_54], xmm0
10028004  call sin
10028009  fstp [esp+54h+var_38]
1002800d  vmovss xmm0, dword ptr [esp+54h+var_18]
10028013  vmulss xmm0, xmm0, [esp+54h+var_38]
10028019  vmovss dword ptr [esi+1Ch], xmm0
1002801e  add esp, 4Ch
10028021  pop esi
10028022  pop edi
10028023  retn 8

; Weapon_CanFireFromTickbase @ 0x10023180; complete 139 instructions
10023180  push ebp
10023181  mov ebp, esp
10023183  push ebx
10023184  push edi
10023185  push esi
10023186  sub esp, 18h
10023189  mov edi, ecx
1002318b  mov esi, [ebp+arg_0]
1002318e  mov [ebp+var_1C], esp
10023191  mov [ebp+var_10], 0FFFFFFFFh
10023198  lea eax, [ebp+var_18]
1002319b  mov [ebp+var_14], offset loc_10026B80
100231a2  mov ecx, large fs:0
100231a9  mov [ebp+var_18], ecx
100231ac  mov large fs:0, eax
100231b2  test esi, esi
100231b4  jz short loc_10023218
100231b6  mov eax, dword_103EEAF0
100231bb  mov ecx, TlsIndex
100231c1  mov edx, large fs:2Ch
100231c8  mov ecx, [edx+ecx*4]
100231cb  cmp eax, [ecx+4]
100231d1  jg loc_1002325E
100231d7  mov eax, dword_103EEAEC
100231dc  vcvtsi2ss xmm0, xmm0, dword ptr [eax+esi]
100231e1  mov eax, dword_103EC8D0
100231e6  vmulss xmm0, xmm0, dword ptr [eax+1Ch]
100231eb  mov eax, dword_103EEAF8
100231f0  mov ecx, TlsIndex
100231f6  mov edx, large fs:2Ch
100231fd  mov ecx, [edx+ecx*4]
10023200  cmp eax, [ecx+4]
10023206  jg loc_100232B1
1002320c  mov eax, dword_103EEAF4
10023211  vucomiss xmm0, dword ptr [eax+edi]
10023216  jnb short loc_1002321C
10023218  xor eax, eax
1002321a  jmp short loc_1002324A
1002321c  mov eax, dword_103EEB00
10023221  mov ecx, TlsIndex
10023227  mov edx, large fs:2Ch
1002322e  mov ecx, [edx+ecx*4]
10023231  cmp eax, [ecx+4]
10023237  jg loc_10023313
1002323d  mov eax, dword_103EEAFC
10023242  vucomiss xmm0, dword ptr [eax+esi]
10023247  setnb al
1002324a  mov ecx, [ebp+var_18]
1002324d  mov large fs:0, ecx
10023254  add esp, 18h
10023257  pop esi
10023258  pop edi
10023259  pop ebx
1002325a  pop ebp
1002325b  retn 4
1002325e  push offset dword_103EEAF0
10023263  call sub_101809C3
10023268  add esp, 4
1002326b  cmp dword_103EEAF0, 0FFFFFFFFh
10023272  jnz loc_100231D7
10023278  mov [ebp+var_24], edi
1002327b  mov [ebp+var_10], 0
10023282  push offset aMNtickbase; "m_nTickBase"
10023287  push offset aCbaseplayer; "CBasePlayer"
1002328c  call sub_1017C730
10023291  add esp, 8
10023294  mov dword_103EEAEC, eax
10023299  push offset dword_103EEAF0
1002329e  call sub_10180A43
100232a3  add esp, 4
100232a6  mov esi, [ebp+arg_0]
100232a9  mov edi, [ebp+var_24]
100232ac  jmp loc_100231D7
100232b1  push offset dword_103EEAF8
100232b6  vmovss [ebp+var_20], xmm0
100232bb  call sub_101809C3
100232c0  vmovss xmm0, [ebp+var_20]
100232c5  add esp, 4
100232c8  cmp dword_103EEAF8, 0FFFFFFFFh
100232cf  jnz loc_1002320C
100232d5  mov [ebp+var_24], edi
100232d8  mov [ebp+var_10], 1
100232df  push offset aMFlnextprimary; "m_flNextPrimaryAttack"
100232e4  push offset aCbasecombatwea; "CBaseCombatWeapon"
100232e9  call sub_1017C730
100232ee  add esp, 8
100232f1  mov dword_103EEAF4, eax
100232f6  push offset dword_103EEAF8
100232fb  call sub_10180A43
10023300  add esp, 4
10023303  mov esi, [ebp+arg_0]
10023306  mov edi, [ebp+var_24]
10023309  vmovss xmm0, [ebp+var_20]
1002330e  jmp loc_1002320C
10023313  push offset dword_103EEB00
10023318  vmovss [ebp+var_20], xmm0
1002331d  call sub_101809C3
10023322  vmovss xmm0, [ebp+var_20]
10023327  add esp, 4
1002332a  cmp dword_103EEB00, 0FFFFFFFFh
10023331  jnz loc_1002323D
10023337  mov [ebp+var_10], 2
1002333e  push offset aMFlnextattack; "m_flNextAttack"
10023343  push offset aCbasecombatcha; "CBaseCombatCharacter"
10023348  call sub_1017C730
1002334d  add esp, 8
10023350  mov dword_103EEAFC, eax
10023355  push offset dword_103EEB00
1002335a  call sub_10180A43
1002335f  add esp, 4
10023362  mov esi, [ebp+arg_0]
10023365  vmovss xmm0, [ebp+var_20]
1002336a  jmp loc_1002323D
10023370  push ebp
10023371  sub esp, 8
10023374  add ebp, 0Ch
10023377  push offset dword_103EEB00
1002337c  call sub_10180A15
10023381  add esp, 0Ch
10023384  pop ebp
10023385  retn
10023390  push ebp
10023391  sub esp, 8
10023394  add ebp, 0Ch
10023397  push offset dword_103EEAF8
1002339c  call sub_10180A15
100233a1  add esp, 0Ch
100233a4  pop ebp
100233a5  retn
100233b0  push ebp
100233b1  sub esp, 8
100233b4  add ebp, 0Ch
100233b7  push offset dword_103EEAF0
100233bc  call sub_10180A15
100233c1  add esp, 0Ch
100233c4  pop ebp
100233c5  retn
10026b80  mov eax, offset stru_102969F4
10026b85  jmp __CxxFrameHandler3

