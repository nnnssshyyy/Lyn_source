; Raze chat-spy static disassembly (IDA Pro 9.4 via ida-pro-mcp)
; Image base: 0x10000000; analyst-assigned ChatSpy_* names
; Original SHA256: dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a

; ChatSpy_RegisterEvents
ChatSpy_RegisterEvents:
1003f550  push esi
1003f551  mov esi, ecx
1003f553  movzx eax, byte ptr [ecx+8]
1003f557  mov ecx, dword_103EC800
1003f55d  test ecx, ecx
1003f55f  setz dl
1003f562  or dl, al
1003f564  test dl, 1
1003f567  jnz loc_1003F65C
1003f56d  mov eax, [ecx]
1003f56f  push offset String2; "player_say"
1003f574  push esi
1003f575  call dword ptr [eax+10h]
1003f578  test al, al
1003f57a  jnz short loc_1003F58F
1003f57c  mov ecx, dword_103EC800
1003f582  mov eax, [ecx]
1003f584  push 0
1003f586  push offset String2; "player_say"
1003f58b  push esi
1003f58c  call dword ptr [eax+0Ch]
loc_1003F58F:
1003f58f  mov ecx, dword_103EC800
1003f595  mov eax, [ecx]
1003f597  push offset aVoteCastYes; "vote_cast_yes"
1003f59c  push esi
1003f59d  call dword ptr [eax+10h]
1003f5a0  test al, al
1003f5a2  jnz short loc_1003F5B7
1003f5a4  mov ecx, dword_103EC800
1003f5aa  mov eax, [ecx]
1003f5ac  push 0
1003f5ae  push offset aVoteCastYes; "vote_cast_yes"
1003f5b3  push esi
1003f5b4  call dword ptr [eax+0Ch]
loc_1003F5B7:
1003f5b7  mov ecx, dword_103EC800
1003f5bd  mov eax, [ecx]
1003f5bf  push offset aVoteCastNo; "vote_cast_no"
1003f5c4  push esi
1003f5c5  call dword ptr [eax+10h]
1003f5c8  test al, al
1003f5ca  jnz short loc_1003F5DF
1003f5cc  mov ecx, dword_103EC800
1003f5d2  mov eax, [ecx]
1003f5d4  push 0
1003f5d6  push offset aVoteCastNo; "vote_cast_no"
1003f5db  push esi
1003f5dc  call dword ptr [eax+0Ch]
loc_1003F5DF:
1003f5df  mov ecx, dword_103EC800
1003f5e5  mov eax, [ecx]
1003f5e7  push offset aVoteFailed; "vote_failed"
1003f5ec  push esi
1003f5ed  call dword ptr [eax+10h]
1003f5f0  test al, al
1003f5f2  jnz short loc_1003F607
1003f5f4  mov ecx, dword_103EC800
1003f5fa  mov eax, [ecx]
1003f5fc  push 0
1003f5fe  push offset aVoteFailed; "vote_failed"
1003f603  push esi
1003f604  call dword ptr [eax+0Ch]
loc_1003F607:
1003f607  mov ecx, dword_103EC800
1003f60d  mov eax, [ecx]
1003f60f  push offset aVotePassed; "vote_passed"
1003f614  push esi
1003f615  call dword ptr [eax+10h]
1003f618  test al, al
1003f61a  jnz short loc_1003F62F
1003f61c  mov ecx, dword_103EC800
1003f622  mov eax, [ecx]
1003f624  push 0
1003f626  push offset aVotePassed; "vote_passed"
1003f62b  push esi
1003f62c  call dword ptr [eax+0Ch]
loc_1003F62F:
1003f62f  mov ecx, dword_103EC800
1003f635  mov eax, [ecx]
1003f637  push offset aVoteEnded; "vote_ended"
1003f63c  push esi
1003f63d  call dword ptr [eax+10h]
1003f640  test al, al
1003f642  jnz short loc_1003F657
1003f644  mov ecx, dword_103EC800
1003f64a  mov eax, [ecx]
1003f64c  push 0
1003f64e  push offset aVoteEnded; "vote_ended"
1003f653  push esi
1003f654  call dword ptr [eax+0Ch]
loc_1003F657:
1003f657  mov al, 1
1003f659  xchg al, [esi+8]
loc_1003F65C:
1003f65c  pop esi
1003f65d  retn

