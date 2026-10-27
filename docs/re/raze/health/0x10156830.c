char __usercall sub_10156830@<al>(
        _DWORD *a1@<ecx>,
        __int128 _XMM3@<xmm3>,
        __int128 _XMM4@<xmm4>,
        __int128 _XMM6@<xmm6>,
        __int128 _XMM7@<xmm7>)
{
  _DWORD *v5; // esi
  _DWORD *v6; // edi
  _DWORD *v7; // ebx
  unsigned int v8; // eax
  _DWORD *v9; // ecx
  int v10; // eax
  int v11; // eax
  unsigned __int8 (__thiscall *v12)(int); // eax
  int v20; // ebx
  int v21; // esi
  int v22; // edi
  int v23; // edi
  int v24; // esi
  int v25; // ebx
  int v26; // esi
  int v27; // edi
  int v28; // edi
  int v29; // ebx
  int v34; // edi
  int v35; // ecx
  BOOL v36; // ebx
  int v37; // esi
  int v38; // eax
  int v39; // ebx
  bool v48; // zf
  int *v51; // edi
  char v96; // bh
  char v97; // bl
  int v98; // edx
  int v99; // edi
  int v100; // eax
  int v101; // ebx
  int (__thiscall *v102)(_DWORD); // eax
  unsigned int v104; // ebx
  char v116; // cf
  int v117; // esi
  bool v118; // cc
  int v119; // ebx
  int v120; // esi
  int (__thiscall *v121)(int); // eax
  int v122; // eax
  int v123; // esi
  int (__thiscall *v124)(int); // eax
  int v125; // eax
  int v126; // esi
  int v128; // edi
  int v130; // ebx
  int v131; // esi
  int v132; // edi
  int v133; // edx
  int v134; // esi
  int v135; // ebx
  int v136; // ebx
  int v137; // edi
  int v138; // esi
  int v139; // eax
  int v140; // esi
  int v141; // ebx
  int v142; // edi
  int v143; // ebx
  unsigned int v144; // ebx
  int v149; // edi
  unsigned int v152; // ebx
  int v153; // eax
  int v155; // ecx
  unsigned int v158; // ebx
  int v159; // edi
  unsigned int v160; // esi
  int v162; // ebx
  int v163; // ecx
  int v174; // edi
  int v184; // edi
  char v192; // bh
  char v193; // bl
  int v194; // edi
  int v195; // esi
  int v197; // eax
  int v199; // ebx
  int v207; // esi
  int v209; // eax
  int v211; // ebx
  int v219; // esi
  int v226; // edx
  int v227; // ebx
  int v228; // eax
  void (__thiscall *v235)(int, int); // eax
  int v250; // edi
  int (__thiscall *v253)(int); // eax
  int v254; // eax
  char *v255; // ebx
  int v256; // esi
  int v257; // eax
  int v258; // esi
  int v259; // eax
  int v260; // esi
  __int16 v261; // ax
  int v274; // esi
  int v275; // edi
  int v276; // ebx
  __int16 v277; // ax
  int v278; // edi
  int v279; // esi
  _WORD *v280; // eax
  int v299; // edi
  int v300; // ebx
  int v301; // esi
  int v302; // eax
  __int64 v303; // rdi
  int v304; // eax
  int v305; // edi
  int v306; // ebx
  int v307; // eax
  int v309; // [esp-18h] [ebp-1F4h]
  int v310; // [esp-18h] [ebp-1F4h]
  __int128 v311; // [esp-14h] [ebp-1F0h]
  __int128 v312; // [esp-14h] [ebp-1F0h]
  __int128 v313; // [esp-14h] [ebp-1F0h]
  __int128 v314; // [esp-14h] [ebp-1F0h]
  __int128 v315; // [esp-14h] [ebp-1F0h]
  __int128 v316; // [esp-14h] [ebp-1F0h]
  __int128 v317; // [esp-14h] [ebp-1F0h]
  __int128 v318; // [esp-14h] [ebp-1F0h]
  __int128 v319; // [esp-14h] [ebp-1F0h]
  int v320; // [esp-14h] [ebp-1F0h]
  __int128 v321; // [esp-14h] [ebp-1F0h]
  __int128 v322; // [esp-14h] [ebp-1F0h]
  int v323; // [esp-10h] [ebp-1ECh]
  int v324; // [esp-10h] [ebp-1ECh]
  int v325; // [esp-10h] [ebp-1ECh]
  int v326; // [esp-Ch] [ebp-1E8h]
  int v327; // [esp-Ch] [ebp-1E8h]
  int v328; // [esp-Ch] [ebp-1E8h]
  int v329; // [esp-4h] [ebp-1E0h]
  int v330; // [esp-4h] [ebp-1E0h]
  int v331; // [esp-4h] [ebp-1E0h]
  __int128 v332; // [esp+0h] [ebp-1DCh] BYREF
  int v333; // [esp+10h] [ebp-1CCh]
  int v334; // [esp+14h] [ebp-1C8h]
  int v335; // [esp+18h] [ebp-1C4h]
  int v336; // [esp+1Ch] [ebp-1C0h]
  int v337; // [esp+20h] [ebp-1BCh]
  int v338; // [esp+24h] [ebp-1B8h]
  int v339; // [esp+28h] [ebp-1B4h]
  int v340; // [esp+2Ch] [ebp-1B0h]
  int v341; // [esp+30h] [ebp-1ACh]
  int v342; // [esp+34h] [ebp-1A8h]
  int v343; // [esp+38h] [ebp-1A4h]
  int v344; // [esp+3Ch] [ebp-1A0h]
  int v345; // [esp+40h] [ebp-19Ch]
  int v346; // [esp+44h] [ebp-198h]
  int v347; // [esp+48h] [ebp-194h]
  int v348; // [esp+4Ch] [ebp-190h]
  int v350; // [esp+60h] [ebp-17Ch]
  int v351; // [esp+64h] [ebp-178h]
  int v352; // [esp+68h] [ebp-174h]
  int v353; // [esp+6Ch] [ebp-170h]
  int v354; // [esp+70h] [ebp-16Ch]
  int v355; // [esp+74h] [ebp-168h]
  int v356; // [esp+78h] [ebp-164h]
  int v357; // [esp+7Ch] [ebp-160h]
  int v358; // [esp+80h] [ebp-15Ch]
  int v359; // [esp+84h] [ebp-158h]
  int v360; // [esp+88h] [ebp-154h]
  __int128 v361; // [esp+8Ch] [ebp-150h]
  int v362; // [esp+9Ch] [ebp-140h]
  int v363; // [esp+A0h] [ebp-13Ch]
  int v364; // [esp+A4h] [ebp-138h]
  int v365; // [esp+A8h] [ebp-134h]
  int v366; // [esp+ACh] [ebp-130h]
  int v367; // [esp+B0h] [ebp-12Ch] BYREF
  int v368; // [esp+BCh] [ebp-120h] BYREF
  int v371; // [esp+C8h] [ebp-114h]
  char v372; // [esp+CCh] [ebp-110h]
  char v373; // [esp+CDh] [ebp-10Fh]
  char v374; // [esp+CEh] [ebp-10Eh]
  char v375; // [esp+CFh] [ebp-10Dh]
  int v376; // [esp+D0h] [ebp-10Ch]
  int v377; // [esp+D4h] [ebp-108h]
  int v378; // [esp+D8h] [ebp-104h]
  int v379; // [esp+DCh] [ebp-100h]
  int v380; // [esp+E0h] [ebp-FCh]
  char v381; // [esp+E4h] [ebp-F8h]
  char v382; // [esp+E5h] [ebp-F7h]
  char v383; // [esp+E6h] [ebp-F6h]
  char v384; // [esp+E7h] [ebp-F5h]
  char v385; // [esp+E8h] [ebp-F4h]
  char v387; // [esp+EAh] [ebp-F2h]
  char v388; // [esp+EBh] [ebp-F1h]
  int v389; // [esp+ECh] [ebp-F0h]
  int v390; // [esp+F0h] [ebp-ECh]
  int v391; // [esp+F4h] [ebp-E8h]
  int v392; // [esp+F8h] [ebp-E4h]
  int v393; // [esp+FCh] [ebp-E0h]
  int v394; // [esp+100h] [ebp-DCh]
  char v395; // [esp+104h] [ebp-D8h]
  char v396; // [esp+105h] [ebp-D7h]
  char v397; // [esp+106h] [ebp-D6h]
  char v398; // [esp+107h] [ebp-D5h]
  int v399; // [esp+108h] [ebp-D4h]
  char v400; // [esp+10Ch] [ebp-D0h]
  char v401; // [esp+10Dh] [ebp-CFh]
  char v402; // [esp+10Eh] [ebp-CEh]
  char v403; // [esp+10Fh] [ebp-CDh]
  char v404; // [esp+110h] [ebp-CCh]
  char v405; // [esp+111h] [ebp-CBh]
  char v406; // [esp+112h] [ebp-CAh]
  char v407; // [esp+113h] [ebp-C9h]
  int v408; // [esp+114h] [ebp-C8h]
  int v409; // [esp+118h] [ebp-C4h]
  char v410; // [esp+11Ch] [ebp-C0h]
  char v411; // [esp+11Dh] [ebp-BFh]
  char v412; // [esp+11Eh] [ebp-BEh]
  char v413; // [esp+11Fh] [ebp-BDh]
  int v414; // [esp+120h] [ebp-BCh]
  char v415; // [esp+124h] [ebp-B8h]
  char v416; // [esp+125h] [ebp-B7h]
  char v417; // [esp+126h] [ebp-B6h]
  char v418; // [esp+127h] [ebp-B5h]
  char v419; // [esp+128h] [ebp-B4h]
  char v420; // [esp+129h] [ebp-B3h]
  char v421; // [esp+12Ah] [ebp-B2h]
  char v422; // [esp+12Bh] [ebp-B1h]
  int v423; // [esp+12Ch] [ebp-B0h]
  int v424; // [esp+130h] [ebp-ACh]
  char v425; // [esp+134h] [ebp-A8h]
  char v426; // [esp+135h] [ebp-A7h]
  char v427; // [esp+136h] [ebp-A6h]
  char v428; // [esp+137h] [ebp-A5h]
  int v429; // [esp+138h] [ebp-A4h]
  int v430; // [esp+13Ch] [ebp-A0h]
  int v431; // [esp+140h] [ebp-9Ch]
  int v432; // [esp+144h] [ebp-98h]
  int v433; // [esp+148h] [ebp-94h]
  int v434; // [esp+14Ch] [ebp-90h]
  int v435; // [esp+150h] [ebp-8Ch]
  int v436; // [esp+154h] [ebp-88h]
  int v437; // [esp+158h] [ebp-84h]
  char v438; // [esp+15Ch] [ebp-80h] BYREF
  char v439; // [esp+15Dh] [ebp-7Fh]
  char v440; // [esp+15Eh] [ebp-7Eh]
  char v441; // [esp+15Fh] [ebp-7Dh]
  int v442; // [esp+160h] [ebp-7Ch] BYREF
  __int128 v443; // [esp+164h] [ebp-78h]
  int *v444; // [esp+174h] [ebp-68h]
  unsigned int v445; // [esp+178h] [ebp-64h]
  unsigned int v446; // [esp+17Ch] [ebp-60h]
  __int64 v447; // [esp+180h] [ebp-5Ch] BYREF
  int v448; // [esp+188h] [ebp-54h]
  int v449; // [esp+18Ch] [ebp-50h]
  __int128 v450; // [esp+190h] [ebp-4Ch]
  int v451; // [esp+1A0h] [ebp-3Ch]
  __int64 v452; // [esp+1A4h] [ebp-38h] BYREF
  int *v453; // [esp+1ACh] [ebp-30h]
  char v454; // [esp+1B2h] [ebp-2Ah]
  char v455; // [esp+1B3h] [ebp-29h]
  int v456; // [esp+1B4h] [ebp-28h]
  char *v457; // [esp+1B8h] [ebp-24h]
  int v458; // [esp+1BCh] [ebp-20h]
  __int128 *v459; // [esp+1C0h] [ebp-1Ch]
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp+1C4h] [ebp-18h]
  void *v461; // [esp+1C8h] [ebp-14h]
  int v462; // [esp+1CCh] [ebp-10h]

  v5 = a1; /*0x1015683c*/
  v459 = &v332; /*0x1015683e*/
  v462 = -1; /*0x10156841*/
  v461 = &loc_1015B480; /*0x1015684b*/
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList; /*0x10156859*/
  v6 = (_DWORD *)*a1; /*0x10156862*/
  v7 = (_DWORD *)a1[1]; /*0x10156864*/
  if ( (_DWORD *)*a1 != v7 ) /*0x10156869*/
  {
    LODWORD(v450) = a1; /*0x1015686b*/
    do /*0x10156891*/
    {
      v8 = v6[12]; /*0x10156893*/
      if ( v8 >= 0x10 ) /*0x10156899*/
      {
        v9 = (_DWORD *)v6[7]; /*0x1015689b*/
        if ( v8 + 1 >= 0x1000 ) /*0x101568a7*/
        {
          if ( (unsigned int)v9 - *(v9 - 1) - 4 >= 0x20 ) /*0x101568b4*/
            BUG(); /*0x10158ccd*/
          v9 = (_DWORD *)*(v9 - 1); /*0x101568bf*/
        }
        sub_10180976(Block: v9); /*0x10156872*/
      }
      v6[11] = 0; /*0x1015687a*/
      v6[12] = 15; /*0x10156881*/
      *((_BYTE *)v6 + 28) = 0; /*0x10156888*/
      v6 += 16; /*0x1015688c*/
    }
    while ( v6 != v7 ); /*0x10156891*/
    v5 = (_DWORD *)v450; /*0x101568c3*/
    *(_DWORD *)(v450 + 4) = *(_DWORD *)v450; /*0x101568c8*/
  }
  v10 = v5[3]; /*0x101568cb*/
  if ( v10 != v5[4] ) /*0x101568d1*/
    v5[4] = v10; /*0x101568d3*/
  v11 = v5[6]; /*0x101568d6*/
  if ( v11 != v5[7] ) /*0x101568dc*/
    v5[7] = v11; /*0x101568de*/
  if ( byte_103F5396 == 0 && byte_103F5210 == 1 ) /*0x101568f5*/
  {
    v12 = *(unsigned __int8 (__thiscall **)(int))(*(_DWORD *)dword_103EC8A0 + 8); /*0x10156903*/
    v462 = -1; /*0x10156906*/
    if ( v12(a1: dword_103EC8A0) == 0 || (LOBYTE(v11) = (_BYTE)off_1029E02C, *(_BYTE *)off_1029E02C == 1) ) /*0x1015691b*/
    {
      if ( byte_103F4BA8 != 1 /*0x1015693a*/
        || (LOBYTE(v11) = (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 356))(a1: dword_103EC770),
            (_BYTE)v11 == 0) )
      {
        __asm /*0x10156940*/
        {
          vmovss xmm1, ds:dword_10184380
          vmulss xmm0, xmm1, dword ptr xmmword_103F5A3C
          vcvttss2si eax, xmm0
        }
        LODWORD(v450) = _EAX; /*0x10156954*/
        __asm /*0x10156957*/
        {
          vmulss xmm0, xmm1, dword ptr xmmword_103F5A3C+4
          vcvttss2si eax, xmm0
        }
        v444 = _EAX; /*0x10156963*/
        __asm /*0x10156966*/
        {
          vmulss xmm0, xmm1, dword ptr xmmword_103F5A3C+8
          vcvttss2si edi, xmm0
        }
        v20 = dword_103F5218; /*0x10156972*/
        if ( *(_BYTE *)off_1029E02C == 1 ) /*0x10156980*/
          sub_10156390(a1: v5); /*0x10156984*/
        LOBYTE(v11) = dword_103F5220; /*0x10156989*/
        if ( dword_103F5220 == 1 ) /*0x10156991*/
        {
          v26 = v20 / 2; /*0x10156a80*/
          v445 = v20 / 2 + qword_103F529C; /*0x10156a89*/
          v458 = v20 / 2 + HIDWORD(qword_103F529C); /*0x10156a94*/
          v337 = -451668992; /*0x10156a9d*/
          sub_10178BD0(a1: v445, a2: v458, a3: v20 / 2, a4: 100, a5: 0); /*0x10156ab1*/
          LODWORD(v443) = (unsigned __int8)_EDI << 16; /*0x10156ac4*/
          v27 = ((unsigned __int8)v444 << 8) | (unsigned __int8)v450; /*0x10156ad2*/
          v338 = v443 + v27 - 0x1000000; /*0x10156adc*/
          HIDWORD(v313) = 100; /*0x10156ae3*/
          DWORD2(v313) = v20 / 2; /*0x10156ae5*/
          *(_QWORD *)&v313 = __PAIR64__(v458, v445); /*0x10156ae9*/
          sub_10178B80(a1: v313, a2: v338); /*0x10156aec*/
          v444 = (int *)HIDWORD(qword_103F529C); /*0x10156afd*/
          v445 = *(_DWORD *)ArgList; /*0x10156b05*/
          LODWORD(v450) = v443 + v27 + 2130706432; /*0x10156b12*/
          v339 = v450; /*0x10156b15*/
          v28 = v20 / 8; /*0x10156b23*/
          v29 = v20 - v20 / 8; /*0x10156b26*/
          v458 = qword_103F529C; /*0x10156b2b*/
          LODWORD(v443) = qword_103F529C + v29; /*0x10156b31*/
          HIDWORD(v314) = HIDWORD(qword_103F529C) + v26; /*0x10156b3c*/
          DWORD2(v314) = qword_103F529C + v29; /*0x10156b3d*/
          DWORD1(v314) = HIDWORD(qword_103F529C) + v26; /*0x10156b40*/
          LODWORD(v314) = v28 + qword_103F529C; /*0x10156b41*/
          sub_101775B0(a1: v314, a2: v450); /*0x10156b42*/
          v340 = v450; /*0x10156b5a*/
          HIDWORD(v315) = (char *)v444 + v29; /*0x10156b61*/
          DWORD2(v315) = v458 + v26; /*0x10156b62*/
          DWORD1(v315) = (char *)v444 + v28; /*0x10156b63*/
          LODWORD(v315) = v458 + v26; /*0x10156b64*/
          sub_101775B0(a1: v315, a2: v450); /*0x10156b65*/
        }
        else
        {
          if ( dword_103F5220 != 0 ) /*0x10156999*/
            return v11; /*0x10156999*/
          v445 = HIDWORD(qword_103F529C); /*0x101569ad*/
          v458 = qword_103F529C; /*0x101569ad*/
          v333 = -451668992; /*0x101569b6*/
          sub_10177780(a1: qword_103F529C, a2: HIDWORD(qword_103F529C), a3: v20, a4: v20, a5: -451668992); /*0x101569c9*/
          v21 = (unsigned __int8)_EDI << 16; /*0x101569d9*/
          v22 = ((unsigned __int8)v444 << 8) | (unsigned __int8)v450; /*0x101569e7*/
          v334 = v21 + v22 - 0x1000000; /*0x101569f1*/
          sub_101777E0(a1: v458, a2: v445, a3: v20, a4: v20, a5: v334); /*0x10156a00*/
          LODWORD(v450) = v21 + v22 + 2130706432; /*0x10156a13*/
          v335 = v450; /*0x10156a16*/
          v23 = v20 / 2; /*0x10156a23*/
          LODWORD(v443) = v20 / 2 + v445; /*0x10156a2a*/
          v24 = v20 / 8; /*0x10156a35*/
          v25 = v20 - v20 / 8; /*0x10156a38*/
          v444 = (int *)(v458 + v25); /*0x10156a40*/
          HIDWORD(v311) = v443; /*0x10156a4b*/
          DWORD2(v311) = v458 + v25; /*0x10156a4c*/
          DWORD1(v311) = v443; /*0x10156a4f*/
          LODWORD(v311) = v24 + v458; /*0x10156a50*/
          sub_101775B0(a1: v311, a2: v450); /*0x10156a51*/
          v336 = v450; /*0x10156a69*/
          HIDWORD(v312) = v445 + v25; /*0x10156a70*/
          DWORD2(v312) = v458 + v23; /*0x10156a71*/
          DWORD1(v312) = v445 + v24; /*0x10156a72*/
          LODWORD(v312) = v458 + v23; /*0x10156a73*/
          sub_101775B0(a1: v312, a2: v450); /*0x10156a74*/
        }
        v11 = sub_10179540(); /*0x10156b70*/
        v445 = v11; /*0x10156b75*/
        if ( v11 != 0 ) /*0x10156b7a*/
        {
          __asm /*0x10156b80*/
          {
            vmovss xmm0, ds:dword_10184380
            vmulss xmm1, xmm0, dword_103F528C
            vmovss [ebp+var_A4], xmm1
            vmulss xmm1, xmm0, dword_103F5290
            vmovss [ebp+var_8C], xmm1
            vmulss xmm0, xmm0, dword_103F5294
            vmovss [ebp+var_90], xmm0
          }
          if ( byte_103F5210 == 1 ) /*0x10156bbf*/
          {
            v34 = *(_DWORD *)off_1029E014; /*0x10156bcb*/
            v35 = *(_DWORD *)(*(_DWORD *)off_1029E014 + 4); /*0x10156bcd*/
            v36 = false; /*0x10156bd0*/
            if ( *(_BYTE *)(v35 + 13) != 0 ) /*0x10156bd6*/
            {
              v37 = *(_DWORD *)(*(_DWORD *)off_1029E014 + 4); /*0x10156bd8*/
              v38 = *(_DWORD *)off_1029E014; /*0x10156bda*/
            }
            else
            {
              v38 = *(_DWORD *)off_1029E014; /*0x10156bde*/
              do /*0x10156bf6*/
              {
                v37 = v35; /*0x10156be0*/
                v39 = *(_DWORD *)(v35 + 16); /*0x10156be2*/
                if ( v39 >= 0 ) /*0x10156be7*/
                  v38 = v35; /*0x10156be7*/
                v35 = *(_DWORD *)(v35 + 8 * ((unsigned int)v39 >> 31)); /*0x10156bef*/
              }
              while ( *(_BYTE *)(v35 + 13) == 0 ); /*0x10156bf6*/
              v36 = v39 >= 0; /*0x10156bfa*/
            }
            __asm /*0x10156bfd*/
            {
              vmovsd xmm0, qword_103F526C
              vmovups [ebp+var_150], xmm0
              vmovss xmm0, ds:dword_10184380
              vmulss xmm1, xmm0, dword_103F5258
              vmovss [ebp+var_28], xmm1
              vmulss xmm1, xmm0, dword_103F525C
              vmovss [ebp+var_30], xmm1
              vmovss xmm1, dword_103F5274
              vmovss [ebp+var_88], xmm1
              vmulss xmm1, xmm0, dword_103F5260
              vmovss [ebp+var_24], xmm1
              vmovsd xmm1, qword_103F527C
              vmovups [ebp+var_78], xmm1
              vmulss xmm0, xmm0, dword_103F5284
              vmovss [ebp+var_20], xmm0
            }
            if ( *(_BYTE *)(v38 + 13) != 0 || *(int *)(v38 + 16) > 0 ) /*0x10156c70*/
            {
              v48 = *((_DWORD *)off_1029E014 + 1) == 0x7FFFFFF; /*0x10156c72*/
              v462 = -1; /*0x10156c79*/
              LODWORD(v450) = off_1029E014; /*0x10156c80*/
              if ( v48 ) /*0x10156c83*/
                sub_10018B20(); /*0x10158ccf*/
              _EAX = sub_10180946(Size: 0x20u); /*0x10156c8b*/
              __asm /*0x10156c93*/
              {
                vxorps xmm0, xmm0, xmm0
                vmovups xmmword ptr [eax+10h], xmm0
              }
              *(_DWORD *)_EAX = v34; /*0x10156c9c*/
              *(_DWORD *)(_EAX + 4) = v34; /*0x10156c9e*/
              *(_DWORD *)(_EAX + 8) = v34; /*0x10156ca1*/
              *(_WORD *)(_EAX + 12) = 0; /*0x10156ca4*/
              v38 = sub_10018900(a1: v37, a2: v36, a3: _EAX); /*0x10156cb0*/
            }
            v51 = *(int **)(v38 + 20); /*0x10156cb5*/
            v11 = *(_DWORD *)(v38 + 24); /*0x10156cb8*/
            v444 = (int *)v11; /*0x10156cbb*/
            if ( v51 != (int *)v11 ) /*0x10156cc0*/
            {
              __asm /*0x10156cc6*/
              {
                vbroadcastss xmm1, ds:dword_10184380
                vmulps xmm0, xmm1, [ebp+var_150]
                vmovss xmm2, [ebp+var_88]
                vmulss xmm2, xmm2, ds:dword_10184380
                vcvttss2si eax, [ebp+var_28]
                vcvttss2si ecx, [ebp+var_30]
                vcvttss2si edx, [ebp+var_24]
                vcvttps2dq xmm0, xmm0
                vcvttss2si ebx, xmm2
                vpackusdw xmm0, xmm0, xmm0
              }
              v430 = _EAX; /*0x10156d03*/
              __asm { vcvtsi2ss xmm2, xmm3, eax } /*0x10156d0c*/
              v431 = _ECX; /*0x10156d10*/
              __asm { vcvtsi2ss xmm3, xmm3, eax } /*0x10156d19*/
              v432 = _EDX; /*0x10156d1d*/
              __asm /*0x10156d26*/
              {
                vcvtsi2ss xmm4, xmm4, eax
                vpackuswb xmm0, xmm0, xmm0
                vmovdqu [ebp+var_4C], xmm0
              }
              LODWORD(v361) = _EBX; /*0x10156d33*/
              __asm /*0x10156d3c*/
              {
                vpmovzxbd xmm5, xmm0
                vcvtdq2ps xmm5, xmm5
                vcvtsi2ss xmm6, xmm6, eax
                vbroadcastss xmm7, ds:dword_101863C8
                vmulps xmm5, xmm5, xmm7
                vmulps xmm7, xmm1, [ebp+var_78]
                vmovss xmm0, ds:dword_101863C8
                vmulss xmm1, xmm2, xmm0
                vmulss xmm2, xmm3, xmm0
                vmulss xmm3, xmm4, xmm0
                vmulss xmm4, xmm6, ds:dword_101863C8
                vcvttps2dq xmm6, xmm7
                vcvttps2dq xmm5, xmm5
                vpackusdw xmm6, xmm6, xmm6
                vpackusdw xmm5, xmm5, xmm5
                vpackuswb xmm0, xmm6, xmm6
                vpackuswb xmm5, xmm5, xmm5
                vmovdqu [ebp+var_18C], xmm5
              }
              __asm { vcvttss2si eax, [ebp+var_20] }
              v358 = _EAX; /*0x10156da1*/
              __asm { vcvttss2si eax, xmm1 } /*0x10156da7*/
              v362 = _EAX; /*0x10156dab*/
              __asm { vcvttss2si eax, xmm2 } /*0x10156db1*/
              v363 = _EAX; /*0x10156db5*/
              __asm { vcvttss2si eax, xmm3 } /*0x10156dbb*/
              v364 = _EAX; /*0x10156dbf*/
              __asm { vcvttss2si eax, xmm4 } /*0x10156dc5*/
              v365 = _EAX; /*0x10156dc9*/
              __asm /*0x10156dcf*/
              {
                vmovdqu xmm1, [ebp+var_4C]
                vpextrb eax, xmm1, 1
              }
              v436 = _EAX; /*0x10156dda*/
              __asm /*0x10156de0*/
              {
                vmovd [ebp+var_94], xmm1
                vmovd [ebp+var_158], xmm0
                vmovdqu [ebp+var_1DC], xmm0
                vpextrb eax, xmm0, 1
              }
              v360 = _EAX; /*0x10156dfe*/
              do /*0x10156eec*/
              {
                v11 = *v51; /*0x10156eec*/
                if ( *v51 == 0 ) /*0x10156ef0*/
                  goto LABEL_45; /*0x10156ef0*/
                LODWORD(v450) = *v51; /*0x10156ef2*/
                if ( dword_103EEA68 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10156f10*/
                {
                  sub_101809C3(a1: &dword_103EEA68); /*0x1015815e*/
                  if ( dword_103EEA68 == -1 ) /*0x1015816d*/
                  {
                    v453 = v51; /*0x10158173*/
                    v462 = 0; /*0x10158176*/
                    dword_103EEA64 = sub_1017C730(a1: "CBasePlayer", a2: "deadflag"); /*0x1015818f*/
                    sub_10180A43(a1: &dword_103EEA68); /*0x10158199*/
                    v51 = v453; /*0x101581a1*/
                  }
                }
                LOBYTE(v11) = dword_103EEA64; /*0x10156f16*/
                if ( *(_DWORD *)(dword_103EEA64 + v450) != 0 ) /*0x10156f22*/
                  goto LABEL_45; /*0x10156f22*/
                if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10156f3f*/
                {
                  sub_101809C3(a1: &dword_103EEAD8); /*0x101581b1*/
                  if ( dword_103EEAD8 == -1 ) /*0x101581c0*/
                  {
                    v453 = v51; /*0x101581c6*/
                    v462 = 1; /*0x101581c9*/
                    dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x101581e2*/
                    sub_10180A43(a1: &dword_103EEAD8); /*0x101581ec*/
                    v51 = v453; /*0x101581f4*/
                  }
                }
                v101 = v450; /*0x10156f4a*/
                v458 = *(_DWORD *)(dword_103EEAD4 + v450); /*0x10156f50*/
                v102 = *(int (__thiscall **)(_DWORD))(*(_DWORD *)v450 + 960); /*0x10156f55*/
                v462 = -1; /*0x10156f5b*/
                LOBYTE(v443) = v102(a1: v450); /*0x10156f66*/
                if ( dword_103EEF7C > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10156f84*/
                {
                  sub_101809C3(a1: &dword_103EEF7C); /*0x10158204*/
                  if ( dword_103EEF7C == -1 ) /*0x10158213*/
                  {
                    v453 = v51; /*0x10158219*/
                    v462 = 2; /*0x1015821c*/
                    dword_103EEF78 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_zombieClass"); /*0x10158235*/
                    sub_10180A43(a1: &dword_103EEF7C); /*0x1015823f*/
                    v51 = v453; /*0x10158247*/
                    v101 = v450; /*0x1015824d*/
                  }
                }
                v11 = *(_DWORD *)(dword_103EEF78 + v101); /*0x10156f8f*/
                if ( v101 == v445 && byte_103F5211 != 0 ) /*0x10156f9e*/
                  goto LABEL_45; /*0x10156f9e*/
                v456 = *(_DWORD *)(dword_103EEF78 + v101); /*0x10156fa4*/
                v452 = 0; /*0x10156fa7*/
                v462 = -1; /*0x10156fb5*/
                sub_1002A190(a1: &v447); /*0x10156fbf*/
                LOBYTE(v11) = sub_10156560(a1: &v452, a2: (char *)&v452 + 4, a3: &v447); /*0x10156fcd*/
                if ( (_BYTE)v11 == 0 ) /*0x10156fd4*/
                  goto LABEL_45; /*0x10156fd4*/
                __asm /*0x10156fda*/
                {
                  vpxor xmm0, xmm0, xmm0
                  vmovq [ebp+var_5C], xmm0
                }
                v448 = 0; /*0x10156fe3*/
                v104 = v445; /*0x10156ff2*/
                sub_1002A190(a1: &v367); /*0x10156ff6*/
                sub_10177160(a1: &v367, a2: &v447); /*0x1015700f*/
                LOBYTE(v11) = sub_1002A190(a1: &v368); /*0x10157017*/
                __asm /*0x1015701c*/
                {
                  vmovss xmm0, [ebp+var_118]
                  vsubss xmm0, xmm0, [ebp+var_124]
                  vmovss xmm1, [ebp+var_11C]
                  vsubss xmm1, xmm1, [ebp+var_128]
                  vmovss xmm2, [ebp+var_120]
                  vsubss xmm2, xmm2, [ebp+var_12C]
                  vmulss xmm1, xmm1, xmm1
                  vfmadd231ss xmm1, xmm2, xmm2
                  vfmadd231ss xmm1, xmm0, xmm0
                  vcvtss2sd xmm0, xmm1, xmm1
                  vsqrtsd xmm0, xmm0, xmm0
                  vcvtsd2ss xmm0, xmm0, xmm0
                  vucomiss xmm0, dword_103F5214
                }
                if ( !(v116 | v48) ) /*0x1015706e*/
                  goto LABEL_45; /*0x1015706e*/
                v117 = v450; /*0x1015707c*/
                sub_101669E0(a1: &v368, a2: v104, a3: v450); /*0x10157082*/
                HIBYTE(v368) = 100; /*0x10157087*/
                v457 = (char *)off_1029E57C; /*0x10157093*/
                v118 = dword_103EF0D4 <= *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) /*0x101570ab*/
                                                   + 4);
                v119 = v117; /*0x101570b1*/
                v453 = v51; /*0x101570b3*/
                if ( !v118 ) /*0x101570b6*/
                {
                  sub_101809C3(a1: &dword_103EF0D4); /*0x1015825a*/
                  if ( dword_103EF0D4 == -1 ) /*0x10158269*/
                  {
                    v462 = 3; /*0x1015826f*/
                    dword_103EF0D0 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_iMaxHealth"); /*0x10158288*/
                    sub_10180A43(a1: &dword_103EF0D4); /*0x10158292*/
                    v119 = v450; /*0x1015829a*/
                  }
                }
                v120 = *(_DWORD *)(dword_103EF0D0 + v119); /*0x101570c1*/
                v121 = *(int (__thiscall **)(int))(*(_DWORD *)v119 + 468); /*0x101570c6*/
                v462 = -1; /*0x101570cc*/
                v122 = v121(a1: v119); /*0x101570d5*/
                sub_10167260(a1: &v438, a2: v122, a3: v120); /*0x101570e0*/
                v457 = (char *)off_1029E57C; /*0x101570ea*/
                if ( dword_103EF0D4 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10157108*/
                {
                  sub_101809C3(a1: &dword_103EF0D4); /*0x101582a7*/
                  if ( dword_103EF0D4 == -1 ) /*0x101582b6*/
                  {
                    v462 = 4; /*0x101582bc*/
                    dword_103EF0D0 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_iMaxHealth"); /*0x101582d5*/
                    sub_10180A43(a1: &dword_103EF0D4); /*0x101582df*/
                    v119 = v450; /*0x101582e7*/
                  }
                }
                v123 = *(_DWORD *)(dword_103EF0D0 + v119); /*0x10157113*/
                v124 = *(int (__thiscall **)(int))(*(_DWORD *)v119 + 468); /*0x10157118*/
                v462 = -1; /*0x1015711e*/
                v125 = v124(a1: v119); /*0x10157127*/
                sub_101671B0(a1: &v442, a2: v125, a3: v123); /*0x10157132*/
                v126 = (*(int (__thiscall **)(int))(*(_DWORD *)v119 + 468))(a1: v119); /*0x10157141*/
                if ( dword_103EF0D4 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1015715e*/
                {
                  sub_101809C3(a1: &dword_103EF0D4); /*0x101582f4*/
                  if ( dword_103EF0D4 == -1 ) /*0x10158303*/
                  {
                    v451 = v126; /*0x10158309*/
                    v462 = 5; /*0x1015830c*/
                    dword_103EF0D0 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_iMaxHealth"); /*0x10158325*/
                    sub_10180A43(a1: &dword_103EF0D4); /*0x1015832f*/
                    v126 = v451; /*0x10158337*/
                    v119 = v450; /*0x1015833a*/
                  }
                }
                __asm { vcvtsi2ss xmm2, xmm3, esi } /*0x10157164*/
                v128 = *(_DWORD *)(dword_103EF0D0 + v119); /*0x1015716d*/
                __asm { vcvtsi2ss xmm1, xmm3, edi } /*0x10157170*/
                LOBYTE(v11) = v458; /*0x10157174*/
                __asm { vxorps xmm3, xmm3, xmm3 } /*0x1015717a*/
                if ( v458 != 2 ) /*0x1015717e*/
                {
                  if ( v458 != 3 || byte_103F5229 != 1 ) /*0x10157194*/
                    goto LABEL_44; /*0x10157194*/
                  if ( ((unsigned __int8)v443 & (unsigned __int8)byte_103F4BDC) != 0 ) /*0x101571a4*/
                  {
                    v438 = v359; /*0x101571b0*/
                    v458 = v128; /*0x101571b3*/
                    v439 = v360; /*0x101571bc*/
                    v440 = v358; /*0x101571c5*/
                    v441 = -1; /*0x101571c8*/
                    v454 = -1; /*0x101571cc*/
                    LODWORD(v361) = v358; /*0x101571d2*/
                    v436 = v360; /*0x101571da*/
                    v433 = v359; /*0x101571e2*/
                    v455 = v359; /*0x101571e8*/
                    LOBYTE(v443) = v360; /*0x101571eb*/
                    LOBYTE(v449) = v358; /*0x101571f1*/
                    __asm /*0x101571f4*/
                    {
                      vmovups xmm0, [ebp+var_1DC]
                      vmovups [ebp+var_18C], xmm0
                    }
                    v365 = v358; /*0x10157204*/
                  }
                  else
                  {
                    v455 = v438; /*0x101572f6*/
                    LOBYTE(v443) = v439; /*0x101572fd*/
                    LOBYTE(v449) = v440; /*0x10157304*/
                    v454 = v441; /*0x1015730b*/
                  }
                  LOBYTE(v11) = dword_103F522C; /*0x1015730e*/
                  if ( dword_103F522C != 1 ) /*0x10157316*/
                  {
                    if ( dword_103F522C != 0 ) /*0x1015731e*/
                      goto LABEL_44; /*0x1015731e*/
                    v451 = v126; /*0x10157324*/
                    v133 = dword_103F5230; /*0x10157327*/
                    v134 = dword_103F5230 / 2; /*0x10157334*/
                    __asm { vmovss [ebp+var_24], xmm2 } /*0x1015733d*/
                    v458 = dword_103F5230; /*0x10157342*/
                    if ( dword_103F5224 != 0 ) /*0x10157345*/
                    {
                      if ( dword_103F5224 == 1 ) /*0x1015734e*/
                      {
                        LOBYTE(v371) = v433; /*0x10157360*/
                        BYTE1(v371) = v436; /*0x1015736c*/
                        BYTE2(v371) = v361; /*0x10157378*/
                        HIBYTE(v371) = -1; /*0x1015737e*/
                        v456 = HIDWORD(v452) - v134; /*0x1015738a*/
                        v446 = v452 - v134; /*0x10157392*/
                        v462 = -1; /*0x10157395*/
                        v135 = v452 - v134; /*0x101573ab*/
                        __asm { vmovss dword ptr [ebp+var_4C], xmm1 } /*0x101573af*/
                        sub_10177780( /*0x101573b4*/
                          a1: v452 - v134,
                          a2: HIDWORD(v452) - v134,
                          a3: dword_103F5230,
                          a4: dword_103F5230,
                          a5: v371);
                        v350 = -16119286; /*0x101573bf*/
                        sub_10177840(a1: v135, a2: v456, a3: v458, a4: v458, a5: 10, a6: 1); /*0x101573dc*/
                        goto LABEL_82; /*0x101573e1*/
                      }
                    }
                    else
                    {
                      v372 = v433; /*0x1015761b*/
                      v373 = v436; /*0x10157627*/
                      v374 = v361; /*0x10157633*/
                      v375 = -1; /*0x10157639*/
                      v456 = HIDWORD(v452); /*0x10157650*/
                      v446 = v452; /*0x10157656*/
                      v462 = -1; /*0x10157659*/
                      v144 = v452; /*0x1015766d*/
                      __asm { vmovss dword ptr [ebp+var_4C], xmm1 } /*0x10157671*/
                      sub_10178BD0( /*0x10157676*/
                        a1: v452,
                        a2: HIDWORD(v452),
                        a3: (int)(dword_103F5230 + ((unsigned int)(dword_103F5230 + 4) >> 31) + 4) >> 1,
                        a4: 20,
                        a5: v433);
                      v351 = -16119286; /*0x10157681*/
                      HIDWORD(v316) = 20; /*0x101576a4*/
                      DWORD2(v316) = (int)(((unsigned int)(dword_103F5230 + 4) >> 31) + dword_103F5230 + 4) >> 1; /*0x101576a6*/
                      *(_QWORD *)&v316 = __PAIR64__(v456, v144); /*0x101576aa*/
                      sub_10178B80(a1: v316, a2: -16119286); /*0x101576ab*/
LABEL_82:
                      v133 = v458; /*0x101576b0*/
                      __asm /*0x101576b3*/
                      {
                        vmovss xmm1, dword ptr [ebp+var_4C]
                        vmovss xmm2, [ebp+var_24]
                        vxorps xmm3, xmm3, xmm3
                      }
                    }
                    v11 = dword_103F5268; /*0x101576c1*/
                    if ( dword_103F5268 == 1 ) /*0x101576c9*/
                    {
                      LOBYTE(v11) = v451 != 0; /*0x101576d3*/
                      if ( v451 == 0 || v128 == 0 ) /*0x101576d8*/
                        goto LABEL_44; /*0x101576e0*/
                      __asm { vmaxss xmm1, xmm2, xmm1 } /*0x101576e6*/
                      __asm { vcvtsi2ss xmm0, xmm4, edx }
                      __asm
                      {
                        vucomiss xmm1, xmm3
                        vmovss dword ptr [ebp+var_4C], xmm1
                      }
                      v456 = HIDWORD(v452) + v134 + 2; /*0x10157707*/
                      if ( v134 + HIDWORD(v452) != 0 || __SETP__(v134 + HIDWORD(v452), 0) ) /*0x10157710*/
                      {
                        __asm /*0x10157b35*/
                        {
                          vdivss xmm1, xmm2, xmm1
                          vmaxss xmm2, xmm3, xmm1
                          vmovss xmm4, ds:dword_10184184
                          vcmpltss xmm1, xmm4, xmm1
                          vbroadcastss xmm4, ds:dword_10184184
                          vblendvps xmm1, xmm2, xmm4, xmm1
                          vfmadd213ss xmm1, xmm0, xmm3
                        }
                      }
                      else
                      {
                        __asm /*0x10157716*/
                        {
                          vcmpless xmm1, xmm1, xmm2
                          vandps xmm1, xmm1, xmm0
                        }
                      }
                      v462 = -1; /*0x10157b5e*/
                      __asm { vcvttss2si ecx, xmm1 } /*0x10157b65*/
                      v446 = _ECX; /*0x10157b69*/
                      LOBYTE(v376) = v455; /*0x10157b76*/
                      BYTE1(v376) = v443; /*0x10157b80*/
                      BYTE2(v376) = v449; /*0x10157b8a*/
                      HIBYTE(v376) = v454; /*0x10157b94*/
                      v366 = v133 + 2; /*0x10157b9d*/
                      v437 = v452 - v134; /*0x10157ba7*/
                      sub_101777E0(a1: v452 - v134 - 1, a2: v134 + HIDWORD(v452) + 1, a3: v133 + 2, a4: 4, a5: v376); /*0x10157bbe*/
                      LOBYTE(v377) = v455; /*0x10157bcd*/
                      BYTE1(v377) = v443; /*0x10157bd7*/
                      BYTE2(v377) = v449; /*0x10157be1*/
                      HIBYTE(v377) = v454; /*0x10157beb*/
                      sub_10177780(a1: v437, a2: v456, a3: v446, a4: 2, a5: v377); /*0x10157c06*/
                      v133 = v458; /*0x10157c0b*/
                      v11 = dword_103F5268; /*0x10157c0e*/
                      __asm /*0x10157c13*/
                      {
                        vxorps xmm3, xmm3, xmm3
                        vmovss xmm2, [ebp+var_24]
                        vmovss xmm1, dword ptr [ebp+var_4C]
                      }
                    }
                    v51 = v453; /*0x10157c24*/
                    if ( v11 != 2 || v451 == 0 ) /*0x10157c31*/
                      goto LABEL_45; /*0x10157c31*/
                    __asm { vucomiss xmm1, xmm3 } /*0x10157c37*/
                    __asm { vmaxss xmm0, xmm2, xmm1 }
                    __asm { vcvtsi2ss xmm1, xmm5, edx }
                    v174 = HIDWORD(v452) - v134; /*0x10157c56*/
                    __asm { vucomiss xmm0, xmm3 } /*0x10157c58*/
                    LODWORD(v450) = v452 - v134 - 4; /*0x10157c5c*/
                    if ( HIDWORD(v452) != v134 || __SETP__(HIDWORD(v452), v134) ) /*0x10157c65*/
                    {
                      __asm /*0x10156e09*/
                      {
                        vdivss xmm0, xmm2, xmm0
                        vmaxss xmm2, xmm3, xmm0
                        vmovss xmm4, ds:dword_10184184
                        vcmpltss xmm0, xmm4, xmm0
                        vbroadcastss xmm4, ds:dword_10184184
                        vblendvps xmm0, xmm2, xmm4, xmm0
                        vfmadd213ss xmm0, xmm1, xmm3
                      }
                    }
                    else
                    {
                      __asm /*0x10157c6b*/
                      {
                        vcmpless xmm0, xmm0, xmm2
                        vandps xmm0, xmm0, xmm1
                      }
                    }
                    v462 = -1; /*0x10156e32*/
                    __asm { vcvttss2si ecx, xmm0 } /*0x10156e39*/
                    v456 = _ECX; /*0x10156e3d*/
                    v96 = v455; /*0x10156e46*/
                    LOBYTE(v378) = v455; /*0x10156e49*/
                    BYTE1(v378) = v443; /*0x10156e52*/
                    BYTE2(v378) = v449; /*0x10156e5b*/
                    v97 = v454; /*0x10156e61*/
                    HIBYTE(v378) = v454; /*0x10156e64*/
                    v451 = v133 + 2; /*0x10156e6d*/
                    v457 = (char *)(v174 - 1); /*0x10156e73*/
                    sub_101777E0(a1: v452 - v134 - 5, a2: v174 - 1, a3: 4, a4: v133 + 2, a5: v378); /*0x10156e89*/
                    LOBYTE(v379) = v96; /*0x10156e8e*/
                    BYTE1(v379) = v443; /*0x10156e98*/
                    BYTE2(v379) = v449; /*0x10156ea2*/
                    HIBYTE(v379) = v97; /*0x10156ea8*/
                    v98 = v456; /*0x10156eb7*/
                    v99 = v458 + v174 - v456; /*0x10156eba*/
                    v100 = v379; /*0x10156ebc*/
                    goto LABEL_43; /*0x10156ebc*/
                  }
                  __asm { vmovss [ebp+var_24], xmm2 } /*0x101573ed*/
                  if ( dword_103F5224 != 0 ) /*0x101573f2*/
                  {
                    if ( dword_103F5224 == 1 ) /*0x101573fb*/
                    {
                      v136 = dword_103F5234; /*0x10157401*/
                      __asm /*0x10157410*/
                      {
                        vmovdqu xmm0, [ebp+var_18C]
                        vpextrb byte ptr [ebp+var_FC], xmm0, 0
                      }
                      v446 = HIDWORD(v452); /*0x10157425*/
                      v437 = HIDWORD(v452) - dword_103F5234 / 2; /*0x1015742c*/
                      v458 = v128; /*0x10157435*/
                      v451 = v126; /*0x10157438*/
                      v137 = v452; /*0x1015743d*/
                      v138 = v452 - dword_103F5234 / 2; /*0x1015743f*/
                      __asm { vpextrb byte ptr [ebp+var_FC+1], xmm0, 1 } /*0x10157441*/
                      BYTE2(v380) = v365; /*0x10157451*/
                      HIBYTE(v380) = -1; /*0x10157457*/
                      v462 = -1; /*0x1015745e*/
                      __asm { vmovss dword ptr [ebp+var_4C], xmm1 } /*0x10157476*/
                      sub_10177780(a1: v138, a2: v437, a3: dword_103F5234, a4: dword_103F5234, a5: v380); /*0x1015747b*/
                      v366 = *(_DWORD *)ArgList; /*0x10157485*/
                      v139 = sub_101672D0(a1: v456); /*0x10157494*/
                      sub_10178EB0(a1: v137, a2: v446, a3: v136, a4: v136, a5: v139, a6: 24); /*0x101574a8*/
                      v381 = v433; /*0x101574b9*/
                      v382 = v436; /*0x101574c5*/
                      v383 = v361; /*0x101574d1*/
                      v384 = -1; /*0x101574d7*/
                      v310 = v138; /*0x101574ef*/
                      v126 = v451; /*0x101574f0*/
                      v128 = v458; /*0x101574f3*/
                      sub_10177840(a1: v310, a2: v437, a3: v136, a4: v136, a5: v433, a6: 1); /*0x101574f6*/
                      goto LABEL_96; /*0x101574fb*/
                    }
                  }
                  else
                  {
                    __asm /*0x1015783c*/
                    {
                      vmovdqu xmm0, [ebp+var_18C]
                      vpextrb byte ptr [ebp+var_F4], xmm0, 0
                      vpextrb byte ptr [ebp+var_F4+1], xmm0, 1
                    }
                    v387 = v365; /*0x10157864*/
                    v388 = -1; /*0x1015786a*/
                    v446 = HIDWORD(v452); /*0x10157886*/
                    v437 = v452; /*0x1015788c*/
                    v462 = -1; /*0x10157892*/
                    v152 = v452; /*0x101578a9*/
                    __asm { vmovss dword ptr [ebp+var_4C], xmm1 } /*0x101578b0*/
                    sub_10178BD0( /*0x101578b5*/
                      a1: v452,
                      a2: HIDWORD(v452),
                      a3: (int)(((unsigned int)(dword_103F5234 + 4) >> 31) + dword_103F5234 + 4) >> 1,
                      a4: 20,
                      a5: v385);
                    v458 = v128; /*0x101578ba*/
                    v153 = sub_101672D0(a1: v456); /*0x101578ce*/
                    v128 = v458; /*0x101578dd*/
                    sub_10178EB0(a1: v152, a2: v446, a3: dword_103F5234, a4: dword_103F5234, a5: v153, a6: 24); /*0x101578e9*/
                    LOBYTE(v389) = v433; /*0x1015790a*/
                    BYTE1(v389) = v436; /*0x10157916*/
                    BYTE2(v389) = v361; /*0x10157922*/
                    HIBYTE(v389) = -1; /*0x10157928*/
                    HIDWORD(v318) = 20; /*0x10157938*/
                    DWORD2(v318) = (int)(((unsigned int)(dword_103F5234 + 4) >> 31) + dword_103F5234 + 4) >> 1; /*0x1015793a*/
                    *(_QWORD *)&v318 = __PAIR64__(v446, v152); /*0x1015793e*/
                    sub_10178B80(a1: v318, a2: v389); /*0x1015793f*/
LABEL_96:
                    __asm /*0x10157944*/
                    {
                      vmovss xmm1, dword ptr [ebp+var_4C]
                      vmovss xmm2, [ebp+var_24]
                      vxorps xmm3, xmm3, xmm3
                    }
                  }
                  v11 = dword_103F5268; /*0x10157952*/
                  if ( dword_103F5268 != 1 ) /*0x1015795a*/
                    goto LABEL_125; /*0x1015795a*/
                  LOBYTE(v11) = v126 != 0; /*0x10157962*/
                  if ( v126 != 0 && v128 != 0 ) /*0x10157967*/
                  {
                    __asm { vmaxss xmm1, xmm2, xmm1 } /*0x1015797a*/
                    v458 = v452 - dword_103F5234 / 2; /*0x1015798c*/
                    __asm { vcvtsi2ss xmm0, xmm4, eax } /*0x10157992*/
                    v155 = HIDWORD(v452) + dword_103F5234 / 2; /*0x10157999*/
                    v456 = v155 + 2; /*0x1015799e*/
                    __asm { vucomiss xmm1, xmm3 } /*0x101579a1*/
                    v451 = v126; /*0x101579a5*/
                    __asm { vmovss dword ptr [ebp+var_4C], xmm1 } /*0x101579a8*/
                    if ( v155 != -2 || __SETP__(0, 0) ) /*0x101579b3*/
                    {
                      __asm /*0x10157d64*/
                      {
                        vdivss xmm1, xmm2, xmm1
                        vmaxss xmm2, xmm3, xmm1
                        vmovss xmm4, ds:dword_10184184
                        vcmpltss xmm1, xmm4, xmm1
                        vbroadcastss xmm4, ds:dword_10184184
                        vblendvps xmm1, xmm2, xmm4, xmm1
                        vfmadd213ss xmm1, xmm0, xmm3
                      }
                    }
                    else
                    {
                      __asm /*0x101579b9*/
                      {
                        vcmpless xmm1, xmm1, xmm2
                        vandps xmm1, xmm1, xmm0
                      }
                    }
                    v462 = -1; /*0x10157d8d*/
                    __asm { vcvttss2si ecx, xmm1 } /*0x10157d94*/
                    v446 = _ECX; /*0x10157d98*/
                    v192 = v455; /*0x10157da1*/
                    LOBYTE(v390) = v455; /*0x10157da4*/
                    BYTE1(v390) = v443; /*0x10157dad*/
                    BYTE2(v390) = v449; /*0x10157db6*/
                    v193 = v454; /*0x10157dbc*/
                    HIBYTE(v390) = v454; /*0x10157dbf*/
                    v194 = v458; /*0x10157dc9*/
                    v437 = v458 - 1; /*0x10157dcf*/
                    sub_101777E0( /*0x10157de6*/
                      a1: v458 - 1,
                      a2: HIDWORD(v452) + dword_103F5234 / 2 + 1,
                      a3: dword_103F5234 + 2,
                      a4: 4,
                      a5: v390);
                    LOBYTE(v391) = v192; /*0x10157df1*/
                    BYTE1(v391) = v443; /*0x10157dfb*/
                    BYTE2(v391) = v449; /*0x10157e05*/
                    HIBYTE(v391) = v193; /*0x10157e0b*/
                    sub_10177780(a1: v194, a2: v456, a3: v446, a4: 2, a5: v391); /*0x10157e21*/
                    v11 = dword_103F5268; /*0x10157e26*/
                    __asm /*0x10157e2b*/
                    {
                      vxorps xmm3, xmm3, xmm3
                      vmovss xmm2, [ebp+var_24]
                    }
                    v126 = v451; /*0x10157e34*/
                    __asm { vmovss xmm1, dword ptr [ebp+var_4C] } /*0x10157e37*/
LABEL_125:
                    v51 = v453; /*0x10157e3c*/
                    if ( v11 != 2 || v126 == 0 ) /*0x10157e4a*/
                      goto LABEL_45; /*0x10157e4a*/
                    __asm { vucomiss xmm1, xmm3 } /*0x10157e50*/
                    v195 = dword_103F5234; /*0x10157e5c*/
                    __asm { vmaxss xmm0, xmm2, xmm1 } /*0x10157e62*/
                    v197 = v452 - dword_103F5234 / 2; /*0x10157e72*/
                    LODWORD(v450) = v197 - 4; /*0x10157e77*/
                    __asm { vcvtsi2ss xmm1, xmm5, esi } /*0x10157e7a*/
                    v199 = HIDWORD(v452) - dword_103F5234 / 2; /*0x10157e81*/
                    __asm { vucomiss xmm0, xmm3 } /*0x10157e83*/
                    if ( HIDWORD(v452) != dword_103F5234 / 2 || __SETP__(HIDWORD(v452), dword_103F5234 / 2) ) /*0x10157e8d*/
                    {
                      __asm /*0x10158014*/
                      {
                        vdivss xmm0, xmm2, xmm0
                        vmaxss xmm2, xmm3, xmm0
                        vmovss xmm4, ds:dword_10184184
                        vcmpltss xmm0, xmm4, xmm0
                        vbroadcastss xmm4, ds:dword_10184184
                        vblendvps xmm0, xmm2, xmm4, xmm0
                        vfmadd213ss xmm0, xmm1, xmm3
                      }
                    }
                    else
                    {
                      __asm /*0x10157e93*/
                      {
                        vcmpless xmm0, xmm0, xmm2
                        vandps xmm0, xmm0, xmm1
                      }
                    }
                    v462 = -1; /*0x1015803d*/
                    __asm { vcvttss2si ecx, xmm0 } /*0x10158044*/
                    v458 = _ECX; /*0x10158048*/
                    LOBYTE(v392) = v455; /*0x10158055*/
                    BYTE1(v392) = v443; /*0x1015805f*/
                    BYTE2(v392) = v449; /*0x10158069*/
                    HIBYTE(v392) = v454; /*0x10158073*/
                    v456 = v199 - 1; /*0x1015807f*/
                    sub_101777E0(a1: v197 - 5, a2: v199 - 1, a3: 4, a4: dword_103F5234 + 2, a5: v392); /*0x10158093*/
                    LOBYTE(v393) = v455; /*0x1015809c*/
                    BYTE1(v393) = v443; /*0x101580a6*/
                    BYTE2(v393) = v449; /*0x101580b0*/
                    HIBYTE(v393) = v454; /*0x101580ba*/
                    v226 = v458; /*0x101580c8*/
                    v227 = v195 + v199 - v458; /*0x101580cb*/
                    v228 = v393; /*0x101580cd*/
                    goto LABEL_143; /*0x101580d3*/
                  }
                  goto LABEL_44; /*0x1015796f*/
                }
                if ( byte_103F5228 != 1 ) /*0x10157216*/
                  goto LABEL_44; /*0x10157216*/
                LOBYTE(v11) = dword_103F522C; /*0x1015721c*/
                if ( dword_103F522C != 1 ) /*0x10157224*/
                {
                  if ( dword_103F522C != 0 ) /*0x1015722c*/
                    goto LABEL_44; /*0x1015722c*/
                  LODWORD(v443) = dword_103F5230; /*0x1015723c*/
                  v130 = dword_103F5230 / 2; /*0x10157241*/
                  __asm { vmovss [ebp+var_24], xmm2 } /*0x1015724a*/
                  if ( dword_103F5224 != 0 ) /*0x1015724f*/
                  {
                    if ( dword_103F5224 == 1 ) /*0x10157258*/
                    {
                      LOBYTE(v394) = v430; /*0x1015726a*/
                      BYTE1(v394) = v431; /*0x10157276*/
                      BYTE2(v394) = v432; /*0x10157282*/
                      HIBYTE(v394) = -1; /*0x10157288*/
                      v456 = HIDWORD(v452) - v130; /*0x10157296*/
                      v458 = v128; /*0x10157299*/
                      v451 = v126; /*0x1015729c*/
                      v131 = v452 - v130; /*0x101572a2*/
                      v462 = -1; /*0x101572a4*/
                      v132 = v443; /*0x101572b2*/
                      __asm { vmovss dword ptr [ebp+var_4C], xmm1 } /*0x101572b9*/
                      sub_10177780(a1: v452 - v130, a2: HIDWORD(v452) - v130, a3: v443, a4: v443, a5: v394); /*0x101572be*/
                      v352 = -16119286; /*0x101572c9*/
                      v326 = v132; /*0x101572dc*/
                      v323 = v132; /*0x101572dd*/
                      v309 = v131; /*0x101572e1*/
                      v126 = v451; /*0x101572e2*/
                      v128 = v458; /*0x101572e5*/
                      sub_10177840(a1: v309, a2: v456, a3: v323, a4: v326, a5: 10, a6: 1); /*0x101572e8*/
                      goto LABEL_89; /*0x101572ed*/
                    }
                  }
                  else
                  {
                    v395 = v430; /*0x10157730*/
                    v396 = v431; /*0x1015773c*/
                    v397 = v432; /*0x10157748*/
                    v398 = -1; /*0x1015774e*/
                    v456 = HIDWORD(v452); /*0x10157768*/
                    v458 = v452; /*0x1015776e*/
                    v462 = -1; /*0x10157771*/
                    __asm { vmovss dword ptr [ebp+var_4C], xmm1 } /*0x10157788*/
                    sub_10178BD0( /*0x1015778d*/
                      a1: v452,
                      a2: HIDWORD(v452),
                      a3: (int)(v443 + ((unsigned int)(v443 + 4) >> 31) + 4) >> 1,
                      a4: 20,
                      a5: v430);
                    v353 = -16119286; /*0x10157798*/
                    HIDWORD(v317) = 20; /*0x101577bb*/
                    DWORD2(v317) = (int)(((unsigned int)(dword_103F5230 + 4) >> 31) + dword_103F5230 + 4) >> 1; /*0x101577bd*/
                    *(_QWORD *)&v317 = __PAIR64__(v456, v458); /*0x101577c1*/
                    sub_10178B80(a1: v317, a2: -16119286); /*0x101577c4*/
LABEL_89:
                    __asm /*0x101577c9*/
                    {
                      vmovss xmm1, dword ptr [ebp+var_4C]
                      vmovss xmm2, [ebp+var_24]
                      vxorps xmm3, xmm3, xmm3
                    }
                  }
                  v11 = dword_103F5254; /*0x101577d7*/
                  if ( dword_103F5254 == 1 ) /*0x101577df*/
                  {
                    LOBYTE(v11) = v126 != 0; /*0x101577e7*/
                    if ( v126 == 0 || v128 == 0 ) /*0x101577ec*/
                      goto LABEL_44; /*0x101577f4*/
                    __asm { vmaxss xmm1, xmm2, xmm1 } /*0x101577fa*/
                    v149 = v452 - v130; /*0x10157801*/
                    __asm { vcvtsi2ss xmm0, xmm4, dword ptr [ebp+var_78] } /*0x10157806*/
                    v458 = HIDWORD(v452) + v130 + 2; /*0x10157811*/
                    __asm { vucomiss xmm1, xmm3 } /*0x10157816*/
                    v451 = v126; /*0x1015781a*/
                    __asm { vmovss dword ptr [ebp+var_4C], xmm1 } /*0x1015781d*/
                    if ( v130 + HIDWORD(v452) != 0 || __SETP__(v130 + HIDWORD(v452), 0) ) /*0x10157828*/
                    {
                      __asm /*0x10157c79*/
                      {
                        vdivss xmm1, xmm2, xmm1
                        vmaxss xmm2, xmm3, xmm1
                        vmovss xmm4, ds:dword_10184184
                        vcmpltss xmm1, xmm4, xmm1
                        vbroadcastss xmm4, ds:dword_10184184
                        vblendvps xmm1, xmm2, xmm4, xmm1
                        vfmadd213ss xmm1, xmm0, xmm3
                      }
                    }
                    else
                    {
                      __asm /*0x1015782e*/
                      {
                        vcmpless xmm1, xmm1, xmm2
                        vandps xmm1, xmm1, xmm0
                      }
                    }
                    v462 = -1; /*0x10157ca2*/
                    __asm { vcvttss2si ecx, xmm1 } /*0x10157ca9*/
                    v456 = _ECX; /*0x10157cad*/
                    v341 = v442; /*0x10157cb9*/
                    v449 = v443 + 2; /*0x10157cc5*/
                    sub_101777E0(a1: v149 - 1, a2: v130 + HIDWORD(v452) + 1, a3: v443 + 2, a4: 4, a5: v442); /*0x10157cd4*/
                    v342 = v442; /*0x10157ce2*/
                    sub_10177780(a1: v149, a2: v458, a3: v456, a4: 2, a5: v442); /*0x10157cf2*/
                    v11 = dword_103F5254; /*0x10157cf7*/
                    __asm /*0x10157cfc*/
                    {
                      vxorps xmm3, xmm3, xmm3
                      vmovss xmm2, [ebp+var_24]
                    }
                    v126 = v451; /*0x10157d05*/
                    __asm { vmovss xmm1, dword ptr [ebp+var_4C] } /*0x10157d08*/
                  }
                  v51 = v453; /*0x10157d10*/
                  if ( v11 != 2 || v126 == 0 ) /*0x10157d1b*/
                    goto LABEL_45; /*0x10157d1b*/
                  __asm { vucomiss xmm1, xmm3 } /*0x10157d21*/
                  __asm { vmaxss xmm0, xmm2, xmm1 }
                  LODWORD(v450) = v452 - v130 - 4; /*0x10157d39*/
                  __asm { vcvtsi2ss xmm1, xmm5, dword ptr [ebp+var_78] } /*0x10157d3c*/
                  v184 = HIDWORD(v452) - v130; /*0x10157d44*/
                  __asm { vucomiss xmm0, xmm3 } /*0x10157d46*/
                  if ( HIDWORD(v452) != v130 || __SETP__(HIDWORD(v452), v130) ) /*0x10157d50*/
                  {
                    __asm /*0x10157f92*/
                    {
                      vdivss xmm0, xmm2, xmm0
                      vmaxss xmm2, xmm3, xmm0
                      vmovss xmm4, ds:dword_10184184
                      vcmpltss xmm0, xmm4, xmm0
                      vbroadcastss xmm4, ds:dword_10184184
                      vblendvps xmm0, xmm2, xmm4, xmm0
                      vfmadd213ss xmm0, xmm1, xmm3
                    }
                  }
                  else
                  {
                    __asm /*0x10157d56*/
                    {
                      vcmpless xmm0, xmm0, xmm2
                      vandps xmm0, xmm0, xmm1
                    }
                  }
                  v462 = -1; /*0x10157fbb*/
                  __asm { vcvttss2si ecx, xmm0 } /*0x10157fc2*/
                  v458 = _ECX; /*0x10157fc6*/
                  v343 = v442; /*0x10157fd2*/
                  v219 = v443; /*0x10157fd8*/
                  v457 = (char *)(v443 + 2); /*0x10157fde*/
                  v456 = v184 - 1; /*0x10157fe4*/
                  sub_101777E0(a1: v452 - v130 - 5, a2: v184 - 1, a3: 4, a4: v443 + 2, a5: v442); /*0x10157ff4*/
                  v100 = v442; /*0x10157fff*/
                  v344 = v442; /*0x10158002*/
                  v98 = v458; /*0x1015800a*/
                  v99 = v219 + v184 - v458; /*0x1015800d*/
LABEL_43:
                  LOBYTE(v11) = sub_10177780(a1: v450, a2: v99, a3: 2, a4: v98, a5: v100); /*0x10156ec2*/
                  goto LABEL_44; /*0x10156eca*/
                }
                __asm { vmovss [ebp+var_24], xmm2 } /*0x10157507*/
                if ( dword_103F5224 != 0 ) /*0x1015750c*/
                {
                  if ( dword_103F5224 != 1 ) /*0x10157515*/
                    goto LABEL_104; /*0x10157515*/
                  v458 = v128; /*0x1015751b*/
                  v451 = v126; /*0x1015751e*/
                  v140 = dword_103F5234; /*0x10157521*/
                  v141 = dword_103F5234 / 2; /*0x1015752e*/
                  v462 = -1; /*0x10157536*/
                  v329 = v450; /*0x1015753d*/
                  __asm { vmovss dword ptr [ebp+var_4C], xmm1 } /*0x10157540*/
                  LODWORD(v443) = sub_1016B010(a1: v329); /*0x1015754a*/
                  v449 = HIDWORD(v452); /*0x10157550*/
                  v456 = HIDWORD(v452) - v141; /*0x10157557*/
                  v142 = v452; /*0x1015755a*/
                  v143 = v452 - v141; /*0x10157561*/
                  LOBYTE(v399) = v362; /*0x10157569*/
                  BYTE1(v399) = v363; /*0x10157575*/
                  BYTE2(v399) = v364; /*0x10157581*/
                  HIBYTE(v399) = -1; /*0x10157587*/
                  sub_10177780(a1: v143, a2: v456, a3: v140, a4: v140, a5: v399); /*0x1015759f*/
                  sub_10178EB0(a1: v142, a2: v449, a3: dword_103F5234, a4: dword_103F5234, a5: v443, a6: 24); /*0x101575ba*/
                  v400 = v430; /*0x101575cb*/
                  v401 = v431; /*0x101575d7*/
                  v402 = v432; /*0x101575e3*/
                  v403 = -1; /*0x101575e9*/
                  v327 = v140; /*0x101575f9*/
                  v324 = v140; /*0x101575fa*/
                  v126 = v451; /*0x101575fb*/
                  v128 = v458; /*0x101575fe*/
                  sub_10177840(a1: v143, a2: v456, a3: v324, a4: v327, a5: v430, a6: 1); /*0x10157605*/
                }
                else
                {
                  v462 = -1; /*0x101579cd*/
                  v330 = v450; /*0x101579d4*/
                  __asm { vmovss dword ptr [ebp+var_4C], xmm1 } /*0x101579d7*/
                  LODWORD(v443) = sub_1016B010(a1: v330); /*0x101579e1*/
                  v404 = v362; /*0x101579f0*/
                  v405 = v363; /*0x101579fc*/
                  v406 = v364; /*0x10157a08*/
                  v407 = -1; /*0x10157a0e*/
                  v158 = HIDWORD(v452); /*0x10157a27*/
                  v458 = v128; /*0x10157a2a*/
                  v159 = v126; /*0x10157a2d*/
                  v160 = v452; /*0x10157a2f*/
                  sub_10178BD0( /*0x10157a3e*/
                    a1: v452,
                    a2: HIDWORD(v452),
                    a3: (int)(((unsigned int)(dword_103F5234 + 4) >> 31) + dword_103F5234 + 4) >> 1,
                    a4: 20,
                    a5: v362);
                  sub_10178EB0(a1: v160, a2: v158, a3: dword_103F5234, a4: dword_103F5234, a5: v443, a6: 24); /*0x10157a57*/
                  LOBYTE(v408) = v430; /*0x10157a78*/
                  BYTE1(v408) = v431; /*0x10157a84*/
                  BYTE2(v408) = v432; /*0x10157a90*/
                  HIBYTE(v408) = -1; /*0x10157a96*/
                  HIDWORD(v319) = 20; /*0x10157aa6*/
                  DWORD2(v319) = (int)(((unsigned int)(dword_103F5234 + 4) >> 31) + dword_103F5234 + 4) >> 1; /*0x10157aa8*/
                  *(_QWORD *)&v319 = __PAIR64__(v158, v160); /*0x10157aaa*/
                  v126 = v159; /*0x10157aab*/
                  v128 = v458; /*0x10157aad*/
                  sub_10178B80(a1: v319, a2: v408); /*0x10157ab0*/
                }
                __asm /*0x10157ab5*/
                {
                  vmovss xmm1, dword ptr [ebp+var_4C]
                  vmovss xmm2, [ebp+var_24]
                  vxorps xmm3, xmm3, xmm3
                }
LABEL_104:
                v11 = dword_103F5254; /*0x10157ac3*/
                if ( dword_103F5254 != 1 ) /*0x10157acb*/
                  goto LABEL_132; /*0x10157acb*/
                LOBYTE(v11) = v126 != 0; /*0x10157ad3*/
                if ( v126 != 0 && v128 != 0 ) /*0x10157ad8*/
                {
                  __asm { vmaxss xmm1, xmm2, xmm1 } /*0x10157aeb*/
                  v162 = v452 - dword_103F5234 / 2; /*0x10157afb*/
                  __asm { vcvtsi2ss xmm0, xmm4, eax } /*0x10157b00*/
                  v163 = HIDWORD(v452) + dword_103F5234 / 2; /*0x10157b07*/
                  v458 = v163 + 2; /*0x10157b0c*/
                  __asm { vucomiss xmm1, xmm3 } /*0x10157b0f*/
                  v451 = v126; /*0x10157b13*/
                  __asm { vmovss dword ptr [ebp+var_4C], xmm1 } /*0x10157b16*/
                  if ( v163 != -2 || __SETP__(0, 0) ) /*0x10157b21*/
                  {
                    __asm /*0x10157ea1*/
                    {
                      vdivss xmm1, xmm2, xmm1
                      vmaxss xmm2, xmm3, xmm1
                      vmovss xmm4, ds:dword_10184184
                      vcmpltss xmm1, xmm4, xmm1
                      vbroadcastss xmm4, ds:dword_10184184
                      vblendvps xmm1, xmm2, xmm4, xmm1
                      vfmadd213ss xmm1, xmm0, xmm3
                    }
                  }
                  else
                  {
                    __asm /*0x10157b27*/
                    {
                      vcmpless xmm1, xmm1, xmm2
                      vandps xmm1, xmm1, xmm0
                    }
                  }
                  v462 = -1; /*0x10157eca*/
                  __asm { vcvttss2si ecx, xmm1 } /*0x10157ed1*/
                  LODWORD(v443) = _ECX; /*0x10157ed5*/
                  v345 = v442; /*0x10157ee1*/
                  sub_101777E0( /*0x10157ef4*/
                    a1: v162 - 1,
                    a2: HIDWORD(v452) + dword_103F5234 / 2 + 1,
                    a3: dword_103F5234 + 2,
                    a4: 4,
                    a5: v442);
                  v346 = v442; /*0x10157f02*/
                  sub_10177780(a1: v162, a2: v458, a3: v443, a4: 2, a5: v442); /*0x10157f12*/
                  v11 = dword_103F5254; /*0x10157f17*/
                  __asm /*0x10157f1c*/
                  {
                    vxorps xmm3, xmm3, xmm3
                    vmovss xmm2, [ebp+var_24]
                  }
                  v126 = v451; /*0x10157f25*/
                  __asm { vmovss xmm1, dword ptr [ebp+var_4C] } /*0x10157f28*/
LABEL_132:
                  v51 = v453; /*0x10157f2d*/
                  if ( v11 != 2 || v126 == 0 ) /*0x10157f3b*/
                    goto LABEL_45; /*0x10157f3b*/
                  __asm { vucomiss xmm1, xmm3 } /*0x10157f41*/
                  v207 = dword_103F5234; /*0x10157f4d*/
                  __asm { vmaxss xmm0, xmm2, xmm1 } /*0x10157f53*/
                  v209 = v452 - dword_103F5234 / 2; /*0x10157f63*/
                  LODWORD(v450) = v209 - 4; /*0x10157f68*/
                  __asm { vcvtsi2ss xmm1, xmm5, esi } /*0x10157f6b*/
                  v211 = HIDWORD(v452) - dword_103F5234 / 2; /*0x10157f72*/
                  __asm { vucomiss xmm0, xmm3 } /*0x10157f74*/
                  if ( HIDWORD(v452) != dword_103F5234 / 2 || __SETP__(HIDWORD(v452), dword_103F5234 / 2) ) /*0x10157f7e*/
                  {
                    __asm /*0x101580d5*/
                    {
                      vdivss xmm0, xmm2, xmm0
                      vmaxss xmm2, xmm3, xmm0
                      vmovss xmm4, ds:dword_10184184
                      vcmpltss xmm0, xmm4, xmm0
                      vbroadcastss xmm4, ds:dword_10184184
                      vblendvps xmm0, xmm2, xmm4, xmm0
                      vfmadd213ss xmm0, xmm1, xmm3
                    }
                  }
                  else
                  {
                    __asm /*0x10157f84*/
                    {
                      vcmpless xmm0, xmm0, xmm2
                      vandps xmm0, xmm0, xmm1
                    }
                  }
                  v462 = -1; /*0x101580fe*/
                  __asm { vcvttss2si ecx, xmm0 } /*0x10158105*/
                  v458 = _ECX; /*0x10158109*/
                  v347 = v442; /*0x10158115*/
                  v456 = dword_103F5234 + 2; /*0x1015811e*/
                  LODWORD(v443) = v211 - 1; /*0x10158124*/
                  sub_101777E0(a1: v209 - 5, a2: v211 - 1, a3: 4, a4: dword_103F5234 + 2, a5: v442); /*0x10158134*/
                  v228 = v442; /*0x1015813f*/
                  v348 = v442; /*0x10158142*/
                  v226 = v458; /*0x1015814a*/
                  v227 = v207 + v211 - v458; /*0x1015814d*/
LABEL_143:
                  LOBYTE(v11) = sub_10177780(a1: v450, a2: v227, a3: 2, a4: v226, a5: v228); /*0x1015814f*/
                }
LABEL_44:
                v51 = v453; /*0x10156ecf*/
LABEL_45:
                ++v51; /*0x10156ee0*/
              }
              while ( v51 != v444 ); /*0x10156eec*/
            }
          }
          if ( byte_103F5210 == 1 ) /*0x10158349*/
          {
            v235 = *(void (__thiscall **)(int, int))(*(_DWORD *)dword_103EC728 + 420); /*0x10158357*/
            v462 = -1; /*0x1015835d*/
            v235(a1: dword_103EC728, a2: 1065353216); /*0x10158369*/
            v11 = (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC7B8 + 32))(a1: dword_103EC7B8); /*0x10158373*/
            if ( v11 > 0 ) /*0x10158378*/
            {
              __asm /*0x1015837e*/
              {
                vcvttss2si eax, [ebp+var_A4]
                vcvttss2si ecx, [ebp+var_8C]
                vcvttss2si edx, [ebp+var_90]
              }
              LODWORD(v443) = _EAX; /*0x10158396*/
              __asm /*0x1015839c*/
              {
                vxorps xmm7, xmm7, xmm7
                vcvtsi2ss xmm0, xmm7, eax
                vmovss xmm2, ds:dword_101863C8
                vmulss xmm0, xmm0, xmm2
                vcvttss2si eax, xmm0
              }
              v429 = _EAX; /*0x101583b4*/
              v434 = _ECX; /*0x101583ba*/
              __asm /*0x101583c3*/
              {
                vxorps xmm7, xmm7, xmm7
                vcvtsi2ss xmm0, xmm7, eax
                vmulss xmm0, xmm0, xmm2
              }
              v435 = _EDX; /*0x101583cf*/
              __asm /*0x101583d8*/
              {
                vxorps xmm7, xmm7, xmm7
                vcvtsi2ss xmm1, xmm7, eax
                vmulss xmm1, xmm1, xmm2
              }
              v250 = 0; /*0x101583e4*/
              __asm { vcvttss2si eax, xmm0 } /*0x101583e6*/
              v456 = _EAX; /*0x101583ea*/
              __asm { vcvttss2si eax, xmm1 } /*0x101583ed*/
              v453 = _EAX; /*0x101583f1*/
              do /*0x1015842c*/
              {
                v254 = (*(int (__thiscall **)(int, int))(*(_DWORD *)dword_103EC7B8 + 12))( /*0x10158436*/
                         a1: dword_103EC7B8,
                         a2: ++v250);
                if ( v254 == 0 ) /*0x1015843b*/
                  goto LABEL_160; /*0x1015843b*/
                v255 = (char *)v254; /*0x1015843d*/
                v256 = v254 + 8; /*0x10158441*/
                if ( (*(unsigned __int8 (__thiscall **)(int))(*(_DWORD *)(v254 + 8) + 28))(a1: v254 + 8) != 0 ) /*0x1015844e*/
                  goto LABEL_160; /*0x1015844e*/
                v257 = (*(int (__thiscall **)(int))(*(_DWORD *)v256 + 4))(a1: v256); /*0x10158454*/
                if ( v257 == 0 ) /*0x10158459*/
                  goto LABEL_160; /*0x10158459*/
                v258 = v257; /*0x1015845b*/
                (*(void (__thiscall **)(char *))(*(_DWORD *)v255 + 44))(a1: v255); /*0x10158461*/
                v259 = *(_DWORD *)(v258 + 20); /*0x10158464*/
                if ( v259 == 264 ) /*0x1015846c*/
                {
                  if ( dword_103EEF14 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x101586b4*/
                  {
                    sub_101809C3(a1: &dword_103EEF14); /*0x10158baa*/
                    if ( dword_103EEF14 == -1 ) /*0x10158bb9*/
                    {
                      LODWORD(v450) = v258; /*0x10158bbf*/
                      v457 = v255; /*0x10158bc2*/
                      v458 = v250; /*0x10158bc5*/
                      v462 = 9; /*0x10158bc8*/
                      dword_103EEF10 = sub_1017C730(a1: "CBaseEntity", a2: "m_usSolidFlags"); /*0x10158be1*/
                      sub_10180A43(a1: &dword_103EEF14); /*0x10158beb*/
                      v250 = v458; /*0x10158bf3*/
                      v255 = v457; /*0x10158bf6*/
                      v258 = v450; /*0x10158bf9*/
                    }
                  }
                  v277 = *(_WORD *)&v255[dword_103EEF10]; /*0x101586bf*/
                  v462 = -1; /*0x101586c3*/
                  if ( (v277 & 4) != 0 ) /*0x101586cc*/
                    goto LABEL_160; /*0x101586cc*/
                  if ( *(int *)sub_1002A740(a1: v255) > 305 ) /*0x101586df*/
                    goto LABEL_160; /*0x101586df*/
                  LODWORD(v450) = off_1029E57C; /*0x101586ea*/
                  v458 = v250; /*0x101586ed*/
                  v278 = *(_DWORD *)(v258 + 20); /*0x101586f0*/
                  v279 = *(_DWORD *)sub_1002A740(a1: v255); /*0x101586fa*/
                  v280 = (_WORD *)sub_1002A670(a1: v255); /*0x101586fe*/
                  v331 = v278; /*0x10158709*/
                  v250 = v458; /*0x1015870a*/
                  if ( (unsigned __int8)sub_101669A0(a1: *v280, a2: v279, a3: v331) == 0 ) /*0x10158716*/
                    goto LABEL_160; /*0x10158716*/
                  v452 = 0; /*0x1015871c*/
                  sub_1002A190(a1: &v447); /*0x10158730*/
                  if ( (unsigned __int8)sub_10156560(a1: &v452, a2: (char *)&v452 + 4, a3: &v447) == 0 ) /*0x10158745*/
                    goto LABEL_160; /*0x10158745*/
                  __asm /*0x1015874b*/
                  {
                    vxorps xmm0, xmm0, xmm0
                    vmovsd [ebp+var_5C], xmm0
                  }
                  v448 = 0; /*0x10158754*/
                  sub_1002A190(a1: &v367); /*0x10158764*/
                  sub_10177160(a1: &v367, a2: &v447); /*0x10158774*/
                  sub_1002A190(a1: &v368); /*0x10158783*/
                  __asm /*0x10158788*/
                  {
                    vmovss xmm0, [ebp+var_118]
                    vsubss xmm0, xmm0, [ebp+var_124]
                    vmovss xmm1, [ebp+var_11C]
                    vsubss xmm1, xmm1, [ebp+var_128]
                    vmovss xmm2, [ebp+var_120]
                    vsubss xmm2, xmm2, [ebp+var_12C]
                    vmulss xmm1, xmm1, xmm1
                    vfmadd231ss xmm1, xmm2, xmm2
                    vfmadd231ss xmm1, xmm0, xmm0
                    vcvtss2sd xmm0, xmm1, xmm1
                    vsqrtsd xmm0, xmm0, xmm0
                    vcvtsd2ss xmm0, xmm0, xmm0
                    vucomiss xmm0, dword_103F5214
                  }
                  if ( !(v116 | v48) || byte_103F5238 != 1 ) /*0x101587e7*/
                    goto LABEL_160; /*0x101587e7*/
                  __asm /*0x101587ed*/
                  {
                    vmovss xmm1, ds:dword_10184380
                    vmulss xmm0, xmm1, dword_103F5244
                    vcvttss2si edx, xmm0
                    vmulss xmm0, xmm1, dword_103F5248
                    vcvttss2si eax, xmm0
                    vmulss xmm0, xmm1, dword_103F524C
                    vcvttss2si ebx, xmm0
                  }
                  if ( dword_103F5240 == 0 ) /*0x10158821*/
                  {
                    v425 = _EDX; /*0x10158975*/
                    v426 = _EAX; /*0x1015897b*/
                    v427 = _EBX; /*0x10158981*/
                    v428 = -1; /*0x10158987*/
                    v303 = v452; /*0x101589a3*/
                    sub_10178BD0( /*0x101589b2*/
                      a1: v452,
                      a2: HIDWORD(v452),
                      a3: (int)(((unsigned int)(dword_103F523C + 4) >> 31) + dword_103F523C + 4) >> 1,
                      a4: 20,
                      a5: _EDX);
                    v357 = -16119286; /*0x101589bd*/
                    v304 = (int)(((unsigned int)(dword_103F523C + 4) >> 31) + dword_103F523C + 4) >> 1; /*0x101589d7*/
LABEL_194:
                    *((_QWORD *)&v321 + 1) = (unsigned int)v304 | 0x1400000000LL; /*0x10158a66*/
                    *(_QWORD *)&v321 = v303; /*0x10158a6b*/
                    v250 = v458; /*0x10158a6c*/
                    sub_10178B80(a1: v321, a2: -16119286); /*0x10158a6f*/
                    goto LABEL_160; /*0x10158a6f*/
                  }
                  if ( dword_103F5240 == 1 ) /*0x1015882a*/
                  {
                    v274 = dword_103F523C; /*0x10158830*/
                    LOBYTE(v424) = _EDX; /*0x10158836*/
                    BYTE1(v424) = _EAX; /*0x10158845*/
                    BYTE2(v424) = _EBX; /*0x1015884b*/
                    v275 = HIDWORD(v452) - dword_103F523C / 2; /*0x10158854*/
                    v276 = v452 - dword_103F523C / 2; /*0x10158859*/
                    HIBYTE(v424) = -1; /*0x10158861*/
                    sub_10177780(a1: v276, a2: v275, a3: dword_103F523C, a4: dword_103F523C, a5: v424); /*0x10158873*/
                    v356 = -16119286; /*0x1015887e*/
                    goto LABEL_159; /*0x1015888e*/
                  }
                }
                else
                {
                  if ( v259 != 277 ) /*0x10158477*/
                    goto LABEL_160; /*0x10158477*/
                  if ( dword_103EEF14 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10158494*/
                  {
                    sub_101809C3(a1: &dword_103EEF14); /*0x10158b4e*/
                    if ( dword_103EEF14 == -1 ) /*0x10158b5d*/
                    {
                      LODWORD(v450) = v258; /*0x10158b63*/
                      v457 = v255; /*0x10158b66*/
                      v458 = v250; /*0x10158b69*/
                      v462 = 6; /*0x10158b6c*/
                      dword_103EEF10 = sub_1017C730(a1: "CBaseEntity", a2: "m_usSolidFlags"); /*0x10158b85*/
                      sub_10180A43(a1: &dword_103EEF14); /*0x10158b8f*/
                      v250 = v458; /*0x10158b97*/
                      v255 = v457; /*0x10158b9a*/
                      v258 = v450; /*0x10158b9d*/
                    }
                  }
                  if ( *(_WORD *)&v255[dword_103EEF10] == 0 ) /*0x101584a4*/
                    goto LABEL_160; /*0x101584a4*/
                  LODWORD(v450) = off_1029E57C; /*0x101584af*/
                  v444 = *(int **)(v258 + 20); /*0x101584b5*/
                  if ( dword_103EEF1C > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x101584d3*/
                  {
                    sub_101809C3(a1: &dword_103EEF1C); /*0x10158c06*/
                    if ( dword_103EEF1C == -1 ) /*0x10158c15*/
                    {
                      v457 = v255; /*0x10158c1b*/
                      v458 = v250; /*0x10158c1e*/
                      v462 = 7; /*0x10158c21*/
                      dword_103EEF18 = sub_1017C730(a1: "CBaseAnimating", a2: "m_nSequence"); /*0x10158c3a*/
                      sub_10180A43(a1: &dword_103EEF1C); /*0x10158c44*/
                      v250 = v458; /*0x10158c4c*/
                      v255 = v457; /*0x10158c4f*/
                    }
                  }
                  v260 = *(_DWORD *)&v255[dword_103EEF18]; /*0x101584de*/
                  if ( dword_103EEF14 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x101584fc*/
                  {
                    sub_101809C3(a1: &dword_103EEF14); /*0x10158c5c*/
                    if ( dword_103EEF14 == -1 ) /*0x10158c6b*/
                    {
                      LODWORD(v361) = v260; /*0x10158c71*/
                      v457 = v255; /*0x10158c77*/
                      v458 = v250; /*0x10158c7a*/
                      v462 = 8; /*0x10158c7d*/
                      dword_103EEF10 = sub_1017C730(a1: "CBaseEntity", a2: "m_usSolidFlags"); /*0x10158c96*/
                      sub_10180A43(a1: &dword_103EEF14); /*0x10158ca0*/
                      v250 = v458; /*0x10158ca8*/
                      v255 = v457; /*0x10158cab*/
                      v260 = v361; /*0x10158cae*/
                    }
                  }
                  v261 = *(_WORD *)&v255[dword_103EEF10]; /*0x10158507*/
                  v462 = -1; /*0x1015850b*/
                  if ( (unsigned __int8)sub_101669A0(a1: v261, a2: v260, a3: v444) == 0 ) /*0x10158521*/
                    goto LABEL_160; /*0x10158521*/
                  v452 = 0; /*0x10158527*/
                  sub_1002A190(a1: &v447); /*0x1015853b*/
                  if ( (unsigned __int8)sub_10156560(a1: &v452, a2: (char *)&v452 + 4, a3: &v447) == 0 ) /*0x10158550*/
                    goto LABEL_160; /*0x10158550*/
                  __asm /*0x10158556*/
                  {
                    vxorps xmm0, xmm0, xmm0
                    vmovsd [ebp+var_5C], xmm0
                  }
                  v448 = 0; /*0x1015855f*/
                  sub_1002A190(a1: &v367); /*0x1015856f*/
                  sub_10177160(a1: &v367, a2: &v447); /*0x1015857f*/
                  sub_1002A190(a1: &v368); /*0x1015858e*/
                  __asm /*0x10158593*/
                  {
                    vmovss xmm0, [ebp+var_118]
                    vsubss xmm0, xmm0, [ebp+var_124]
                    vmovss xmm1, [ebp+var_11C]
                    vsubss xmm1, xmm1, [ebp+var_128]
                    vmovss xmm2, [ebp+var_120]
                    vsubss xmm2, xmm2, [ebp+var_12C]
                    vmulss xmm1, xmm1, xmm1
                    vfmadd231ss xmm1, xmm2, xmm2
                    vfmadd231ss xmm1, xmm0, xmm0
                    vcvtss2sd xmm0, xmm1, xmm1
                    vsqrtsd xmm0, xmm0, xmm0
                    vcvtsd2ss xmm0, xmm0, xmm0
                    vucomiss xmm0, dword_103F5214
                  }
                  if ( !(v116 | v48) || byte_103F5229 != 1 ) /*0x101585f2*/
                    goto LABEL_160; /*0x101585f2*/
                  if ( dword_103F522C == 1 ) /*0x10158600*/
                  {
                    if ( dword_103F5224 != 0 ) /*0x1015889a*/
                    {
                      if ( dword_103F5224 == 1 ) /*0x101588a3*/
                      {
                        v458 = v250; /*0x101588a9*/
                        v299 = dword_103F5234; /*0x101588ac*/
                        LODWORD(v450) = HIDWORD(v452); /*0x101588be*/
                        v300 = HIDWORD(v452) - dword_103F5234 / 2; /*0x101588c1*/
                        v444 = (int *)v452; /*0x101588c6*/
                        v301 = v452 - dword_103F5234 / 2; /*0x101588c9*/
                        LOBYTE(v414) = v429; /*0x101588d1*/
                        BYTE1(v414) = v456; /*0x101588da*/
                        BYTE2(v414) = (_BYTE)v453; /*0x101588e3*/
                        HIBYTE(v414) = -1; /*0x101588e9*/
                        sub_10177780(a1: v301, a2: v300, a3: dword_103F5234, a4: dword_103F5234, a5: v414); /*0x10158901*/
                        v457 = *(char **)ArgList; /*0x1015890b*/
                        v302 = sub_10168030(a1: off_1029E57C); /*0x10158914*/
                        sub_10178EB0(a1: v444, a2: v450, a3: v299, a4: v299, a5: v302, a6: 24); /*0x10158927*/
                        v415 = v443; /*0x10158935*/
                        v416 = v434; /*0x10158941*/
                        v417 = v435; /*0x1015894d*/
                        v418 = -1; /*0x10158953*/
                        v328 = v299; /*0x10158963*/
                        v325 = v299; /*0x10158964*/
                        v250 = v458; /*0x10158965*/
                        sub_10177840(a1: v301, a2: v300, a3: v325, a4: v328, a5: v443, a6: 1); /*0x1015896a*/
                      }
                    }
                    else
                    {
                      v419 = v429; /*0x10158a80*/
                      v420 = v456; /*0x10158a89*/
                      v421 = (char)v453; /*0x10158a92*/
                      v422 = -1; /*0x10158a98*/
                      v458 = v250; /*0x10158ab1*/
                      v305 = HIDWORD(v452); /*0x10158ab4*/
                      v306 = v452; /*0x10158ab7*/
                      sub_10178BD0( /*0x10158ac6*/
                        a1: v452,
                        a2: HIDWORD(v452),
                        a3: (int)(((unsigned int)(dword_103F5234 + 4) >> 31) + dword_103F5234 + 4) >> 1,
                        a4: 20,
                        a5: v429);
                      v307 = sub_10168030(a1: off_1029E57C); /*0x10158ad7*/
                      sub_10178EB0(a1: v306, a2: v305, a3: dword_103F5234, a4: dword_103F5234, a5: v307, a6: 24); /*0x10158aeb*/
                      LOBYTE(v423) = v443; /*0x10158b09*/
                      BYTE1(v423) = v434; /*0x10158b15*/
                      BYTE2(v423) = v435; /*0x10158b21*/
                      HIBYTE(v423) = -1; /*0x10158b27*/
                      HIDWORD(v322) = 20; /*0x10158b37*/
                      DWORD2(v322) = (int)(((unsigned int)(dword_103F5234 + 4) >> 31) + dword_103F5234 + 4) >> 1; /*0x10158b39*/
                      DWORD1(v322) = v305; /*0x10158b3a*/
                      v250 = v458; /*0x10158b3b*/
                      LODWORD(v322) = v306; /*0x10158b3e*/
                      sub_10178B80(a1: v322, a2: v423); /*0x10158b3f*/
                    }
                    goto LABEL_160; /*0x1015896a*/
                  }
                  if ( dword_103F522C == 0 ) /*0x10158608*/
                  {
                    if ( dword_103F5224 == 0 ) /*0x10158615*/
                    {
                      v410 = v443; /*0x101589ed*/
                      v411 = v434; /*0x101589f9*/
                      v412 = v435; /*0x10158a05*/
                      v413 = -1; /*0x10158a0b*/
                      v458 = v250; /*0x10158a27*/
                      v303 = v452; /*0x10158a2a*/
                      sub_10178BD0( /*0x10158a39*/
                        a1: v452,
                        a2: HIDWORD(v452),
                        a3: (int)(((unsigned int)(dword_103F5230 + 4) >> 31) + dword_103F5230 + 4) >> 1,
                        a4: 20,
                        a5: v443);
                      v355 = -16119286; /*0x10158a44*/
                      v304 = (int)(((unsigned int)(dword_103F5230 + 4) >> 31) + dword_103F5230 + 4) >> 1; /*0x10158a5e*/
                      goto LABEL_194; /*0x10158a5e*/
                    }
                    if ( dword_103F5224 == 1 ) /*0x1015861e*/
                    {
                      v274 = dword_103F5230; /*0x10158624*/
                      v458 = v250; /*0x10158633*/
                      v275 = HIDWORD(v452) - dword_103F5230 / 2; /*0x10158639*/
                      v276 = v452 - dword_103F5230 / 2; /*0x1015863e*/
                      LOBYTE(v409) = v443; /*0x10158649*/
                      BYTE1(v409) = v434; /*0x10158655*/
                      BYTE2(v409) = v435; /*0x10158661*/
                      HIBYTE(v409) = -1; /*0x10158667*/
                      sub_10177780(a1: v276, a2: v275, a3: dword_103F5230, a4: dword_103F5230, a5: v409); /*0x10158679*/
                      v354 = -16119286; /*0x10158684*/
LABEL_159:
                      v320 = v275; /*0x101583f6*/
                      v250 = v458; /*0x101583fc*/
                      sub_10177840(a1: v276, a2: v320, a3: v274, a4: v274, a5: 10, a6: 1); /*0x10158400*/
                    }
                  }
                }
LABEL_160:
                v253 = *(int (__thiscall **)(int))(*(_DWORD *)dword_103EC7B8 + 32); /*0x10158410*/
                v462 = -1; /*0x1015841b*/
                v11 = v253(a1: dword_103EC7B8); /*0x10158422*/
              }
              while ( v250 < v11 ); /*0x1015842c*/
            }
          }
        }
      }
    }
  }
  return v11; /*0x10158cb9*/
}