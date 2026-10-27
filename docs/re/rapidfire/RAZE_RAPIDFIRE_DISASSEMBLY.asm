; RapidFire_ArmExtraMoves 0x101486e0 / 331 instructions
0x101486e0  push ebp
0x101486e1  mov ebp, esp
0x101486e3  push ebx
0x101486e4  push edi
0x101486e5  push esi
0x101486e6  sub esp, 18h
0x101486e9  mov esi, [ebp+arg_4]
0x101486ec  mov edi, [ebp+arg_0]
0x101486ef  mov [ebp+var_1C], esp
0x101486f2  mov [ebp+var_10], 0FFFFFFFFh
0x101486f9  lea eax, [ebp+var_18]
0x101486fc  mov [ebp+var_14], offset loc_101491F0
0x10148703  mov ecx, large fs:0
0x1014870a  mov [ebp+var_18], ecx
0x1014870d  mov large fs:0, eax
0x10148713  mov ebx, [esi+4]
0x10148716  mov ecx, 1B4E81B5h
0x1014871b  mov eax, ebx
0x1014871d  imul ecx
0x1014871f  mov eax, edx
0x10148721  shr eax, 1Fh
0x10148724  sar edx, 4
0x10148727  add edx, eax
0x10148729  imul eax, edx, 96h
0x1014872f  sub ebx, eax
0x10148731  cmp byte_103F5380, 1
0x10148738  jnz short loc_10148761
0x1014873a  push dword_103F5384; vKey ; ['vKey']
0x10148740  call ds:GetAsyncKeyState; USER32
0x10148746  test ax, ax
0x10148749  js loc_101487E8
0x1014874f  mov eax, [edi+10h]
0x10148752  cmp eax, 2
0x10148755  mov ecx, 1
0x1014875a  cmovge ecx, eax
0x1014875d  dec ecx
0x1014875e  mov [edi+10h], ecx
0x10148761  cmp byte ptr word_103F53E0+1, 1
0x10148768  jnz loc_1014884E
0x1014876e  mov ecx, off_1029E014
0x10148774  call sub_10179540
0x10148779  test eax, eax
0x1014877b  jz loc_1014884E
0x10148781  mov esi, eax
0x10148783  mov eax, dword_103EEAD8
0x10148788  mov ecx, TlsIndex
0x1014878e  mov edx, large fs:2Ch
0x10148795  mov ecx, [edx+ecx*4]
0x10148798  cmp eax, [ecx+4]
0x1014879e  jg loc_10148B49
0x101487a4  mov eax, dword_103EEAD4
0x101487a9  cmp dword ptr [eax+esi], 3
0x101487ad  jnz loc_1014884E
0x101487b3  push dword_103F53E4; vKey ; ['vKey']
0x101487b9  call ds:GetAsyncKeyState; USER32
0x101487bf  test ax, ax
0x101487c2  js loc_101488CF
0x101487c8  mov eax, [esi+10h]
0x101487cb  cmp eax, 2
0x101487ce  mov ecx, 1
0x101487d3  cmovge ecx, eax
0x101487d6  dec ecx
0x101487d7  mov [esi+10h], ecx
0x101487da  cmp byte_103F53F0, 1
0x101487e1  jz short loc_1014885B
0x101487e3  jmp loc_1014897C
0x101487e8  mov eax, dword_103EC8D0
0x101487ed  vmovss xmm0, dword ptr [eax+1Ch]
0x101487f2  vmovss dword ptr [edi+10h], xmm0
0x101487f7  mov eax, [esi+4]
0x101487fa  mov [edi+14h], eax
0x101487fd  mov eax, dword_103EC8D0
0x10148802  vmovss xmm0, ds:dword_101852F4
0x1014880a  vdivss xmm0, xmm0, dword ptr [eax+1Ch]
0x1014880f  vcvttss2si eax, xmm0
0x10148813  mov ecx, eax
0x10148815  sar ecx, 1Fh
0x10148818  andn eax, ecx, eax
0x1014881d  vcvtsi2ss xmm0, xmm1, eax
0x10148821  mov RapidFire_TickbaseShiftRing[ebx*8], eax
0x10148828  vaddss xmm0, xmm0, ds:dword_10184184
0x10148830  vmulss xmm0, xmm0, dword ptr [edi+10h]
0x10148835  vmovss dword ptr [edi+10h], xmm0
0x1014883a  mov RapidFire_ForceSend, 1
0x10148841  cmp byte ptr word_103F53E0+1, 1
0x10148848  jz loc_1014876E
0x1014884e  cmp byte_103F53F0, 1
0x10148855  jnz loc_1014897C
0x1014885b  mov ecx, off_1029E014
0x10148861  mov [ebp+var_10], 0FFFFFFFFh
0x10148868  call sub_10179540
0x1014886d  test eax, eax
0x1014886f  jz loc_1014897C
0x10148875  mov esi, eax
0x10148877  mov eax, dword_103EEAD8
0x1014887c  mov ecx, TlsIndex
0x10148882  mov edx, large fs:2Ch
0x10148889  mov ecx, [edx+ecx*4]
0x1014888c  cmp eax, [ecx+4]
0x10148892  jg loc_10148BA2
0x10148898  mov eax, dword_103EEAD4
0x1014889d  cmp dword ptr [eax+esi], 2
0x101488a1  jnz loc_1014897C
0x101488a7  push dword_103F53F8; vKey ; ['vKey']
0x101488ad  call ds:GetAsyncKeyState; USER32
0x101488b3  test ax, ax
0x101488b6  js short loc_1014892D
0x101488b8  mov eax, [esi+10h]
0x101488bb  cmp eax, 2
0x101488be  mov ecx, 1
0x101488c3  cmovge ecx, eax
0x101488c6  dec ecx
0x101488c7  mov [esi+10h], ecx
0x101488ca  jmp loc_1014897C
0x101488cf  mov eax, dword_103EC8D0
0x101488d4  vmovss xmm0, ds:dword_10191CD0
0x101488dc  vdivss xmm0, xmm0, dword ptr [eax+1Ch]
0x101488e1  vaddss xmm0, xmm0, ds:dword_10184EDC
0x101488e9  vcvttss2si eax, xmm0
0x101488ed  mov ecx, dword_103F53EC
0x101488f3  cmp ecx, eax
0x101488f5  cmovg eax, ecx
0x101488f8  xor ecx, ecx
0x101488fa  test eax, eax
0x101488fc  cmovns ecx, eax
0x101488ff  mov RapidFire_ExtraMovesRemaining, eax
0x10148904  mov RapidFire_TickbaseShiftRing[ebx*8], ecx
0x1014890b  mov eax, [esi+10h]
0x1014890e  inc eax
0x1014890f  cmp eax, ecx
0x10148911  cmovge eax, ecx
0x10148914  mov [esi+10h], eax
0x10148917  mov RapidFire_ForceSend, 1
0x1014891e  cmp byte_103F53F0, 1
0x10148925  jz loc_1014885B
0x1014892b  jmp short loc_1014897C
0x1014892d  mov eax, dword_103EC8D0
0x10148932  vmovss xmm0, ds:dword_10191CD0
0x1014893a  vdivss xmm0, xmm0, dword ptr [eax+1Ch]
0x1014893f  vaddss xmm0, xmm0, ds:dword_10184EDC
0x10148947  vcvttss2si eax, xmm0
0x1014894b  mov ecx, dword_103F5400
0x10148951  cmp ecx, eax
0x10148953  cmovg eax, ecx
0x10148956  xor ecx, ecx
0x10148958  test eax, eax
0x1014895a  cmovns ecx, eax
0x1014895d  mov RapidFire_ExtraMovesRemaining, eax
0x10148962  mov RapidFire_TickbaseShiftRing[ebx*8], ecx
0x10148969  mov eax, [esi+10h]
0x1014896c  inc eax
0x1014896d  cmp eax, ecx
0x1014896f  cmovge eax, ecx
0x10148972  mov [esi+10h], eax
0x10148975  mov RapidFire_ForceSend, 1
0x1014897c  cmp RapidFire_Enabled, 1
0x10148983  jnz loc_10148B36
0x10148989  mov eax, RapidFire_Method
0x1014898e  test eax, eax
0x10148990  jz short loc_101489AD
0x10148992  cmp eax, 1
0x10148995  jz loc_10148A39
0x1014899b  cmp RapidFire_Method, 2
0x101489a2  jz loc_10148AC2
0x101489a8  jmp loc_10148B36
0x101489ad  push RapidFire_Key; vKey ; ['vKey']
0x101489b3  call ds:GetAsyncKeyState; USER32
0x101489b9  test ax, ax
0x101489bc  js short loc_101489DC
0x101489be  mov eax, [edi+10h]
0x101489c1  cmp eax, 2
0x101489c4  mov ecx, 1
0x101489c9  cmovge ecx, eax
0x101489cc  dec ecx
0x101489cd  mov [edi+10h], ecx
0x101489d0  mov eax, RapidFire_Method
0x101489d5  cmp eax, 1
0x101489d8  jnz short loc_1014899B
0x101489da  jmp short loc_10148A39
0x101489dc  mov eax, dword_103EC8D0
0x101489e1  vmovss xmm0, ds:dword_10191CD0; Rapidfire method=0: N=max(config StormSpeed, int(0.06f/interval+0.5f)); sets shared CL_Move counter and command_number%150 tickbase correction. No NextPrimaryAttack write in this block. ; ['Rapidfire method=0: N=max(config StormSpeed, int(0.06f/interval+0.5f)); sets shared CL_Move counter and command_number%150 tickbase correction. No NextPrimaryAttack write in this block.']
0x101489e9  vdivss xmm0, xmm0, dword ptr [eax+1Ch]
0x101489ee  vaddss xmm0, xmm0, ds:dword_10184EDC
0x101489f6  vcvttss2si eax, xmm0
0x101489fa  mov ecx, RapidFire_StormSpeed
0x10148a00  cmp ecx, eax
0x10148a02  cmovg eax, ecx
0x10148a05  xor ecx, ecx
0x10148a07  test eax, eax
0x10148a09  cmovns ecx, eax
0x10148a0c  mov RapidFire_ExtraMovesRemaining, eax
0x10148a11  mov RapidFire_TickbaseShiftRing[ebx*8], ecx
0x10148a18  mov eax, [edi+10h]
0x10148a1b  inc eax
0x10148a1c  cmp eax, ecx
0x10148a1e  cmovge eax, ecx
0x10148a21  mov [edi+10h], eax
0x10148a24  mov RapidFire_ForceSend, 1
0x10148a2b  mov eax, RapidFire_Method
0x10148a30  cmp eax, 1
0x10148a33  jnz loc_1014899B
0x10148a39  push RapidFire_Key; vKey ; ['vKey']
0x10148a3f  call ds:GetAsyncKeyState; USER32
0x10148a45  test ax, ax
0x10148a48  js short loc_10148A6A
0x10148a4a  mov eax, [edi+10h]
0x10148a4d  cmp eax, 2
0x10148a50  mov ecx, 1
0x10148a55  cmovge ecx, eax
0x10148a58  dec ecx
0x10148a59  mov [edi+10h], ecx
0x10148a5c  cmp RapidFire_Method, 2
0x10148a63  jz short loc_10148AC2
0x10148a65  jmp loc_10148B36
0x10148a6a  mov eax, dword_103EC8D0
0x10148a6f  vmovss xmm0, ds:dword_10191CD0
0x10148a77  vdivss xmm0, xmm0, dword ptr [eax+1Ch]
0x10148a7c  vaddss xmm0, xmm0, ds:dword_10184EDC
0x10148a84  vcvttss2si eax, xmm0
0x10148a88  mov ecx, RapidFire_Method2Speed
0x10148a8e  cmp ecx, eax
0x10148a90  cmovg eax, ecx
0x10148a93  xor ecx, ecx
0x10148a95  test eax, eax
0x10148a97  cmovns ecx, eax
0x10148a9a  mov RapidFire_ExtraMovesRemaining, eax
0x10148a9f  mov RapidFire_TickbaseShiftRing[ebx*8], ecx
0x10148aa6  mov eax, [edi+10h]
0x10148aa9  inc eax
0x10148aaa  cmp eax, ecx
0x10148aac  cmovge eax, ecx
0x10148aaf  mov [edi+10h], eax
0x10148ab2  mov RapidFire_ForceSend, 1
0x10148ab9  cmp RapidFire_Method, 2
0x10148ac0  jnz short loc_10148B36
0x10148ac2  push RapidFire_Key; vKey ; ['vKey']
0x10148ac8  call ds:GetAsyncKeyState; USER32
0x10148ace  test ax, ax
0x10148ad1  js short loc_10148AE7
0x10148ad3  mov eax, [edi+10h]
0x10148ad6  cmp eax, 2
0x10148ad9  mov ecx, 1
0x10148ade  cmovge ecx, eax
0x10148ae1  dec ecx
0x10148ae2  mov [edi+10h], ecx
0x10148ae5  jmp short loc_10148B36
0x10148ae7  mov eax, dword_103EC8D0
0x10148aec  vmovss xmm0, ds:dword_10191CD0
0x10148af4  vdivss xmm0, xmm0, dword ptr [eax+1Ch]
0x10148af9  vaddss xmm0, xmm0, ds:dword_10184EDC
0x10148b01  vcvttss2si eax, xmm0
0x10148b05  mov ecx, RapidFire_Method3Speed
0x10148b0b  cmp ecx, eax
0x10148b0d  cmovg eax, ecx
0x10148b10  xor ecx, ecx
0x10148b12  test eax, eax
0x10148b14  cmovns ecx, eax
0x10148b17  mov RapidFire_ExtraMovesRemaining, eax
0x10148b1c  mov RapidFire_TickbaseShiftRing[ebx*8], ecx
0x10148b23  mov eax, [edi+10h]
0x10148b26  inc eax
0x10148b27  cmp eax, ecx
0x10148b29  cmovge eax, ecx
0x10148b2c  mov [edi+10h], eax
0x10148b2f  mov RapidFire_ForceSend, 1
0x10148b36  mov eax, [ebp+var_18]
0x10148b39  mov large fs:0, eax
0x10148b3f  add esp, 18h
0x10148b42  pop esi
0x10148b43  pop edi
0x10148b44  pop ebx
0x10148b45  pop ebp
0x10148b46  retn 8
0x10148b49  push offset dword_103EEAD8
0x10148b4e  call sub_101809C3
0x10148b53  add esp, 4
0x10148b56  cmp dword_103EEAD8, 0FFFFFFFFh
0x10148b5d  jnz loc_101487A4
0x10148b63  mov [ebp+var_20], esi
0x10148b66  mov [ebp+var_24], ebx
0x10148b69  mov [ebp+var_10], 0
0x10148b70  push offset aMIteamnum; "m_iTeamNum"
0x10148b75  push offset aCbaseentity; "CBaseEntity"
0x10148b7a  call sub_1017C730
0x10148b7f  add esp, 8
0x10148b82  mov dword_103EEAD4, eax
0x10148b87  push offset dword_103EEAD8
0x10148b8c  call sub_10180A43
0x10148b91  add esp, 4
0x10148b94  mov edi, [ebp+arg_0]
0x10148b97  mov ebx, [ebp+var_24]
0x10148b9a  mov esi, [ebp+var_20]
0x10148b9d  jmp loc_101487A4
0x10148ba2  push offset dword_103EEAD8
0x10148ba7  call sub_101809C3
0x10148bac  add esp, 4
0x10148baf  cmp dword_103EEAD8, 0FFFFFFFFh
0x10148bb6  jnz loc_10148898
0x10148bbc  mov [ebp+var_20], esi
0x10148bbf  mov [ebp+var_24], ebx
0x10148bc2  mov [ebp+var_10], 1
0x10148bc9  push offset aMIteamnum; "m_iTeamNum"
0x10148bce  push offset aCbaseentity; "CBaseEntity"
0x10148bd3  call sub_1017C730
0x10148bd8  add esp, 8
0x10148bdb  mov dword_103EEAD4, eax
0x10148be0  push offset dword_103EEAD8
0x10148be5  call sub_10180A43
0x10148bea  add esp, 4
0x10148bed  mov edi, [ebp+arg_0]
0x10148bf0  mov ebx, [ebp+var_24]
0x10148bf3  mov esi, [ebp+var_20]
0x10148bf6  jmp loc_10148898
0x10148c00  push ebp
0x10148c01  sub esp, 8
0x10148c04  add ebp, 0Ch
0x10148c07  push offset dword_103EEAD8
0x10148c0c  call sub_10180A15
0x10148c11  add esp, 0Ch
0x10148c14  pop ebp
0x10148c15  retn
0x10148c20  push ebp
0x10148c21  sub esp, 8
0x10148c24  add ebp, 0Ch
0x10148c27  push offset dword_103EEAD8
0x10148c2c  call sub_10180A15
0x10148c31  add esp, 0Ch
0x10148c34  pop ebp
0x10148c35  retn
0x101491f0  mov eax, offset stru_1029BB10
0x101491f5  jmp __CxxFrameHandler3