; ChatSpy_OnGameEvent
ChatSpy_OnGameEvent:
10040550  push ebp; Static analysis: handles received player_say events; userid -> index 1..64; sender/local m_iTeamNum inequality; stores 76-byte record before output gates. No server-side message acquisition established.
10040551  mov ebp, esp
10040553  push ebx
10040554  push edi
10040555  push esi
10040556  sub esp, 120h
1004055c  mov ebx, ecx
1004055e  mov edi, [ebp+arg_0]
10040561  mov [ebp+var_1C], esp
10040564  mov [ebp+var_10], 0FFFFFFFFh
1004056b  lea eax, [ebp+var_18]
1004056e  mov [ebp+var_14], offset loc_10048100
10040575  mov ecx, large fs:0
1004057c  mov [ebp+var_18], ecx
1004057f  mov large fs:0, eax
10040585  test edi, edi
10040587  jz loc_10040C30
1004058d  mov eax, [edi]
1004058f  mov ecx, edi
10040591  call dword ptr [eax+4]
10040594  test eax, eax
10040596  jz loc_10040C30
1004059c  mov esi, eax
1004059e  push offset String2; "player_say"
100405a3  push eax; String1
100405a4  call ds:_stricmp; api-ms-win-crt-string-l1-1-0
100405aa  add esp, 8
100405ad  test eax, eax
100405af  jz loc_100406A6
100405b5  push offset aVoteCastYes; "vote_cast_yes"
100405ba  push esi; String1
100405bb  call ds:_stricmp; api-ms-win-crt-string-l1-1-0
100405c1  add esp, 8
100405c4  test eax, eax
100405c6  jz loc_1004066A
100405cc  push offset aVoteCastNo; "vote_cast_no"
100405d1  push esi; String1
100405d2  call ds:_stricmp; api-ms-win-crt-string-l1-1-0
100405d8  add esp, 8
100405db  test eax, eax
100405dd  jz loc_1004066A
100405e3  push offset aVoteFailed; "vote_failed"
100405e8  push esi; String1
100405e9  call ds:_stricmp; api-ms-win-crt-string-l1-1-0
100405ef  add esp, 8
100405f2  test eax, eax
100405f4  jz short loc_10040620
100405f6  push offset aVotePassed; "vote_passed"
100405fb  push esi; String1
100405fc  call ds:_stricmp; api-ms-win-crt-string-l1-1-0
10040602  add esp, 8
10040605  test eax, eax
10040607  jz short loc_10040620
10040609  push offset aVoteEnded; "vote_ended"
1004060e  push esi; String1
1004060f  call ds:_stricmp; api-ms-win-crt-string-l1-1-0
10040615  add esp, 8
10040618  test eax, eax
1004061a  jnz loc_10040C30
loc_10040620:
10040620  mov byte ptr [ebx+34h], 1
10040624  call _Query_perf_frequency
10040629  mov edi, eax
1004062b  mov esi, edx
1004062d  call _Query_perf_counter
10040632  mov ecx, edi
10040634  xor ecx, 16E3600h
1004063a  or ecx, esi
1004063c  jz loc_1004096A
10040642  mov ecx, edi
10040644  xor ecx, 989680h
1004064a  or ecx, esi
1004064c  jnz loc_100409E7
10040652  mov ecx, 64h ; 'd'
10040657  mov esi, edx
10040659  mov edx, eax
1004065b  mulx ecx, eax, ecx
10040660  imul edx, esi, 64h ; 'd'
10040663  add edx, ecx
10040665  jmp loc_10040A6C
loc_1004066A:
1004066a  mov eax, [edi]
1004066c  mov ecx, edi
1004066e  push 0
10040670  push offset aEntityid; "entityid"
10040675  call dword ptr [eax+18h]
10040678  test eax, eax
1004067a  jle loc_10040C30
10040680  mov edi, eax
10040682  push offset aVoteCastYes; "vote_cast_yes"
10040687  push esi; String1
10040688  call ds:_stricmp; api-ms-win-crt-string-l1-1-0
1004068e  add esp, 8
10040691  xor edx, edx
10040693  test eax, eax
10040695  setz dl
10040698  mov ecx, ebx
1004069a  push edx
1004069b  push edi
1004069c  call sub_1003F790
100406a1  jmp loc_10040C30
loc_100406A6:
100406a6  mov eax, [edi]
100406a8  mov ecx, edi
100406aa  push 0
100406ac  push offset aUserid; "userid"
100406b1  call dword ptr [eax+18h]
100406b4  mov ecx, dword_103EC770
100406ba  mov edx, [ecx]
100406bc  push eax
100406bd  call dword ptr [edx+24h]
100406c0  mov esi, eax
100406c2  add eax, 0FFFFFFBFh
100406c5  cmp eax, 0FFFFFFC0h
100406c8  jb loc_10040C30
100406ce  mov ecx, dword_103EC7B8
100406d4  mov eax, [ecx]
100406d6  push esi
100406d7  call dword ptr [eax+0Ch]
100406da  test eax, eax
100406dc  jz loc_10040C30
100406e2  mov [ebp+Str], eax
100406e5  mov ecx, off_1029E014
100406eb  call sub_10179540
100406f0  mov [ebp+var_24], eax
100406f3  mov eax, dword_103EEAD8
100406f8  mov ecx, TlsIndex
100406fe  mov edx, large fs:2Ch
10040705  mov ecx, [edx+ecx*4]
10040708  cmp eax, [ecx+4]
1004070e  jg loc_10040C46
loc_10040714:
10040714  mov ecx, dword_103EEAD4
1004071a  mov edx, [ebp+var_24]
1004071d  mov ecx, [ecx+edx]
10040720  mov [ebp+var_24], ecx
10040723  mov ecx, TlsIndex
10040729  mov edx, large fs:2Ch
10040730  mov ecx, [edx+ecx*4]
10040733  cmp eax, [ecx+4]
10040739  jg loc_10040CA5
loc_1004073F:
1004073f  mov eax, dword_103EEAD4
10040744  mov ecx, [ebp+Str]
10040747  mov eax, [eax+ecx]
1004074a  cmp eax, [ebp+var_24]
1004074d  jz loc_10040C30
10040753  mov [ebp+Str], eax
10040756  mov [ebp+var_28], ebx
10040759  mov [ebp+var_10], 0FFFFFFFFh
10040760  lea eax, [ebp+var_98]
10040766  push esi; int
10040767  push eax; void *
10040768  call ChatSpy_GetPlayerName
1004076d  cmp [ebp+var_88], 0
10040774  jnz short loc_10040790
10040776  mov [ebp+var_10], 2
1004077d  lea ecx, [ebp+var_98]; void *
10040783  push offset aUnknown; "Unknown"
10040788  call sub_1002BB30
1004078d  mov edi, [ebp+arg_0]
loc_10040790:
10040790  mov eax, [edi]
10040792  mov eax, [eax+24h]
10040795  mov [ebp+var_10], 2
1004079c  mov ecx, edi
1004079e  push offset unk_1029205C
100407a3  push (offset aContext+3); "text"
100407a8  call eax
100407aa  test eax, eax
100407ac  jz loc_10040C25
100407b2  cmp word ptr [eax], 0
100407b6  jz loc_10040C25
100407bc  lea ecx, [ebp+Src]; void *
100407c2  push eax; String
100407c3  mov [ebp+var_24], eax
100407c6  call sub_100387C0
100407cb  mov ecx, [ebp+var_24]
100407ce  mov [ebp+var_10], 3
100407d5  lea eax, [ebp+var_B0]
100407db  push ecx; int
100407dc  push eax; Src
100407dd  call ChatSpy_PackWideCodeUnits; Reads 16-bit words, emits low byte then high byte, stops at first zero BYTE (including high-byte zero). Compatible with reconstructing a raw C string; wchar_t decompiler types do NOT prove that the event text is UTF-16. Not a Unicode conversion.
100407e2  add esp, 8
100407e5  mov [ebp+var_10], 4
100407ec  lea ecx, [ebp+var_FC]
100407f2  lea eax, [ebp+var_98]
100407f8  push eax; Src
100407f9  call sub_100385F0
100407fe  lea ecx, [ebp+var_E4]
10040804  mov [ebp+var_10], 11h
1004080b  lea eax, [ebp+Src]
10040811  push eax; Src
10040812  call sub_1001F3F0
10040817  lea ecx, [ebp+var_CC]
1004081d  mov [ebp+var_10], 12h
10040824  lea eax, [ebp+var_B0]
1004082a  push eax; Src
1004082b  call sub_100385F0
10040830  mov eax, [ebp+var_28]
10040833  lea ecx, [eax+18h]
10040836  mov eax, [ebp+Str]
10040839  mov [ebp+var_B4], eax
1004083f  mov [ebp+var_10], 13h
10040846  lea eax, [ebp+var_FC]
1004084c  push eax
1004084d  call ChatSpy_AppendEnemyRecord
10040852  lea ecx, [ebp+var_FC]
10040858  call ChatSpy_DestroyEnemyRecord
1004085d  cmp ChatSpy_ShowHud, 1
10040864  jnz loc_10040C0F
1004086a  mov [ebp+var_10], 4
10040871  mov ecx, [ebp+var_28]
10040874  lea eax, [ebp+var_B0]
1004087a  push eax
1004087b  lea eax, [ebp+var_98]
10040881  push eax
10040882  call ChatSpy_HasMatchingHudMessage; Compare normalized (name, text) pair against normal-HUD cache. No userid, sequence, or time limit in this comparator. Return value consumed as AL bool.
10040887  test al, al
10040889  jnz loc_10040C0F
1004088f  cmp ChatSpy_Enabled, 1
10040896  jnz loc_10040C0F
1004089c  lea ecx, [ebp+var_FC]; void *
100408a2  push offset Buf; Str
100408a7  call sub_1000E5E0
100408ac  cmp ChatSpy_HideHudTeamPrefix, 0
100408b3  jz loc_10040A77
100408b9  mov [ebp+var_10], 5
100408c0  lea ecx, [ebp+var_114]; void *
100408c6  push offset byte_10291CA8; Str
100408cb  call sub_1000E5E0
100408d0  mov [ebp+var_10], 6
100408d7  lea ecx, [ebp+var_44]
100408da  lea eax, [ebp+var_98]
100408e0  push eax; Src
100408e1  lea eax, [ebp+var_114]
100408e7  push eax; void *
100408e8  push ecx; int
100408e9  call sub_10040230
100408ee  add esp, 0Ch
100408f1  mov [ebp+var_10], 7
100408f8  lea ecx, [ebp+var_5C]
100408fb  push offset byte_10290911; Str
10040900  lea eax, [ebp+var_44]
10040903  push eax; Src
10040904  push ecx; int
10040905  call sub_1000BBE0
1004090a  add esp, 0Ch
1004090d  mov [ebp+var_10], 8
10040914  lea ecx, [ebp+var_74]
10040917  lea eax, [ebp+var_B0]
1004091d  push eax; Src
1004091e  lea eax, [ebp+var_5C]
10040921  push eax; void *
10040922  push ecx; int
10040923  call sub_10040230
10040928  add esp, 0Ch
1004092b  lea ecx, [ebp+var_5C]
1004092e  call sub_1000BC90
10040933  lea ecx, [ebp+var_44]
10040936  call sub_1000BC90
1004093b  lea ecx, [ebp+var_114]
10040941  call sub_1000BC90
10040946  mov ecx, dword_103EC840
1004094c  cmp [ebp+var_60], 10h
10040950  lea eax, [ebp+var_74]
10040953  jb short loc_10040958
10040955  mov eax, [ebp+var_74]
loc_10040958:
10040958  mov [ebp+var_10], 10h
1004095f  push eax
10040960  call ChatSpy_PrintToHud
10040965  jmp loc_10040BFC
loc_1004096A:
1004096a  push 0
1004096c  push 16E3600h
10040971  push edx
10040972  push eax
10040973  mov edi, eax
10040975  mov [ebp+Str], edx
10040978  call sub_10180AB0
1004097d  mov ecx, edx
1004097f  mov edx, eax
10040981  mov esi, 3B9ACA00h
10040986  mulx esi, edx, esi
1004098b  mov [ebp+var_2C], esi
1004098e  mov esi, 3B9ACA00h
10040993  mov [ebp+var_24], edx
10040996  mov [ebp+var_28], ebx
10040999  imul ebx, ecx, 3B9ACA00h
1004099f  add ebx, [ebp+var_2C]
100409a2  mov edx, eax
100409a4  mov eax, 16E3600h
100409a9  mulx eax, edx, eax
100409ae  imul ecx, 16E3600h
100409b4  add ecx, eax
100409b6  sub edi, edx
100409b8  mov edx, edi
100409ba  mulx edx, eax, esi
100409bf  mov esi, [ebp+Str]
100409c2  sbb esi, ecx
100409c4  imul ecx, esi, 3B9ACA00h
100409ca  add edx, ecx
100409cc  push 0
100409ce  push 16E3600h
100409d3  push edx
100409d4  push eax
100409d5  call sub_10180AB0
100409da  add eax, [ebp+var_24]
100409dd  adc edx, ebx
100409df  mov ebx, [ebp+var_28]
100409e2  jmp loc_10040A6C
loc_100409E7:
100409e7  push esi
100409e8  push edi
100409e9  push edx
100409ea  push eax
100409eb  mov [ebp+var_80], eax
100409ee  mov [ebp+Str], edx
100409f1  call sub_10180AB0
100409f6  mov ecx, eax
100409f8  mov [ebp+var_24], eax
100409fb  mov eax, edx
100409fd  mov edx, ecx
100409ff  mov ecx, 3B9ACA00h
10040a04  mulx ecx, edx, ecx
10040a09  mov [ebp+var_2C], ecx
10040a0c  mov [ebp+var_28], edx
10040a0f  mov edx, [ebp+var_24]
10040a12  mulx ecx, edx, edi
10040a17  mov dword ptr [ebp+var_7C+4], ecx
10040a1a  mov dword ptr [ebp+var_7C], edx
10040a1d  imul edx, eax, 3B9ACA00h
10040a23  add edx, [ebp+var_2C]
10040a26  mov [ebp+var_2C], edx
10040a29  mov edx, ebx
10040a2b  mov ebx, esi
10040a2d  imul ebx, [ebp+var_24]
10040a31  imul eax, edi
10040a34  add eax, ebx
10040a36  mov ebx, edx
10040a38  add eax, dword ptr [ebp+var_7C+4]
10040a3b  mov edx, [ebp+var_80]
10040a3e  sub edx, dword ptr [ebp+var_7C]
10040a41  mov ecx, 3B9ACA00h
10040a46  mulx edx, ecx, ecx
10040a4b  mov [ebp+var_24], ecx
10040a4e  mov ecx, [ebp+Str]
10040a51  sbb ecx, eax
10040a53  imul eax, ecx, 3B9ACA00h
10040a59  add edx, eax
10040a5b  push esi
10040a5c  push edi
10040a5d  push edx
10040a5e  push [ebp+var_24]
10040a61  call sub_10180AB0
10040a66  add eax, [ebp+var_28]
10040a69  adc edx, [ebp+var_2C]
loc_10040A6C:
10040a6c  mov [ebx+28h], eax
10040a6f  mov [ebx+2Ch], edx
10040a72  jmp loc_10040C30
loc_10040A77:
10040a77  mov eax, dword_103F59E4
10040a7c  cmp eax, 1
10040a7f  jz short loc_10040A92
10040a81  cmp eax, 2
10040a84  jnz short loc_10040A9E
10040a86  mov ecx, offset aZarazhyonnyy; "(Zarazhyonnyy) "
10040a8b  mov edx, offset aVyzhivshiy; "(Vyzhivshiy) "
10040a90  jmp short loc_10040AA8
loc_10040A92:
10040a92  mov ecx, offset aInfectado; "(Infectado) "
10040a97  mov edx, offset aSuperviviente; "(Superviviente) "
10040a9c  jmp short loc_10040AA8
loc_10040A9E:
10040a9e  mov ecx, offset aInfected; "(Infected) "
10040aa3  mov edx, offset aSurvivor; "(Survivor) "
loc_10040AA8:
10040aa8  mov eax, [ebp+Str]
10040aab  cmp eax, 3
10040aae  jnz short loc_10040AFF
10040ab0  mov [ebp+Str], ecx
10040ab3  mov [ebp+var_10], 5
10040aba  lea ecx, [ebp+var_44]; void *
10040abd  push offset byte_10291CAA; Str
10040ac2  call sub_1000E5E0
10040ac7  mov [ebp+var_10], 0Ah
10040ace  lea eax, [ebp+var_5C]
10040ad1  push [ebp+Str]; Str
10040ad4  lea ecx, [ebp+var_44]
10040ad7  push ecx; Src
10040ad8  push eax; int
10040ad9  call sub_1000BBE0
10040ade  add esp, 0Ch
10040ae1  mov [ebp+var_10], 0Bh
10040ae8  lea eax, [ebp+var_74]
10040aeb  push offset byte_10291CA8; Str
10040af0  lea ecx, [ebp+var_5C]
10040af3  push ecx; Src
10040af4  push eax; int
10040af5  call sub_1000BBE0
10040afa  add esp, 0Ch
10040afd  jmp short loc_10040B4C
loc_10040AFF:
10040aff  mov [ebp+Str], edx
10040b02  mov [ebp+var_10], 5
10040b09  lea ecx, [ebp+var_44]; void *
10040b0c  push offset byte_10291CAC; Str
10040b11  call sub_1000E5E0
10040b16  mov [ebp+var_10], 0Ch
10040b1d  lea eax, [ebp+var_5C]
10040b20  push [ebp+Str]; Str
10040b23  lea ecx, [ebp+var_44]
10040b26  push ecx; Src
10040b27  push eax; int
10040b28  call sub_1000BBE0
10040b2d  add esp, 0Ch
10040b30  mov [ebp+var_10], 0Dh
10040b37  lea eax, [ebp+var_74]
10040b3a  push offset byte_10291CA8; Str
10040b3f  lea ecx, [ebp+var_5C]
10040b42  push ecx; Src
10040b43  push eax; int
10040b44  call sub_1000BBE0
10040b49  add esp, 0Ch
loc_10040B4C:
10040b4c  lea ecx, [ebp+var_FC]
10040b52  lea esi, [ebp+var_74]
10040b55  push esi
10040b56  call sub_1002DD40
10040b5b  mov ecx, esi
10040b5d  call sub_1000BC90
10040b62  lea ecx, [ebp+var_5C]
10040b65  call sub_1000BC90
10040b6a  lea ecx, [ebp+var_44]
10040b6d  call sub_1000BC90
10040b72  mov [ebp+var_10], 5
10040b79  lea ecx, [ebp+var_44]
10040b7c  lea eax, [ebp+var_98]
10040b82  push eax; int
10040b83  lea eax, [ebp+var_FC]
10040b89  push eax; int
10040b8a  push ecx; void *
10040b8b  call sub_100402F0
10040b90  add esp, 0Ch
10040b93  mov [ebp+var_10], 0Eh
10040b9a  lea ecx, [ebp+var_5C]
10040b9d  push offset byte_10290911; Str
10040ba2  lea eax, [ebp+var_44]
10040ba5  push eax; Src
10040ba6  push ecx; int
10040ba7  call sub_1000BBE0
10040bac  add esp, 0Ch
10040baf  mov [ebp+var_10], 0Fh
10040bb6  lea ecx, [ebp+var_74]
10040bb9  lea eax, [ebp+var_B0]
10040bbf  push eax; Src
10040bc0  lea eax, [ebp+var_5C]
10040bc3  push eax; void *
10040bc4  push ecx; int
10040bc5  call sub_10040230
10040bca  add esp, 0Ch
10040bcd  lea ecx, [ebp+var_5C]
10040bd0  call sub_1000BC90
10040bd5  lea ecx, [ebp+var_44]
10040bd8  call sub_1000BC90
10040bdd  mov ecx, dword_103EC840
10040be3  cmp [ebp+var_60], 10h
10040be7  lea eax, [ebp+var_74]
10040bea  jb short loc_10040BEF
10040bec  mov eax, [ebp+var_74]
loc_10040BEF:
10040bef  mov [ebp+var_10], 9
10040bf6  push eax
10040bf7  call ChatSpy_PrintToHud
loc_10040BFC:
10040bfc  lea ecx, [ebp+var_74]
10040bff  call sub_1000BC90
10040c04  lea ecx, [ebp+var_FC]
10040c0a  call sub_1000BC90
loc_10040C0F:
10040c0f  lea ecx, [ebp+var_B0]
10040c15  call sub_1000BC90
10040c1a  lea ecx, [ebp+Src]
10040c20  call sub_1000BCE0
loc_10040C25:
10040c25  lea ecx, [ebp+var_98]
10040c2b  call sub_1000BC90
loc_10040C30:
10040c30  mov eax, [ebp+var_18]
10040c33  mov large fs:0, eax
10040c39  add esp, 120h
10040c3f  pop esi
10040c40  pop edi
10040c41  pop ebx
10040c42  pop ebp
10040c43  retn 4
loc_10040C46:
10040c46  push offset dword_103EEAD8
10040c4b  call sub_101809C3
10040c50  add esp, 4
10040c53  mov eax, dword_103EEAD8
10040c58  cmp eax, 0FFFFFFFFh
10040c5b  jnz loc_10040714
10040c61  mov [ebp+var_2C], esi
10040c64  mov [ebp+var_28], ebx
10040c67  mov [ebp+var_10], 0
10040c6e  push offset aMIteamnum; "m_iTeamNum"
10040c73  push offset aCbaseentity; "CBaseEntity"
10040c78  call sub_1017C730
10040c7d  add esp, 8
10040c80  mov dword_103EEAD4, eax
10040c85  push offset dword_103EEAD8
10040c8a  call sub_10180A43
10040c8f  add esp, 4
10040c92  mov eax, dword_103EEAD8
10040c97  mov ebx, [ebp+var_28]
10040c9a  mov edi, [ebp+arg_0]
10040c9d  mov esi, [ebp+var_2C]
10040ca0  jmp loc_10040714
loc_10040CA5:
10040ca5  push offset dword_103EEAD8
10040caa  call sub_101809C3
10040caf  add esp, 4
10040cb2  cmp dword_103EEAD8, 0FFFFFFFFh
10040cb9  jnz loc_1004073F
10040cbf  mov [ebp+var_2C], esi
10040cc2  mov [ebp+var_28], ebx
10040cc5  mov [ebp+var_10], 1
10040ccc  push offset aMIteamnum; "m_iTeamNum"
10040cd1  push offset aCbaseentity; "CBaseEntity"
10040cd6  call sub_1017C730
10040cdb  add esp, 8
10040cde  mov dword_103EEAD4, eax
10040ce3  push offset dword_103EEAD8
10040ce8  call sub_10180A43
10040ced  add esp, 4
10040cf0  mov ebx, [ebp+var_28]
10040cf3  mov edi, [ebp+arg_0]
10040cf6  mov esi, [ebp+var_2C]
10040cf9  jmp loc_1004073F
loc_10040D00:
10040d00  push ebp
10040d01  sub esp, 10h
10040d04  add ebp, 0Ch
10040d07  lea ecx, [ebp+var_E4]
10040d0d  call sub_1000BCE0
10040d12  add esp, 10h
10040d15  pop ebp
10040d16  retn
loc_10040D20:
10040d20  push ebp
10040d21  sub esp, 10h
10040d24  add ebp, 0Ch
10040d27  lea ecx, [ebp+var_FC]
10040d2d  call sub_1000BC90
10040d32  add esp, 10h
10040d35  pop ebp
10040d36  retn
loc_10040D40:
10040d40  push ebp
10040d41  sub esp, 10h
10040d44  add ebp, 0Ch
10040d47  lea ecx, [ebp+var_FC]
10040d4d  call ChatSpy_DestroyEnemyRecord
10040d52  add esp, 10h
10040d55  pop ebp
10040d56  retn
loc_10040D60:
10040d60  push ebp
10040d61  sub esp, 10h
10040d64  add ebp, 0Ch
10040d67  lea ecx, [ebp+var_5C]
10040d6a  call sub_1000BC90
10040d6f  add esp, 10h
10040d72  pop ebp
10040d73  retn
loc_10040D80:
10040d80  push ebp
10040d81  sub esp, 10h
10040d84  add ebp, 0Ch
10040d87  lea ecx, [ebp+var_44]
10040d8a  call sub_1000BC90
10040d8f  add esp, 10h
10040d92  pop ebp
10040d93  retn
loc_10040DA0:
10040da0  push ebp
10040da1  sub esp, 10h
10040da4  add ebp, 0Ch
10040da7  lea ecx, [ebp+var_5C]
10040daa  call sub_1000BC90
10040daf  add esp, 10h
10040db2  pop ebp
10040db3  retn
loc_10040DC0:
10040dc0  push ebp
10040dc1  sub esp, 10h
10040dc4  add ebp, 0Ch
10040dc7  lea ecx, [ebp+var_44]
10040dca  call sub_1000BC90
10040dcf  add esp, 10h
10040dd2  pop ebp
10040dd3  retn
loc_10040DE0:
10040de0  push ebp
10040de1  sub esp, 10h
10040de4  add ebp, 0Ch
10040de7  lea ecx, [ebp+var_5C]
10040dea  call sub_1000BC90
10040def  add esp, 10h
10040df2  pop ebp
10040df3  retn
loc_10040E00:
10040e00  push ebp
10040e01  sub esp, 10h
10040e04  add ebp, 0Ch
10040e07  lea ecx, [ebp+var_44]
10040e0a  call sub_1000BC90
10040e0f  add esp, 10h
10040e12  pop ebp
10040e13  retn
loc_10040E20:
10040e20  push ebp
10040e21  sub esp, 10h
10040e24  add ebp, 0Ch
10040e27  lea ecx, [ebp+var_74]
10040e2a  call sub_1000BC90
10040e2f  add esp, 10h
10040e32  pop ebp
10040e33  retn
loc_10040E40:
10040e40  push ebp
10040e41  sub esp, 10h
10040e44  add ebp, 0Ch
10040e47  lea ecx, [ebp+var_5C]
10040e4a  call sub_1000BC90
10040e4f  add esp, 10h
10040e52  pop ebp
10040e53  retn
loc_10040E60:
10040e60  push ebp
10040e61  sub esp, 10h
10040e64  add ebp, 0Ch
10040e67  lea ecx, [ebp+var_44]
10040e6a  call sub_1000BC90
10040e6f  add esp, 10h
10040e72  pop ebp
10040e73  retn
loc_10040E80:
10040e80  push ebp
10040e81  sub esp, 10h
10040e84  add ebp, 0Ch
10040e87  lea ecx, [ebp+var_114]
10040e8d  call sub_1000BC90
10040e92  add esp, 10h
10040e95  pop ebp
10040e96  retn
loc_10040EA0:
10040ea0  push ebp
10040ea1  sub esp, 10h
10040ea4  add ebp, 0Ch
10040ea7  lea ecx, [ebp+var_74]
10040eaa  call sub_1000BC90
10040eaf  add esp, 10h
10040eb2  pop ebp
10040eb3  retn
loc_10040EC0:
10040ec0  push ebp
10040ec1  sub esp, 10h
10040ec4  add ebp, 0Ch
10040ec7  lea ecx, [ebp+var_FC]
10040ecd  call sub_1000BC90
10040ed2  add esp, 10h
10040ed5  pop ebp
10040ed6  retn
loc_10040EE0:
10040ee0  push ebp
10040ee1  sub esp, 10h
10040ee4  add ebp, 0Ch
10040ee7  lea ecx, [ebp+var_B0]
10040eed  call sub_1000BC90
10040ef2  add esp, 10h
10040ef5  pop ebp
10040ef6  retn
loc_10040F00:
10040f00  push ebp
10040f01  sub esp, 10h
10040f04  add ebp, 0Ch
10040f07  lea ecx, [ebp+Src]
10040f0d  call sub_1000BCE0
10040f12  add esp, 10h
10040f15  pop ebp
10040f16  retn
loc_10040F20:
10040f20  push ebp
10040f21  sub esp, 10h
10040f24  add ebp, 0Ch
10040f27  lea ecx, [ebp+var_98]
10040f2d  call sub_1000BC90
10040f32  add esp, 10h
10040f35  pop ebp
10040f36  retn
loc_10040F40:
10040f40  push ebp
10040f41  sub esp, 10h
10040f44  add ebp, 0Ch
10040f47  push offset dword_103EEAD8
10040f4c  call sub_10180A15
10040f51  add esp, 14h
10040f54  pop ebp
10040f55  retn
loc_10040F60:
10040f60  push ebp
10040f61  sub esp, 10h
10040f64  add ebp, 0Ch
10040f67  push offset dword_103EEAD8
10040f6c  call sub_10180A15
10040f71  add esp, 14h
10040f74  pop ebp
10040f75  retn
loc_10048100:
10048100  mov eax, offset stru_102973B4
10048105  jmp __CxxFrameHandler3

