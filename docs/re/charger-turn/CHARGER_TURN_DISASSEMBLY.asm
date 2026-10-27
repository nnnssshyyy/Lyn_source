; Static analysis only. Original binaries and IDA instruction bytes were not patched.
; Raze SHA256 dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a
; Client SHA256 c0a8e1e88f7312db11bcfd4b0ec574d224a34b2d9a615d6088b28335eeb4af1f

; charger-turn-full / ChargerTurn_ApplySetting
1017cca0  push ebp
1017cca1  push ebx
1017cca2  push edi
1017cca3  push esi
1017cca4  sub esp, 8
1017cca7  mov eax, ___security_cookie
1017ccac  xor eax, esp
1017ccae  mov [esp+18h+var_14], eax
1017ccb2  mov eax, ChargerTurn_PatchSite
1017ccb7  test eax, eax
1017ccb9  jz short loc_1017CCCC
1017ccbb  movzx edx, ChargerTurn_Enabled
1017ccc2  cmp dl, ChargerTurn_Applied
1017ccc8  jnz short loc_1017CCF8
1017ccca  jmp short loc_1017CD15
1017cccc  push offset aClientDll; "client.dll"
1017ccd1  mov esi, ecx
1017ccd3  call ds:GetModuleHandleA; KERNEL32
1017ccd9  test eax, eax
1017ccdb  jz short loc_1017CD15
1017ccdd  mov ecx, esi
1017ccdf  add eax, 65DB4h; Charger turn target is client.dll + 0x65DB4 (byte RVA; Hex-Rays HMODULE pointer arithmetic may show 104301). Verified against installed client SHA256 c0a8e1e88f7312db11bcfd4b0ec574d224a34b2d9a615d6088b28335eeb4af1f. ; Charger turn target is client.dll + 0x65DB4 (byte RVA; Hex-Rays HMODULE pointer arithmetic may show 104301). Verified against installed client SHA256 c0a8e1e88f7312db11bcfd4b0ec574d224a34b2d9a615d6088b28335eeb4af1f.
1017cce4  mov ChargerTurn_PatchSite, eax
1017cce9  movzx edx, ChargerTurn_Enabled
1017ccf0  cmp dl, ChargerTurn_Applied
1017ccf6  jz short loc_1017CD15
1017ccf8  test dl, dl
1017ccfa  jz short loc_1017CD28
1017ccfc  push 1; dwSize ; dwSize
1017ccfe  push 0EBh; char ; char
1017cd03  push eax; lpAddress ; lpAddress
1017cd04  call PatchRegistry_BackupAndFill; On toggle ON: backup original byte then fill exactly 1 byte with EB. With installed client original 74 2D (JZ +2D), EB 2D always skips the old-viewangles overwrite. No local class/carry gate in this function. ; On toggle ON: backup original byte then fill exactly 1 byte with EB. With installed client original 74 2D (JZ +2D), EB 2D always skips the old-viewangles overwrite. No local class/carry gate in this function.
1017cd09  movzx eax, ChargerTurn_Enabled
1017cd10  mov ChargerTurn_Applied, al
1017cd15  mov ecx, [esp+18h+var_14]
1017cd19  xor ecx, esp; StackCookie ; StackCookie
1017cd1b  call @__security_check_cookie@4; __security_check_cookie(x)
1017cd20  add esp, 8
1017cd23  pop esi
1017cd24  pop edi
1017cd25  pop ebx
1017cd26  pop ebp
1017cd27  retn
1017cd28  mov esi, [ecx]
1017cd2a  mov ecx, [ecx+4]
1017cd2d  jmp short loc_1017CD33
1017cd30  add esi, 10h
1017cd33  cmp esi, ecx
1017cd35  jz short loc_1017CD09
1017cd37  cmp [esi], eax
1017cd39  jnz short loc_1017CD30
1017cd3b  mov edx, [esi+8]
1017cd3e  sub edx, [esi+4]
1017cd41  cmp edx, 1
1017cd44  jnz short loc_1017CD30
1017cd46  mov ebx, esp
1017cd48  mov edi, ds:VirtualProtect; KERNEL32
1017cd4e  push ebx; lpflOldProtect ; lpflOldProtect
1017cd4f  push 40h ; '@'; flNewProtect ; flNewProtect
1017cd51  push 1; dwSize ; dwSize
1017cd53  push eax; lpAddress ; lpAddress
1017cd54  mov ebp, eax
1017cd56  call edi ; VirtualProtect; KERNEL32
1017cd58  mov eax, [esi+4]
1017cd5b  movzx eax, byte ptr [eax]
1017cd5e  mov [ebp+0], al; On toggle OFF: restore the first matching one-byte original from patch registry. Protect results are not checked in Raze. ; On toggle OFF: restore the first matching one-byte original from patch registry. Protect results are not checked in Raze.
1017cd61  push ebx; lpflOldProtect ; lpflOldProtect
1017cd62  push [esp+1Ch+flNewProtect]; flNewProtect ; flNewProtect
1017cd66  push 1; dwSize ; dwSize
1017cd68  push ebp; lpAddress ; lpAddress
1017cd69  call edi ; VirtualProtect; KERNEL32
1017cd6b  jmp short loc_1017CD09