; RapidFire_ApplyNetChannelMode 0x10148c40 / 108 instructions
0x10148c40  push ebp
0x10148c41  push ebx
0x10148c42  push edi
0x10148c43  push esi
0x10148c44  sub esp, 0Ch
0x10148c47  mov eax, [esp+1Ch+arg_4]
0x10148c4b  mov ecx, ___security_cookie
0x10148c51  xor ecx, esp
0x10148c53  mov [esp+1Ch+var_14], ecx
0x10148c57  mov edi, [eax+4]
0x10148c5a  mov ecx, 1B4E81B5h
0x10148c5f  mov eax, edi
0x10148c61  imul ecx
0x10148c63  mov ebx, edx
0x10148c65  lea eax, [edi-1]
0x10148c68  imul ecx
0x10148c6a  mov ebp, edx
0x10148c6c  push offset ModuleName; "engine.dll"
0x10148c71  call ds:GetModuleHandleA; KERNEL32
0x10148c77  cmp RapidFire_Enabled, 1
0x10148c7e  jnz loc_10148D34
0x10148c84  mov esi, eax
0x10148c86  mov eax, RapidFire_Method
0x10148c8b  test eax, eax
0x10148c8d  jz short loc_10148C96
0x10148c8f  cmp eax, 1
0x10148c92  jz short loc_10148CC1
0x10148c94  jmp short loc_10148CFC
0x10148c96  mov eax, [esi+4268ECh]
0x10148c9c  mov eax, [eax+18h]
0x10148c9f  mov [esp+1Ch+var_1C], eax
0x10148ca2  push RapidFire_Key; vKey ; ['vKey']
0x10148ca8  call ds:GetAsyncKeyState; USER32
0x10148cae  test ax, ax
0x10148cb1  js loc_10148D49
0x10148cb7  mov eax, RapidFire_Method
0x10148cbc  cmp eax, 1
0x10148cbf  jnz short loc_10148CFC
0x10148cc1  push RapidFire_Key; vKey ; ['vKey']
0x10148cc7  call ds:GetAsyncKeyState; USER32
0x10148ccd  test ax, ax
0x10148cd0  jns short loc_10148CFC
0x10148cd2  mov ecx, dword_103EC770
0x10148cd8  mov eax, [ecx]
0x10148cda  call dword ptr [eax+128h]
0x10148ce0  test eax, eax
0x10148ce2  jz short loc_10148CFC
0x10148ce4  mov eax, RapidFire_Method2Choked; UI method 2 (index=1): netchannel+0x1C = configured ticks. Distinct from clientstate choked_commands. ; ['UI method 2 (index=1): netchannel+0x1C = configured ticks. Distinct from clientstate choked_commands.']
0x10148ce9  mov ecx, dword_103EC9A8
0x10148cef  mov ecx, [ecx+18h]
0x10148cf2  mov [ecx+1Ch], eax
0x10148cf5  mov RapidFire_ForceSend, 1
0x10148cfc  cmp RapidFire_Method, 2
0x10148d03  jnz short loc_10148D34
0x10148d05  push RapidFire_Key; vKey ; ['vKey']
0x10148d0b  call ds:GetAsyncKeyState; USER32
0x10148d11  test ax, ax
0x10148d14  jns short loc_10148D34
0x10148d16  mov eax, [esi+4268ECh]
0x10148d1c  mov eax, [eax+18h]
0x10148d1f  mov dword ptr [eax+10h], 0FFFFFFFFh; UI method 3 (index=2): netchannel+0x10=-1; netchannel+0x1C=255. Current engine SendDatagram serializes low 8 bits of +0x1C; do not infer extra damage. ; ['UI method 3 (index=2): netchannel+0x10=-1; netchannel+0x1C=255. Current engine SendDatagram serializes low 8 bits of +0x1C; do not infer extra damage.']
0x10148d26  mov dword ptr [eax+1Ch], 0FFh
0x10148d2d  mov RapidFire_ForceSend, 1
0x10148d34  mov ecx, [esp+1Ch+var_14]
0x10148d38  xor ecx, esp; StackCookie ; ['StackCookie']
0x10148d3a  call @__security_check_cookie@4; __security_check_cookie(x)
0x10148d3f  add esp, 0Ch
0x10148d42  pop esi
0x10148d43  pop edi
0x10148d44  pop ebx
0x10148d45  pop ebp
0x10148d46  retn 8
0x10148d49  mov eax, RapidFire_StormTicks
0x10148d4e  test eax, eax
0x10148d50  jle loc_10148CB7
0x10148d56  mov ecx, ebx
0x10148d58  shr ecx, 1Fh
0x10148d5b  sar ebx, 4
0x10148d5e  add ebx, ecx
0x10148d60  imul edx, ebx, 96h
0x10148d66  mov ecx, ebp
0x10148d68  shr ecx, 1Fh
0x10148d6b  sar ebp, 4
0x10148d6e  add ebp, ecx
0x10148d70  imul ecx, ebp, 96h
0x10148d76  neg ecx
0x10148d78  add ecx, edi
0x10148d7a  dec ecx
0x10148d7b  sub edi, edx
0x10148d7d  cmp edi, ecx
0x10148d7f  jnz short loc_10148DB4; Storm branch compares cmd%150 to (cmd-1)%150; unequal for normal positive commands. Preserve as observed; do not silently replace with a recharge condition. ; ['Storm branch compares cmd%150 to (cmd-1)%150; unequal for normal positive commands. Preserve as observed; do not silently replace with a recharge condition.']
0x10148d81  mov RapidFire_TickbaseShiftRing[edi*8], 0
0x10148d8c  mov edx, dword_103EC8D0
0x10148d92  vmovss xmm0, ds:dword_10189E04
0x10148d9a  vdivss xmm1, xmm0, dword ptr [edx+1Ch]
0x10148d9f  vaddss xmm0, xmm1, xmm0
0x10148da3  vcvttss2si edx, xmm0
0x10148da7  cmp eax, edx
0x10148da9  cmovg edx, eax
0x10148dac  mov RapidFire_ExtraMovesRemaining, edx
0x10148db2  jmp short loc_10148DBA
0x10148db4  inc dword_103F68E8
0x10148dba  cmp RapidFire_TickbaseShiftRing[ecx*8], 0
0x10148dc2  jnz loc_10148CB7
0x10148dc8  mov eax, [esp+1Ch+var_1C]
0x10148dcb  mov dword ptr [eax+10h], 0FFFFFFFFh
0x10148dd2  mov dword ptr [eax+1Ch], 0FFh
0x10148dd9  jmp loc_10148CB7