; ChatSpy_RememberHudMessage
ChatSpy_RememberHudMessage:
1003eab0  push ebp; Normal-HUD dedup cache: normalize sender and text; append pair of 24-byte strings; when count >= 251 pop_front, leaving at most 250 entries.
1003eab1  mov ebp, esp
1003eab3  push ebx
1003eab4  push edi
1003eab5  push esi
1003eab6  sub esp, 68h
1003eab9  mov [ebp+var_20], ecx
1003eabc  mov edx, [ebp+arg_0]
1003eabf  mov [ebp+var_1C], esp
1003eac2  mov [ebp+var_10], 0FFFFFFFFh
1003eac9  lea eax, [ebp+var_18]
1003eacc  mov [ebp+var_14], offset loc_100480C0
1003ead3  mov ecx, large fs:0
1003eada  mov [ebp+var_18], ecx
1003eadd  mov large fs:0, eax
1003eae3  vxorps xmm0, xmm0, xmm0
1003eae7  vmovups xmmword ptr [ebp+Block], xmm0
1003eaec  mov dword ptr [ebp+var_4C], 0
1003eaf3  mov dword ptr [ebp+var_4C+4], 0Fh
1003eafa  vmovups xmmword ptr [ebp+var_44], xmm0
1003eaff  mov [ebp+var_34], 0
1003eb06  mov [ebp+var_30], 0Fh
1003eb0d  mov [ebp+var_10], 0
1003eb14  lea ecx, [ebp+Src]; Src
1003eb17  call ChatSpy_NormalizeText
1003eb1c  mov eax, dword ptr [ebp+var_4C+4]
1003eb1f  cmp eax, 10h
1003eb22  jb short loc_1003EB54
1003eb24  mov ecx, [ebp+Block]
1003eb27  lea esi, [eax+1]
1003eb2a  cmp esi, 1000h
1003eb30  jb short loc_1003EB4A
1003eb32  mov edx, [ecx-4]
1003eb35  add ecx, 0FFFFFFFCh
1003eb38  sub ecx, edx
1003eb3a  cmp ecx, 20h ; ' '
1003eb3d  jnb loc_1003ECD9
1003eb43  add eax, 24h ; '$'
1003eb46  mov esi, eax
1003eb48  mov ecx, edx
loc_1003EB4A:
1003eb4a  push esi
1003eb4b  push ecx; Block
1003eb4c  call sub_10180976
1003eb51  add esp, 8
loc_1003EB54:
1003eb54  mov edx, [ebp+arg_4]
1003eb57  vmovsd xmm0, [ebp+var_64]
1003eb5c  vmovsd [ebp+var_4C], xmm0
1003eb61  vmovups xmm0, [ebp+Src]
1003eb66  vmovups xmmword ptr [ebp+Block], xmm0
1003eb6b  lea ecx, [ebp+Src]; Src
1003eb6e  call ChatSpy_NormalizeText
1003eb73  mov eax, [ebp+var_30]
1003eb76  cmp eax, 10h
1003eb79  jb short loc_1003EBAB
1003eb7b  mov ecx, [ebp+var_44]
1003eb7e  lea esi, [eax+1]
1003eb81  cmp esi, 1000h
1003eb87  jb short loc_1003EBA1
1003eb89  mov edx, [ecx-4]
1003eb8c  add ecx, 0FFFFFFFCh
1003eb8f  sub ecx, edx
1003eb91  cmp ecx, 20h ; ' '
1003eb94  jnb loc_1003ECD9
1003eb9a  add eax, 24h ; '$'
1003eb9d  mov esi, eax
1003eb9f  mov ecx, edx
loc_1003EBA1:
1003eba1  push esi
1003eba2  push ecx; Block
1003eba3  call sub_10180976
1003eba8  add esp, 8
loc_1003EBAB:
1003ebab  lea eax, [ebp+var_44]
1003ebae  vmovsd xmm0, [ebp+var_64]
1003ebb3  vmovsd qword ptr [eax+10h], xmm0
1003ebb8  vmovups xmm0, [ebp+Src]
1003ebbd  vmovups xmmword ptr [eax], xmm0
1003ebc1  mov edx, [ebp+var_20]
1003ebc4  lea eax, [edx+38h]
1003ebc7  mov [ebp+var_24], eax
1003ebca  mov esi, [edx+40h]
1003ebcd  mov eax, [edx+48h]
1003ebd0  lea ecx, [eax+1]
1003ebd3  cmp esi, ecx
1003ebd5  ja short loc_1003EBEA
1003ebd7  mov ecx, [ebp+var_24]
1003ebda  push 1
1003ebdc  call sub_10047C10
1003ebe1  mov edx, [ebp+var_20]
1003ebe4  mov esi, [edx+40h]
1003ebe7  mov eax, [edx+48h]
loc_1003EBEA:
1003ebea  dec esi
1003ebeb  mov edi, [edx+44h]
1003ebee  and edi, esi
1003ebf0  mov [edx+44h], edi
1003ebf3  add edi, eax
1003ebf5  and esi, edi
1003ebf7  mov eax, [edx+3Ch]
1003ebfa  mov ecx, [eax+esi*4]
1003ebfd  test ecx, ecx
1003ebff  jnz short loc_1003EC2A
1003ec01  mov [ebp+var_28], edi
1003ec04  mov [ebp+var_2C], esi
1003ec07  push 30h ; '0'; Size
1003ec09  call sub_10180946
1003ec0e  add esp, 4
1003ec11  mov edx, [ebp+var_20]
1003ec14  mov ecx, [edx+3Ch]
1003ec17  mov esi, [ebp+var_2C]
1003ec1a  mov [ecx+esi*4], eax
1003ec1d  mov eax, [edx+3Ch]
1003ec20  mov ecx, [edx+40h]
1003ec23  dec ecx
1003ec24  and ecx, [ebp+var_28]
1003ec27  mov ecx, [eax+ecx*4]
loc_1003EC2A:
1003ec2a  lea eax, [ebp+Block]
1003ec2d  push eax; Src
1003ec2e  call sub_10047E00
1003ec33  mov ecx, [ebp+var_20]
1003ec36  mov eax, [ecx+48h]
1003ec39  inc eax
1003ec3a  mov [ecx+48h], eax
1003ec3d  cmp eax, 0FBh
1003ec42  jb short loc_1003EC4C
1003ec44  mov ecx, [ebp+var_24]
1003ec47  call ChatSpy_PopOldestHudMessage
loc_1003EC4C:
1003ec4c  mov eax, [ebp+var_30]
1003ec4f  cmp eax, 10h
1003ec52  jb short loc_1003EC80
1003ec54  mov ecx, [ebp+var_44]
1003ec57  lea esi, [eax+1]
1003ec5a  cmp esi, 1000h
1003ec60  jb short loc_1003EC76
1003ec62  mov edx, [ecx-4]
1003ec65  add ecx, 0FFFFFFFCh
1003ec68  sub ecx, edx
1003ec6a  cmp ecx, 20h ; ' '
1003ec6d  jnb short loc_1003ECD9
1003ec6f  add eax, 24h ; '$'
1003ec72  mov esi, eax
1003ec74  mov ecx, edx
loc_1003EC76:
1003ec76  push esi
1003ec77  push ecx; Block
1003ec78  call sub_10180976
1003ec7d  add esp, 8
loc_1003EC80:
1003ec80  mov [ebp+var_34], 0
1003ec87  mov [ebp+var_30], 0Fh
1003ec8e  mov byte ptr [ebp+var_44], 0
1003ec92  mov eax, dword ptr [ebp+var_4C+4]
1003ec95  cmp eax, 10h
1003ec98  jb short loc_1003ECC6
1003ec9a  mov ecx, [ebp+Block]
1003ec9d  lea esi, [eax+1]
1003eca0  cmp esi, 1000h
1003eca6  jb short loc_1003ECBC
1003eca8  mov edx, [ecx-4]
1003ecab  add ecx, 0FFFFFFFCh
1003ecae  sub ecx, edx
1003ecb0  cmp ecx, 20h ; ' '
1003ecb3  jnb short loc_1003ECD9
1003ecb5  add eax, 24h ; '$'
1003ecb8  mov esi, eax
1003ecba  mov ecx, edx
loc_1003ECBC:
1003ecbc  push esi
1003ecbd  push ecx; Block
1003ecbe  call sub_10180976
1003ecc3  add esp, 8
loc_1003ECC6:
1003ecc6  mov eax, [ebp+var_18]
1003ecc9  mov large fs:0, eax
1003eccf  add esp, 68h
1003ecd2  pop esi
1003ecd3  pop edi
1003ecd4  pop ebx
1003ecd5  pop ebp
1003ecd6  retn 8
loc_1003ECD9:
1003ecd9  ud2
loc_1003ECE0:
1003ece0  push ebp
1003ece1  sub esp, 8
1003ece4  add ebp, 0Ch
1003ece7  lea ecx, [ebp+Block]
1003ecea  call sub_1003EFE0
1003ecef  add esp, 8
1003ecf2  pop ebp
1003ecf3  retn
loc_100480C0:
100480c0  mov eax, offset stru_102972A8
100480c5  jmp __CxxFrameHandler3