; patch-helper-full / PatchRegistry_BackupAndFill
1017c8e0  push ebp
1017c8e1  mov ebp, esp
1017c8e3  push ebx
1017c8e4  push edi
1017c8e5  push esi
1017c8e6  sub esp, 38h
1017c8e9  mov [ebp+var_44], ecx
1017c8ec  mov edi, [ebp+dwSize]
1017c8ef  mov ebx, [ebp+lpAddress]
1017c8f2  mov [ebp+var_1C], esp
1017c8f5  mov [ebp+var_10], 0FFFFFFFFh
1017c8fc  lea eax, [ebp+var_18]
1017c8ff  mov [ebp+var_14], offset loc_1017D4E0
1017c906  mov ecx, large fs:0
1017c90d  mov [ebp+var_18], ecx
1017c910  mov large fs:0, eax
1017c916  lea eax, [ebp+flOldProtect]
1017c919  push eax; lpflOldProtect ; lpflOldProtect
1017c91a  push 40h ; '@'; flNewProtect ; flNewProtect
1017c91c  push edi; dwSize ; dwSize
1017c91d  push ebx; lpAddress ; lpAddress
1017c91e  call ds:VirtualProtect; KERNEL32
1017c924  mov [ebp+var_3C], 0
1017c92b  mov [ebp+var_38], 0
1017c932  mov [ebp+var_34], 0
1017c939  test edi, edi
1017c93b  jz short loc_1017C969
1017c93d  js loc_1017CB39
1017c943  cmp edi, 1000h
1017c949  jb loc_1017CA09
1017c94f  lea eax, [edi+23h]
1017c952  push eax; Size ; Size
1017c953  call sub_10180946
1017c958  add esp, 4
1017c95b  lea ecx, [eax+23h]
1017c95e  and ecx, 0FFFFFFE0h
1017c961  mov [ecx-4], eax
1017c964  jmp loc_1017CA14
1017c969  mov [ebp+var_30], ebx
1017c96c  mov [ebp+Block], 0
1017c973  mov [ebp+var_28], 0
1017c97a  mov [ebp+var_24], 0
1017c981  mov [ebp+Src], 0
1017c988  xor esi, esi
1017c98a  xor edi, edi
1017c98c  mov ecx, [ebp+var_44]
1017c98f  mov eax, [ecx+4]
1017c992  cmp eax, [ecx+8]
1017c995  jnz loc_1017CAAF
1017c99b  mov [ebp+var_10], 1
1017c9a2  lea edx, [ebp+var_30]
1017c9a5  push edx
1017c9a6  push eax
1017c9a7  call sub_1017CF40
1017c9ac  mov ecx, [ebp+Block]
1017c9af  test ecx, ecx
1017c9b1  mov ebx, [ebp+lpAddress]
1017c9b4  mov esi, [ebp+dwSize]
1017c9b7  jz loc_1017CAD6
1017c9bd  mov eax, [ebp+var_24]
1017c9c0  sub eax, ecx
1017c9c2  cmp eax, 1000h
1017c9c7  jb short loc_1017C9DF
1017c9c9  mov edx, [ecx-4]
1017c9cc  add ecx, 0FFFFFFFCh
1017c9cf  sub ecx, edx
1017c9d1  cmp ecx, 20h ; ' '
1017c9d4  jnb loc_1017CB3E
1017c9da  add eax, 23h ; '#'
1017c9dd  mov ecx, edx
1017c9df  push eax
1017c9e0  push ecx; Block ; Block
1017c9e1  call sub_10180976
1017c9e6  add esp, 8
1017c9e9  mov [ebp+Block], 0
1017c9f0  mov [ebp+var_28], 0
1017c9f7  mov [ebp+var_24], 0
1017c9fe  mov eax, [ebp+var_3C]
1017ca01  mov [ebp+Src], eax
1017ca04  jmp loc_1017CAD6
1017ca09  push edi; Size ; Size
1017ca0a  call sub_10180946
1017ca0f  add esp, 4
1017ca12  mov ecx, eax
1017ca14  mov [ebp+var_3C], ecx
1017ca17  lea esi, [ecx+edi]
1017ca1a  mov [ebp+var_34], esi
1017ca1d  push edi; Size ; Size
1017ca1e  push ebx; Src ; Src
1017ca1f  mov [ebp+Src], ecx
1017ca22  push ecx; void * ; void *
1017ca23  call memmove
1017ca28  add esp, 0Ch
1017ca2b  mov [ebp+var_38], esi
1017ca2e  mov [ebp+var_30], ebx
1017ca31  mov [ebp+Block], 0
1017ca38  mov [ebp+var_28], 0
1017ca3f  mov [ebp+var_24], 0
1017ca46  cmp edi, 1000h
1017ca4c  jb short loc_1017CA6E
1017ca4e  lea eax, [edi+23h]
1017ca51  mov [ebp+var_10], 0
1017ca58  push eax; Size ; Size
1017ca59  call sub_10180946
1017ca5e  add esp, 4
1017ca61  mov esi, eax
1017ca63  add esi, 23h ; '#'
1017ca66  and esi, 0FFFFFFE0h
1017ca69  mov [esi-4], eax
1017ca6c  jmp short loc_1017CA80
1017ca6e  mov [ebp+var_10], 0
1017ca75  push edi; Size ; Size
1017ca76  call sub_10180946
1017ca7b  add esp, 4
1017ca7e  mov esi, eax
1017ca80  mov ecx, [ebp+Src]
1017ca83  mov [ebp+Block], esi
1017ca86  mov eax, [ebp+dwSize]
1017ca89  lea edi, [esi+eax]
1017ca8c  mov [ebp+var_24], edi
1017ca8f  push eax; Size ; Size
1017ca90  push ecx; Src ; Src
1017ca91  push esi; void * ; void *
1017ca92  call memmove
1017ca97  add esp, 0Ch
1017ca9a  mov [ebp+var_28], edi
1017ca9d  mov ebx, [ebp+lpAddress]
1017caa0  mov ecx, [ebp+var_44]
1017caa3  mov eax, [ecx+4]
1017caa6  cmp eax, [ecx+8]
1017caa9  jz loc_1017C99B
1017caaf  mov [eax], ebx
1017cab1  mov [ebp+var_24], 0
1017cab8  mov [ebp+var_28], 0
1017cabf  mov [ebp+Block], 0
1017cac6  mov [eax+4], esi
1017cac9  mov [eax+8], edi
1017cacc  mov [eax+0Ch], edi
1017cacf  add dword ptr [ecx+4], 10h
1017cad3  mov esi, [ebp+dwSize]
1017cad6  movzx eax, [ebp+arg_4]
1017cada  movzx eax, al
1017cadd  push esi; Size ; Size
1017cade  push eax; Val ; Val
1017cadf  push ebx; void * ; void *
1017cae0  call memset
1017cae5  add esp, 0Ch
1017cae8  lea eax, [ebp+flOldProtect]
1017caeb  push eax; lpflOldProtect ; lpflOldProtect
1017caec  push [ebp+flOldProtect]; flNewProtect ; flNewProtect
1017caef  push esi; dwSize ; dwSize
1017caf0  push ebx; lpAddress ; lpAddress
1017caf1  call ds:VirtualProtect; KERNEL32
1017caf7  mov edx, [ebp+Src]
1017cafa  test edx, edx
1017cafc  jz short loc_1017CB26
1017cafe  mov eax, [ebp+var_34]
1017cb01  sub eax, edx
1017cb03  cmp eax, 1000h
1017cb08  jb short loc_1017CB1C
1017cb0a  mov ecx, [edx-4]
1017cb0d  add edx, 0FFFFFFFCh
1017cb10  sub edx, ecx
1017cb12  cmp edx, 20h ; ' '
1017cb15  jnb short loc_1017CB3E
1017cb17  add eax, 23h ; '#'
1017cb1a  mov edx, ecx
1017cb1c  push eax
1017cb1d  push edx; Block ; Block
1017cb1e  call sub_10180976
1017cb23  add esp, 8
1017cb26  mov eax, [ebp+var_18]
1017cb29  mov large fs:0, eax
1017cb2f  add esp, 38h
1017cb32  pop esi
1017cb33  pop edi
1017cb34  pop ebx
1017cb35  pop ebp
1017cb36  retn 0Ch
1017cb39  call sub_100119B0
1017cb3e  ud2
1017cb40  push ebp
1017cb41  sub esp, 10h
1017cb44  add ebp, 0Ch
1017cb47  lea ecx, [ebp+var_30]
1017cb4a  call sub_10015A80
1017cb4f  add esp, 10h
1017cb52  pop ebp
1017cb53  retn
1017cb60  push ebp
1017cb61  sub esp, 10h
1017cb64  add ebp, 0Ch
1017cb67  lea ecx, [ebp+var_3C]
1017cb6a  call sub_10015AD0
1017cb6f  add esp, 10h
1017cb72  pop ebp
1017cb73  retn
1017d4e0  mov eax, offset stru_1029D234
1017d4e5  jmp __CxxFrameHandler3