; RapidFire_Hook_CL_Move 0x1016ce80 / 24 instructions
0x1016ce80  push esi
0x1016ce81  sub esp, 8
0x1016ce84  mov eax, 0FFFFFFFFh
0x1016ce89  movzx esi, [esp+0Ch+arg_4]
0x1016ce8e  jmp short loc_1016CEB2
0x1016ce90  mov [esp+0Ch+var_8], esi
0x1016ce94  vmovss xmm0, [esp+0Ch+arg_0]
0x1016ce9a  vmovss [esp+0Ch+var_C], xmm0
0x1016ce9f  call RapidFire_Original_CL_Move
0x1016cea5  mov ecx, RapidFire_ExtraMovesRemaining
0x1016ceab  lea eax, [ecx-1]
0x1016ceae  test ecx, ecx
0x1016ceb0  jle short loc_1016CED3
0x1016ceb2  mov RapidFire_ExtraMovesRemaining, eax; Reset -1 before base CL_Move; initial CreateMove may arm N; after each move decrement counter and repeat while previous value >0. Total N+1 native calls. Same original args each call. ; ['Reset -1 before base CL_Move; initial CreateMove may arm N; after each move decrement counter and repeat while previous value >0. Total N+1 native calls. Same original args each call.']
0x1016ceb7  cmp byte ptr word_103F538C, 1
0x1016cebe  jnz short loc_1016CE90
0x1016cec0  mov ecx, off_102A4248
0x1016cec6  mov [esp+0Ch+var_C], esi
0x1016cec9  call sub_1014B6C0
0x1016cece  sub esp, 4
0x1016ced1  jmp short loc_1016CE90
0x1016ced3  add esp, 8
0x1016ced6  pop esi
0x1016ced7  retn