; ChatSpy_NormalizeText
ChatSpy_NormalizeText:
1003ed00  push ebp
1003ed01  mov ebp, esp
1003ed03  push ebx
1003ed04  push edi
1003ed05  push esi
1003ed06  sub esp, 2Ch
1003ed09  mov esi, edx
1003ed0b  mov edi, ecx
1003ed0d  mov [ebp+var_1C], esp
1003ed10  mov [ebp+var_10], 0FFFFFFFFh
1003ed17  lea eax, [ebp+var_18]
1003ed1a  mov [ebp+var_14], offset loc_100480D0
1003ed21  mov ecx, large fs:0
1003ed28  mov [ebp+var_18], ecx
1003ed2b  mov large fs:0, eax
1003ed31  vxorps xmm0, xmm0, xmm0
1003ed35  vmovups xmmword ptr [edi], xmm0
1003ed39  mov dword ptr [edi+10h], 0
1003ed40  mov dword ptr [edi+14h], 0Fh
1003ed47  mov ebx, [edx+10h]
1003ed4a  cmp ebx, 10h
1003ed4d  mov [ebp+var_28], edi
1003ed50  jb short loc_1003ED81
1003ed52  mov [ebp+var_10], 0
1003ed59  movzx eax, [ebp+var_20]
1003ed5d  mov byte ptr [esp+38h+Src], al; int
1003ed61  mov [esp+38h+C], ebx; int
1003ed64  mov ecx, edi; Src
1003ed66  mov [ebp+var_24], esi
1003ed69  call sub_10044CB0
1003ed6e  sub esp, 8
1003ed71  mov esi, [ebp+var_24]
1003ed74  mov edi, [ebp+var_28]
1003ed77  mov dword ptr [edi+10h], 0
1003ed7e  mov ebx, [esi+10h]
loc_1003ED81:
1003ed81  cmp dword ptr [esi+14h], 10h
1003ed85  jb short loc_1003ED89
1003ed87  mov esi, [esi]
loc_1003ED89:
1003ed89  add ebx, esi
1003ed8b  xor eax, eax
1003ed8d  mov [ebp+var_2C], ebx
1003ed90  jmp short loc_1003EDA8
loc_1003ED92:
1003ed92  mov [esi+edi], al
1003ed95  mov byte ptr [esi+edi+1], 0
1003ed9a  mov esi, [ebp+var_24]
1003ed9d  mov ebx, [ebp+var_2C]
loc_1003EDA0:
1003eda0  mov edi, [ebp+var_28]
1003eda3  movzx eax, [ebp+var_1D]
loc_1003EDA7:
1003eda7  inc esi
loc_1003EDA8:
1003eda8  cmp esi, ebx
1003edaa  jz loc_1003EE9D
1003edb0  mov edi, eax
1003edb2  movzx ebx, byte ptr [esi]
1003edb5  mov [esp+38h+C], ebx; C
1003edb8  call ds:isspace; api-ms-win-crt-string-l1-1-0
1003edbe  test eax, eax
1003edc0  setnz al
1003edc3  jz short loc_1003EE00
1003edc5  mov ecx, edi
1003edc7  test cl, 1
1003edca  mov ebx, [ebp+var_2C]
1003edcd  mov edi, [ebp+var_28]
1003edd0  jnz short loc_1003EDA7
1003edd2  mov [ebp+var_1D], al
1003edd5  mov eax, [edi+10h]
1003edd8  mov ecx, [edi+14h]
1003eddb  cmp eax, ecx
1003eddd  jnb loc_1003EE67
1003ede3  lea edx, [eax+1]
1003ede6  mov [edi+10h], edx
1003ede9  mov edx, edi
1003edeb  cmp ecx, 10h
1003edee  jb short loc_1003EDF2
1003edf0  mov edx, [edi]
loc_1003EDF2:
1003edf2  mov word ptr [edx+eax], 20h ; ' '
1003edf8  jmp short loc_1003EDA0
loc_1003EE00:
1003ee00  mov [ebp+var_1D], al
1003ee03  mov [ebp+var_24], esi
1003ee06  movsx eax, bl
1003ee09  mov [esp+38h+C], eax; C
1003ee0c  call ds:tolower; api-ms-win-crt-string-l1-1-0
1003ee12  mov ecx, [ebp+var_28]; Src
1003ee15  mov edi, [ecx+10h]
1003ee18  mov edx, [ecx+14h]
1003ee1b  cmp edi, edx
1003ee1d  jnb short loc_1003EE37
1003ee1f  lea esi, [edi+1]
1003ee22  mov [ecx+10h], esi
1003ee25  mov esi, ecx
1003ee27  cmp edx, 10h
1003ee2a  jb loc_1003ED92
1003ee30  mov esi, [ecx]
1003ee32  jmp loc_1003ED92
loc_1003EE37:
1003ee37  mov [ebp+var_10], 0
1003ee3e  movzx edx, [ebp+var_20]
1003ee42  mov byte ptr [esp+38h+Src], dl; int
1003ee46  movsx eax, al
1003ee49  mov [esp+38h+Size], eax; char
1003ee4d  mov [esp+38h+C], 1; int
1003ee54  call sub_1000DEB0
1003ee59  sub esp, 0Ch
1003ee5c  mov esi, [ebp+var_24]
1003ee5f  mov ebx, [ebp+var_2C]
1003ee62  jmp loc_1003EDA0
loc_1003EE67:
1003ee67  mov [ebp+var_10], 0
1003ee6e  movzx eax, [ebp+var_20]
1003ee72  mov byte ptr [esp+38h+Src], al; int
1003ee76  mov [esp+38h+Size], 20h ; ' '; char
1003ee7e  mov [esp+38h+C], 1; int
1003ee85  mov ecx, edi; Src
1003ee87  mov [ebp+var_24], esi
1003ee8a  call sub_1000DEB0
1003ee8f  sub esp, 0Ch
1003ee92  mov ebx, [ebp+var_2C]
1003ee95  mov esi, [ebp+var_24]
1003ee98  jmp loc_1003EDA0
loc_1003EE9D:
1003ee9d  mov esi, [edi+10h]
1003eea0  test esi, esi
1003eea2  jz short loc_1003EEE8
1003eea4  cmp dword ptr [edi+14h], 10h
1003eea8  mov eax, edi
1003eeaa  jb short loc_1003EEAE
1003eeac  mov eax, [edi]
loc_1003EEAE:
1003eeae  cmp byte ptr [eax], 20h ; ' '
1003eeb1  jnz short loc_1003EECC
1003eeb3  lea ecx, [eax+1]
1003eeb6  mov [esp+38h+Size], esi; Size
1003eeba  mov [esp+38h+Src], ecx; Src
1003eebe  mov [esp+38h+C], eax; void *
1003eec1  call memmove
1003eec6  dec esi
1003eec7  mov [edi+10h], esi
1003eeca  jz short loc_1003EEE8
loc_1003EECC:
1003eecc  cmp dword ptr [edi+14h], 10h
1003eed0  mov eax, edi
1003eed2  jb short loc_1003EED6
1003eed4  mov eax, [edi]
loc_1003EED6:
1003eed6  cmp byte ptr [eax+esi-1], 20h ; ' '
1003eedb  jnz short loc_1003EEE8
1003eedd  lea ecx, [esi-1]
1003eee0  mov [edi+10h], ecx
1003eee3  mov byte ptr [eax+esi-1], 0
loc_1003EEE8:
1003eee8  mov eax, [ebp+var_18]
1003eeeb  mov large fs:0, eax
1003eef1  add esp, 2Ch
1003eef4  pop esi
1003eef5  pop edi
1003eef6  pop ebx
1003eef7  pop ebp
1003eef8  retn
loc_1003EF00:
1003ef00  push ebp
1003ef01  sub esp, 0Ch
1003ef04  add ebp, 0Ch
1003ef07  mov ecx, [ebp+var_28]
1003ef0a  call sub_1000BC90
1003ef0f  add esp, 0Ch
1003ef12  pop ebp
1003ef13  retn
loc_100480D0:
100480d0  mov eax, offset stru_102972D4
100480d5  jmp __CxxFrameHandler3