; frame-stage-full / Hooks_FrameStageNotify
1016c610  push ebp
1016c611  mov ebp, esp
1016c613  push ebx
1016c614  push edi
1016c615  push esi
1016c616  sub esp, 14h
1016c619  mov esi, [ebp+arg_0]
1016c61c  mov [ebp+var_1C], esp
1016c61f  mov [ebp+var_10], 0FFFFFFFFh
1016c626  lea eax, [ebp+var_18]
1016c629  mov [ebp+var_14], offset loc_10176990
1016c630  mov edi, large fs:0
1016c637  mov [ebp+var_18], edi
1016c63a  mov large fs:0, eax
1016c640  push esi
1016c641  call dword_103F743C
1016c647  dec esi; switch 6 cases ; switch 6 cases
1016c648  cmp esi, 5
1016c64b  ja def_1016C651; jumptable 1016C651 default case, cases 2,3
1016c651  jmp ds:jpt_1016C651[esi*4]; switch jump ; switch jump
1016c658  mov eax, off_1029E014; jumptable 1016C651 case 1 ; jumptable 1016C651 case 1
1016c65d  mov eax, [eax]
1016c65f  mov ecx, [eax]
1016c661  cmp ecx, eax
1016c663  jnz short loc_1016C67A
1016c665  jmp def_1016C651; jumptable 1016C651 default case, cases 2,3
1016c670  mov ecx, edx
1016c672  cmp edx, eax
1016c674  jz def_1016C651; jumptable 1016C651 default case, cases 2,3
1016c67a  mov edx, [ecx+14h]
1016c67d  cmp edx, [ecx+18h]
1016c680  jz short loc_1016C685
1016c682  mov [ecx+18h], edx
1016c685  mov esi, [ecx+8]
1016c688  cmp byte ptr [esi+0Dh], 0
1016c68c  jz short loc_1016C6B0
1016c68e  xchg ax, ax
1016c690  mov edx, [ecx+4]
1016c693  cmp byte ptr [edx+0Dh], 0
1016c697  jnz short loc_1016C670
1016c699  mov esi, ecx
1016c69b  mov ecx, edx
1016c69d  cmp esi, [edx+8]
1016c6a0  jz short loc_1016C690
1016c6a2  mov edx, ecx
1016c6a4  jmp short loc_1016C670
1016c6b0  mov edx, esi
1016c6b2  mov esi, [esi]
1016c6b4  cmp byte ptr [esi+0Dh], 0
1016c6b8  jz short loc_1016C6B0
1016c6ba  jmp short loc_1016C670
1016c6bc  mov ecx, off_1029E4E0; jumptable 1016C651 case 5 ; jumptable 1016C651 case 5
1016c6c2  call sub_1017B9E0
1016c6c7  mov ecx, off_1029E028
1016c6cd  call sub_1016BDB0
1016c6d2  mov ecx, off_1029E004
1016c6d8  call ChargerTurn_ApplySetting; Apply ChargerTurn patch during FrameStageNotify case 5, after calling original frame-stage handler. ; Apply ChargerTurn patch during FrameStageNotify case 5, after calling original frame-stage handler.
1016c6dd  mov ecx, off_1029E004
1016c6e3  call sub_1017CD70
1016c6e8  mov ecx, off_1029E004
1016c6ee  call sub_1017CE70
1016c6f3  mov ecx, off_103EBDF0
1016c6f9  call sub_101470E0
1016c6fe  cmp byte ptr dword_103F4B80, 1
1016c705  jnz def_1016C651; jumptable 1016C651 default case, cases 2,3
1016c70b  mov ecx, off_1029E014
1016c711  call sub_10179540
1016c716  test eax, eax
1016c718  jz def_1016C651; jumptable 1016C651 default case, cases 2,3
1016c71e  cmp byte_103EEAA9, 1
1016c725  jnz def_1016C651; jumptable 1016C651 default case, cases 2,3
1016c72b  mov esi, eax
1016c72d  mov eax, dword_103EF190
1016c732  mov ecx, TlsIndex
1016c738  mov edx, large fs:2Ch
1016c73f  mov ecx, [edx+ecx*4]
1016c742  cmp eax, [ecx+4]
1016c748  jg loc_1016C81A
1016c74e  mov eax, dword_103EF18C
1016c753  mov eax, [eax+esi]
1016c756  test eax, eax
1016c758  jz short loc_1016C77B
1016c75a  vmovss xmm0, dword_103F7440
1016c762  vmovss dword ptr [eax+80h], xmm0
1016c76a  vmovss dword ptr [eax+7Ch], xmm0
1016c76f  vmovss dword ptr [eax+74h], xmm0
1016c774  mov [ebp+var_10], 0FFFFFFFFh
1016c77b  mov eax, [esi]
1016c77d  mov eax, [eax+328h]
1016c783  mov [ebp+var_10], 0FFFFFFFFh
1016c78a  mov ecx, esi
1016c78c  call eax
1016c78e  jmp short def_1016C651; jumptable 1016C651 default case, cases 2,3
1016c790  mov ecx, off_1029E014; jumptable 1016C651 case 4 ; jumptable 1016C651 case 4
1016c796  call sub_10179590
1016c79b  jmp short def_1016C651; jumptable 1016C651 default case, cases 2,3
1016c79d  cmp byte ptr dword_103F4B80, 1; jumptable 1016C651 case 6 ; jumptable 1016C651 case 6
1016c7a4  jnz short def_1016C651; jumptable 1016C651 default case, cases 2,3
1016c7a6  cmp byte_103EEAA9, 1
1016c7ad  jnz short def_1016C651; jumptable 1016C651 default case, cases 2,3
1016c7af  mov ecx, off_1029E014
1016c7b5  call sub_10179540
1016c7ba  test eax, eax
1016c7bc  jz short def_1016C651; jumptable 1016C651 default case, cases 2,3
1016c7be  mov esi, eax
1016c7c0  mov eax, dword_103EF190
1016c7c5  mov ecx, TlsIndex
1016c7cb  mov edx, large fs:2Ch
1016c7d2  mov ecx, [edx+ecx*4]
1016c7d5  cmp eax, [ecx+4]
1016c7db  jg loc_1016C86D
1016c7e1  mov eax, dword_103EF18C
1016c7e6  mov eax, [eax+esi]
1016c7e9  test eax, eax
1016c7eb  jz short def_1016C651; jumptable 1016C651 default case, cases 2,3
1016c7ed  vmovss xmm0, dword_103F7444
1016c7f5  vmovss dword ptr [eax+80h], xmm0
1016c7fd  vmovss dword ptr [eax+7Ch], xmm0
1016c802  vmovss dword ptr [eax+74h], xmm0
1016c807  mov eax, [ebp+var_18]; jumptable 1016C651 default case, cases 2,3 ; jumptable 1016C651 default case, cases 2,3
1016c80a  mov large fs:0, eax
1016c810  add esp, 14h
1016c813  pop esi
1016c814  pop edi
1016c815  pop ebx
1016c816  pop ebp
1016c817  retn 4
1016c81a  push offset dword_103EF190
1016c81f  call sub_101809C3
1016c824  add esp, 4
1016c827  cmp dword_103EF190, 0FFFFFFFFh
1016c82e  jnz loc_1016C74E
1016c834  mov [ebp+var_20], esi
1016c837  mov [ebp+var_10], 0
1016c83e  push offset aMIthrowgrenade; "m_iThrowGrenadeCounter"
1016c843  push offset aCterrorplayer; "CTerrorPlayer"
1016c848  call sub_1017C730
1016c84d  add esp, 8
1016c850  add eax, 0FFFFFFE8h
1016c853  mov dword_103EF18C, eax
1016c858  push offset dword_103EF190
1016c85d  call sub_10180A43
1016c862  add esp, 4
1016c865  mov esi, [ebp+var_20]
1016c868  jmp loc_1016C74E
1016c86d  push offset dword_103EF190
1016c872  call sub_101809C3
1016c877  add esp, 4
1016c87a  cmp dword_103EF190, 0FFFFFFFFh
1016c881  jnz loc_1016C7E1
1016c887  mov [ebp+var_20], esi
1016c88a  mov [ebp+var_10], 1
1016c891  push offset aMIthrowgrenade; "m_iThrowGrenadeCounter"
1016c896  push offset aCterrorplayer; "CTerrorPlayer"
1016c89b  call sub_1017C730
1016c8a0  add esp, 8
1016c8a3  add eax, 0FFFFFFE8h
1016c8a6  mov dword_103EF18C, eax
1016c8ab  push offset dword_103EF190
1016c8b0  call sub_10180A43
1016c8b5  add esp, 4
1016c8b8  mov esi, [ebp+var_20]
1016c8bb  jmp loc_1016C7E1
1016c8c0  push ebp
1016c8c1  sub esp, 8
1016c8c4  add ebp, 0Ch
1016c8c7  push offset dword_103EF190
1016c8cc  call sub_10180A15
1016c8d1  add esp, 0Ch
1016c8d4  pop ebp
1016c8d5  retn
1016c8e0  push ebp
1016c8e1  sub esp, 8
1016c8e4  add ebp, 0Ch
1016c8e7  push offset dword_103EF190
1016c8ec  call sub_10180A15
1016c8f1  add esp, 0Ch
1016c8f4  pop ebp
1016c8f5  retn
10176990  mov eax, offset stru_1029CD78
10176995  jmp __CxxFrameHandler3