; RapidFire_Hook_RunCommand 0x1016cf10 / 220 instructions
0x1016cf10  push ebp
0x1016cf11  mov ebp, esp
0x1016cf13  push ebx
0x1016cf14  push edi
0x1016cf15  push esi
0x1016cf16  sub esp, 20h
0x1016cf19  mov esi, edx
0x1016cf1b  mov ebx, ecx
0x1016cf1d  mov edi, [ebp+arg_0]
0x1016cf20  mov [ebp+var_1C], esp
0x1016cf23  mov [ebp+var_10], 0FFFFFFFFh
0x1016cf2a  lea eax, [ebp+var_18]
0x1016cf2d  mov [ebp+var_14], offset loc_101769A0
0x1016cf34  mov ecx, large fs:0
0x1016cf3b  mov [ebp+var_18], ecx
0x1016cf3e  mov large fs:0, eax
0x1016cf44  cmp byte_103EEA60, 1
0x1016cf4b  jnz short loc_1016CF6A
0x1016cf4d  mov ecx, off_1029E014
0x1016cf53  call sub_10179540
0x1016cf58  test eax, eax
0x1016cf5a  setnz cl
0x1016cf5d  cmp edi, eax
0x1016cf5f  setz al
0x1016cf62  test cl, al
0x1016cf64  jnz loc_1016D12B
0x1016cf6a  mov [ebp+var_2C], ebx
0x1016cf6d  mov [ebp+var_20], esi
0x1016cf70  mov ecx, off_1029E014
0x1016cf76  call sub_10179540
0x1016cf7b  test eax, eax
0x1016cf7d  jz loc_1016D00F
0x1016cf83  mov esi, eax
0x1016cf85  mov eax, [ebp+arg_4]
0x1016cf88  mov ecx, [eax+4]
0x1016cf8b  mov edx, 1B4E81B5h
0x1016cf90  mov eax, ecx
0x1016cf92  imul edx
0x1016cf94  mov eax, edx
0x1016cf96  shr eax, 1Fh
0x1016cf99  sar edx, 4
0x1016cf9c  add edx, eax
0x1016cf9e  imul eax, edx, 96h
0x1016cfa4  sub ecx, eax
0x1016cfa6  mov ebx, RapidFire_TickbaseShiftRing[ecx*8]
0x1016cfad  mov eax, dword_103EEAF0
0x1016cfb2  mov ecx, TlsIndex
0x1016cfb8  mov edx, large fs:2Ch
0x1016cfbf  mov ecx, [edx+ecx*4]
0x1016cfc2  cmp eax, [ecx+4]
0x1016cfc8  jg loc_1016D13E
0x1016cfce  mov eax, dword_103EEAEC
0x1016cfd3  sub [eax+esi], ebx; Confirmed instruction: local m_nTickBase -= per-command ring shift before original Prediction::RunCommand. Not server tickbase or weapon damage. ; ['Confirmed instruction: local m_nTickBase -= per-command ring shift before original Prediction::RunCommand. Not server tickbase or weapon damage.']
0x1016cfd6  cmp edi, esi
0x1016cfd8  jnz short loc_1016D00F
0x1016cfda  mov eax, dword_103EEAF0
0x1016cfdf  mov ecx, TlsIndex
0x1016cfe5  mov edx, large fs:2Ch
0x1016cfec  mov ecx, [edx+ecx*4]
0x1016cfef  cmp eax, [ecx+4]
0x1016cff5  jg loc_1016D197
0x1016cffb  mov eax, dword_103EEAEC
0x1016d000  mov eax, [eax+esi]
0x1016d003  mov dword_103F7460, eax
0x1016d008  mov [ebp+var_10], 0FFFFFFFFh
0x1016d00f  mov esi, [ebp+arg_8]
0x1016d012  mov [ebp+var_10], 0FFFFFFFFh
0x1016d019  mov ecx, [ebp+var_2C]
0x1016d01c  mov edx, [ebp+var_20]
0x1016d01f  push esi
0x1016d020  push [ebp+arg_4]
0x1016d023  push edi
0x1016d024  call RapidFire_Original_RunCommand
0x1016d02a  mov dword_103EF050, esi
0x1016d030  mov ecx, off_1029E014
0x1016d036  call sub_10179540
0x1016d03b  test eax, eax
0x1016d03d  jz loc_1016D12B
0x1016d043  cmp byte_103F5D42, 1
0x1016d04a  jnz loc_1016D12B
0x1016d050  cmp byte_103EEA60, 0
0x1016d057  jnz loc_1016D12B
0x1016d05d  mov esi, eax
0x1016d05f  cmp edi, eax
0x1016d061  jnz loc_1016D12B
0x1016d067  mov eax, [ebp+arg_4]
0x1016d06a  cmp byte ptr [eax+3Ch], 0
0x1016d06e  jnz loc_1016D12B
0x1016d074  mov eax, dword_103EF190
0x1016d079  mov ecx, TlsIndex
0x1016d07f  mov edx, large fs:2Ch
0x1016d086  mov ecx, [edx+ecx*4]
0x1016d089  cmp eax, [ecx+4]
0x1016d08f  jg loc_1016D1EA
0x1016d095  mov eax, dword_103EF18C
0x1016d09a  mov ecx, [eax+edi]
0x1016d09d  test ecx, ecx
0x1016d09f  jz loc_1016D12B
0x1016d0a5  mov eax, dword_103EC8D0
0x1016d0aa  vmovss xmm0, dword ptr [eax+10h]
0x1016d0af  vmovss [ebp+var_20], xmm0
0x1016d0b4  mov edx, dword_103EC918
0x1016d0ba  vxorps xmm0, xmm0, xmm0
0x1016d0be  cmp byte ptr [edx+0Ah], 0
0x1016d0c2  jnz short loc_1016D0D0
0x1016d0c4  vmovss xmm0, dword ptr [eax+1Ch]
0x1016d0c9  mov [ebp+var_10], 0FFFFFFFFh
0x1016d0d0  vmovss dword ptr [eax+10h], xmm0
0x1016d0d5  mov eax, [ebp+arg_4]
0x1016d0d8  vmovss xmm0, dword ptr [eax+0Ch]
0x1016d0dd  vmovss xmm1, dword ptr [eax+10h]
0x1016d0e2  mov eax, [ecx]
0x1016d0e4  mov eax, [eax+0Ch]
0x1016d0e7  mov [ebp+var_10], 0FFFFFFFFh
0x1016d0ee  sub esp, 8
0x1016d0f1  vmovss [esp+34h+var_30], xmm0
0x1016d0f7  vmovss [esp+34h+var_34], xmm1
0x1016d0fc  call eax
0x1016d0fe  mov eax, dword_103EC8D0
0x1016d103  vmovss xmm0, dword ptr [eax+10h]
0x1016d108  mov eax, [esi]
0x1016d10a  sub esp, 4
0x1016d10d  vmovss [esp+30h+var_30], xmm0
0x1016d112  mov ecx, esi
0x1016d114  call dword ptr [eax+320h]
0x1016d11a  fstp st
0x1016d11c  mov eax, dword_103EC8D0
0x1016d121  vmovss xmm0, [ebp+var_20]
0x1016d126  vmovss dword ptr [eax+10h], xmm0
0x1016d12b  mov eax, [ebp+var_18]
0x1016d12e  mov large fs:0, eax
0x1016d134  add esp, 20h
0x1016d137  pop esi
0x1016d138  pop edi
0x1016d139  pop ebx
0x1016d13a  pop ebp
0x1016d13b  retn 0Ch
0x1016d13e  push offset dword_103EEAF0
0x1016d143  call sub_101809C3
0x1016d148  add esp, 4
0x1016d14b  cmp dword_103EEAF0, 0FFFFFFFFh
0x1016d152  jnz loc_1016CFCE
0x1016d158  mov [ebp+var_28], ebx
0x1016d15b  mov [ebp+var_24], esi
0x1016d15e  mov [ebp+var_10], 0
0x1016d165  push offset aMNtickbase; "m_nTickBase"
0x1016d16a  push offset aCbaseplayer; "CBasePlayer"
0x1016d16f  call sub_1017C730
0x1016d174  add esp, 8
0x1016d177  mov dword_103EEAEC, eax
0x1016d17c  push offset dword_103EEAF0
0x1016d181  call sub_10180A43
0x1016d186  add esp, 4
0x1016d189  mov edi, [ebp+arg_0]
0x1016d18c  mov esi, [ebp+var_24]
0x1016d18f  mov ebx, [ebp+var_28]
0x1016d192  jmp loc_1016CFCE
0x1016d197  push offset dword_103EEAF0
0x1016d19c  call sub_101809C3
0x1016d1a1  add esp, 4
0x1016d1a4  cmp dword_103EEAF0, 0FFFFFFFFh
0x1016d1ab  jnz loc_1016CFFB
0x1016d1b1  mov [ebp+var_24], esi
0x1016d1b4  mov [ebp+var_10], 1
0x1016d1bb  push offset aMNtickbase; "m_nTickBase"
0x1016d1c0  push offset aCbaseplayer; "CBasePlayer"
0x1016d1c5  call sub_1017C730
0x1016d1ca  add esp, 8
0x1016d1cd  mov dword_103EEAEC, eax
0x1016d1d2  push offset dword_103EEAF0
0x1016d1d7  call sub_10180A43
0x1016d1dc  add esp, 4
0x1016d1df  mov edi, [ebp+arg_0]
0x1016d1e2  mov esi, [ebp+var_24]
0x1016d1e5  jmp loc_1016CFFB
0x1016d1ea  push offset dword_103EF190
0x1016d1ef  call sub_101809C3
0x1016d1f4  add esp, 4
0x1016d1f7  cmp dword_103EF190, 0FFFFFFFFh
0x1016d1fe  jnz loc_1016D095
0x1016d204  mov [ebp+var_20], esi
0x1016d207  mov [ebp+var_10], 2
0x1016d20e  push offset aMIthrowgrenade; "m_iThrowGrenadeCounter"
0x1016d213  push offset aCterrorplayer; "CTerrorPlayer"
0x1016d218  call sub_1017C730
0x1016d21d  add esp, 8
0x1016d220  add eax, 0FFFFFFE8h
0x1016d223  mov dword_103EF18C, eax
0x1016d228  push offset dword_103EF190
0x1016d22d  call sub_10180A43
0x1016d232  add esp, 4
0x1016d235  mov edi, [ebp+arg_0]
0x1016d238  mov esi, [ebp+var_20]
0x1016d23b  jmp loc_1016D095
0x1016d240  push ebp
0x1016d241  sub esp, 0Ch
0x1016d244  add ebp, 0Ch
0x1016d247  push offset dword_103EEAF0
0x1016d24c  call sub_10180A15
0x1016d251  add esp, 10h
0x1016d254  pop ebp
0x1016d255  retn
0x1016d260  push ebp
0x1016d261  sub esp, 0Ch
0x1016d264  add ebp, 0Ch
0x1016d267  push offset dword_103EEAF0
0x1016d26c  call sub_10180A15
0x1016d271  add esp, 10h
0x1016d274  pop ebp
0x1016d275  retn
0x1016d280  push ebp
0x1016d281  sub esp, 0Ch
0x1016d284  add ebp, 0Ch
0x1016d287  push offset dword_103EF190
0x1016d28c  call sub_10180A15
0x1016d291  add esp, 10h
0x1016d294  pop ebp
0x1016d295  retn
0x101769a0  mov eax, offset stru_1029CDAC
0x101769a5  jmp __CxxFrameHandler3