; ChatSpy_PopOldestHudMessage
ChatSpy_PopOldestHudMessage:
1003ef20  push ebx
1003ef21  push edi
1003ef22  push esi
1003ef23  mov esi, ecx
1003ef25  mov eax, [ecx+8]
1003ef28  dec eax
1003ef29  and eax, [ecx+0Ch]
1003ef2c  mov ecx, [ecx+4]
1003ef2f  mov edi, [ecx+eax*4]
1003ef32  mov eax, [edi+2Ch]
1003ef35  cmp eax, 10h
1003ef38  jb short loc_1003EF66
1003ef3a  mov ecx, [edi+18h]
1003ef3d  lea ebx, [eax+1]
1003ef40  cmp ebx, 1000h
1003ef46  jb short loc_1003EF5C
1003ef48  mov edx, [ecx-4]
1003ef4b  add ecx, 0FFFFFFFCh
1003ef4e  sub ecx, edx
1003ef50  cmp ecx, 20h ; ' '
1003ef53  jnb short loc_1003EFCF
1003ef55  add eax, 24h ; '$'
1003ef58  mov ebx, eax
1003ef5a  mov ecx, edx
loc_1003EF5C:
1003ef5c  push ebx
1003ef5d  push ecx; Block
1003ef5e  call sub_10180976
1003ef63  add esp, 8
loc_1003EF66:
1003ef66  mov dword ptr [edi+28h], 0
1003ef6d  mov dword ptr [edi+2Ch], 0Fh
1003ef74  mov byte ptr [edi+18h], 0
1003ef78  mov eax, [edi+14h]
1003ef7b  cmp eax, 10h
1003ef7e  jb short loc_1003EFAB
1003ef80  mov ecx, [edi]
1003ef82  lea ebx, [eax+1]
1003ef85  cmp ebx, 1000h
1003ef8b  jb short loc_1003EFA1
1003ef8d  mov edx, [ecx-4]
1003ef90  add ecx, 0FFFFFFFCh
1003ef93  sub ecx, edx
1003ef95  cmp ecx, 20h ; ' '
1003ef98  jnb short loc_1003EFCF
1003ef9a  add eax, 24h ; '$'
1003ef9d  mov ebx, eax
1003ef9f  mov ecx, edx
loc_1003EFA1:
1003efa1  push ebx
1003efa2  push ecx; Block
1003efa3  call sub_10180976
1003efa8  add esp, 8
loc_1003EFAB:
1003efab  mov dword ptr [edi+10h], 0
1003efb2  mov dword ptr [edi+14h], 0Fh
1003efb9  mov byte ptr [edi], 0
1003efbc  mov eax, [esi+0Ch]
1003efbf  inc eax
1003efc0  xor ecx, ecx
1003efc2  dec dword ptr [esi+10h]
1003efc5  cmovnz ecx, eax
1003efc8  mov [esi+0Ch], ecx
1003efcb  pop esi
1003efcc  pop edi
1003efcd  pop ebx
1003efce  retn
loc_1003EFCF:
1003efcf  ud2

; ChatSpy_HasMatchingHudMessage
ChatSpy_HasMatchingHudMessage:
1003f080  push ebp; Compare normalized (name, text) pair against normal-HUD cache. No userid, sequence, or time limit in this comparator. Return value consumed as AL bool.
1003f081  mov ebp, esp
1003f083  push ebx
1003f084  push edi
1003f085  push esi
1003f086  sub esp, 5Ch
1003f089  mov [ebp+var_24], ecx
1003f08c  mov esi, [ebp+arg_4]
1003f08f  mov edx, [ebp+arg_0]
1003f092  mov [ebp+var_1C], esp
1003f095  mov [ebp+var_10], 0FFFFFFFFh
1003f09c  lea eax, [ebp+var_18]
1003f09f  mov [ebp+var_14], offset loc_100480E0
1003f0a6  mov ecx, large fs:0
1003f0ad  mov [ebp+var_18], ecx
1003f0b0  mov large fs:0, eax
1003f0b6  lea ecx, [ebp+Src]; Src
1003f0b9  mov [ebp+Buf2], ecx
1003f0bc  call ChatSpy_NormalizeText
1003f0c1  mov [ebp+var_10], 0
1003f0c8  lea ecx, [ebp+Block]; Src
1003f0cb  mov [ebp+var_30], ecx
1003f0ce  mov edx, esi
1003f0d0  call ChatSpy_NormalizeText
1003f0d5  mov esi, [ebp+var_24]
1003f0d8  mov edi, [esi+48h]
1003f0db  test edi, edi
1003f0dd  jz loc_1003F1F4
1003f0e3  mov eax, [esi+38h]
1003f0e6  mov eax, [eax]
1003f0e8  cmp [ebp+var_3C], 10h
1003f0ec  jb short loc_1003F0F4
1003f0ee  mov ecx, [ebp+Src]
1003f0f1  mov [ebp+Buf2], ecx
loc_1003F0F4:
1003f0f4  mov edx, [eax+8]
1003f0f7  mov ecx, [ebp+var_40]
1003f0fa  mov [ebp+Size], ecx
1003f0fd  mov ebx, [ebp+var_58]
1003f100  mov ecx, [ebp+var_54]
1003f103  mov [ebp+var_20], ecx
1003f106  cmp ecx, 10h
1003f109  jb short loc_1003F111
1003f10b  mov ecx, [ebp+Block]
1003f10e  mov [ebp+var_30], ecx
loc_1003F111:
1003f111  mov esi, [esi+44h]
1003f114  mov ecx, [eax+4]
1003f117  dec edx
1003f118  cmp [ebp+Size], 0
1003f11c  mov [ebp+var_24], edx
1003f11f  mov [ebp+var_2C], ecx
1003f122  jz loc_1003F213
1003f128  test ebx, ebx
1003f12a  jz loc_1003F1D1
1003f130  mov [ebp+var_38], ebx
1003f133  jmp short loc_1003F161
loc_1003F140:
1003f140  push [ebp+Size]; Size
1003f143  push [ebp+Buf2]; Buf2
1003f146  push eax; Buf1
1003f147  call memcmp
1003f14c  add esp, 0Ch
1003f14f  test eax, eax
1003f151  mov edx, [ebp+var_24]
1003f154  mov ecx, [ebp+var_2C]
1003f157  jz short loc_1003F17C
loc_1003F159:
1003f159  inc esi
1003f15a  dec edi
1003f15b  jz loc_1003F281
loc_1003F161:
1003f161  mov eax, edx
1003f163  and eax, esi
1003f165  mov ebx, [ecx+eax*4]
1003f168  mov eax, [ebp+Size]
1003f16b  cmp [ebx+10h], eax
1003f16e  jnz short loc_1003F159
1003f170  cmp dword ptr [ebx+14h], 10h
1003f174  mov eax, ebx
1003f176  jb short loc_1003F140
1003f178  mov eax, [ebx]
1003f17a  jmp short loc_1003F140
loc_1003F17C:
1003f17c  mov eax, [ebp+var_38]
1003f17f  cmp [ebx+28h], eax
1003f182  jnz short loc_1003F159
1003f184  cmp dword ptr [ebx+2Ch], 10h
1003f188  jb short loc_1003F18F
1003f18a  mov ebx, [ebx+18h]
1003f18d  jmp short loc_1003F192
loc_1003F18F:
1003f18f  add ebx, 18h
loc_1003F192:
1003f192  push [ebp+var_38]; Size
1003f195  push [ebp+var_30]; Buf2
1003f198  push ebx; Buf1
1003f199  call memcmp
1003f19e  add esp, 0Ch
1003f1a1  test eax, eax
1003f1a3  mov edx, [ebp+var_24]
1003f1a6  mov ecx, [ebp+var_2C]
1003f1a9  jnz short loc_1003F159
1003f1ab  jmp loc_1003F237
loc_1003F1B0:
1003f1b0  push [ebp+Size]; Size
1003f1b3  push [ebp+Buf2]; Buf2
1003f1b6  push eax; Buf1
1003f1b7  call memcmp
1003f1bc  add esp, 0Ch
1003f1bf  test eax, eax
1003f1c1  mov edx, [ebp+var_24]
1003f1c4  mov ecx, [ebp+var_2C]
1003f1c7  jz short loc_1003F1EC
loc_1003F1C9:
1003f1c9  inc esi
1003f1ca  dec edi
1003f1cb  jz loc_1003F281
loc_1003F1D1:
1003f1d1  mov eax, edx
1003f1d3  and eax, esi
1003f1d5  mov ebx, [ecx+eax*4]
1003f1d8  mov eax, [ebp+Size]
1003f1db  cmp [ebx+10h], eax
1003f1de  jnz short loc_1003F1C9
1003f1e0  cmp dword ptr [ebx+14h], 10h
1003f1e4  mov eax, ebx
1003f1e6  jb short loc_1003F1B0
1003f1e8  mov eax, [ebx]
1003f1ea  jmp short loc_1003F1B0
loc_1003F1EC:
1003f1ec  cmp dword ptr [ebx+28h], 0
1003f1f0  jnz short loc_1003F1C9
1003f1f2  jmp short loc_1003F237
loc_1003F1F4:
1003f1f4  mov esi, [ebp+var_54]
1003f1f7  xor ebx, ebx
1003f1f9  cmp esi, 10h
1003f1fc  jnb loc_1003F28F
loc_1003F202:
1003f202  mov eax, [ebp+var_3C]
1003f205  cmp eax, 10h
1003f208  jnb loc_1003F2C3
1003f20e  jmp loc_1003F2EF
loc_1003F213:
1003f213  test ebx, ebx
1003f215  jnz short loc_1003F264
1003f217  jmp short loc_1003F224
loc_1003F220:
1003f220  inc esi
1003f221  dec edi
1003f222  jz short loc_1003F281
loc_1003F224:
1003f224  mov eax, edx
1003f226  and eax, esi
1003f228  mov eax, [ecx+eax*4]
1003f22b  cmp dword ptr [eax+10h], 0
1003f22f  jnz short loc_1003F220
1003f231  cmp dword ptr [eax+28h], 0
1003f235  jnz short loc_1003F220
loc_1003F237:
1003f237  mov bl, 1
1003f239  mov esi, [ebp+var_20]
1003f23c  cmp esi, 10h
1003f23f  jb short loc_1003F202
1003f241  jmp short loc_1003F28F
loc_1003F243:
1003f243  mov eax, [eax+18h]
loc_1003F246:
1003f246  push ebx; Size
1003f247  push [ebp+var_30]; Buf2
1003f24a  push eax; Buf1
1003f24b  call memcmp
1003f250  add esp, 0Ch
1003f253  test eax, eax
1003f255  mov edx, [ebp+var_24]
1003f258  mov ecx, [ebp+var_2C]
1003f25b  jz short loc_1003F237
1003f25d  nop dword ptr [eax]
loc_1003F260:
1003f260  inc esi
1003f261  dec edi
1003f262  jz short loc_1003F281
loc_1003F264:
1003f264  mov eax, edx
1003f266  and eax, esi
1003f268  mov eax, [ecx+eax*4]
1003f26b  cmp dword ptr [eax+10h], 0
1003f26f  jnz short loc_1003F260
1003f271  cmp [eax+28h], ebx
1003f274  jnz short loc_1003F260
1003f276  cmp dword ptr [eax+2Ch], 10h
1003f27a  jnb short loc_1003F243
1003f27c  add eax, 18h
1003f27f  jmp short loc_1003F246
loc_1003F281:
1003f281  xor ebx, ebx
1003f283  mov esi, [ebp+var_20]
1003f286  cmp esi, 10h
1003f289  jb loc_1003F202
loc_1003F28F:
1003f28f  mov eax, [ebp+Block]
1003f292  lea edx, [esi+1]
1003f295  cmp edx, 1000h
1003f29b  jb short loc_1003F2B1
1003f29d  mov ecx, [eax-4]
1003f2a0  add eax, 0FFFFFFFCh
1003f2a3  sub eax, ecx
1003f2a5  cmp eax, 20h ; ' '
1003f2a8  jnb short loc_1003F304
1003f2aa  add esi, 24h ; '$'
1003f2ad  mov edx, esi
1003f2af  mov eax, ecx
loc_1003F2B1:
1003f2b1  push edx
1003f2b2  push eax; Block
1003f2b3  call sub_10180976
1003f2b8  add esp, 8
1003f2bb  mov eax, [ebp+var_3C]
1003f2be  cmp eax, 10h
1003f2c1  jb short loc_1003F2EF
loc_1003F2C3:
1003f2c3  mov ecx, [ebp+Src]
1003f2c6  lea esi, [eax+1]
1003f2c9  cmp esi, 1000h
1003f2cf  jb short loc_1003F2E5
1003f2d1  mov edx, [ecx-4]
1003f2d4  add ecx, 0FFFFFFFCh
1003f2d7  sub ecx, edx
1003f2d9  cmp ecx, 20h ; ' '
1003f2dc  jnb short loc_1003F304
1003f2de  add eax, 24h ; '$'
1003f2e1  mov esi, eax
1003f2e3  mov ecx, edx
loc_1003F2E5:
1003f2e5  push esi
1003f2e6  push ecx; Block
1003f2e7  call sub_10180976
1003f2ec  add esp, 8
loc_1003F2EF:
1003f2ef  mov eax, [ebp+var_18]
1003f2f2  mov large fs:0, eax
1003f2f8  mov eax, ebx
1003f2fa  add esp, 5Ch
1003f2fd  pop esi
1003f2fe  pop edi
1003f2ff  pop ebx
1003f300  pop ebp
1003f301  retn 8
loc_1003F304:
1003f304  ud2
loc_1003F310:
1003f310  push ebp
1003f311  sub esp, 0Ch
1003f314  add ebp, 0Ch
1003f317  lea ecx, [ebp+Src]
1003f31a  call sub_1000BC90
1003f31f  add esp, 0Ch
1003f322  pop ebp
1003f323  retn
loc_100480E0:
100480e0  mov eax, offset stru_10297300
100480e5  jmp __CxxFrameHandler3