; hook-install-full / Hooks_InstallFrameStageNotify
1016c5f0  mov eax, dword_103EC788
1016c5f5  mov eax, [eax]
1016c5f7  push offset dword_103F743C; int ; int
1016c5fc  push offset Hooks_FrameStageNotify; LPCVOID ; LPCVOID
1016c601  push dword ptr [eax+88h]; lpAddress ; lpAddress
1016c607  call sub_1017E990
1016c60c  retn

; client-createmove-full / L4D2_BasePlayer_CreateMove
10065cd0  push ebp; Identified structurally as C_BasePlayer::CreateMove from prologue, weapon/vehicle view processing, cached angles and frozen flag handling. Corresponds to Valve Source SDK C_BasePlayer::CreateMove; symbols inferred. ; Identified structurally as C_BasePlayer::CreateMove from prologue, weapon/vehicle view processing, cached angles and frozen flag handling. Corresponds to Valve Source SDK C_BasePlayer::CreateMove; symbols inferred.
10065cd1  mov ebp, esp
10065cd3  push esi
10065cd4  mov esi, ecx
10065cd6  mov ecx, [esi+1408h]
10065cdc  push edi
10065cdd  mov edi, [ebp+command]
10065ce0  cmp ecx, 0FFFFFFFFh
10065ce3  jz short loc_10065D46
10065ce5  mov edx, off_106C50A8
10065ceb  mov eax, ecx
10065ced  and eax, 0FFFh
10065cf2  add eax, eax
10065cf4  lea eax, [edx+eax*8+4]
10065cf8  test eax, eax
10065cfa  jz short loc_10065D46
10065cfc  shr ecx, 0Ch
10065cff  cmp [eax+4], ecx
10065d02  jnz short loc_10065D46
10065d04  cmp dword ptr [eax], 0
10065d07  jz short loc_10065D46
10065d09  test eax, eax
10065d0b  jz short loc_10065D16
10065d0d  cmp [eax+4], ecx
10065d10  jnz short loc_10065D16
10065d12  mov eax, [eax]
10065d14  jmp short loc_10065D18
10065d16  xor eax, eax
10065d18  mov edx, [eax]
10065d1a  mov ecx, eax
10065d1c  mov eax, [edx+178h]
10065d22  call eax
10065d24  test eax, eax
10065d26  jz short loc_10065DA4
10065d28  mov edx, [eax]
10065d2a  push edi
10065d2b  mov ecx, eax
10065d2d  mov eax, [edx+24h]
10065d30  push esi
10065d31  call eax
10065d33  mov ecx, dword_10735E3C
10065d39  mov edx, [ecx]
10065d3b  lea eax, [edi+0Ch]
10065d3e  push eax
10065d3f  mov eax, [edx+50h]
10065d42  call eax
10065d44  jmp short loc_10065DA4
10065d46  mov ecx, dword_107598D4
10065d4c  cmp dword ptr [ecx+30h], 0
10065d50  jz short loc_10065D79
10065d52  mov ecx, off_106E2F04
10065d58  mov edx, [ecx]
10065d5a  mov eax, [edx+28h]
10065d5d  push offset unk_10759FA0
10065d62  call eax
10065d64  fldz
10065d66  fxch st(1)
10065d68  fucomip st, st(1)
10065d6a  fstp st
10065d6c  lahf
10065d6d  test ah, 44h
10065d70  jnp short loc_10065D79
10065d72  or dword ptr [edi+24h], 20000h
10065d79  mov edx, [esi]
10065d7b  mov eax, [edx+3D4h]
10065d81  mov ecx, esi
10065d83  call eax
10065d85  test eax, eax
10065d87  jz short loc_10065DA4
10065d89  mov edx, [eax]
10065d8b  fld [ebp+inputSampleTime]
10065d8e  lea ecx, [esi+14BCh]
10065d94  push ecx
10065d95  push edi
10065d96  push ecx
10065d97  mov ecx, eax
10065d99  fstp [esp-4+var_s4]
10065d9c  mov eax, [edx+420h]
10065da2  call eax
10065da4  test byte ptr [esi+0F0h], 20h
10065dab  jz short loc_10065DEC
10065dad  cmp byte ptr [esi+14C8h], 0
10065db4  jz short loc_10065DE3; Raze Charger turn patches only opcode 74->EB here, leaving displacement 2D. Original JZ checks previous frozen state; EB always reaches 65DE3 and bypasses cmd old-angle copy + engine SetViewAngles while bit 20 is set. Bytes NOT patched in this analysis. ; Raze Charger turn patches only opcode 74->EB here, leaving displacement 2D. Original JZ checks previous frozen state; EB always reaches 65DE3 and bypasses cmd old-angle copy + engine SetViewAngles while bit 20 is set. Bytes NOT patched in this analysis.
10065db6  fld dword ptr [esi+14BCh]; Frozen view overwrite: player+14BC/14C0/14C4 -> cmd+0C/10/14; followed by engine vtable slot 20 SetViewAngles. Raze makes this block unreachable while enabled. ; Frozen view overwrite: player+14BC/14C0/14C4 -> cmd+0C/10/14; followed by engine vtable slot 20 SetViewAngles. Raze makes this block unreachable while enabled.
10065dbc  lea eax, [edi+0Ch]
10065dbf  fstp dword ptr [eax]
10065dc1  push eax
10065dc2  fld dword ptr [esi+14C0h]
10065dc8  fstp dword ptr [eax+4]
10065dcb  fld dword ptr [esi+14C4h]
10065dd1  fstp dword ptr [eax+8]
10065dd4  mov ecx, dword_10735E3C
10065dda  mov edx, [ecx]
10065ddc  mov eax, [edx+50h]
10065ddf  call eax
10065de1  jmp short loc_10065DF3
10065de3  mov byte ptr [esi+14C8h], 1
10065dea  jmp short loc_10065DF3
10065dec  mov byte ptr [esi+14C8h], 0
10065df3  fld dword ptr [edi+0Ch]
10065df6  push edi
10065df7  fstp dword ptr [esi+14BCh]
10065dfd  mov ecx, esi
10065dff  fld dword ptr [edi+10h]
10065e02  fstp dword ptr [esi+14C0h]
10065e08  fld dword ptr [edi+14h]
10065e0b  fstp dword ptr [esi+14C4h]
10065e11  call sub_10065A90
10065e16  pop edi
10065e17  mov al, 1
10065e19  pop esi
10065e1a  pop ebp
10065e1b  retn 8