; RapidFire_Hook_CreateMove 0x1016ff00 / 749 instructions
0x1016ff00  push ebp
0x1016ff01  mov ebp, esp
0x1016ff03  push ebx
0x1016ff04  push edi
0x1016ff05  push esi
0x1016ff06  sub esp, 3Ch
0x1016ff09  mov esi, edx
0x1016ff0b  mov edi, ecx
0x1016ff0d  mov ebx, [ebp+arg_4]
0x1016ff10  mov [ebp+var_1C], esp
0x1016ff13  mov [ebp+var_10], 0FFFFFFFFh
0x1016ff1a  lea eax, [ebp+var_18]
0x1016ff1d  mov [ebp+var_14], offset loc_10176A00
0x1016ff24  mov ecx, large fs:0
0x1016ff2b  mov [ebp+var_18], ecx
0x1016ff2e  mov large fs:0, eax
0x1016ff34  mov byte_103EEAA9, 0
0x1016ff3b  mov byte_103EEAAA, 0
0x1016ff42  mov byte_103EEA61, 0
0x1016ff49  mov ecx, off_1029E014
0x1016ff4f  call sub_10179540
0x1016ff54  mov [ebp+var_28], eax
0x1016ff57  test ebx, ebx
0x1016ff59  jz loc_1017072A
0x1016ff5f  mov eax, [ebx+4]
0x1016ff62  test eax, eax
0x1016ff64  jz loc_1017072A
0x1016ff6a  mov [ebp+var_2C], edi
0x1016ff6d  mov [ebp+var_30], esi
0x1016ff70  mov ecx, 1B4E81B5h
0x1016ff75  mov esi, eax
0x1016ff77  imul ecx
0x1016ff79  mov eax, edx
0x1016ff7b  shr eax, 1Fh
0x1016ff7e  sar edx, 4
0x1016ff81  add edx, eax
0x1016ff83  imul eax, edx, 96h
0x1016ff89  sub esi, eax
0x1016ff8b  mov [ebp+var_48], esi
0x1016ff8e  mov RapidFire_TickbaseShiftRing[esi*8], 0
0x1016ff99  mov ecx, off_1029E014
0x1016ff9f  call sub_10179540
0x1016ffa4  test eax, eax
0x1016ffa6  jz short loc_1016FFCD
0x1016ffa8  cmp RapidFire_ExtraMovesRemaining, 0FFFFFFFFh
0x1016ffaf  jnz short loc_1016FFCD
0x1016ffb1  mov ecx, off_103EBDF0
0x1016ffb7  push ebx
0x1016ffb8  push eax
0x1016ffb9  mov esi, eax
0x1016ffbb  call sub_101471E0
0x1016ffc0  mov ecx, off_103EBDF0
0x1016ffc6  push ebx
0x1016ffc7  push esi
0x1016ffc8  call RapidFire_ArmExtraMoves
0x1016ffcd  mov ecx, off_1029E014
0x1016ffd3  call sub_10179540
0x1016ffd8  mov [ebp+var_44], eax
0x1016ffdb  mov ecx, off_1029E014
0x1016ffe1  call sub_10179560
0x1016ffe6  mov ecx, dword_103EC918
0x1016ffec  mov eax, dword_103EC9A8
0x1016fff1  mov esi, [eax+4A50h]
0x1016fff7  add esi, [eax+4A4Ch]
0x1016fffd  mov edi, [eax+0D4h]
0x10170003  xor edx, edx
0x10170005  test edi, edi
0x10170007  setnle dl
0x1017000a  mov ebx, [ecx]
0x1017000c  push esi
0x1017000d  push dword ptr [eax+4A54h]
0x10170013  push edx
0x10170014  push edi
0x10170015  call dword ptr [ebx+0Ch]
0x10170018  cmp byte_103EEA60, 1
0x1017001f  jnz short loc_10170040
0x10170021  mov ebx, [ebp+arg_4]
0x10170024  mov eax, [ebx+24h]
0x10170027  test al, 2
0x10170029  mov esi, [ebp+var_30]
0x1017002c  mov edi, [ebp+var_2C]
0x1017002f  jz loc_1017072A
0x10170035  and eax, 0FFFFFFFDh
0x10170038  mov [ebx+24h], eax
0x1017003b  jmp loc_1017072A
0x10170040  mov eax, [ebp+var_s0]
0x10170043  mov eax, [eax]
0x10170045  mov eax, [eax]
0x10170047  mov [ebp+var_24], eax
0x1017004a  mov ecx, off_1029E014
0x10170050  call sub_10179540
0x10170055  test eax, eax
0x10170057  mov ebx, [ebp+arg_4]
0x1017005a  jz short loc_10170089
0x1017005c  mov esi, eax
0x1017005e  mov ecx, off_1029E014
0x10170064  call sub_10179560
0x10170069  test eax, eax
0x1017006b  jz short loc_10170089
0x1017006d  mov edi, eax
0x1017006f  mov ecx, eax
0x10170071  push esi
0x10170072  call sub_10023180
0x10170077  mov byte_103EEAA8, al
0x1017007c  mov ecx, edi
0x1017007e  push esi
0x1017007f  call sub_10170B70
0x10170084  mov byte_103F74EC, al
0x10170089  mov eax, dword_103F73EC
0x1017008e  mov ecx, TlsIndex
0x10170094  mov edx, large fs:2Ch
0x1017009b  mov ecx, [edx+ecx*4]
0x1017009e  cmp eax, [ecx+4]
0x101700a4  jg loc_10170759
0x101700aa  movzx eax, byte_103EEAA8
0x101700b1  cmp al, byte_103F73E9
0x101700b7  mov esi, [ebp+var_30]
0x101700ba  mov edi, [ebp+var_2C]
0x101700bd  jnz short loc_101700CB
0x101700bf  test al, al
0x101700c1  jz short loc_101700DC
0x101700c3  inc dword_103F74F0
0x101700c9  jmp short loc_101700E6
0x101700cb  mov dword_103F74F0, 0
0x101700d5  mov byte_103F73E9, al
0x101700da  jmp short loc_101700E6
0x101700dc  mov dword_103F74F0, 0
0x101700e6  add [ebp+var_24], 0FFFFFFE3h
0x101700ea  mov ecx, off_103EBDF0
0x101700f0  push ebx
0x101700f1  call sub_101462D0
0x101700f6  mov ecx, off_103EBDF0
0x101700fc  push ebx
0x101700fd  push [ebp+var_28]
0x10170100  call sub_101472B0
0x10170105  mov ecx, off_102A3C78
0x1017010b  call sub_101493C0
0x10170110  mov ecx, off_103EBE18
0x10170116  push ebx
0x10170117  call sub_1002D2B0
0x1017011c  cmp byte_103F5341, 1
0x10170123  jnz short loc_1017018A
0x10170125  cmp byte_103F5340, 1
0x1017012c  jnz short loc_1017018A
0x1017012e  mov ecx, off_1029E014
0x10170134  call sub_10179540
0x10170139  test eax, eax
0x1017013b  jz short loc_1017018A
0x1017013d  mov [ebp+var_20], eax
0x10170140  mov eax, dword_103EF058
0x10170145  mov ecx, TlsIndex
0x1017014b  mov edx, large fs:2Ch
0x10170152  mov ecx, [edx+ecx*4]
0x10170155  cmp eax, [ecx+4]
0x1017015b  jg loc_101709DE
0x10170161  mov eax, dword_103EF054
0x10170166  mov ecx, [ebp+var_20]
0x10170169  test byte ptr [eax+ecx], 1
0x1017016d  jz short loc_1017018A
0x1017016f  mov eax, off_103EBE18
0x10170174  test byte ptr [eax+0B4h], 1
0x1017017b  jnz short loc_1017018A
0x1017017d  mov eax, [ebp+var_24]
0x10170180  mov byte ptr [eax], 0
0x10170183  mov [ebp+var_10], 0FFFFFFFFh
0x1017018a  mov ecx, off_103EBDF4
0x10170190  mov [ebp+var_10], 0FFFFFFFFh
0x10170197  push ebx
0x10170198  call sub_101652D0
0x1017019d  mov ecx, off_103EBE24
0x101701a3  push ebx
0x101701a4  call sub_10020070
0x101701a9  mov ecx, off_103EBDF0
0x101701af  push [ebp+var_24]
0x101701b2  push ebx
0x101701b3  call sub_10148F80
0x101701b8  mov ecx, off_103EBDF0
0x101701be  push ebx
0x101701bf  call sub_10146F50
0x101701c4  mov ecx, off_103EBE28
0x101701ca  push ebx
0x101701cb  call sub_10026D60
0x101701d0  mov ecx, off_103EBE18
0x101701d6  call sub_1002D780
0x101701db  mov eax, dword_103F73F4
0x101701e0  mov ecx, TlsIndex
0x101701e6  mov edx, large fs:2Ch
0x101701ed  mov ecx, [edx+ecx*4]
0x101701f0  cmp eax, [ecx+4]
0x101701f6  jg loc_10170791
0x101701fc  mov eax, dword_103EEA5C
0x10170201  cmp eax, dword_103F73F0
0x10170207  jnz short loc_10170212
0x10170209  mov ecx, dword_103F74F4
0x1017020f  inc ecx
0x10170210  jmp short loc_10170219
0x10170212  mov dword_103F73F0, eax
0x10170217  xor ecx, ecx
0x10170219  xor edx, edx
0x1017021b  cmp eax, 2
0x1017021e  cmovge edx, ecx
0x10170221  mov dword_103F74F4, edx
0x10170227  movzx eax, byte_103F5CDC
0x1017022e  test al, al
0x10170230  jnz short loc_1017023D
0x10170232  mov cl, 1
0x10170234  cmp byte_103F4BF2, 0
0x1017023b  jnz short loc_10170247
0x1017023d  xor al, 1
0x1017023f  and al, byte_103F4BFC
0x10170245  mov ecx, eax
0x10170247  vmovss xmm0, dword ptr [ebx+1Ch]
0x1017024c  vmovups xmm1, xmmword ptr [ebx+0Ch]
0x10170251  cmp [ebp+var_28], 0
0x10170255  jz loc_101702FD
0x1017025b  test cl, cl
0x1017025d  jz loc_101702FD
0x10170263  mov eax, dword_103EEAD8
0x10170268  mov ecx, TlsIndex
0x1017026e  mov edx, large fs:2Ch
0x10170275  mov ecx, [edx+ecx*4]
0x10170278  cmp eax, [ecx+4]
0x1017027e  jg loc_1017091A
0x10170284  mov eax, dword_103EEAD4
0x10170289  mov ecx, [ebp+var_28]
0x1017028c  cmp dword ptr [eax+ecx], 3
0x10170290  jz short loc_101702FD
0x10170292  vmovups [ebp+var_40], xmm1
0x10170297  vmovss [ebp+var_20], xmm0
0x1017029c  mov ecx, off_1029E014
0x101702a2  mov [ebp+var_10], 0FFFFFFFFh
0x101702a9  call sub_10179560
0x101702ae  test eax, eax
0x101702b0  jz short loc_101702E3
0x101702b2  mov ecx, eax
0x101702b4  push [ebp+var_28]
0x101702b7  call sub_10023180
0x101702bc  test al, al
0x101702be  jz short loc_101702E3
0x101702c0  mov eax, [ebx+24h]
0x101702c3  and eax, 1
0x101702c6  jz short loc_101702E3
0x101702c8  cmp byte_103EEAAA, 1
0x101702cf  jnz short loc_101702E3
0x101702d1  mov eax, [ebp+var_24]
0x101702d4  mov byte ptr [eax], 0
0x101702d7  vmovss xmm0, [ebp+var_20]
0x101702dc  vmovups xmm1, [ebp+var_40]
0x101702e1  jmp short loc_101702FD
0x101702e3  mov eax, [ebp+var_24]
0x101702e6  mov byte ptr [eax], 1
0x101702e9  vmovss xmm0, [ebp+var_20]
0x101702ee  vmovss dword ptr [ebx+1Ch], xmm0
0x101702f3  vmovups xmm1, [ebp+var_40]
0x101702f8  vmovups xmmword ptr [ebx+0Ch], xmm1
0x101702fd  cmp byte_103EEAA9, 1
0x10170304  jnz short loc_1017031C
0x10170306  cmp dword_103F5CF8, 0
0x1017030d  jnz short loc_1017031C
0x1017030f  mov al, 1
0x10170311  cmp byte_103F5CDC, 0
0x10170318  mov cl, 1
0x1017031a  jnz short loc_1017032C
0x1017031c  xor eax, eax
0x1017031e  cmp byte_103F73F8, 1
0x10170325  mov ecx, 0
0x1017032a  jnz short loc_10170337
0x1017032c  mov edx, [ebp+var_24]
0x1017032f  mov [edx], al
0x10170331  mov byte_103F73F8, cl
0x10170337  cmp dword_103F5CF8, 3
0x1017033e  jnz loc_101703F5
0x10170344  cmp [ebp+var_28], 0
0x10170348  jz loc_101703F5
0x1017034e  test byte_103F5CDC, 1
0x10170355  jz loc_101703F5
0x1017035b  mov eax, dword_103EEAD8
0x10170360  mov ecx, TlsIndex
0x10170366  mov edx, large fs:2Ch
0x1017036d  mov ecx, [edx+ecx*4]
0x10170370  cmp eax, [ecx+4]
0x10170376  jg loc_10170838
0x1017037c  mov eax, dword_103EEAD4
0x10170381  mov ecx, [ebp+var_28]
0x10170384  cmp dword ptr [eax+ecx], 3
0x10170388  jz short loc_101703F5
0x1017038a  vmovups [ebp+var_40], xmm1
0x1017038f  vmovss [ebp+var_20], xmm0
0x10170394  mov ecx, off_1029E014
0x1017039a  mov [ebp+var_10], 0FFFFFFFFh
0x101703a1  call sub_10179560
0x101703a6  test eax, eax
0x101703a8  jz short loc_101703DB
0x101703aa  mov ecx, eax
0x101703ac  push [ebp+var_28]
0x101703af  call sub_10023180
0x101703b4  test al, al
0x101703b6  jz short loc_101703DB
0x101703b8  mov eax, [ebx+24h]
0x101703bb  and eax, 1
0x101703be  jz short loc_101703DB
0x101703c0  cmp byte_103EEAAA, 1
0x101703c7  jnz short loc_101703DB
0x101703c9  mov eax, [ebp+var_24]
0x101703cc  mov byte ptr [eax], 0
0x101703cf  vmovss xmm0, [ebp+var_20]
0x101703d4  vmovups xmm1, [ebp+var_40]
0x101703d9  jmp short loc_101703F5
0x101703db  mov eax, [ebp+var_24]
0x101703de  mov byte ptr [eax], 1
0x101703e1  vmovss xmm0, [ebp+var_20]
0x101703e6  vmovss dword ptr [ebx+1Ch], xmm0
0x101703eb  vmovups xmm1, [ebp+var_40]
0x101703f0  vmovups xmmword ptr [ebx+0Ch], xmm1
0x101703f5  cmp dword_103F5CF8, 2
0x101703fc  jnz loc_101704B3
0x10170402  cmp [ebp+var_28], 0
0x10170406  jz loc_101704B3
0x1017040c  test byte_103F5CDC, 1
0x10170413  jz loc_101704B3
0x10170419  mov eax, dword_103EEAD8
0x1017041e  mov ecx, TlsIndex
0x10170424  mov edx, large fs:2Ch
0x1017042b  mov ecx, [edx+ecx*4]
0x1017042e  cmp eax, [ecx+4]
0x10170434  jg loc_101708A9
0x1017043a  mov eax, dword_103EEAD4
0x1017043f  mov ecx, [ebp+var_28]
0x10170442  cmp dword ptr [eax+ecx], 3
0x10170446  jz short loc_101704B3
0x10170448  vmovups [ebp+var_40], xmm1
0x1017044d  vmovss [ebp+var_20], xmm0
0x10170452  mov ecx, off_1029E014
0x10170458  mov [ebp+var_10], 0FFFFFFFFh
0x1017045f  call sub_10179560
0x10170464  test eax, eax
0x10170466  jz short loc_10170499
0x10170468  mov ecx, eax
0x1017046a  push [ebp+var_28]
0x1017046d  call sub_10023180
0x10170472  test al, al
0x10170474  jz short loc_10170499
0x10170476  mov eax, [ebx+24h]
0x10170479  and eax, 1
0x1017047c  jz short loc_10170499
0x1017047e  cmp byte_103EEAAA, 1
0x10170485  jnz short loc_10170499
0x10170487  mov eax, [ebp+var_24]
0x1017048a  mov byte ptr [eax], 0
0x1017048d  vmovss xmm0, [ebp+var_20]
0x10170492  vmovups xmm1, [ebp+var_40]
0x10170497  jmp short loc_101704B3
0x10170499  mov eax, [ebp+var_24]
0x1017049c  mov byte ptr [eax], 1
0x1017049f  vmovss xmm0, [ebp+var_20]
0x101704a4  vmovss dword ptr [ebx+1Ch], xmm0
0x101704a9  vmovups xmm1, [ebp+var_40]
0x101704ae  vmovups xmmword ptr [ebx+0Ch], xmm1
0x101704b3  cmp byte_103EEAA9, 1
0x101704ba  jnz short loc_10170503
0x101704bc  cmp dword_103F5CF8, 1
0x101704c3  jnz short loc_10170503
0x101704c5  cmp [ebp+var_28], 0
0x101704c9  jz short loc_10170503
0x101704cb  test byte_103F5CDC, 1
0x101704d2  jz short loc_10170503
0x101704d4  mov eax, dword_103EEAD8
0x101704d9  mov ecx, TlsIndex
0x101704df  mov edx, large fs:2Ch
0x101704e6  mov ecx, [edx+ecx*4]
0x101704e9  cmp eax, [ecx+4]
0x101704ef  jg loc_101707C7
0x101704f5  mov eax, dword_103EEAD4
0x101704fa  mov ecx, [ebp+var_28]
0x101704fd  cmp dword ptr [eax+ecx], 3
0x10170501  jnz short loc_10170520
0x10170503  cmp byte_103F73F9, 1
0x1017050a  jnz short loc_1017052D
0x1017050c  mov eax, [ebp+var_24]
0x1017050f  mov byte ptr [eax], 1
0x10170512  vmovss dword ptr [ebx+1Ch], xmm0
0x10170517  vmovups xmmword ptr [ebx+0Ch], xmm1
0x1017051c  xor eax, eax
0x1017051e  jmp short loc_10170528
0x10170520  mov eax, [ebp+var_24]
0x10170523  mov byte ptr [eax], 0
0x10170526  mov al, 1
0x10170528  mov byte_103F73F9, al
0x1017052d  movzx eax, byte_103F5D43
0x10170534  mov [ebp+var_10], 0FFFFFFFFh
0x1017053b  cmp al, 1
0x1017053d  jnz loc_101706DD
0x10170543  mov ecx, off_1029E014
0x10170549  call sub_10179540
0x1017054e  mov [ebp+var_20], eax
0x10170551  test eax, eax
0x10170553  jz loc_101706DD
0x10170559  mov eax, dword_103EEAD8
0x1017055e  mov ecx, TlsIndex
0x10170564  mov edx, large fs:2Ch
0x1017056b  mov ecx, [edx+ecx*4]
0x1017056e  cmp eax, [ecx+4]
0x10170574  jg loc_1017098B
0x1017057a  mov eax, dword_103EEAD4
0x1017057f  mov ecx, [ebp+var_20]
0x10170582  cmp dword ptr [eax+ecx], 3
0x10170586  jnz loc_101706DD
0x1017058c  mov eax, dword_103EEA68
0x10170591  mov ecx, TlsIndex
0x10170597  mov edx, large fs:2Ch
0x1017059e  mov ecx, [edx+ecx*4]
0x101705a1  cmp eax, [ecx+4]
0x101705a7  jg loc_10170A31
0x101705ad  mov eax, dword_103EEA64
0x101705b2  mov ecx, [ebp+var_44]
0x101705b5  cmp dword ptr [eax+ecx], 0
0x101705b9  mov [ebp+var_10], 0FFFFFFFFh
0x101705c0  jnz loc_101706DD
0x101705c6  mov ecx, [ebp+var_44]
0x101705c9  call sub_10035DA0
0x101705ce  cmp dword ptr [eax], 4
0x101705d1  jz short loc_101705E4
0x101705d3  mov ecx, [ebp+var_20]
0x101705d6  call sub_10035DA0
0x101705db  cmp dword ptr [eax], 2
0x101705de  jnz loc_1017067A
0x101705e4  cmp RapidFire_ExtraMovesRemaining, 0FFFFFFFFh
0x101705eb  jnz loc_1017067A
0x101705f1  push dword_103F5D44; vKey ; ['vKey']
0x101705f7  call ds:GetAsyncKeyState; USER32
0x101705fd  test ax, ax
0x10170600  js short loc_10170619
0x10170602  mov edx, [ebp+var_20]
0x10170605  mov eax, [edx+10h]
0x10170608  cmp eax, 2
0x1017060b  mov ecx, 1
0x10170610  cmovge ecx, eax
0x10170613  dec ecx
0x10170614  mov [edx+10h], ecx
0x10170617  jmp short loc_1017067A
0x10170619  mov eax, dword_103EC8D0
0x1017061e  vmovss xmm0, ds:dword_10191CD0
0x10170626  vdivss xmm0, xmm0, dword ptr [eax+1Ch]
0x1017062b  vaddss xmm0, xmm0, ds:dword_10184EDC
0x10170633  vcvttss2si eax, xmm0
0x10170637  cmp eax, 97h
0x1017063c  mov ecx, 96h
0x10170641  cmovge ecx, eax
0x10170644  mov RapidFire_ExtraMovesRemaining, ecx
0x1017064a  mov eax, [ebp+var_48]
0x1017064d  mov RapidFire_TickbaseShiftRing[eax*8], ecx
0x10170654  mov edx, [ebp+var_20]
0x10170657  mov eax, [edx+10h]
0x1017065a  inc eax
0x1017065b  cmp eax, ecx
0x1017065d  cmovge eax, ecx
0x10170660  mov [edx+10h], eax
0x10170663  mov RapidFire_ForceSend, 1
0x1017066a  and dword ptr [ebx+24h], 0FFFFF9E7h
0x10170671  vxorps xmm0, xmm0, xmm0
0x10170675  vmovsd qword ptr [ebx+18h], xmm0
0x1017067a  mov ecx, [ebp+var_44]
0x1017067d  call sub_10035DA0
0x10170682  cmp dword ptr [eax], 4
0x10170685  jz short loc_10170694
0x10170687  mov ecx, [ebp+var_20]
0x1017068a  call sub_10035DA0
0x1017068f  cmp dword ptr [eax], 2
0x10170692  jnz short loc_101706DD
0x10170694  push dword_103F5D44; vKey ; ['vKey']
0x1017069a  call ds:GetAsyncKeyState; USER32
0x101706a0  test ax, ax
0x101706a3  jns short loc_101706DD
0x101706a5  mov ecx, dword_103EC770
0x101706ab  mov eax, [ecx]
0x101706ad  call dword ptr [eax+128h]
0x101706b3  test eax, eax
0x101706b5  jz short loc_101706DD
0x101706b7  mov eax, dword_103EC9A8
0x101706bc  mov eax, [eax+18h]
0x101706bf  mov dword ptr [eax+1Ch], 0C8h
0x101706c6  mov RapidFire_ForceSend, 1
0x101706cd  and dword ptr [ebx+24h], 0FFFFF9E7h
0x101706d4  vxorps xmm0, xmm0, xmm0
0x101706d8  vmovsd qword ptr [ebx+18h], xmm0
0x101706dd  mov ecx, off_103EBDF0
0x101706e3  mov [ebp+var_10], 0FFFFFFFFh
0x101706ea  push ebx
0x101706eb  push [ebp+var_28]
0x101706ee  call RapidFire_ApplyNetChannelMode
0x101706f3  cmp RapidFire_ForceSend, 1
0x101706fa  jnz short loc_1017070B
0x101706fc  cmp byte_103EEAAA, 0
0x10170703  jnz short loc_1017070B
0x10170705  mov eax, [ebp+var_24]
0x10170708  mov byte ptr [eax], 1; Shared force-send flag sets caller sendPacket true unless suppressed by another feature. Raze does not implement the pending project single-packet charging policy here. ; ['Shared force-send flag sets caller sendPacket true unless suppressed by another feature. Raze does not implement the pending project single-packet charging policy here.']
0x1017070b  cmp byte_103EEAA9, 0
0x10170712  jnz short loc_1017071D
0x10170714  cmp byte_103EEAAA, 0
0x1017071b  jz short loc_1017072A
0x1017071d  mov eax, [ebp+var_18]
0x10170720  mov large fs:0, eax
0x10170726  xor eax, eax
0x10170728  jmp short loc_1017074F
0x1017072a  vmovss xmm0, [ebp+arg_0]
0x1017072f  sub esp, 8
0x10170732  mov [esp+50h+var_4C], ebx
0x10170736  vmovss [esp+50h+var_50], xmm0
0x1017073b  mov ecx, edi
0x1017073d  mov edx, esi
0x1017073f  call RapidFire_Original_CreateMove
0x10170745  mov ecx, [ebp+var_18]
0x10170748  mov large fs:0, ecx
0x1017074f  add esp, 3Ch
0x10170752  pop esi
0x10170753  pop edi
0x10170754  pop ebx
0x10170755  pop ebp
0x10170756  retn 8
0x10170759  push offset dword_103F73EC
0x1017075e  call sub_101809C3
0x10170763  add esp, 4
0x10170766  cmp dword_103F73EC, 0FFFFFFFFh
0x1017076d  jnz loc_101700AA
0x10170773  movzx eax, byte_103EEAA8
0x1017077a  mov byte_103F73E9, al
0x1017077f  push offset dword_103F73EC
0x10170784  call sub_10180A43
0x10170789  add esp, 4
0x1017078c  jmp loc_101700AA
0x10170791  push offset dword_103F73F4
0x10170796  call sub_101809C3
0x1017079b  add esp, 4
0x1017079e  cmp dword_103F73F4, 0FFFFFFFFh
0x101707a5  jnz loc_101701FC
0x101707ab  mov eax, dword_103EEA5C
0x101707b0  mov dword_103F73F0, eax
0x101707b5  push offset dword_103F73F4
0x101707ba  call sub_10180A43
0x101707bf  add esp, 4
0x101707c2  jmp loc_101701FC
0x101707c7  push offset dword_103EEAD8
0x101707cc  vmovss [ebp+var_20], xmm0
0x101707d1  vmovups [ebp+var_40], xmm1
0x101707d6  call sub_101809C3
0x101707db  vmovups xmm1, [ebp+var_40]
0x101707e0  vmovss xmm0, [ebp+var_20]
0x101707e5  add esp, 4
0x101707e8  cmp dword_103EEAD8, 0FFFFFFFFh
0x101707ef  jnz loc_101704F5
0x101707f5  mov [ebp+var_10], 4
0x101707fc  push offset aMIteamnum; "m_iTeamNum"
0x10170801  push offset aCbaseentity; "CBaseEntity"
0x10170806  call sub_1017C730
0x1017080b  add esp, 8
0x1017080e  mov dword_103EEAD4, eax
0x10170813  push offset dword_103EEAD8
0x10170818  call sub_10180A43
0x1017081d  add esp, 4
0x10170820  mov esi, [ebp+var_30]
0x10170823  mov edi, [ebp+var_2C]
0x10170826  mov ebx, [ebp+arg_4]
0x10170829  vmovss xmm0, [ebp+var_20]
0x1017082e  vmovups xmm1, [ebp+var_40]
0x10170833  jmp loc_101704F5
0x10170838  push offset dword_103EEAD8
0x1017083d  vmovss [ebp+var_20], xmm0
0x10170842  vmovups [ebp+var_40], xmm1
0x10170847  call sub_101809C3
0x1017084c  vmovups xmm1, [ebp+var_40]
0x10170851  vmovss xmm0, [ebp+var_20]
0x10170856  add esp, 4
0x10170859  cmp dword_103EEAD8, 0FFFFFFFFh
0x10170860  jnz loc_1017037C
0x10170866  mov [ebp+var_10], 2
0x1017086d  push offset aMIteamnum; "m_iTeamNum"
0x10170872  push offset aCbaseentity; "CBaseEntity"
0x10170877  call sub_1017C730
0x1017087c  add esp, 8
0x1017087f  mov dword_103EEAD4, eax
0x10170884  push offset dword_103EEAD8
0x10170889  call sub_10180A43
0x1017088e  add esp, 4
0x10170891  mov esi, [ebp+var_30]
0x10170894  mov edi, [ebp+var_2C]
0x10170897  mov ebx, [ebp+arg_4]
0x1017089a  vmovss xmm0, [ebp+var_20]
0x1017089f  vmovups xmm1, [ebp+var_40]
0x101708a4  jmp loc_1017037C
0x101708a9  push offset dword_103EEAD8
0x101708ae  vmovss [ebp+var_20], xmm0
0x101708b3  vmovups [ebp+var_40], xmm1
0x101708b8  call sub_101809C3
0x101708bd  vmovups xmm1, [ebp+var_40]
0x101708c2  vmovss xmm0, [ebp+var_20]
0x101708c7  add esp, 4
0x101708ca  cmp dword_103EEAD8, 0FFFFFFFFh
0x101708d1  jnz loc_1017043A
0x101708d7  mov [ebp+var_10], 3
0x101708de  push offset aMIteamnum; "m_iTeamNum"
0x101708e3  push offset aCbaseentity; "CBaseEntity"
0x101708e8  call sub_1017C730
0x101708ed  add esp, 8
0x101708f0  mov dword_103EEAD4, eax
0x101708f5  push offset dword_103EEAD8
0x101708fa  call sub_10180A43
0x101708ff  add esp, 4
0x10170902  mov esi, [ebp+var_30]
0x10170905  mov edi, [ebp+var_2C]
0x10170908  mov ebx, [ebp+arg_4]
0x1017090b  vmovss xmm0, [ebp+var_20]
0x10170910  vmovups xmm1, [ebp+var_40]
0x10170915  jmp loc_1017043A
0x1017091a  push offset dword_103EEAD8
0x1017091f  vmovss [ebp+var_20], xmm0
0x10170924  vmovups [ebp+var_40], xmm1
0x10170929  call sub_101809C3
0x1017092e  vmovups xmm1, [ebp+var_40]
0x10170933  vmovss xmm0, [ebp+var_20]
0x10170938  add esp, 4
0x1017093b  cmp dword_103EEAD8, 0FFFFFFFFh
0x10170942  jnz loc_10170284
0x10170948  mov [ebp+var_10], 1
0x1017094f  push offset aMIteamnum; "m_iTeamNum"
0x10170954  push offset aCbaseentity; "CBaseEntity"
0x10170959  call sub_1017C730
0x1017095e  add esp, 8
0x10170961  mov dword_103EEAD4, eax
0x10170966  push offset dword_103EEAD8
0x1017096b  call sub_10180A43
0x10170970  add esp, 4
0x10170973  mov esi, [ebp+var_30]
0x10170976  mov edi, [ebp+var_2C]
0x10170979  mov ebx, [ebp+arg_4]
0x1017097c  vmovss xmm0, [ebp+var_20]
0x10170981  vmovups xmm1, [ebp+var_40]
0x10170986  jmp loc_10170284
0x1017098b  push offset dword_103EEAD8
0x10170990  call sub_101809C3
0x10170995  add esp, 4
0x10170998  cmp dword_103EEAD8, 0FFFFFFFFh
0x1017099f  jnz loc_1017057A
0x101709a5  mov [ebp+var_10], 5
0x101709ac  push offset aMIteamnum; "m_iTeamNum"
0x101709b1  push offset aCbaseentity; "CBaseEntity"
0x101709b6  call sub_1017C730
0x101709bb  add esp, 8
0x101709be  mov dword_103EEAD4, eax
0x101709c3  push offset dword_103EEAD8
0x101709c8  call sub_10180A43
0x101709cd  add esp, 4
0x101709d0  mov esi, [ebp+var_30]
0x101709d3  mov edi, [ebp+var_2C]
0x101709d6  mov ebx, [ebp+arg_4]
0x101709d9  jmp loc_1017057A
0x101709de  push offset dword_103EF058
0x101709e3  call sub_101809C3
0x101709e8  add esp, 4
0x101709eb  cmp dword_103EF058, 0FFFFFFFFh
0x101709f2  jnz loc_10170161
0x101709f8  mov [ebp+var_10], 0
0x101709ff  push offset aMFflags; "m_fFlags"
0x10170a04  push offset aCbaseplayer; "CBasePlayer"
0x10170a09  call sub_1017C730
0x10170a0e  add esp, 8
0x10170a11  mov dword_103EF054, eax
0x10170a16  push offset dword_103EF058
0x10170a1b  call sub_10180A43
0x10170a20  add esp, 4
0x10170a23  mov esi, [ebp+var_30]
0x10170a26  mov edi, [ebp+var_2C]
0x10170a29  mov ebx, [ebp+arg_4]
0x10170a2c  jmp loc_10170161
0x10170a31  push offset dword_103EEA68
0x10170a36  call sub_101809C3
0x10170a3b  add esp, 4
0x10170a3e  cmp dword_103EEA68, 0FFFFFFFFh
0x10170a45  jnz loc_101705AD
0x10170a4b  mov [ebp+var_10], 6
0x10170a52  push offset aDeadflag; "deadflag"
0x10170a57  push offset aCbaseplayer; "CBasePlayer"
0x10170a5c  call sub_1017C730
0x10170a61  add esp, 8
0x10170a64  mov dword_103EEA64, eax
0x10170a69  push offset dword_103EEA68
0x10170a6e  call sub_10180A43
0x10170a73  add esp, 4
0x10170a76  mov esi, [ebp+var_30]
0x10170a79  mov edi, [ebp+var_2C]
0x10170a7c  mov ebx, [ebp+arg_4]
0x10170a7f  jmp loc_101705AD
0x10170a90  push ebp
0x10170a91  sub esp, 10h
0x10170a94  add ebp, 0Ch
0x10170a97  push offset dword_103EF058
0x10170a9c  call sub_10180A15
0x10170aa1  add esp, 14h
0x10170aa4  pop ebp
0x10170aa5  retn
0x10170ab0  push ebp
0x10170ab1  sub esp, 10h
0x10170ab4  add ebp, 0Ch
0x10170ab7  push offset dword_103EEAD8
0x10170abc  call sub_10180A15
0x10170ac1  add esp, 14h
0x10170ac4  pop ebp
0x10170ac5  retn
0x10170ad0  push ebp
0x10170ad1  sub esp, 10h
0x10170ad4  add ebp, 0Ch
0x10170ad7  push offset dword_103EEAD8
0x10170adc  call sub_10180A15
0x10170ae1  add esp, 14h
0x10170ae4  pop ebp
0x10170ae5  retn
0x10170af0  push ebp
0x10170af1  sub esp, 10h
0x10170af4  add ebp, 0Ch
0x10170af7  push offset dword_103EEAD8
0x10170afc  call sub_10180A15
0x10170b01  add esp, 14h
0x10170b04  pop ebp
0x10170b05  retn
0x10170b10  push ebp
0x10170b11  sub esp, 10h
0x10170b14  add ebp, 0Ch
0x10170b17  push offset dword_103EEAD8
0x10170b1c  call sub_10180A15
0x10170b21  add esp, 14h
0x10170b24  pop ebp
0x10170b25  retn
0x10170b30  push ebp
0x10170b31  sub esp, 10h
0x10170b34  add ebp, 0Ch
0x10170b37  push offset dword_103EEAD8
0x10170b3c  call sub_10180A15
0x10170b41  add esp, 14h
0x10170b44  pop ebp
0x10170b45  retn
0x10170b50  push ebp
0x10170b51  sub esp, 10h
0x10170b54  add ebp, 0Ch
0x10170b57  push offset dword_103EEA68
0x10170b5c  call sub_10180A15
0x10170b61  add esp, 14h
0x10170b64  pop ebp
0x10170b65  retn
0x10176a00  mov eax, offset stru_1029CEFC
0x10176a05  jmp __CxxFrameHandler3