; ChatSpy_InstallChatPrintfHook
ChatSpy_InstallChatPrintfHook:
10171b90  push offset ChatSpy_OriginalChatPrintf; int
10171b95  push offset ChatSpy_HookChatPrintf; LPCVOID
10171b9a  push dword_103EC658; lpAddress
10171ba0  call sub_1017E990
10171ba5  retn

; ChatSpy_HookChatPrintf
ChatSpy_HookChatPrintf:
10171bb0  push ebp; ChatPrintf detour: format into 2048-byte buffer, remove control bytes 0x01..0x07, split first ASCII " : ", remember normalized pair, call original. Decompiled varargs signature is uncertain.
10171bb1  mov ebp, esp
10171bb3  push ebx
10171bb4  push edi
10171bb5  push esi
10171bb6  sub esp, 86Ch
10171bbc  mov edi, [ebp+Format]
10171bbf  mov [ebp+var_1C], esp
10171bc2  mov [ebp+var_10], 0FFFFFFFFh
10171bc9  lea eax, [ebp+var_18]
10171bcc  mov [ebp+var_14], offset loc_10176A40
10171bd3  mov ecx, large fs:0
10171bda  mov [ebp+var_18], ecx
10171bdd  mov large fs:0, eax
10171be3  test edi, edi
10171be5  jz loc_1017228C
10171beb  lea ebx, [ebp+Buffer]
10171bf1  push 800h; Size
10171bf6  push 0; Val
10171bf8  push ebx; void *
10171bf9  call memset
10171bfe  add esp, 0Ch
10171c01  lea esi, [ebp+ArgList]
10171c04  mov [ebp+var_78], esi
10171c07  call sub_1001A9D0
10171c0c  mov ecx, [eax]
10171c0e  or ecx, 2
10171c11  push esi; ArgList
10171c12  push 0; Locale
10171c14  push edi; Format
10171c15  push 800h; BufferCount
10171c1a  push ebx; Buffer
10171c1b  push dword ptr [eax+4]
10171c1e  push ecx; Options
10171c1f  call ds:__stdio_common_vsprintf; api-ms-win-crt-stdio-l1-1-0
10171c25  add esp, 1Ch
10171c28  vxorps xmm0, xmm0, xmm0
10171c2c  vmovsd qword ptr [ebp+Size], xmm0
10171c31  vxorps xmm0, xmm0, xmm0
10171c35  vmovups xmmword ptr [ebp+Src], xmm0
10171c3a  push ebx; Str
10171c3b  call strlen
10171c40  add esp, 4
10171c43  test eax, eax
10171c45  js loc_1017232C
10171c4b  mov esi, eax
10171c4d  cmp eax, 0Fh
10171c50  ja short loc_10171C71
10171c52  mov [ebp+Size], esi
10171c55  mov [ebp+Size+4], 0Fh
10171c5c  lea eax, [ebp+Src]
10171c5f  push esi; Size
10171c60  push ebx; Src
10171c61  push eax; void *
10171c62  call memcpy
10171c67  add esp, 0Ch
10171c6a  mov byte ptr [ebp+esi+Src], 0
10171c6f  jmp short loc_10171CCB
loc_10171C71:
10171c71  mov eax, esi
10171c73  or eax, 0Fh
10171c76  cmp eax, 17h
10171c79  mov ebx, 16h
10171c7e  cmovnb ebx, eax
10171c81  cmp eax, 0FFFh
10171c86  jb short loc_10171C9F
10171c88  lea eax, [ebx+24h]
10171c8b  push eax; Size
10171c8c  call sub_10180946
10171c91  add esp, 4
10171c94  lea edi, [eax+23h]
10171c97  and edi, 0FFFFFFE0h
10171c9a  mov [edi-4], eax
10171c9d  jmp short loc_10171CAD
loc_10171C9F:
10171c9f  lea eax, [ebx+1]
10171ca2  push eax; Size
10171ca3  call sub_10180946
10171ca8  add esp, 4
10171cab  mov edi, eax
loc_10171CAD:
10171cad  mov [ebp+Src], edi
10171cb0  mov [ebp+Size], esi
10171cb3  mov [ebp+Size+4], ebx
10171cb6  push esi; Size
10171cb7  lea eax, [ebp+Buffer]
10171cbd  push eax; Src
10171cbe  push edi; void *
10171cbf  call memcpy
10171cc4  add esp, 0Ch
10171cc7  mov byte ptr [edi+esi], 0
loc_10171CCB:
10171ccb  mov edx, [ebp+Size+4]
10171cce  cmp edx, 10h
10171cd1  mov esi, [ebp+Src]
10171cd4  lea ebx, [ebp+Src]
10171cd7  cmovnb ebx, esi
10171cda  mov edi, [ebp+Size]
10171cdd  lea eax, [ebx+edi]
10171ce0  mov [ebp+var_20], eax
10171ce3  mov eax, ebx
10171ce5  test edi, edi
10171ce7  jz short loc_10171D0A
10171ce9  xor eax, eax
10171ceb  nop dword ptr [eax+eax+00h]
loc_10171CF0:
10171cf0  movzx ecx, byte ptr [ebx+eax]
10171cf4  dec cl
10171cf6  cmp cl, 7
10171cf9  jb short loc_10171D08
10171cfb  inc eax
10171cfc  cmp edi, eax
10171cfe  jnz short loc_10171CF0
10171d00  mov eax, [ebp+var_20]
10171d03  lea ebx, [ebp+Src]
10171d06  jmp short loc_10171D51
loc_10171D08:
10171d08  add eax, ebx
loc_10171D0A:
10171d0a  mov [ebp+var_28], edx
10171d0d  mov ecx, [ebp+var_20]
10171d10  cmp eax, ecx
10171d12  setz byte ptr [ebp+var_24]
10171d16  lea edx, [eax+1]
10171d19  cmp edx, ecx
10171d1b  setz ch
10171d1e  or ch, byte ptr [ebp+var_24]
10171d21  jz short loc_10171D2B
10171d23  lea ebx, [ebp+Src]
10171d26  mov edx, [ebp+var_28]
10171d29  jmp short loc_10171D51
loc_10171D2B:
10171d2b  add ebx, edi
10171d2d  mov esi, eax
10171d2f  not esi
10171d31  add esi, ebx
10171d33  sub ebx, eax
10171d35  add ebx, 0FFFFFFFEh
10171d38  and esi, 3
10171d3b  jnz short loc_10171DA4
loc_10171D3D:
10171d3d  cmp ebx, 3
10171d40  lea ebx, [ebp+Src]
10171d43  mov esi, [ebp+var_20]
10171d46  jnb short loc_10171DC7
loc_10171D48:
10171d48  mov edx, [ebp+Size+4]
10171d4b  mov esi, [ebp+Src]
10171d4e  mov edi, [ebp+Size]
loc_10171D51:
10171d51  cmp edx, 10h
10171d54  cmovb esi, ebx
10171d57  mov ecx, eax
10171d59  sub ecx, esi
10171d5b  mov esi, [ebp+var_20]
10171d5e  sub esi, eax
10171d60  mov edx, edi
10171d62  sub edx, ecx
10171d64  cmp edx, esi
10171d66  cmovnb edx, esi
10171d69  sub edi, edx
10171d6b  mov esi, edi
10171d6d  sub esi, ecx
10171d6f  inc esi
10171d70  add edx, eax
10171d72  push esi; Size
10171d73  push edx; Src
10171d74  push eax; void *
10171d75  call memmove
10171d7a  add esp, 0Ch
10171d7d  mov [ebp+Size], edi
10171d80  mov eax, [ebp+Size+4]
10171d83  cmp edi, 3
10171d86  jnb loc_10171E08
10171d8c  mov edi, [ebp+Format]
10171d8f  cmp eax, 10h
10171d92  jnb loc_10172255
10171d98  jmp loc_10172285
loc_10171DA0:
10171da0  inc edx
10171da1  dec esi
10171da2  jz short loc_10171D3D
loc_10171DA4:
10171da4  movzx ecx, byte ptr [edx]
10171da7  mov ch, cl
10171da9  dec ch
10171dab  cmp ch, 7
10171dae  jb short loc_10171DA0
10171db0  mov [eax], cl
10171db2  inc eax
10171db3  jmp short loc_10171DA0
loc_10171DC0:
10171dc0  add edx, 4
10171dc3  cmp edx, esi
10171dc5  jz short loc_10171D48
loc_10171DC7:
10171dc7  movzx ecx, byte ptr [edx]
10171dca  mov ch, cl
10171dcc  dec ch
10171dce  cmp ch, 7
10171dd1  jb short loc_10171DD6
10171dd3  mov [eax], cl
10171dd5  inc eax
loc_10171DD6:
10171dd6  movzx ecx, byte ptr [edx+1]
10171dda  mov ch, cl
10171ddc  dec ch
10171dde  cmp ch, 7
10171de1  jb short loc_10171DE6
10171de3  mov [eax], cl
10171de5  inc eax
loc_10171DE6:
10171de6  movzx ecx, byte ptr [edx+2]
10171dea  mov ch, cl
10171dec  dec ch
10171dee  cmp ch, 7
10171df1  jb short loc_10171DF6
10171df3  mov [eax], cl
10171df5  inc eax
loc_10171DF6:
10171df6  movzx ecx, byte ptr [edx+3]
10171dfa  mov ch, cl
10171dfc  dec ch
10171dfe  cmp ch, 7
10171e01  jb short loc_10171DC0
10171e03  mov [eax], cl
10171e05  inc eax
10171e06  jmp short loc_10171DC0
loc_10171E08:
10171e08  cmp eax, 10h
10171e0b  jb short loc_10171E10
10171e0d  mov ebx, [ebp+Src]
loc_10171E10:
10171e10  add edi, ebx
10171e12  push 3
10171e14  push offset unk_10290912
10171e19  push edi
10171e1a  push ebx
10171e1b  call sub_101814C0
10171e20  mov esi, eax
10171e22  cmp eax, edi
10171e24  setz al
10171e27  sub esi, ebx
10171e29  cmp esi, 0FFFFFFFFh
10171e2c  setz cl
10171e2f  or cl, al
10171e31  mov edi, [ebp+Format]
10171e34  jnz loc_1017224D
10171e3a  vxorps xmm0, xmm0, xmm0
10171e3e  vmovsd [ebp+var_60], xmm0
10171e43  vxorps xmm0, xmm0, xmm0
10171e47  vmovups xmmword ptr [ebp+var_70], xmm0
10171e4c  mov edi, [ebp+Size]
10171e4f  cmp edi, esi
10171e51  mov ebx, esi
10171e53  cmovb ebx, edi
10171e56  cmp [ebp+Size+4], 10h
10171e5a  jb short loc_10171E61
10171e5c  mov ecx, [ebp+Src]
10171e5f  jmp short loc_10171E64
loc_10171E61:
10171e61  lea ecx, [ebp+Src]
loc_10171E64:
10171e64  test ebx, ebx
10171e66  js loc_10172333
10171e6c  cmp ebx, 0Fh
10171e6f  ja short loc_10171E90
10171e71  mov dword ptr [ebp+var_60], ebx
10171e74  mov dword ptr [ebp+var_60+4], 0Fh
10171e7b  lea eax, [ebp+var_70]
10171e7e  push ebx; Size
10171e7f  push ecx; Src
10171e80  push eax; void *
10171e81  call memcpy
10171e86  add esp, 0Ch
10171e89  mov byte ptr [ebp+ebx+var_70], 0
10171e8e  jmp short loc_10171F08
loc_10171E90:
10171e90  mov [ebp+var_28], ecx
10171e93  mov [ebp+var_20], esi
10171e96  mov [ebp+var_24], ebx
10171e99  mov eax, ebx
10171e9b  or eax, 0Fh
10171e9e  cmp eax, 17h
10171ea1  mov ecx, 16h
10171ea6  cmovnb ecx, eax
10171ea9  mov [ebp+var_10], 0
10171eb0  cmp eax, 0FFFh
10171eb5  mov [ebp+var_74], ecx
10171eb8  jb short loc_10171ED3
10171eba  lea eax, [ecx+24h]
10171ebd  push eax; Size
10171ebe  call sub_10180946
10171ec3  add esp, 4
10171ec6  mov esi, eax
10171ec8  add esi, 23h ; '#'
10171ecb  and esi, 0FFFFFFE0h
10171ece  mov [esi-4], eax
10171ed1  jmp short loc_10171EE1
loc_10171ED3:
10171ed3  lea eax, [ecx+1]
10171ed6  push eax; Size
10171ed7  call sub_10180946
10171edc  add esp, 4
10171edf  mov esi, eax
loc_10171EE1:
10171ee1  mov edi, [ebp+var_24]
10171ee4  mov eax, [ebp+var_28]
10171ee7  mov [ebp+var_70], esi
10171eea  mov dword ptr [ebp+var_60], edi
10171eed  mov ecx, [ebp+var_74]
10171ef0  mov dword ptr [ebp+var_60+4], ecx
10171ef3  push edi; Size
10171ef4  push eax; Src
10171ef5  push esi; void *
10171ef6  call memcpy
10171efb  add esp, 0Ch
10171efe  mov byte ptr [esi+edi], 0
10171f02  mov edi, [ebp+Size]
10171f05  mov esi, [ebp+var_20]
loc_10171F08:
10171f08  add esi, 3
10171f0b  vxorps xmm0, xmm0, xmm0
10171f0f  vmovsd [ebp+var_48], xmm0
10171f14  vxorps xmm0, xmm0, xmm0
10171f18  vmovups xmmword ptr [ebp+Block], xmm0
10171f1d  cmp edi, esi
10171f1f  jb loc_1017233F
10171f25  cmp [ebp+Size+4], 10h
10171f29  jb short loc_10171F30
10171f2b  mov ecx, [ebp+Src]
10171f2e  jmp short loc_10171F33
loc_10171F30:
10171f30  lea ecx, [ebp+Src]
loc_10171F33:
10171f33  sub edi, esi
10171f35  js loc_1017234B
10171f3b  add ecx, esi
10171f3d  cmp edi, 0Fh
10171f40  ja short loc_10171F61
10171f42  mov dword ptr [ebp+var_48], edi
10171f45  mov dword ptr [ebp+var_48+4], 0Fh
10171f4c  lea eax, [ebp+Block]
10171f4f  push edi; Size
10171f50  push ecx; Src
10171f51  push eax; void *
10171f52  call memcpy
10171f57  add esp, 0Ch
10171f5a  mov byte ptr [ebp+edi+Block], 0
10171f5f  jmp short loc_10171FD0
loc_10171F61:
10171f61  mov [ebp+var_24], ecx
10171f64  mov [ebp+var_20], edi
10171f67  mov eax, edi
10171f69  or eax, 0Fh
10171f6c  cmp eax, 17h
10171f6f  mov ecx, 16h
10171f74  cmovnb ecx, eax
10171f77  mov [ebp+var_10], 1
10171f7e  cmp eax, 0FFFh
10171f83  mov [ebp+var_28], ecx
10171f86  jb short loc_10171FA1
10171f88  lea eax, [ecx+24h]
10171f8b  push eax; Size
10171f8c  call sub_10180946
10171f91  add esp, 4
10171f94  mov esi, eax
10171f96  add esi, 23h ; '#'
10171f99  and esi, 0FFFFFFE0h
10171f9c  mov [esi-4], eax
10171f9f  jmp short loc_10171FAF
loc_10171FA1:
10171fa1  lea eax, [ecx+1]
10171fa4  push eax; Size
10171fa5  call sub_10180946
10171faa  add esp, 4
10171fad  mov esi, eax
loc_10171FAF:
10171faf  mov edi, [ebp+var_20]
10171fb2  mov eax, [ebp+var_24]
10171fb5  mov [ebp+Block], esi
10171fb8  mov dword ptr [ebp+var_48], edi
10171fbb  mov ecx, [ebp+var_28]
10171fbe  mov dword ptr [ebp+var_48+4], ecx
10171fc1  push edi; Size
10171fc2  push eax; Src
10171fc3  push esi; void *
10171fc4  call memcpy
10171fc9  add esp, 0Ch
10171fcc  mov byte ptr [esi+edi], 0
loc_10171FD0:
10171fd0  mov ecx, dword ptr [ebp+var_60+4]
10171fd3  cmp ecx, 10h
10171fd6  mov ebx, [ebp+var_70]
10171fd9  lea edi, [ebp+var_70]
10171fdc  cmovnb edi, ebx
10171fdf  mov esi, dword ptr [ebp+var_60]
10171fe2  lea eax, [edi+esi]
10171fe5  mov [ebp+var_20], eax
10171fe8  mov eax, edi
10171fea  test esi, esi
10171fec  jz short loc_10172009
10171fee  xor eax, eax
loc_10171FF0:
10171ff0  movzx edx, byte ptr [edi+eax]
10171ff4  dec dl
10171ff6  cmp dl, 7
10171ff9  jb short loc_10172007
10171ffb  inc eax
10171ffc  cmp esi, eax
10171ffe  jnz short loc_10171FF0
10172000  mov edi, [ebp+var_20]
10172003  mov eax, edi
10172005  jmp short loc_10172055
loc_10172007:
10172007  add eax, edi
loc_10172009:
10172009  mov [ebp+var_28], ecx
1017200c  mov ecx, [ebp+var_20]
1017200f  cmp eax, ecx
10172011  setz byte ptr [ebp+var_24]
10172015  lea edx, [eax+1]
10172018  cmp edx, ecx
1017201a  setz cl
1017201d  or cl, byte ptr [ebp+var_24]
10172020  jz short loc_1017202A
10172022  mov edi, [ebp+var_20]
10172025  mov ecx, [ebp+var_28]
10172028  jmp short loc_10172055
loc_1017202A:
1017202a  add edi, esi
1017202c  mov esi, eax
1017202e  not esi
10172030  add esi, edi
10172032  sub edi, eax
10172034  add edi, 0FFFFFFFEh
10172037  and esi, 3
1017203a  jnz loc_101720D8
loc_10172040:
10172040  cmp edi, 3
10172043  mov edi, [ebp+var_20]
10172046  jnb loc_101720FB
loc_1017204C:
1017204c  mov ecx, dword ptr [ebp+var_60+4]
1017204f  mov ebx, [ebp+var_70]
10172052  mov esi, dword ptr [ebp+var_60]
loc_10172055:
10172055  cmp ecx, 10h
10172058  lea ecx, [ebp+var_70]
1017205b  cmovb ebx, ecx
1017205e  mov ecx, eax
10172060  sub ecx, ebx
10172062  sub edi, eax
10172064  mov edx, esi
10172066  sub edx, ecx
10172068  cmp edx, edi
1017206a  cmovnb edx, edi
1017206d  sub esi, edx
1017206f  mov edi, esi
10172071  sub edi, ecx
10172073  inc edi
10172074  add edx, eax
10172076  push edi; Size
10172077  push edx; Src
10172078  push eax; void *
10172079  call memmove
1017207e  add esp, 0Ch
10172081  mov dword ptr [ebp+var_60], esi
10172084  mov edx, dword ptr [ebp+var_48+4]
10172087  cmp edx, 10h
1017208a  mov edi, [ebp+Block]
1017208d  lea ebx, [ebp+Block]
10172090  cmovnb ebx, edi
10172093  mov esi, dword ptr [ebp+var_48]
10172096  lea eax, [ebx+esi]
10172099  mov [ebp+var_20], eax
1017209c  mov eax, ebx
1017209e  test esi, esi
101720a0  jz loc_1017213E
101720a6  xor eax, eax
101720a8  nop dword ptr [eax+eax+00000000h]
loc_101720B0:
101720b0  movzx ecx, byte ptr [ebx+eax]
101720b4  dec cl
101720b6  cmp cl, 7
101720b9  jb loc_1017213C
101720bf  inc eax
101720c0  cmp esi, eax
101720c2  jnz short loc_101720B0
101720c4  mov eax, [ebp+var_20]
101720c7  lea ebx, [ebp+Block]
101720ca  jmp loc_10172194
loc_101720D0:
101720d0  inc edx
101720d1  dec esi
101720d2  jz loc_10172040
loc_101720D8:
101720d8  movzx ebx, byte ptr [edx]
101720db  mov ecx, ebx
101720dd  dec cl
101720df  cmp cl, 7
101720e2  jb short loc_101720D0
101720e4  mov [eax], bl
101720e6  inc eax
101720e7  jmp short loc_101720D0
loc_101720F0:
101720f0  add edx, 4
101720f3  cmp edx, edi
101720f5  jz loc_1017204C
loc_101720FB:
101720fb  movzx ebx, byte ptr [edx]
101720fe  mov ecx, ebx
10172100  dec cl
10172102  cmp cl, 7
10172105  jb short loc_1017210A
10172107  mov [eax], bl
10172109  inc eax
loc_1017210A:
1017210a  movzx ebx, byte ptr [edx+1]
1017210e  mov ecx, ebx
10172110  dec cl
10172112  cmp cl, 7
10172115  jb short loc_1017211A
10172117  mov [eax], bl
10172119  inc eax
loc_1017211A:
1017211a  movzx ebx, byte ptr [edx+2]
1017211e  mov ecx, ebx
10172120  dec cl
10172122  cmp cl, 7
10172125  jb short loc_1017212A
10172127  mov [eax], bl
10172129  inc eax
loc_1017212A:
1017212a  movzx ebx, byte ptr [edx+3]
1017212e  mov ecx, ebx
10172130  dec cl
10172132  cmp cl, 7
10172135  jb short loc_101720F0
10172137  mov [eax], bl
10172139  inc eax
1017213a  jmp short loc_101720F0
loc_1017213C:
1017213c  add eax, ebx
loc_1017213E:
1017213e  mov [ebp+var_28], edx
10172141  mov ecx, [ebp+var_20]
10172144  cmp eax, ecx
10172146  setz byte ptr [ebp+var_24]
1017214a  lea edx, [eax+1]
1017214d  cmp edx, ecx
1017214f  setz ch
10172152  or ch, byte ptr [ebp+var_24]
10172155  jz short loc_1017215F
10172157  lea ebx, [ebp+Block]
1017215a  mov edx, [ebp+var_28]
1017215d  jmp short loc_10172194
loc_1017215F:
1017215f  add ebx, esi
10172161  mov esi, eax
10172163  not esi
10172165  add esi, ebx
10172167  sub ebx, eax
10172169  add ebx, 0FFFFFFFEh
1017216c  and esi, 3
1017216f  jnz loc_101722C8
loc_10172175:
10172175  cmp ebx, 3
10172178  lea ebx, [ebp+Block]
1017217b  mov esi, [ebp+var_20]
1017217e  jnb loc_101722EB
loc_10172184:
10172184  mov edx, dword ptr [ebp+var_48+4]
10172187  mov edi, [ebp+Block]
1017218a  mov esi, dword ptr [ebp+var_48]
1017218d  mov [ebp+var_10], 2
loc_10172194:
10172194  cmp edx, 10h
10172197  cmovb edi, ebx
1017219a  mov ecx, eax
1017219c  sub ecx, edi
1017219e  mov edi, [ebp+var_20]
101721a1  sub edi, eax
101721a3  mov edx, esi
101721a5  sub edx, ecx
101721a7  cmp edx, edi
101721a9  cmovnb edx, edi
101721ac  sub esi, edx
101721ae  mov edi, esi
101721b0  sub edi, ecx
101721b2  inc edi
101721b3  add edx, eax
101721b5  push edi; Size
101721b6  push edx; Src
101721b7  push eax; void *
101721b8  call memmove
101721bd  add esp, 0Ch
101721c0  mov dword ptr [ebp+var_48], esi
101721c3  mov ecx, off_1029E01C
101721c9  mov [ebp+var_10], 2
101721d0  push ebx
101721d1  lea eax, [ebp+var_70]
101721d4  push eax
101721d5  call ChatSpy_RememberHudMessage; Normal-HUD dedup cache: normalize sender and text; append pair of 24-byte strings; when count >= 251 pop_front, leaving at most 250 entries.
101721da  mov eax, dword ptr [ebp+var_48+4]
101721dd  cmp eax, 10h
101721e0  mov edi, [ebp+Format]
101721e3  jb short loc_10172215
101721e5  mov ecx, [ebp+Block]
101721e8  lea esi, [eax+1]
101721eb  cmp esi, 1000h
101721f1  jb short loc_1017220B
101721f3  mov edx, [ecx-4]
101721f6  add ecx, 0FFFFFFFCh
101721f9  sub ecx, edx
101721fb  cmp ecx, 20h ; ' '
101721fe  jnb loc_10172331
10172204  add eax, 24h ; '$'
10172207  mov esi, eax
10172209  mov ecx, edx
loc_1017220B:
1017220b  push esi
1017220c  push ecx; Block
1017220d  call sub_10180976
10172212  add esp, 8
loc_10172215:
10172215  mov eax, dword ptr [ebp+var_60+4]
10172218  cmp eax, 10h
1017221b  jb short loc_1017224D
1017221d  mov ecx, [ebp+var_70]
10172220  lea esi, [eax+1]
10172223  cmp esi, 1000h
10172229  jb short loc_10172243
1017222b  mov edx, [ecx-4]
1017222e  add ecx, 0FFFFFFFCh
10172231  sub ecx, edx
10172233  cmp ecx, 20h ; ' '
10172236  jnb loc_10172331
1017223c  add eax, 24h ; '$'
1017223f  mov esi, eax
10172241  mov ecx, edx
loc_10172243:
10172243  push esi
10172244  push ecx; Block
10172245  call sub_10180976
1017224a  add esp, 8
loc_1017224D:
1017224d  mov eax, [ebp+Size+4]
10172250  cmp eax, 10h
10172253  jb short loc_10172285
loc_10172255:
10172255  mov ecx, [ebp+Src]
10172258  lea esi, [eax+1]
1017225b  cmp esi, 1000h
10172261  jb short loc_1017227B
10172263  mov edx, [ecx-4]
10172266  add ecx, 0FFFFFFFCh
10172269  sub ecx, edx
1017226b  cmp ecx, 20h ; ' '
1017226e  jnb loc_10172331
10172274  add eax, 24h ; '$'
10172277  mov esi, eax
10172279  mov ecx, edx
loc_1017227B:
1017227b  push esi
1017227c  push ecx; Block
1017227d  call sub_10180976
10172282  add esp, 8
loc_10172285:
10172285  mov [ebp+var_10], 0FFFFFFFFh
loc_1017228C:
1017228c  push edi
1017228d  push [ebp+arg_C]
10172290  push [ebp+arg_8]
10172293  push [ebp+arg_4]
10172296  push [ebp+arg_0]
10172299  call ChatSpy_OriginalChatPrintf
1017229f  add esp, 14h
101722a2  mov eax, [ebp+var_18]
101722a5  mov large fs:0, eax
101722ab  add esp, 86Ch
101722b1  pop esi
101722b2  pop edi
101722b3  pop ebx
101722b4  pop ebp
101722b5  retn
loc_101722C0:
101722c0  inc edx
101722c1  dec esi
101722c2  jz loc_10172175
loc_101722C8:
101722c8  movzx ecx, byte ptr [edx]
101722cb  mov ch, cl
101722cd  dec ch
101722cf  cmp ch, 7
101722d2  jb short loc_101722C0
101722d4  mov [eax], cl
101722d6  inc eax
101722d7  jmp short loc_101722C0
loc_101722E0:
101722e0  add edx, 4
101722e3  cmp edx, esi
101722e5  jz loc_10172184
loc_101722EB:
101722eb  movzx ecx, byte ptr [edx]
101722ee  mov ch, cl
101722f0  dec ch
101722f2  cmp ch, 7
101722f5  jb short loc_101722FA
101722f7  mov [eax], cl
101722f9  inc eax
loc_101722FA:
101722fa  movzx ecx, byte ptr [edx+1]
101722fe  mov ch, cl
10172300  dec ch
10172302  cmp ch, 7
10172305  jb short loc_1017230A
10172307  mov [eax], cl
10172309  inc eax
loc_1017230A:
1017230a  movzx ecx, byte ptr [edx+2]
1017230e  mov ch, cl
10172310  dec ch
10172312  cmp ch, 7
10172315  jb short loc_1017231A
10172317  mov [eax], cl
10172319  inc eax
loc_1017231A:
1017231a  movzx ecx, byte ptr [edx+3]
1017231e  mov ch, cl
10172320  dec ch
10172322  cmp ch, 7
10172325  jb short loc_101722E0
10172327  mov [eax], cl
10172329  inc eax
1017232a  jmp short loc_101722E0
loc_1017232C:
1017232c  call sub_1000DFF0
loc_10172331:
10172331  ud2
loc_10172333:
10172333  mov [ebp+var_10], 0
1017233a  call sub_1000DFF0
loc_1017233F:
1017233f  mov [ebp+var_10], 1
10172346  call sub_1003CA00
loc_1017234B:
1017234b  mov [ebp+var_10], 1
10172352  call sub_1000DFF0
loc_10172360:
10172360  push ebp
10172361  sub esp, 1Ch
10172364  add ebp, 0Ch
10172367  lea ecx, [ebp+Block]
1017236a  call sub_1000BC90
1017236f  add esp, 1Ch
10172372  pop ebp
10172373  retn
loc_10172380:
10172380  push ebp
10172381  sub esp, 1Ch
10172384  add ebp, 0Ch
10172387  lea ecx, [ebp+var_70]
1017238a  call sub_1000BC90
1017238f  add esp, 1Ch
10172392  pop ebp
10172393  retn
loc_101723A0:
101723a0  push ebp
101723a1  sub esp, 1Ch
101723a4  add ebp, 0Ch
101723a7  lea ecx, [ebp+Src]
101723aa  call sub_1000BC90
101723af  add esp, 1Ch
101723b2  pop ebp
101723b3  retn
loc_10176A40:
10176a40  mov eax, offset stru_1029D020
10176a45  jmp __CxxFrameHandler3

; ChatSpy_PrintToHud
ChatSpy_PrintToHud:
10040410  push ebp
10040411  mov ebp, esp
10040413  push ebx
10040414  push edi
10040415  push esi
10040416  sub esp, 10h
10040419  mov [ebp+var_1C], esp
1004041c  mov [ebp+var_10], 0FFFFFFFFh
10040423  lea eax, [ebp+var_18]
10040426  mov [ebp+var_14], offset loc_10048120
1004042d  mov edx, large fs:0
10040434  mov [ebp+var_18], edx
10040437  mov large fs:0, eax
1004043d  mov eax, dword_103EF154
10040442  mov edx, TlsIndex
10040448  mov esi, large fs:2Ch
1004044f  mov edx, [esi+edx*4]
10040452  cmp eax, [edx+4]
10040458  jg short loc_100404B7
loc_1004045A:
1004045a  cmp dword_103EF150, 0
10040461  jz short loc_100404A4
10040463  mov eax, dword_103EF15C
10040468  mov ecx, TlsIndex
1004046e  mov edx, large fs:2Ch
10040475  mov ecx, [edx+ecx*4]
10040478  cmp eax, [ecx+4]
1004047e  jg short loc_100404F4
loc_10040480:
10040480  mov eax, dword_103EF158
10040485  test eax, eax
10040487  jz short loc_100404A4
10040489  mov ecx, [ebp+arg_0]
1004048c  mov edx, dword_103EF150
10040492  mov [ebp+var_10], 0FFFFFFFFh
10040499  push ecx
1004049a  push 0
1004049c  push 0
1004049e  push edx
1004049f  call eax ; dword_103EF158
100404a1  add esp, 10h
loc_100404A4:
100404a4  mov eax, [ebp+var_18]
100404a7  mov large fs:0, eax
100404ad  add esp, 10h
100404b0  pop esi
100404b1  pop edi
100404b2  pop ebx
100404b3  pop ebp
100404b4  retn 4
loc_100404B7:
100404b7  push offset dword_103EF154
100404bc  mov esi, ecx
100404be  call sub_101809C3
100404c3  add esp, 4
100404c6  cmp dword_103EF154, 0FFFFFFFFh
100404cd  jnz short loc_1004045A
100404cf  mov ecx, esi
100404d1  mov [ebp+var_10], 0
100404d8  call sub_10044DD0
100404dd  mov dword_103EF150, eax
100404e2  push offset dword_103EF154
100404e7  call sub_10180A43
100404ec  add esp, 4
100404ef  jmp loc_1004045A
loc_100404F4:
100404f4  push offset dword_103EF15C
100404f9  call sub_101809C3
100404fe  add esp, 4
10040501  cmp dword_103EF15C, 0FFFFFFFFh
10040508  jnz loc_10040480
1004050e  mov eax, dword_103EC658
10040513  mov dword_103EF158, eax
10040518  push offset dword_103EF15C
1004051d  call sub_10180A43
10040522  add esp, 4
10040525  jmp loc_10040480
loc_10040530:
10040530  push ebp
10040531  sub esp, 10h
10040534  add ebp, 0Ch
10040537  push offset dword_103EF154
1004053c  call sub_10180A15
10040541  add esp, 14h
10040544  pop ebp
10040545  retn
loc_10048120:
10048120  mov eax, offset stru_10297388
10048125  jmp __CxxFrameHandler3

; ChatSpy_PackWideCodeUnits
ChatSpy_PackWideCodeUnits:
1003e910  push ebp; Reads 16-bit words, emits low byte then high byte, stops at first zero BYTE (including high-byte zero). Compatible with reconstructing a raw C string; wchar_t decompiler types do NOT prove that the event text is UTF-16. Not a Unicode conversion.
1003e911  mov ebp, esp
1003e913  push ebx
1003e914  push edi
1003e915  push esi
1003e916  sub esp, 28h
1003e919  mov ecx, [ebp+Src]
1003e91c  mov edi, [ebp+arg_4]
1003e91f  mov [ebp+var_1C], esp
1003e922  mov [ebp+var_10], 0FFFFFFFFh
1003e929  lea eax, [ebp+var_18]
1003e92c  mov [ebp+var_14], offset loc_100480B0
1003e933  mov edx, large fs:0
1003e93a  mov [ebp+var_18], edx
1003e93d  mov large fs:0, eax
1003e943  test edi, edi
1003e945  jz loc_1003EA5D
1003e94b  movzx ebx, word ptr [edi]
1003e94e  test bx, bx
1003e951  jz loc_1003EA5D
1003e957  vxorps xmm0, xmm0, xmm0
1003e95b  vmovups xmmword ptr [ecx], xmm0
1003e95f  mov dword ptr [ecx+10h], 0
1003e966  mov dword ptr [ecx+14h], 0Fh
1003e96d  add edi, 2
1003e970  jmp short loc_1003E99A
loc_1003E980:
1003e980  mov [edx+eax], bh
1003e983  mov byte ptr [edx+eax+1], 0
loc_1003E988:
1003e988  movzx ebx, word ptr [edi]
1003e98b  add edi, 2
1003e98e  test bx, bx
1003e991  mov ecx, [ebp+Src]; Src
1003e994  jz loc_1003EA73
loc_1003E99A:
1003e99a  test bl, bl
1003e99c  jz loc_1003EA73
1003e9a2  mov eax, [ecx+10h]
1003e9a5  mov esi, [ecx+14h]
1003e9a8  cmp eax, esi
1003e9aa  mov [ebp+var_28], edi
1003e9ad  jnb short loc_1003E9D0
1003e9af  lea edx, [eax+1]
1003e9b2  mov [ecx+10h], edx
1003e9b5  mov edx, ecx
1003e9b7  cmp esi, 10h
1003e9ba  jb short loc_1003E9BE
1003e9bc  mov edx, [ecx]
loc_1003E9BE:
1003e9be  mov [edx+eax], bl
1003e9c1  mov byte ptr [edx+eax+1], 0
1003e9c6  jmp short loc_1003EA00
loc_1003E9D0:
1003e9d0  mov [ebp+var_10], 0
1003e9d7  movzx eax, [ebp+var_20]
1003e9db  mov byte ptr [esp+34h+var_30], al; int
1003e9df  movsx eax, bl
1003e9e2  mov dword ptr [esp+34h+var_2C], eax; char
1003e9e6  mov [esp+34h+var_34], 1; int
1003e9ed  mov [ebp+var_22], bx
1003e9f1  call sub_1000DEB0
1003e9f6  sub esp, 0Ch
1003e9f9  movzx ebx, [ebp+var_22]
1003e9fd  mov edi, [ebp+var_28]
loc_1003EA00:
1003ea00  movzx eax, bx
1003ea03  cmp eax, 100h
1003ea08  mov ecx, [ebp+Src]; Src
1003ea0b  jb short loc_1003EA73
1003ea0d  mov eax, [ecx+10h]
1003ea10  mov esi, [ecx+14h]
1003ea13  cmp eax, esi
1003ea15  jnb short loc_1003EA30
1003ea17  lea edx, [eax+1]
1003ea1a  mov [ecx+10h], edx
1003ea1d  mov edx, ecx
1003ea1f  cmp esi, 10h
1003ea22  jb loc_1003E980
1003ea28  mov edx, [ecx]
1003ea2a  jmp loc_1003E980
loc_1003EA30:
1003ea30  mov [ebp+var_10], 0
1003ea37  movzx eax, [ebp+var_20]
1003ea3b  mov byte ptr [esp+34h+var_30], al; int
1003ea3f  movsx eax, bh
1003ea42  mov dword ptr [esp+34h+var_2C], eax; char
1003ea46  mov [esp+34h+var_34], 1; int
1003ea4d  call sub_1000DEB0
1003ea52  sub esp, 0Ch
1003ea55  mov edi, [ebp+var_28]
1003ea58  jmp loc_1003E988
loc_1003EA5D:
1003ea5d  vxorps xmm0, xmm0, xmm0
1003ea61  vmovups xmmword ptr [ecx], xmm0
1003ea65  mov dword ptr [ecx+10h], 0
1003ea6c  mov dword ptr [ecx+14h], 0Fh
loc_1003EA73:
1003ea73  mov eax, [ebp+var_18]
1003ea76  mov large fs:0, eax
1003ea7c  mov eax, ecx
1003ea7e  add esp, 28h
1003ea81  pop esi
1003ea82  pop edi
1003ea83  pop ebx
1003ea84  pop ebp
1003ea85  retn
loc_1003EA90:
1003ea90  push ebp
1003ea91  sub esp, 0Ch
1003ea94  add ebp, 0Ch
1003ea97  mov ecx, [ebp+Src]
1003ea9a  call sub_1000BC90
1003ea9f  add esp, 0Ch
1003eaa2  pop ebp
1003eaa3  retn
loc_100480B0:
100480b0  mov eax, offset stru_1029727C
100480b5  jmp __CxxFrameHandler3

