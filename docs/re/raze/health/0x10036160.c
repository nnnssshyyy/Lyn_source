int __usercall sub_10036160@<eax>(
        _DWORD *a1@<ecx>,
        __int128 _XMM0@<xmm0>,
        __int128 _XMM2@<xmm2>,
        __int128 _XMM3@<xmm3>)
{
  _DWORD *v6; // eax
  __int128 *v7; // edi
  __int128 *v8; // ecx
  unsigned int v9; // eax
  _DWORD *v10; // ecx
  _DWORD *v11; // ecx
  __int128 *v12; // edi
  __int128 *v13; // ecx
  unsigned int v14; // eax
  _DWORD *v15; // ecx
  _DWORD *v16; // ecx
  __int128 *v17; // edi
  __int128 *v18; // ecx
  unsigned int v19; // eax
  _DWORD *v20; // ecx
  _DWORD *v21; // ecx
  __int128 *v22; // edi
  __int128 *v23; // ecx
  _DWORD *v24; // ecx
  unsigned int v25; // eax
  _DWORD *v26; // ecx
  __int128 *v27; // edi
  __int128 *v28; // ecx
  unsigned int v29; // eax
  _DWORD *v30; // ecx
  _DWORD *v31; // ecx
  _DWORD *v32; // ecx
  int v33; // eax
  int result; // eax
  int (__thiscall *v35)(int); // eax
  int v36; // eax
  int v37; // ecx
  BOOL v38; // edi
  bool v39; // zf
  int v40; // ebx
  int v41; // edi
  __int128 *v44; // ecx
  __int128 *v45; // edi
  int v46; // ebx
  void *v47; // ecx
  int (*v48)(void); // eax
  __int128 *v49; // ebx
  int v50; // ebx
  int v51; // edi
  int (__thiscall *v52)(__int128 *); // eax
  int v53; // eax
  int v54; // eax
  int (__thiscall *v55)(int); // eax
  __int128 *v56; // edi
  int v57; // ebx
  int (__thiscall *v70)(char *); // eax
  int v84; // eax
  size_t Size; // eax
  char *v86; // edi
  size_t v87; // ebx
  int (__thiscall *v89)(__int128 *); // eax
  int v94; // ecx
  int v96; // ecx
  __int128 *v114; // ecx
  unsigned int v126; // edi
  int v128; // ecx
  int v129; // ecx
  _DWORD *v130; // eax
  int (__thiscall *v131)(__int128 *); // eax
  int v135; // eax
  int v136; // ecx
  int v137; // ebx
  unsigned int v138; // eax
  int v139; // ecx
  int v140; // eax
  unsigned int v141; // edi
  char *v142; // ebx
  void (__thiscall *v145)(int); // eax
  char v157; // cf
  void (__thiscall *v158)(int); // eax
  void *v170; // ecx
  void *v171; // ecx
  int v172; // edi
  int v173; // eax
  int v174; // eax
  int v175; // edi
  int v176; // edx
  int v177; // eax
  const char *v178; // edi
  const char *v183; // eax
  void *v184; // edi
  int v185; // edx
  int v186; // ebx
  __int16 v187; // ax
  int v188; // edi
  char v189; // al
  int v216; // edi
  int v217; // eax
  int v218; // eax
  int v219; // edi
  int v220; // edx
  int v221; // eax
  const char *v222; // edi
  int v227; // edx
  int v228; // edi
  __int16 v229; // ax
  __int128 *v256; // edx
  void *v257; // ecx
  int v258; // eax
  int v259; // eax
  int v260; // eax
  int v261; // edi
  int v262; // ebx
  int v263; // eax
  int v264; // ebx
  int v295; // edi
  int v296; // eax
  int v297; // eax
  int v298; // edi
  int v299; // ebx
  int v300; // eax
  int v301; // ebx
  int v306; // eax
  int v307; // ebx
  int v308; // ebx
  __int128 *v321; // eax
  int v323; // eax
  char *v324; // eax
  wchar_t *v326; // eax
  _DWORD *v327; // eax
  __int128 *v332; // ecx
  int v333; // ebx
  int v334; // edx
  int v335; // edi
  int v336; // eax
  int v337; // ecx
  int v338; // edx
  __int128 *v341; // ecx
  int v342; // ebx
  void *v343; // ecx
  int v344; // edi
  int (__thiscall *v345)(int); // eax
  int v369; // eax
  int v371; // ecx
  int v373; // eax
  __int128 *v381; // [esp+0h] [ebp-260h]
  int v382; // [esp+Ch] [ebp-254h]
  _BYTE v383[28]; // [esp+14h] [ebp-24Ch] BYREF
  int v384; // [esp+30h] [ebp-230h]
  __int128 *v385; // [esp+34h] [ebp-22Ch]
  int v386; // [esp+38h] [ebp-228h]
  int v387; // [esp+3Ch] [ebp-224h]
  __int128 *v388; // [esp+40h] [ebp-220h]
  const char *v389; // [esp+44h] [ebp-21Ch]
  int v390; // [esp+48h] [ebp-218h] BYREF
  _DWORD v391[2]; // [esp+4Ch] [ebp-214h] BYREF
  void *v392; // [esp+54h] [ebp-20Ch]
  int v393; // [esp+58h] [ebp-208h]
  unsigned int v394; // [esp+5Ch] [ebp-204h]
  char *v395; // [esp+60h] [ebp-200h]
  int v396; // [esp+64h] [ebp-1FCh]
  int v397; // [esp+68h] [ebp-1F8h]
  char v398[16]; // [esp+6Ch] [ebp-1F4h] BYREF
  int v399; // [esp+7Ch] [ebp-1E4h]
  unsigned int v400; // [esp+80h] [ebp-1E0h]
  int v401; // [esp+90h] [ebp-1D0h]
  int v402; // [esp+94h] [ebp-1CCh]
  int v403; // [esp+98h] [ebp-1C8h]
  int v404; // [esp+9Ch] [ebp-1C4h]
  int v405; // [esp+A0h] [ebp-1C0h]
  int v406; // [esp+A4h] [ebp-1BCh]
  void *v407; // [esp+A8h] [ebp-1B8h]
  int v408; // [esp+ACh] [ebp-1B4h]
  int v409; // [esp+B0h] [ebp-1B0h]
  int v410; // [esp+B4h] [ebp-1ACh]
  void *v411; // [esp+B8h] [ebp-1A8h]
  int v412; // [esp+BCh] [ebp-1A4h]
  int v413; // [esp+C0h] [ebp-1A0h]
  void *v414; // [esp+C4h] [ebp-19Ch]
  int v415; // [esp+C8h] [ebp-198h]
  char v416[24]; // [esp+CCh] [ebp-194h] BYREF
  int v417; // [esp+E4h] [ebp-17Ch] BYREF
  _DWORD v418[3]; // [esp+E8h] [ebp-178h] BYREF
  int v419; // [esp+F4h] [ebp-16Ch] BYREF
  int v420; // [esp+F8h] [ebp-168h]
  unsigned int v421; // [esp+104h] [ebp-15Ch]
  unsigned int v422; // [esp+108h] [ebp-158h]
  int v423; // [esp+10Ch] [ebp-154h]
  int v424; // [esp+110h] [ebp-150h]
  int v425; // [esp+114h] [ebp-14Ch]
  _DWORD v426[4]; // [esp+118h] [ebp-148h] BYREF
  int v427; // [esp+128h] [ebp-138h]
  unsigned int v428; // [esp+12Ch] [ebp-134h]
  __int128 *v429; // [esp+138h] [ebp-128h]
  char v430; // [esp+13Ch] [ebp-124h]
  char v431; // [esp+13Dh] [ebp-123h]
  char v432; // [esp+13Eh] [ebp-122h]
  char v433; // [esp+13Fh] [ebp-121h]
  char v434; // [esp+140h] [ebp-120h]
  char v435; // [esp+141h] [ebp-11Fh]
  char v436; // [esp+142h] [ebp-11Eh]
  char v437; // [esp+143h] [ebp-11Dh]
  char v438; // [esp+144h] [ebp-11Ch]
  char v439; // [esp+145h] [ebp-11Bh]
  char v440; // [esp+146h] [ebp-11Ah]
  char v441; // [esp+147h] [ebp-119h]
  char v442; // [esp+148h] [ebp-118h]
  char v443; // [esp+149h] [ebp-117h]
  char v444; // [esp+14Ah] [ebp-116h]
  char v445; // [esp+14Bh] [ebp-115h]
  char v446; // [esp+14Ch] [ebp-114h]
  char v447; // [esp+14Dh] [ebp-113h]
  char v448; // [esp+14Eh] [ebp-112h]
  char v449; // [esp+14Fh] [ebp-111h]
  char v450; // [esp+150h] [ebp-110h]
  char v451; // [esp+151h] [ebp-10Fh]
  char v452; // [esp+152h] [ebp-10Eh]
  char v453; // [esp+153h] [ebp-10Dh]
  _DWORD v454[2]; // [esp+154h] [ebp-10Ch] BYREF
  char Src[8]; // [esp+15Ch] [ebp-104h] BYREF
  _DWORD v456[4]; // [esp+164h] [ebp-FCh] BYREF
  int v457; // [esp+174h] [ebp-ECh]
  unsigned int v458; // [esp+178h] [ebp-E8h]
  int v459; // [esp+17Ch] [ebp-E4h]
  _DWORD *v460; // [esp+1F8h] [ebp-68h]
  _DWORD *v461; // [esp+1FCh] [ebp-64h]
  _DWORD *v462; // [esp+200h] [ebp-60h]
  _DWORD *v463; // [esp+204h] [ebp-5Ch]
  unsigned int v464; // [esp+208h] [ebp-58h]
  int v465; // [esp+210h] [ebp-50h]
  _DWORD *v466; // [esp+214h] [ebp-4Ch]
  int v467; // [esp+218h] [ebp-48h] BYREF
  int v468; // [esp+21Ch] [ebp-44h] BYREF
  int v469; // [esp+220h] [ebp-40h] BYREF
  int v470; // [esp+224h] [ebp-3Ch] BYREF
  _DWORD v471[14]; // [esp+228h] [ebp-38h] BYREF
  int savedregs; // [esp+260h] [ebp+0h] BYREF

  _ESI = v383; /*0x1003616f*/
  v471[1] = &savedregs; /*0x10036171*/
  v471[2] = v383; /*0x10036179*/
  v471[5] = -1; /*0x1003617f*/
  v471[4] = &loc_1003E490; /*0x1003618f*/
  v471[3] = NtCurrentTeb()->NtTib.ExceptionList; /*0x100361a0*/
  sub_10035CD0(a1); /*0x100361ae*/
  v391[1] = a1 + 3; /*0x100361b6*/
  sub_10035CD0(a1: a1 + 3); /*0x100361b9*/
  v6 = a1; /*0x100361be*/
  v7 = (__int128 *)a1[6]; /*0x100361c0*/
  v8 = (__int128 *)v6[7]; /*0x100361c3*/
  v384 = (int)v6; /*0x100361c8*/
  if ( v7 != v8 ) /*0x100361cb*/
  {
    v385 = v8; /*0x100361cd*/
    do /*0x10036204*/
    {
      v9 = *((_DWORD *)v7 + 9); /*0x10036206*/
      if ( v9 >= 0x10 ) /*0x1003620c*/
      {
        v10 = *((_DWORD **)v7 + 4); /*0x1003620e*/
        if ( v9 + 1 >= 0x1000 ) /*0x1003621a*/
        {
          if ( (unsigned int)v10 - *(v10 - 1) - 4 >= 0x20 ) /*0x10036227*/
LABEL_36:
            BUG(); /*0x10036380*/
          v10 = (_DWORD *)*(v10 - 1); /*0x10036232*/
        }
        sub_10180976(Block: v10); /*0x100361e2*/
        v8 = v385; /*0x100361ea*/
      }
      *((_DWORD *)v7 + 8) = 0; /*0x100361ed*/
      *((_DWORD *)v7 + 9) = 15; /*0x100361f4*/
      *((_BYTE *)v7 + 16) = 0; /*0x100361fb*/
      v7 = (__int128 *)((char *)v7 + 44); /*0x100361ff*/
    }
    while ( v7 != v8 ); /*0x10036204*/
    v11 = (_DWORD *)v384; /*0x10036236*/
    *(_DWORD *)(v384 + 28) = *(_DWORD *)(v384 + 24); /*0x1003623c*/
    v6 = v11; /*0x1003623f*/
  }
  v12 = (__int128 *)v6[9]; /*0x10036241*/
  v13 = (__int128 *)v6[10]; /*0x10036244*/
  if ( v12 != v13 ) /*0x10036249*/
  {
    v385 = (__int128 *)v6[10]; /*0x1003624b*/
    do /*0x10036274*/
    {
      v14 = *((_DWORD *)v12 + 9); /*0x10036276*/
      if ( v14 >= 0x10 ) /*0x1003627c*/
      {
        v15 = *((_DWORD **)v12 + 4); /*0x1003627e*/
        if ( v14 + 1 >= 0x1000 ) /*0x1003628a*/
        {
          if ( (unsigned int)v15 - *(v15 - 1) - 4 >= 0x20 ) /*0x10036297*/
            goto LABEL_36; /*0x10036297*/
          v15 = (_DWORD *)*(v15 - 1); /*0x100362a2*/
        }
        sub_10180976(Block: v15); /*0x10036252*/
        v13 = v385; /*0x1003625a*/
      }
      *((_DWORD *)v12 + 8) = 0; /*0x1003625d*/
      *((_DWORD *)v12 + 9) = 15; /*0x10036264*/
      *((_BYTE *)v12 + 16) = 0; /*0x1003626b*/
      v12 = (__int128 *)((char *)v12 + 44); /*0x1003626f*/
    }
    while ( v12 != v13 ); /*0x10036274*/
    v16 = (_DWORD *)v384; /*0x100362a6*/
    *(_DWORD *)(v384 + 40) = *(_DWORD *)(v384 + 36); /*0x100362ac*/
    v6 = v16; /*0x100362af*/
  }
  v17 = (__int128 *)v6[12]; /*0x100362b1*/
  v18 = (__int128 *)v6[13]; /*0x100362b4*/
  if ( v17 != v18 ) /*0x100362b9*/
  {
    v385 = (__int128 *)v6[13]; /*0x100362bb*/
    do /*0x100362e4*/
    {
      v19 = *((_DWORD *)v17 + 9); /*0x100362e6*/
      if ( v19 >= 0x10 ) /*0x100362ec*/
      {
        v20 = *((_DWORD **)v17 + 4); /*0x100362ee*/
        if ( v19 + 1 >= 0x1000 ) /*0x100362fa*/
        {
          if ( (unsigned int)v20 - *(v20 - 1) - 4 >= 0x20 ) /*0x10036307*/
            goto LABEL_36; /*0x10036307*/
          v20 = (_DWORD *)*(v20 - 1); /*0x1003630e*/
        }
        sub_10180976(Block: v20); /*0x100362c2*/
        v18 = v385; /*0x100362ca*/
      }
      *((_DWORD *)v17 + 8) = 0; /*0x100362cd*/
      *((_DWORD *)v17 + 9) = 15; /*0x100362d4*/
      *((_BYTE *)v17 + 16) = 0; /*0x100362db*/
      v17 = (__int128 *)((char *)v17 + 44); /*0x100362df*/
    }
    while ( v17 != v18 ); /*0x100362e4*/
    v21 = (_DWORD *)v384; /*0x10036312*/
    *(_DWORD *)(v384 + 52) = *(_DWORD *)(v384 + 48); /*0x10036318*/
    v6 = v21; /*0x1003631b*/
  }
  v22 = (__int128 *)v6[15]; /*0x1003631d*/
  v23 = (__int128 *)v6[16]; /*0x10036320*/
  if ( v22 != v23 ) /*0x10036325*/
  {
    v385 = (__int128 *)v6[16]; /*0x10036327*/
    do /*0x1003635b*/
    {
      v25 = *((_DWORD *)v22 + 9); /*0x1003635d*/
      if ( v25 >= 0x10 ) /*0x10036363*/
      {
        v24 = *((_DWORD **)v22 + 4); /*0x10036365*/
        if ( v25 + 1 >= 0x1000 ) /*0x10036371*/
        {
          if ( (unsigned int)v24 - *(v24 - 1) - 4 >= 0x20 ) /*0x1003637e*/
            goto LABEL_36; /*0x1003637e*/
          v24 = (_DWORD *)*(v24 - 1); /*0x10036335*/
        }
        sub_10180976(Block: v24); /*0x10036339*/
        v23 = v385; /*0x10036341*/
      }
      *((_DWORD *)v22 + 8) = 0; /*0x10036344*/
      *((_DWORD *)v22 + 9) = 15; /*0x1003634b*/
      *((_BYTE *)v22 + 16) = 0; /*0x10036352*/
      v22 = (__int128 *)((char *)v22 + 44); /*0x10036356*/
    }
    while ( v22 != v23 ); /*0x1003635b*/
    v26 = (_DWORD *)v384; /*0x10036382*/
    *(_DWORD *)(v384 + 64) = *(_DWORD *)(v384 + 60); /*0x10036388*/
    v6 = v26; /*0x1003638b*/
  }
  v27 = (__int128 *)v6[18]; /*0x1003638d*/
  v28 = (__int128 *)v6[19]; /*0x10036390*/
  if ( v27 != v28 ) /*0x10036395*/
  {
    v385 = (__int128 *)v6[19]; /*0x10036397*/
    do /*0x100363c4*/
    {
      v29 = *((_DWORD *)v27 + 9); /*0x100363c6*/
      if ( v29 >= 0x10 ) /*0x100363cc*/
      {
        v30 = *((_DWORD **)v27 + 4); /*0x100363ce*/
        if ( v29 + 1 >= 0x1000 ) /*0x100363da*/
        {
          if ( (unsigned int)v30 - *(v30 - 1) - 4 >= 0x20 ) /*0x100363e7*/
            goto LABEL_36; /*0x100363e7*/
          v30 = (_DWORD *)*(v30 - 1); /*0x100363ee*/
        }
        sub_10180976(Block: v30); /*0x100363a2*/
        v28 = v385; /*0x100363aa*/
      }
      *((_DWORD *)v27 + 8) = 0; /*0x100363ad*/
      *((_DWORD *)v27 + 9) = 15; /*0x100363b4*/
      *((_BYTE *)v27 + 16) = 0; /*0x100363bb*/
      v27 = (__int128 *)((char *)v27 + 44); /*0x100363bf*/
    }
    while ( v27 != v28 ); /*0x100363c4*/
    v31 = (_DWORD *)v384; /*0x100363f2*/
    *(_DWORD *)(v384 + 76) = *(_DWORD *)(v384 + 72); /*0x100363f8*/
    v6 = v31; /*0x100363fb*/
  }
  v32 = v6; /*0x100363fd*/
  v33 = v6[21]; /*0x100363ff*/
  if ( v33 != v32[22] ) /*0x10036405*/
    v32[22] = v33; /*0x10036407*/
  result = v32[24]; /*0x1003640a*/
  if ( result != v32[25] ) /*0x10036410*/
    v32[25] = result; /*0x10036412*/
  if ( (_BYTE)word_103F4BDD != 1 ) /*0x1003641c*/
    return result; /*0x1003641c*/
  v35 = *(int (__thiscall **)(_DWORD))(*(_DWORD *)dword_103EC8A0 + 8); /*0x1003642a*/
  v471[5] = -1; /*0x1003642d*/
  result = v35(a1: dword_103EC8A0); /*0x10036437*/
  if ( (_BYTE)result != 0 ) /*0x1003643b*/
    return result; /*0x1003643b*/
  if ( byte_103EEF74 != 0 ) /*0x10036448*/
    return result; /*0x10036448*/
  if ( byte_103F4BA8 == 1 ) /*0x10036455*/
  {
    result = (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 356))(a1: dword_103EC770); /*0x1003645f*/
    if ( (_BYTE)result != 0 ) /*0x10036467*/
      return result; /*0x10036467*/
  }
  result = sub_10179540(); /*0x10036473*/
  if ( result == 0 ) /*0x1003647a*/
    return result; /*0x1003647a*/
  v386 = result; /*0x10036480*/
  v36 = *(_DWORD *)off_1029E014; /*0x10036489*/
  v37 = *(_DWORD *)(*(_DWORD *)off_1029E014 + 4); /*0x1003648b*/
  v38 = false; /*0x1003648e*/
  v39 = *(_BYTE *)(v37 + 13) == 0; /*0x10036490*/
  v385 = *(__int128 **)off_1029E014; /*0x10036494*/
  if ( v39 ) /*0x10036497*/
  {
    do /*0x100364c6*/
    {
      v40 = v37; /*0x100364b0*/
      v41 = *(_DWORD *)(v37 + 16); /*0x100364b2*/
      if ( v41 >= 0 ) /*0x100364b7*/
        v36 = v37; /*0x100364b7*/
      v37 = *(_DWORD *)(v37 + 8 * ((unsigned int)v41 >> 31)); /*0x100364bf*/
    }
    while ( *(_BYTE *)(v37 + 13) == 0 ); /*0x100364c6*/
    v38 = v41 >= 0; /*0x100364ca*/
    if ( *(_BYTE *)(v36 + 13) != 0 ) /*0x100364d1*/
      goto LABEL_65; /*0x100364d1*/
  }
  else
  {
    v40 = v37; /*0x10036499*/
    if ( *(_BYTE *)(v36 + 13) != 0 ) /*0x1003649f*/
    {
LABEL_65:
      v39 = *((_DWORD *)off_1029E014 + 1) == 0x7FFFFFF; /*0x100364d9*/
      v471[5] = -1; /*0x100364e0*/
      v388 = off_1029E014; /*0x100364ea*/
      if ( v39 ) /*0x100364ed*/
        goto LABEL_260; /*0x100364ed*/
      _EAX = sub_10180946(Size: 0x20u); /*0x100364f5*/
      __asm /*0x100364fd*/
      {
        vxorps xmm0, xmm0, xmm0
        vmovups xmmword ptr [eax+10h], xmm0
      }
      v44 = v385; /*0x10036506*/
      *(_DWORD *)_EAX = v385; /*0x10036509*/
      *(_DWORD *)(_EAX + 4) = v44; /*0x1003650b*/
      *(_DWORD *)(_EAX + 8) = v44; /*0x1003650e*/
      *(_WORD *)(_EAX + 12) = 0; /*0x10036511*/
      v36 = sub_10018900(a1: v40, a2: v38, a3: _EAX); /*0x1003651d*/
      goto LABEL_67; /*0x1003651d*/
    }
  }
  if ( *(int *)(v36 + 16) > 0 ) /*0x100364d7*/
    goto LABEL_65; /*0x100364d7*/
LABEL_67:
  v461 = (_DWORD *)(v384 + 24); /*0x10036522*/
  v462 = (_DWORD *)(v384 + 36); /*0x10036531*/
  v45 = *(__int128 **)(v36 + 20); /*0x10036537*/
  v388 = *(__int128 **)(v36 + 24); /*0x1003653d*/
  v463 = (_DWORD *)(v384 + 48); /*0x10036543*/
  v460 = (_DWORD *)(v384 + 60); /*0x1003654c*/
  v466 = (_DWORD *)(v384 + 72); /*0x10036555*/
  v46 = v386; /*0x1003655b*/
  while ( v45 != v388 ) /*0x10036586*/
  {
    if ( *(_DWORD *)v45 == 0 ) /*0x10036590*/
      goto LABEL_70; /*0x10036590*/
    v385 = *(__int128 **)v45; /*0x10036592*/
    if ( dword_103EEA68 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x100365b0*/
    {
      sub_101809C3(a1: &dword_103EEA68); /*0x1003708f*/
      if ( dword_103EEA68 == -1 ) /*0x1003709e*/
      {
        v387 = (int)v45; /*0x100370a4*/
        v471[5] = 0; /*0x100370a7*/
        dword_103EEA64 = sub_1017C730(a1: "CBasePlayer", a2: "deadflag"); /*0x100370c3*/
        sub_10180A43(a1: &dword_103EEA68); /*0x100370cd*/
        v46 = v386; /*0x100370d5*/
        v45 = (__int128 *)v387; /*0x100370d8*/
      }
    }
    if ( *(_DWORD *)((char *)v385 + dword_103EEA64) != 0 ) /*0x100365c2*/
      goto LABEL_70; /*0x100365c2*/
    if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x100365df*/
    {
      sub_101809C3(a1: &dword_103EEAD8); /*0x100370e5*/
      if ( dword_103EEAD8 == -1 ) /*0x100370f4*/
      {
        v387 = (int)v45; /*0x100370fa*/
        v471[5] = 1; /*0x100370fd*/
        dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x10037119*/
        sub_10180A43(a1: &dword_103EEAD8); /*0x10037123*/
        v46 = v386; /*0x1003712b*/
        v45 = (__int128 *)v387; /*0x1003712e*/
      }
    }
    if ( v385 == (__int128 *)v46 ) /*0x100365ea*/
      goto LABEL_70; /*0x100365ea*/
    v393 = *(_DWORD *)((char *)v385 + dword_103EEAD4); /*0x100365f4*/
    v48 = *(int (**)(void))(*(_DWORD *)v385 + 960); /*0x100365f9*/
    v471[5] = -1; /*0x100365ff*/
    v49 = v385; /*0x10036609*/
    LOBYTE(v392) = v48(); /*0x1003660d*/
    v381 = v49; /*0x10036626*/
    v46 = v386; /*0x10036627*/
    if ( (unsigned __int8)sub_1002E000(a1: v381, a2: v391, a3: &v390, a4: &v470, a5: &v469) == 0 ) /*0x10036631*/
      goto LABEL_70; /*0x10036631*/
    v387 = (int)v45; /*0x10036637*/
    v389 = (const char *)off_1029E57C; /*0x1003663f*/
    if ( dword_103EF0D4 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1003665d*/
    {
      sub_101809C3(a1: &dword_103EF0D4); /*0x1003713b*/
      if ( dword_103EF0D4 == -1 ) /*0x1003714a*/
      {
        v471[5] = 2; /*0x10037150*/
        dword_103EF0D0 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_iMaxHealth"); /*0x1003716c*/
        sub_10180A43(a1: &dword_103EF0D4); /*0x10037176*/
      }
    }
    v50 = (int)v385; /*0x10036668*/
    v51 = *(_DWORD *)((char *)v385 + dword_103EF0D0); /*0x1003666b*/
    v52 = *(int (__thiscall **)(_DWORD))(*(_DWORD *)v385 + 468); /*0x10036670*/
    v471[5] = -1; /*0x10036676*/
    v53 = v52(a1: v385); /*0x10036682*/
    sub_101671B0(a1: &v468, a2: v53, a3: v51); /*0x10036690*/
    sub_1016A520(a1: &v467, a2: v386, a3: v50); /*0x100366a6*/
    if ( dword_103EEF7C > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x100366c6*/
    {
      sub_101809C3(a1: &dword_103EEF7C); /*0x10037188*/
      if ( dword_103EEF7C == -1 ) /*0x10037197*/
      {
        v471[5] = 3; /*0x1003719d*/
        dword_103EEF78 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_zombieClass"); /*0x100371b9*/
        sub_10180A43(a1: &dword_103EEF7C); /*0x100371c3*/
        v50 = (int)v385; /*0x100371cb*/
      }
    }
    v54 = *(_DWORD *)(dword_103EEF78 + v50); /*0x100366d1*/
    v389 = "Bot"; /*0x100366d7*/
    if ( (unsigned int)(v54 - 1) <= 7 ) /*0x100366e1*/
    {
      v389 = (const char *)dword_1018581C[v54]; /*0x100366ea*/
      v471[5] = -1; /*0x100366ed*/
    }
    v55 = *(int (__thiscall **)(_DWORD))(*(_DWORD *)v50 + 468); /*0x100366f9*/
    v471[5] = -1; /*0x100366ff*/
    v56 = (__int128 *)v50; /*0x1003670b*/
    v465 = v55(a1: v50); /*0x1003670f*/
    v57 = v386; /*0x10036730*/
    if ( dword_103EF0D4 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10036733*/
    {
      sub_101809C3(a1: &dword_103EF0D4); /*0x100371d8*/
      if ( dword_103EF0D4 == -1 ) /*0x100371e7*/
      {
        v471[5] = 4; /*0x100371ed*/
        dword_103EF0D0 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_iMaxHealth"); /*0x10037209*/
        sub_10180A43(a1: &dword_103EF0D4); /*0x10037213*/
        v57 = v386; /*0x1003721b*/
        v56 = v385; /*0x1003721e*/
      }
    }
    __asm /*0x1003673e*/
    {
      vcvtsi2ss xmm0, xmm3, dword ptr [eax+edi]
      vmovss dword ptr [esi+1F8h], xmm0
    }
    v471[5] = -1; /*0x10036751*/
    sub_101669E0(a1: v471, a2: v57, a3: v56); /*0x10036764*/
    __asm /*0x10036769*/
    {
      vmovss xmm0, ds:dword_10184380
      vmulss xmm1, xmm0, dword ptr qword_103F4C74
      vmovss dword ptr [esi+98h], xmm1
      vmulss xmm1, xmm0, dword ptr qword_103F4C74+4
      vmovss dword ptr [esi+0A8h], xmm1
      vmulss xmm1, xmm0, dword_103F4C7C
      vmovss dword ptr [esi+0ACh], xmm1
      vmulss xmm1, xmm0, dword_103F4C28
      vmovss dword ptr [esi+90h], xmm1
      vmulss xmm1, xmm0, dword_103F4C2C
      vmovss dword ptr [esi+94h], xmm1
      vmulss xmm1, xmm0, dword_103F4C30
      vmovss dword ptr [esi+9Ch], xmm1
      vmulss xmm1, xmm0, dword_103F4C5C
      vmovss dword ptr [esi+0A4h], xmm1
      vmulss xmm1, xmm0, dword_103F4C60
      vmovss dword ptr [esi+0B0h], xmm1
      vxorps xmm1, xmm1, xmm1
      vmovups ymmword ptr [esi+1B0h], ymm1
      vmovaps ymmword ptr [esi+1A0h], ymm1
      vmovaps ymmword ptr [esi+180h], ymm1
      vmovaps ymmword ptr [esi+160h], ymm1
      vmovaps ymmword ptr [esi+140h], ymm1
      vmovaps xmm1, ds:xmmword_10185540
      vmovups xmmword ptr [esi+60h], xmm1
    }
    strcpy(v398, "unknown"); /*0x1003682a*/
    v396 = dword_103EC770; /*0x10036841*/
    v70 = *(int (__thiscall **)(_DWORD))(*((_DWORD *)v56 + 2) + 32); /*0x1003684a*/
    v471[5] = 5; /*0x1003684d*/
    __asm /*0x10036857*/
    {
      vmulss xmm1, xmm0, dword_103F4C64
      vmovss dword ptr [esi+48h], xmm1
      vmulss xmm1, xmm0, dword_103F4C88
      vmovss dword ptr [esi+4Ch], xmm1
      vmulss xmm1, xmm0, dword_103F4C8C
      vmovss dword ptr [esi+70h], xmm1
      vmulss xmm1, xmm0, dword_103F4C90
      vmovss dword ptr [esi+8Ch], xmm1
      vmulss xmm1, xmm0, dword ptr xmmword_103F4CE0
      vmovss dword ptr [esi+74h], xmm1
      vmulss xmm1, xmm0, dword ptr xmmword_103F4CE0+4
      vmovss dword ptr [esi+78h], xmm1
      vmulss xmm1, xmm0, dword ptr xmmword_103F4CE0+8
      vmovss dword ptr [esi+80h], xmm1
      vmulss xmm1, xmm0, dword_103F4CF4
      vmovss dword ptr [esi+0B4h], xmm1
      vmulss xmm1, xmm0, dword_103F4CF8
      vmovss dword ptr [esi+88h], xmm1
      vmulss xmm1, xmm0, dword_103F4CFC
      vmovss dword ptr [esi+7Ch], xmm1
      vmulss xmm1, xmm0, dword ptr xmmword_103F4D08
      vmovss dword ptr [esi+54h], xmm1
      vmulss xmm1, xmm0, dword ptr xmmword_103F4D08+4
      vmovss dword ptr [esi+0A0h], xmm1
      vmulss xmm0, xmm0, dword ptr xmmword_103F4D08+8
      vmovss dword ptr [esi+84h], xmm0
      vzeroupper
    }
    v84 = v70(a1: (char *)v56 + 8); /*0x10036915*/
    if ( (*(unsigned __int8 (__thiscall **)(int, int, _DWORD *))(*(_DWORD *)v396 + 32))(a1: v396, a2: v84, a3: v454) != 0 ) /*0x10036929*/
    {
      Size = strlen(Str: Src); /*0x10036932*/
      if ( Size <= v400 ) /*0x1003693f*/
      {
        v86 = v398; /*0x10036960*/
        if ( v400 >= 0x10 ) /*0x10036966*/
          v86 = *(char **)v398; /*0x10036968*/
        v399 = Size; /*0x1003696b*/
        v87 = Size; /*0x10036971*/
        memmove(a1: v86, Src, Size); /*0x10036973*/
        v86[v87] = 0; /*0x1003697b*/
      }
      else
      {
        LOBYTE(v382) = v417; /*0x1003694b*/
        sub_1001C600(Size, a2: v382, Src); /*0x10036959*/
      }
    }
    __asm /*0x1003697f*/
    {
      vmovaps xmm0, ds:xmmword_10185540
      vmovups xmmword ptr [esi+0C0h], xmm0
    }
    strcpy(v416, "unknown"); /*0x1003698f*/
    v89 = *(int (__thiscall **)(_DWORD))(*(_DWORD *)v385 + 44); /*0x100369af*/
    v471[5] = 6; /*0x100369b2*/
    _EAX = v89(a1: v385); /*0x100369bc*/
    if ( ((unsigned __int8)byte_103F4BDC & (unsigned __int8)v392) == 1 ) /*0x100369cb*/
    {
      v397 = _EAX; /*0x100369d1*/
      __asm /*0x100369d4*/
      {
        vcvttss2si eax, dword ptr [esi+98h]
        vcvttss2si edx, dword ptr [esi+0A8h]
        vcvttss2si ebx, dword ptr [esi+0ACh]
      }
      v407 = _EAX; /*0x100369ee*/
      v404 = _EDX; /*0x100369f6*/
      v401 = _EBX; /*0x100369fe*/
      v411 = _EAX; /*0x10036a03*/
      v406 = _EDX; /*0x10036a0b*/
      v402 = _EBX; /*0x10036a13*/
      v414 = _EAX; /*0x10036a1b*/
      v409 = _EDX; /*0x10036a23*/
      v405 = _EBX; /*0x10036a2b*/
      v392 = _EAX; /*0x10036a33*/
      v413 = _EDX; /*0x10036a38*/
      v408 = _EBX; /*0x10036a40*/
      v410 = _EDX; /*0x10036a4a*/
      v94 = _EDX; /*0x10036a50*/
      _EDX = (int)_EAX; /*0x10036a52*/
      v396 = v94; /*0x10036a54*/
      v403 = _EBX; /*0x10036a57*/
      v96 = _EBX; /*0x10036a5d*/
      _EBX = (int)_EAX; /*0x10036a5f*/
      _EAX = v397; /*0x10036a61*/
      v412 = v96; /*0x10036a64*/
    }
    else
    {
      __asm /*0x10036a6f*/
      {
        vcvttss2si edx, dword ptr [esi+90h]
        vcvttss2si ecx, dword ptr [esi+94h]
      }
      v396 = _ECX; /*0x10036a7f*/
      __asm { vcvttss2si ecx, dword ptr [esi+9Ch] } /*0x10036a82*/
      v412 = _ECX; /*0x10036a8a*/
      __asm { vcvttss2si ecx, dword ptr [esi+0A4h] } /*0x10036a90*/
      v392 = _ECX; /*0x10036a98*/
      __asm { vcvttss2si ecx, dword ptr [esi+0B0h] } /*0x10036a9b*/
      v413 = _ECX; /*0x10036aa3*/
      __asm { vcvttss2si ecx, dword ptr [esi+48h] } /*0x10036aa9*/
      v408 = _ECX; /*0x10036aae*/
      __asm { vcvttss2si ecx, dword ptr [esi+4Ch] } /*0x10036ab4*/
      v414 = _ECX; /*0x10036ab9*/
      __asm { vcvttss2si ecx, dword ptr [esi+70h] } /*0x10036abf*/
      v409 = _ECX; /*0x10036ac4*/
      __asm { vcvttss2si ecx, dword ptr [esi+8Ch] } /*0x10036aca*/
      v405 = _ECX; /*0x10036ad2*/
      __asm { vcvttss2si ecx, dword ptr [esi+74h] } /*0x10036ad8*/
      v411 = _ECX; /*0x10036add*/
      __asm { vcvttss2si ecx, dword ptr [esi+78h] } /*0x10036ae3*/
      v406 = _ECX; /*0x10036ae8*/
      __asm { vcvttss2si ecx, dword ptr [esi+80h] } /*0x10036aee*/
      v402 = _ECX; /*0x10036af6*/
      __asm { vcvttss2si ecx, dword ptr [esi+0B4h] } /*0x10036afc*/
      v407 = _ECX; /*0x10036b04*/
      __asm { vcvttss2si ecx, dword ptr [esi+88h] } /*0x10036b0a*/
      v404 = _ECX; /*0x10036b12*/
      __asm { vcvttss2si ecx, dword ptr [esi+7Ch] } /*0x10036b18*/
      v401 = _ECX; /*0x10036b1d*/
      __asm /*0x10036b20*/
      {
        vcvttss2si ebx, dword ptr [esi+54h]
        vcvttss2si ecx, dword ptr [esi+0A0h]
      }
      v410 = _ECX; /*0x10036b2d*/
      __asm { vcvttss2si ecx, dword ptr [esi+84h] } /*0x10036b33*/
      v403 = _ECX; /*0x10036b3b*/
    }
    v114 = v385; /*0x10036b41*/
    __asm /*0x10036b44*/
    {
      vmovss xmm1, dword ptr [eax]
      vmovss xmm0, dword ptr [eax+4]
      vmovss dword ptr [esi+78h], xmm0
      vmovss xmm0, dword ptr [eax+8]
      vmovss dword ptr [esi+74h], xmm0
      vmovss xmm0, dword ptr [esi+38h]
      vmovss dword ptr [esi+0D0h], xmm0
      vmovss xmm0, dword ptr [esi+34h]
      vmovss dword ptr [esi+0D4h], xmm0
      vmovss xmm0, dword ptr [esi+210h]
      vmovss dword ptr [esi+0D8h], xmm0
      vmovss xmm0, dword ptr [esi+20Ch]
      vmovss dword ptr [esi+0DCh], xmm0
    }
    _EAX = &v419; /*0x10036b96*/
    __asm /*0x10036b9c*/
    {
      vxorps xmm0, xmm0, xmm0
      vmovsd qword ptr [eax+10h], xmm0
      vxorps xmm0, xmm0, xmm0
      vmovups xmmword ptr [eax], xmm0
    }
    _EAX = v398; /*0x10036bb1*/
    if ( v400 >= 0x10 ) /*0x10036bb4*/
      _EAX = *(char **)v398; /*0x10036bb6*/
    v126 = v399; /*0x10036bb9*/
    if ( v399 < 0 ) /*0x10036bbe*/
      sub_1000DFF0(a1: v114); /*0x100382ba*/
    v397 = _EDX; /*0x10036bc7*/
    v415 = _EBX; /*0x10036bca*/
    __asm { vmovss dword ptr [esi+70h], xmm1 } /*0x10036bd0*/
    if ( v126 > 0xF ) /*0x10036bd5*/
    {
      v395 = _EAX; /*0x10036bf7*/
      v394 = v126; /*0x10036bfa*/
      v128 = 22; /*0x10036c05*/
      if ( (v126 | 0xF) >= 0x17 ) /*0x10036c0a*/
        v128 = v126 | 0xF; /*0x10036c0a*/
      v464 = v128; /*0x10036c12*/
      if ( (v126 | 0xF) < 0xFFF ) /*0x10036c18*/
      {
        v130 = (_DWORD *)sub_10180946(Size: v128 + 1); /*0x10036c37*/
      }
      else
      {
        v129 = sub_10180946(Size: v128 + 36); /*0x10036c26*/
        v130 = (_DWORD *)((v129 + 35) & 0xFFFFFFE0); /*0x10036c2b*/
        *(v130 - 1) = v129; /*0x10036c2e*/
      }
      v419 = (int)v130; /*0x10036c42*/
      v421 = v394; /*0x10036c48*/
      v422 = v464; /*0x10036c54*/
      memcpy(a1: v130, Src: v395, Size: v394 + 1); /*0x10036c60*/
      v114 = v385; /*0x10036c68*/
    }
    else
    {
      v421 = v126; /*0x10036bd7*/
      v422 = 15; /*0x10036bdd*/
      __asm { vmovups xmm0, xmmword ptr [eax] } /*0x10036be7*/
      _EAX = &v419; /*0x10036beb*/
      __asm { vmovups xmmword ptr [eax], xmm0 } /*0x10036bf1*/
    }
    v131 = *(int (__thiscall **)(_DWORD))(*(_DWORD *)v114 + 468); /*0x10036c6d*/
    v471[5] = 7; /*0x10036c73*/
    v423 = v131(a1: v114); /*0x10036c7f*/
    v424 = v468; /*0x10036c8b*/
    v425 = v467; /*0x10036c97*/
    __asm { vxorps xmm0, xmm0, xmm0 } /*0x10036c9d*/
    _EAX = v426; /*0x10036ca1*/
    __asm /*0x10036ca7*/
    {
      vmovsd qword ptr [eax+10h], xmm0
      vxorps xmm0, xmm0, xmm0
      vmovups xmmword ptr [eax], xmm0
    }
    v135 = strlen(Str: v389); /*0x10036cb7*/
    v45 = (__int128 *)v387; /*0x10036cc1*/
    if ( v135 < 0 ) /*0x10036cc4*/
      sub_1000DFF0(a1: v136); /*0x100382b5*/
    if ( (unsigned int)v135 > 0xF ) /*0x10036ccd*/
    {
      v395 = (char *)v135; /*0x10036cfe*/
      v138 = v135 | 0xF; /*0x10036d01*/
      v139 = 22; /*0x10036d07*/
      if ( v138 >= 0x17 ) /*0x10036d0c*/
        v139 = v138; /*0x10036d0c*/
      v394 = v139; /*0x10036d14*/
      if ( v138 < 0xFFF ) /*0x10036d17*/
      {
        v141 = sub_10180946(Size: v139 + 1); /*0x10036d3e*/
      }
      else
      {
        v140 = sub_10180946(Size: v139 + 36); /*0x10036d1d*/
        v141 = (v140 + 35) & 0xFFFFFFE0; /*0x10036d2a*/
        *(_DWORD *)(v141 - 4) = v140; /*0x10036d2d*/
      }
      v142 = v395; /*0x10036d40*/
      v426[0] = v141; /*0x10036d43*/
      v427 = (int)v395; /*0x10036d49*/
      v428 = v394; /*0x10036d52*/
      memcpy(a1: (void *)v141, Src: v389, Size: (size_t)v395); /*0x10036d5d*/
      v142[v141] = 0; /*0x10036d65*/
      v45 = (__int128 *)v387; /*0x10036d69*/
    }
    else
    {
      v427 = v135; /*0x10036ccf*/
      v428 = 15; /*0x10036cd5*/
      v137 = v135; /*0x10036ce3*/
      memcpy(a1: v426, Src: v389, Size: v135); /*0x10036cec*/
      *((_BYTE *)v426 + v137) = 0; /*0x10036cf4*/
    }
    __asm /*0x10036d6c*/
    {
      vcvtsi2ss xmm0, xmm2, dword ptr [esi+1FCh]
      vmovss dword ptr [esi+11Ch], xmm0
      vmovss xmm0, dword ptr [esi+1F8h]
      vmovss dword ptr [esi+120h], xmm0
    }
    v429 = v385; /*0x10036d8f*/
    v430 = v397; /*0x10036d98*/
    v431 = v396; /*0x10036da1*/
    v432 = v412; /*0x10036dad*/
    v433 = -1; /*0x10036db3*/
    v434 = (char)v392; /*0x10036dbd*/
    v435 = v413; /*0x10036dc9*/
    v436 = v408; /*0x10036dd5*/
    v437 = -1; /*0x10036ddb*/
    v438 = (char)v414; /*0x10036de8*/
    v439 = v409; /*0x10036df4*/
    v440 = v405; /*0x10036e00*/
    v441 = -1; /*0x10036e06*/
    v442 = (char)v411; /*0x10036e13*/
    v443 = v406; /*0x10036e1f*/
    v444 = v402; /*0x10036e2b*/
    v445 = -1; /*0x10036e31*/
    v446 = (char)v407; /*0x10036e3e*/
    v447 = v404; /*0x10036e4a*/
    v448 = v401; /*0x10036e53*/
    v449 = -1; /*0x10036e59*/
    v450 = v415; /*0x10036e66*/
    v451 = v410; /*0x10036e72*/
    v452 = v403; /*0x10036e7e*/
    v453 = -1; /*0x10036e84*/
    if ( v393 == 3 ) /*0x10036e91*/
    {
      v46 = v386; /*0x10036f3c*/
      if ( byte_103F4BE4 == 1 && byte_103F4C22 == 1 ) /*0x10036f48*/
      {
        v158 = *(void (__thiscall **)(_DWORD))(*(_DWORD *)v386 + 44); /*0x10036f4c*/
        v471[5] = 8; /*0x10036f4f*/
        v158(a1: v386); /*0x10036f5b*/
        v45 = (__int128 *)v387; /*0x10036f5d*/
        v46 = v386; /*0x10036f60*/
        __asm /*0x10036f63*/
        {
          vmovss xmm0, dword ptr [esi+74h]
          vsubss xmm0, xmm0, dword ptr [eax+8]
          vmovss xmm1, dword ptr [esi+78h]
          vsubss xmm1, xmm1, dword ptr [eax+4]
          vmovss xmm2, dword ptr [esi+70h]
          vsubss xmm2, xmm2, dword ptr [eax]
          vmulss xmm1, xmm1, xmm1
          vfmadd231ss xmm1, xmm2, xmm2
          vfmadd231ss xmm1, xmm0, xmm0
          vcvtss2sd xmm0, xmm1, xmm1
          vsqrtsd xmm0, xmm0, xmm0
          vcvtsd2ss xmm0, xmm0, xmm0
          vucomiss xmm0, dword_103F4BE0
        }
        if ( v157 | v39 ) /*0x10036fa2*/
        {
LABEL_121:
          sub_100386A0(a1: &v417); /*0x10036fa4*/
          v45 = (__int128 *)v387; /*0x10036fb3*/
          v46 = v386; /*0x10036fb6*/
        }
      }
    }
    else
    {
      v46 = v386; /*0x10036e9a*/
      if ( v393 == 2 && byte_103F4BE5 == 1 && byte_103F4D18 == 1 ) /*0x10036eb7*/
      {
        v145 = *(void (__thiscall **)(_DWORD))(*(_DWORD *)v386 + 44); /*0x10036ebf*/
        v471[5] = 8; /*0x10036ec2*/
        v145(a1: v386); /*0x10036ece*/
        v45 = (__int128 *)v387; /*0x10036ed0*/
        v46 = v386; /*0x10036ed3*/
        __asm /*0x10036ed6*/
        {
          vmovss xmm0, dword ptr [esi+74h]
          vsubss xmm0, xmm0, dword ptr [eax+8]
          vmovss xmm1, dword ptr [esi+78h]
          vsubss xmm1, xmm1, dword ptr [eax+4]
          vmovss xmm2, dword ptr [esi+70h]
          vsubss xmm2, xmm2, dword ptr [eax]
          vmulss xmm1, xmm1, xmm1
          vfmadd231ss xmm1, xmm2, xmm2
          vfmadd231ss xmm1, xmm0, xmm0
          vcvtss2sd xmm0, xmm1, xmm1
          vsqrtsd xmm0, xmm0, xmm0
          vcvtsd2ss xmm0, xmm0, xmm0
          vucomiss xmm0, dword_103F4BE0
        }
        if ( v157 | v39 ) /*0x10036f15*/
          goto LABEL_121; /*0x10036f15*/
      }
    }
    if ( v428 >= 0x10 ) /*0x10036fc2*/
    {
      v170 = (void *)v426[0]; /*0x10036fc4*/
      if ( v428 + 1 >= 0x1000 ) /*0x10036fd3*/
      {
        if ( (unsigned int)(v426[0] - 4 - *(_DWORD *)(v426[0] - 4)) >= 0x20 ) /*0x10036fe0*/
          goto LABEL_36; /*0x10036fe0*/
        v170 = *(void **)(v426[0] - 4); /*0x10036feb*/
      }
      sub_10180976(Block: v170); /*0x10036fef*/
      v45 = (__int128 *)v387; /*0x10036ff7*/
    }
    v427 = 0; /*0x10036ffa*/
    v428 = 15; /*0x10037004*/
    LOBYTE(v426[0]) = 0; /*0x1003700e*/
    if ( v422 >= 0x10 ) /*0x1003701e*/
    {
      v171 = (void *)v419; /*0x10037020*/
      if ( v422 + 1 >= 0x1000 ) /*0x1003702f*/
      {
        if ( (unsigned int)(v419 - 4 - *(_DWORD *)(v419 - 4)) >= 0x20 ) /*0x1003703c*/
          goto LABEL_36; /*0x1003703c*/
        v171 = *(void **)(v419 - 4); /*0x10037047*/
      }
      sub_10180976(Block: v171); /*0x1003704b*/
      v45 = (__int128 *)v387; /*0x10037053*/
    }
    if ( v400 >= 0x10 ) /*0x1003705c*/
    {
      v47 = *(void **)v398; /*0x10037062*/
      if ( v400 + 1 >= 0x1000 ) /*0x1003706e*/
      {
        if ( (unsigned int)(*(_DWORD *)v398 - 4 - *(_DWORD *)(*(_DWORD *)v398 - 4)) >= 0x20 ) /*0x1003707f*/
          goto LABEL_36; /*0x1003707f*/
        v47 = *(void **)(*(_DWORD *)v398 - 4); /*0x10036565*/
      }
      sub_10180976(Block: v47); /*0x10036569*/
      v45 = (__int128 *)v387; /*0x10036571*/
    }
LABEL_70:
    v45 = (__int128 *)((char *)v45 + 4); /*0x10036580*/
  }
  v471[5] = -1; /*0x10037226*/
  v172 = 1; /*0x10037230*/
  while ( v172 <= (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC7B8 + 32))(a1: dword_103EC7B8) ) /*0x1003726b*/
  {
    v173 = *(_DWORD *)dword_103EC7B8; /*0x10037277*/
    v385 = (__int128 *)v172; /*0x10037279*/
    v174 = (*(int (__thiscall **)(int, int))(v173 + 12))(a1: dword_103EC7B8, a2: v172); /*0x1003727d*/
    if ( v174 != 0 ) /*0x10037282*/
    {
      v175 = v174 + 8; /*0x10037286*/
      v176 = *(_DWORD *)(v174 + 8); /*0x10037289*/
      v384 = v174; /*0x1003728e*/
      if ( (*(unsigned __int8 (__thiscall **)(int))(v176 + 28))(a1: v174 + 8) == 0 ) /*0x10037296*/
      {
        v177 = (*(int (__thiscall **)(int))(*(_DWORD *)v175 + 4))(a1: v175); /*0x1003729c*/
        if ( v177 != 0 ) /*0x100372a4*/
        {
          v178 = (const char *)v177; /*0x100372a6*/
          _EAX = (*(int (__thiscall **)(int))(*(_DWORD *)v384 + 44))(a1: v384); /*0x100372aa*/
          __asm /*0x100372ad*/
          {
            vmovss xmm0, dword ptr [eax]
            vmovss dword ptr [esi+2Ch], xmm0
            vmovss xmm0, dword ptr [eax+4]
            vmovss dword ptr [esi+3Ch], xmm0
            vmovss xmm0, dword ptr [eax+8]
            vmovss dword ptr [esi+28h], xmm0
          }
          *(_DWORD *)v398 = 0; /*0x100372ca*/
          *(_DWORD *)v416 = 0; /*0x100372d1*/
          v391[0] = 0; /*0x100372db*/
          v390 = 0; /*0x100372e2*/
          if ( *((_DWORD *)v178 + 5) == 277 ) /*0x100372f0*/
          {
            if ( dword_103EEF14 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10037311*/
            {
              sub_101809C3(a1: &dword_103EEF14); /*0x10037506*/
              if ( dword_103EEF14 == -1 ) /*0x10037515*/
              {
                v389 = v178; /*0x1003751b*/
                v471[5] = 9; /*0x1003751e*/
                dword_103EEF10 = sub_1017C730(a1: "CBaseEntity", a2: "m_usSolidFlags"); /*0x1003753a*/
                sub_10180A43(a1: &dword_103EEF14); /*0x10037544*/
                v46 = v386; /*0x1003754c*/
                v178 = v389; /*0x1003754f*/
              }
            }
            if ( *(_WORD *)(dword_103EEF10 + v384) != 0 ) /*0x10037324*/
            {
              v183 = v178; /*0x1003732a*/
              v184 = off_1029E57C; /*0x1003732c*/
              v389 = *((const char **)v183 + 5); /*0x10037335*/
              if ( dword_103EEF1C > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10037353*/
              {
                sub_101809C3(a1: &dword_103EEF1C); /*0x1003755c*/
                if ( dword_103EEF1C == -1 ) /*0x1003756b*/
                {
                  v392 = v184; /*0x10037571*/
                  v471[5] = 10; /*0x10037574*/
                  dword_103EEF18 = sub_1017C730(a1: "CBaseAnimating", a2: "m_nSequence"); /*0x10037590*/
                  sub_10180A43(a1: &dword_103EEF1C); /*0x1003759a*/
                  v184 = v392; /*0x100375a2*/
                }
              }
              v185 = v384; /*0x1003735e*/
              v186 = *(_DWORD *)(dword_103EEF18 + v384); /*0x10037361*/
              if ( dword_103EEF14 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1003737f*/
              {
                sub_101809C3(a1: &dword_103EEF14); /*0x100375af*/
                v185 = v384; /*0x100375b4*/
                if ( dword_103EEF14 == -1 ) /*0x100375c1*/
                {
                  v393 = v186; /*0x100375c7*/
                  v392 = v184; /*0x100375ca*/
                  v471[5] = 11; /*0x100375cd*/
                  dword_103EEF10 = sub_1017C730(a1: "CBaseEntity", a2: "m_usSolidFlags"); /*0x100375e9*/
                  sub_10180A43(a1: &dword_103EEF14); /*0x100375f3*/
                  v185 = v384; /*0x100375fb*/
                  v186 = v393; /*0x10037601*/
                }
              }
              v187 = *(_WORD *)(dword_103EEF10 + v185); /*0x1003738a*/
              v471[5] = -1; /*0x1003738e*/
              v188 = v185; /*0x1003739f*/
              v189 = sub_101669A0(a1: v187, a2: v186, a3: v389); /*0x100373a1*/
              v46 = v386; /*0x100373a8*/
              if ( v189 != 0 && (unsigned __int8)sub_1002E000(a1: v188, a2: v398, a3: v416, a4: v391, a5: &v390) != 0 ) /*0x100373cc*/
              {
                (*(void (__thiscall **)(int))(*(_DWORD *)v46 + 44))(a1: v46); /*0x100373d6*/
                __asm /*0x100373d9*/
                {
                  vmovss xmm0, dword ptr [esi+28h]
                  vsubss xmm0, xmm0, dword ptr [eax+8]
                  vmovss xmm1, dword ptr [esi+3Ch]
                  vsubss xmm1, xmm1, dword ptr [eax+4]
                  vmovss xmm2, dword ptr [esi+2Ch]
                  vsubss xmm2, xmm2, dword ptr [eax]
                  vmulss xmm1, xmm1, xmm1
                  vfmadd231ss xmm1, xmm2, xmm2
                  vfmadd231ss xmm1, xmm0, xmm0
                  vcvtss2sd xmm0, xmm1, xmm1
                  vsqrtsd xmm0, xmm0, xmm0
                  vcvtsd2ss xmm0, xmm0, xmm0
                  vucomiss xmm0, dword_103F4F00
                }
                if ( v157 | v39 ) /*0x10037418*/
                {
                  __asm /*0x1003741e*/
                  {
                    vmovss xmm0, dword ptr xmmword_103F4F04
                    vmovss dword ptr [esi+1Ch], xmm0
                    vmovss xmm0, dword ptr xmmword_103F4F04+4
                    vmovss dword ptr [esi+2Ch], xmm0
                    vmovss xmm0, dword ptr xmmword_103F4F04+8
                    vmovss dword ptr [esi+3Ch], xmm0
                  }
                  sub_1000E5E0(a1: &v417, Str: "Witch"); /*0x10037452*/
                  __asm /*0x10037457*/
                  {
                    vmovss xmm0, dword ptr [esi+58h]
                    vmovss dword ptr [esi+140h], xmm0
                    vmovss xmm0, dword ptr [esi+0B8h]
                    vmovss dword ptr [esi+144h], xmm0
                    vmovss xmm0, dword ptr [esi+38h]
                    vmovss dword ptr [esi+148h], xmm0
                    vmovss xmm0, dword ptr [esi+34h]
                    vmovss dword ptr [esi+14Ch], xmm0
                  }
                  v471[5] = 12; /*0x1003748e*/
                  sub_100385F0(Src: &v417); /*0x1003749f*/
                  __asm /*0x100374a4*/
                  {
                    vmovss xmm1, ds:dword_10184380
                    vmulss xmm0, xmm1, dword ptr [esi+3Ch]
                    vcvttss2si eax, xmm0
                    vmulss xmm0, xmm1, dword ptr [esi+2Ch]
                    vcvttss2si ecx, xmm0
                    vmulss xmm0, xmm1, dword ptr [esi+1Ch]
                    vcvttss2si edx, xmm0
                  }
                  LOBYTE(v459) = _EDX; /*0x100374c7*/
                  BYTE1(v459) = _ECX; /*0x100374cd*/
                  BYTE2(v459) = _EAX; /*0x100374d3*/
                  HIBYTE(v459) = -1; /*0x100374d9*/
                  v471[5] = 13; /*0x100374e0*/
                  sub_1003D950(a1: v454); /*0x100374f7*/
                  sub_10038770(a1: v454); /*0x1003723d*/
                  sub_1000BC90(a1: &v417); /*0x10037248*/
                  v46 = v386; /*0x1003724d*/
                }
              }
            }
          }
        }
      }
    }
    v172 = (int)v385 + 1; /*0x10037253*/
    v471[5] = -1; /*0x10037254*/
  }
  v216 = 1; /*0x10037609*/
  while ( v216 <= (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC7B8 + 32))(a1: dword_103EC7B8) ) /*0x1003764b*/
  {
    v217 = *(_DWORD *)dword_103EC7B8; /*0x10037657*/
    v385 = (__int128 *)v216; /*0x10037659*/
    v218 = (*(int (__thiscall **)(int, int))(v217 + 12))(a1: dword_103EC7B8, a2: v216); /*0x1003765d*/
    if ( v218 != 0 ) /*0x10037662*/
    {
      v219 = v218 + 8; /*0x10037666*/
      v220 = *(_DWORD *)(v218 + 8); /*0x10037669*/
      v384 = v218; /*0x1003766e*/
      if ( (*(unsigned __int8 (__thiscall **)(int))(v220 + 28))(a1: v218 + 8) == 0 ) /*0x10037676*/
      {
        v221 = (*(int (__thiscall **)(int))(*(_DWORD *)v219 + 4))(a1: v219); /*0x1003767c*/
        if ( v221 != 0 ) /*0x10037684*/
        {
          v222 = (const char *)v221; /*0x10037686*/
          _EAX = (*(int (__thiscall **)(int))(*(_DWORD *)v384 + 44))(a1: v384); /*0x1003768a*/
          __asm /*0x1003768d*/
          {
            vmovss xmm0, dword ptr [eax]
            vmovss dword ptr [esi+2Ch], xmm0
            vmovss xmm0, dword ptr [eax+4]
            vmovss dword ptr [esi+3Ch], xmm0
            vmovss xmm0, dword ptr [eax+8]
            vmovss dword ptr [esi+28h], xmm0
          }
          *(_DWORD *)v398 = 0; /*0x100376aa*/
          *(_DWORD *)v416 = 0; /*0x100376b1*/
          v391[0] = 0; /*0x100376bb*/
          v390 = 0; /*0x100376c2*/
          if ( *((_DWORD *)v222 + 5) == 264 ) /*0x100376d0*/
          {
            if ( dword_103EEF14 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x100376f1*/
            {
              sub_101809C3(a1: &dword_103EEF14); /*0x100378ec*/
              if ( dword_103EEF14 == -1 ) /*0x100378fb*/
              {
                v389 = v222; /*0x10037901*/
                v471[5] = 14; /*0x10037904*/
                dword_103EEF10 = sub_1017C730(a1: "CBaseEntity", a2: "m_usSolidFlags"); /*0x10037920*/
                sub_10180A43(a1: &dword_103EEF14); /*0x1003792a*/
                v46 = v386; /*0x10037932*/
                v222 = v389; /*0x10037935*/
              }
            }
            if ( *(_WORD *)(dword_103EEF10 + v384) != 0 ) /*0x10037704*/
            {
              v471[5] = -1; /*0x1003770a*/
              if ( (unsigned __int8)sub_1002E000(a1: v384, a2: v398, a3: v416, a4: v391, a5: &v390) != 0 ) /*0x1003772f*/
              {
                v389 = (const char *)off_1029E57C; /*0x1003773a*/
                v392 = *((void **)v222 + 5); /*0x10037740*/
                if ( dword_103EEF1C > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1003775e*/
                {
                  sub_101809C3(a1: &dword_103EEF1C); /*0x10037942*/
                  if ( dword_103EEF1C == -1 ) /*0x10037951*/
                  {
                    v471[5] = 15; /*0x10037957*/
                    dword_103EEF18 = sub_1017C730(a1: "CBaseAnimating", a2: "m_nSequence"); /*0x10037973*/
                    sub_10180A43(a1: &dword_103EEF1C); /*0x1003797d*/
                    v46 = v386; /*0x10037985*/
                  }
                }
                v227 = v384; /*0x10037769*/
                v228 = *(_DWORD *)(dword_103EEF18 + v384); /*0x1003776c*/
                if ( dword_103EEF14 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1003778a*/
                {
                  sub_101809C3(a1: &dword_103EEF14); /*0x10037992*/
                  v227 = v384; /*0x10037997*/
                  if ( dword_103EEF14 == -1 ) /*0x100379a4*/
                  {
                    v393 = v228; /*0x100379aa*/
                    v471[5] = 16; /*0x100379ad*/
                    dword_103EEF10 = sub_1017C730(a1: "CBaseEntity", a2: "m_usSolidFlags"); /*0x100379c9*/
                    sub_10180A43(a1: &dword_103EEF14); /*0x100379d3*/
                    v46 = v386; /*0x100379db*/
                    v227 = v384; /*0x100379de*/
                    v228 = v393; /*0x100379e1*/
                  }
                }
                v229 = *(_WORD *)(dword_103EEF10 + v227); /*0x10037795*/
                v471[5] = -1; /*0x10037799*/
                if ( (unsigned __int8)sub_101669A0(a1: v229, a2: v228, a3: v392) != 0 ) /*0x100377b2*/
                {
                  (*(void (__thiscall **)(int))(*(_DWORD *)v46 + 44))(a1: v46); /*0x100377bc*/
                  __asm /*0x100377bf*/
                  {
                    vmovss xmm0, dword ptr [esi+28h]
                    vsubss xmm0, xmm0, dword ptr [eax+8]
                    vmovss xmm1, dword ptr [esi+3Ch]
                    vsubss xmm1, xmm1, dword ptr [eax+4]
                    vmovss xmm2, dword ptr [esi+2Ch]
                    vsubss xmm2, xmm2, dword ptr [eax]
                    vmulss xmm1, xmm1, xmm1
                    vfmadd231ss xmm1, xmm2, xmm2
                    vfmadd231ss xmm1, xmm0, xmm0
                    vcvtss2sd xmm0, xmm1, xmm1
                    vsqrtsd xmm0, xmm0, xmm0
                    vcvtsd2ss xmm0, xmm0, xmm0
                    vucomiss xmm0, dword_103F4F3C
                  }
                  if ( v157 | v39 ) /*0x100377fe*/
                  {
                    __asm /*0x10037804*/
                    {
                      vmovss xmm0, dword ptr xmmword_103F4F40
                      vmovss dword ptr [esi+1Ch], xmm0
                      vmovss xmm0, dword ptr xmmword_103F4F40+4
                      vmovss dword ptr [esi+2Ch], xmm0
                      vmovss xmm0, dword ptr xmmword_103F4F40+8
                      vmovss dword ptr [esi+3Ch], xmm0
                    }
                    sub_1000E5E0(a1: &v417, Str: (char *)"Common"); /*0x10037838*/
                    __asm /*0x1003783d*/
                    {
                      vmovss xmm0, dword ptr [esi+58h]
                      vmovss dword ptr [esi+140h], xmm0
                      vmovss xmm0, dword ptr [esi+0B8h]
                      vmovss dword ptr [esi+144h], xmm0
                      vmovss xmm0, dword ptr [esi+38h]
                      vmovss dword ptr [esi+148h], xmm0
                      vmovss xmm0, dword ptr [esi+34h]
                      vmovss dword ptr [esi+14Ch], xmm0
                    }
                    v471[5] = 17; /*0x10037874*/
                    sub_100385F0(Src: &v417); /*0x10037885*/
                    __asm /*0x1003788a*/
                    {
                      vmovss xmm1, ds:dword_10184380
                      vmulss xmm0, xmm1, dword ptr [esi+3Ch]
                      vcvttss2si eax, xmm0
                      vmulss xmm0, xmm1, dword ptr [esi+2Ch]
                      vcvttss2si ecx, xmm0
                      vmulss xmm0, xmm1, dword ptr [esi+1Ch]
                      vcvttss2si edx, xmm0
                    }
                    LOBYTE(v459) = _EDX; /*0x100378ad*/
                    BYTE1(v459) = _ECX; /*0x100378b3*/
                    BYTE2(v459) = _EAX; /*0x100378b9*/
                    HIBYTE(v459) = -1; /*0x100378bf*/
                    v471[5] = 18; /*0x100378c6*/
                    sub_1003D950(a1: v454); /*0x100378dd*/
                    sub_10038770(a1: v454); /*0x10037616*/
                    sub_1000BC90(a1: &v417); /*0x10037621*/
                    v46 = v386; /*0x10037626*/
                  }
                }
              }
            }
          }
        }
      }
    }
    v216 = (int)v385 + 1; /*0x10037633*/
    v471[5] = -1; /*0x10037634*/
  }
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC7B8 + 32))(a1: dword_103EC7B8) > 0 ) /*0x100379f6*/
  {
    v256 = nullptr; /*0x100379fc*/
    do /*0x10037a3d*/
    {
      v259 = *(_DWORD *)dword_103EC7B8; /*0x10037a3d*/
      v385 = (__int128 *)((char *)v256 + 1); /*0x10037a3f*/
      v260 = (*(int (__thiscall **)(int, int))(v259 + 12))(a1: dword_103EC7B8, a2: (int)v256 + 1); /*0x10037a43*/
      if ( v260 != 0 ) /*0x10037a48*/
      {
        v261 = v260; /*0x10037a4a*/
        v262 = v260 + 8; /*0x10037a4e*/
        if ( (*(unsigned __int8 (__thiscall **)(int))(*(_DWORD *)(v260 + 8) + 28))(a1: v260 + 8) == 0 ) /*0x10037a5b*/
        {
          v263 = (*(int (__thiscall **)(int))(*(_DWORD *)v262 + 4))(a1: v262); /*0x10037a61*/
          if ( v263 != 0 ) /*0x10037a66*/
          {
            v264 = v263; /*0x10037a68*/
            _EAX = (*(int (__thiscall **)(int))(*(_DWORD *)v261 + 44))(a1: v261); /*0x10037a6e*/
            __asm /*0x10037a71*/
            {
              vmovss xmm0, dword ptr [eax]
              vmovss dword ptr [esi+1Ch], xmm0
              vmovss xmm0, dword ptr [eax+4]
              vmovss dword ptr [esi+2Ch], xmm0
              vmovss xmm0, dword ptr [eax+8]
              vmovss dword ptr [esi+3Ch], xmm0
            }
            *(_DWORD *)v398 = 0; /*0x10037a8e*/
            *(_DWORD *)v416 = 0; /*0x10037a95*/
            v391[0] = 0; /*0x10037a9f*/
            v390 = 0; /*0x10037aa6*/
            if ( *(_DWORD *)(v264 + 20) == 13 /*0x10037ad2*/
              && (unsigned __int8)sub_1002E000(a1: v261, a2: v398, a3: v416, a4: v391, a5: &v390) != 0 )
            {
              (*(void (__thiscall **)(int))(*(_DWORD *)v386 + 44))(a1: v386); /*0x10037add*/
              __asm /*0x10037ae0*/
              {
                vmovss xmm0, dword ptr [esi+3Ch]
                vsubss xmm0, xmm0, dword ptr [eax+8]
                vmovss xmm1, dword ptr [esi+2Ch]
                vsubss xmm1, xmm1, dword ptr [eax+4]
                vmovss xmm2, dword ptr [esi+1Ch]
                vsubss xmm2, xmm2, dword ptr [eax]
                vmulss xmm1, xmm1, xmm1
                vfmadd231ss xmm1, xmm2, xmm2
                vfmadd231ss xmm1, xmm0, xmm0
                vcvtss2sd xmm0, xmm1, xmm1
                vsqrtsd xmm0, xmm0, xmm0
                vcvtsd2ss xmm0, xmm0, xmm0
                vucomiss xmm0, dword_103F4F74
              }
              if ( v157 | v39 ) /*0x10037b1f*/
              {
                __asm /*0x10037b25*/
                {
                  vmovss xmm1, ds:dword_10184380
                  vmulss xmm2, xmm1, dword ptr xmmword_103F4F78+8
                  vmulss xmm0, xmm1, dword ptr xmmword_103F4F78+4
                  vmulss xmm1, xmm1, dword ptr xmmword_103F4F78
                }
                _EAX = v418; /*0x10037b45*/
                __asm /*0x10037b4b*/
                {
                  vxorps xmm3, xmm3, xmm3
                  vmovsd qword ptr [eax], xmm3
                }
                v418[2] = 0; /*0x10037b53*/
                v419 = 4; /*0x10037b5a*/
                v420 = 15; /*0x10037b64*/
                v417 = 1801678674; /*0x10037b6e*/
                __asm /*0x10037b78*/
                {
                  vmovss xmm3, dword ptr [esi+58h]
                  vmovss dword ptr [esi+140h], xmm3
                  vmovss xmm3, dword ptr [esi+0B8h]
                  vmovss dword ptr [esi+144h], xmm3
                  vmovss xmm3, dword ptr [esi+38h]
                  vmovss dword ptr [esi+148h], xmm3
                  vmovss xmm3, dword ptr [esi+34h]
                  vmovss dword ptr [esi+14Ch], xmm3
                }
                v457 = 4; /*0x10037baf*/
                v458 = 15; /*0x10037bb9*/
                __asm /*0x10037bc3*/
                {
                  vmovups xmm3, xmmword ptr [esi+0D0h]
                  vcvttss2si eax, xmm2
                  vcvttss2si ecx, xmm1
                }
                _EDX = v456; /*0x10037bd3*/
                __asm { vmovups xmmword ptr [edx], xmm3 } /*0x10037bd9*/
                LOBYTE(v459) = _ECX; /*0x10037bdd*/
                __asm { vcvttss2si ecx, xmm0 } /*0x10037be3*/
                BYTE1(v459) = _ECX; /*0x10037be7*/
                BYTE2(v459) = _EAX; /*0x10037bed*/
                HIBYTE(v459) = -1; /*0x10037bf3*/
                v471[5] = 19; /*0x10037bfa*/
                sub_1003D950(a1: v454); /*0x10037c11*/
                if ( v458 >= 0x10 ) /*0x10037c1f*/
                {
                  v257 = (void *)v456[0]; /*0x10037c25*/
                  if ( v458 + 1 >= 0x1000 ) /*0x10037c34*/
                  {
                    if ( (unsigned int)(v456[0] - 4 - *(_DWORD *)(v456[0] - 4)) >= 0x20 ) /*0x10037c45*/
                      goto LABEL_36; /*0x10037c45*/
                    v257 = *(void **)(v456[0] - 4); /*0x10037a05*/
                  }
                  sub_10180976(Block: v257); /*0x10037a09*/
                }
              }
            }
            v471[5] = -1; /*0x10037a11*/
          }
        }
      }
      v258 = (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC7B8 + 32))(a1: dword_103EC7B8); /*0x10037a28*/
      v256 = v385; /*0x10037a2b*/
    }
    while ( (int)v385 < v258 ); /*0x10037a3d*/
  }
  v295 = 1; /*0x10037c50*/
  while ( v295 <= (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC7B8 + 32))(a1: dword_103EC7B8) ) /*0x10037c8b*/
  {
    v296 = *(_DWORD *)dword_103EC7B8; /*0x10037c97*/
    v385 = (__int128 *)v295; /*0x10037c99*/
    v297 = (*(int (__thiscall **)(int, int))(v296 + 12))(a1: dword_103EC7B8, a2: v295); /*0x10037c9d*/
    if ( v297 != 0 ) /*0x10037ca2*/
    {
      v298 = v297; /*0x10037ca4*/
      v299 = v297 + 8; /*0x10037ca8*/
      if ( (*(unsigned __int8 (__thiscall **)(int))(*(_DWORD *)(v297 + 8) + 28))(a1: v297 + 8) == 0 ) /*0x10037cb5*/
      {
        v300 = (*(int (__thiscall **)(int))(*(_DWORD *)v299 + 4))(a1: v299); /*0x10037cbb*/
        if ( v300 != 0 ) /*0x10037cc0*/
        {
          v301 = v300; /*0x10037cc2*/
          _EAX = (*(int (__thiscall **)(int))(*(_DWORD *)v298 + 44))(a1: v298); /*0x10037cc8*/
          __asm /*0x10037ccb*/
          {
            vmovss xmm0, dword ptr [eax]
            vmovss dword ptr [esi+1Ch], xmm0
            vmovss xmm0, dword ptr [eax+4]
            vmovss dword ptr [esi+2Ch], xmm0
            vmovss xmm0, dword ptr [eax+8]
            vmovss dword ptr [esi+3Ch], xmm0
          }
          *(_DWORD *)v398 = 0; /*0x10037ce8*/
          *(_DWORD *)v416 = 0; /*0x10037cef*/
          v391[0] = 0; /*0x10037cf9*/
          v390 = 0; /*0x10037d00*/
          if ( *(_DWORD *)(v301 + 20) == 260 ) /*0x10037d0e*/
          {
            v306 = (*(int (__thiscall **)(int))(*(_DWORD *)v298 + 876))(a1: v298); /*0x10037d18*/
            v307 = v306 & ~(v306 >> 31); /*0x10037d23*/
            if ( v307 >= 38 ) /*0x10037d30*/
              v307 = 38; /*0x10037d30*/
            v308 = 4 * v307; /*0x10037d33*/
            switch ( dword_10185600[3 * v308 / 4u] ) /*0x10037d46*/
            {
              case 0: /*0x10037d46*/
                if ( byte_103F4DF0 == 1 ) /*0x10037d54*/
                  goto LABEL_223; /*0x10037d54*/
                break; /*0x10037d54*/
              case 1: /*0x10037d46*/
                if ( byte_103F4DF1 != 0 ) /*0x10037d90*/
                  goto LABEL_223; /*0x10037d90*/
                break; /*0x10037d90*/
              case 2: /*0x10037d46*/
                if ( byte_103F4DF4 != 0 ) /*0x10037d72*/
                  goto LABEL_223; /*0x10037d72*/
                break; /*0x10037d72*/
              case 3: /*0x10037d46*/
                if ( byte_103F4DF2 != 0 ) /*0x10037d81*/
                  goto LABEL_223; /*0x10037d81*/
                break; /*0x10037d81*/
              case 4: /*0x10037d46*/
                if ( byte_103F4DF3 != 0 ) /*0x10037d63*/
                {
LABEL_223:
                  v387 = dword_10185600[3 * v308 / 4u]; /*0x10037d96*/
                  if ( (unsigned __int8)sub_1002E000(a1: v298, a2: v398, a3: v416, a4: v391, a5: &v390) != 0 ) /*0x10037db4*/
                  {
                    (*(void (__thiscall **)(int))(*(_DWORD *)v386 + 44))(a1: v386); /*0x10037dbf*/
                    __asm /*0x10037dc5*/
                    {
                      vmovss xmm0, dword ptr [esi+3Ch]
                      vsubss xmm0, xmm0, dword ptr [eax+8]
                      vmovss xmm1, dword ptr [esi+2Ch]
                      vsubss xmm1, xmm1, dword ptr [eax+4]
                      vmovss xmm2, dword ptr [esi+1Ch]
                      vsubss xmm2, xmm2, dword ptr [eax]
                      vmulss xmm1, xmm1, xmm1
                      vfmadd231ss xmm1, xmm2, xmm2
                      vfmadd231ss xmm1, xmm0, xmm0
                      vcvtss2sd xmm0, xmm1, xmm1
                      vsqrtsd xmm0, xmm0, xmm0
                      vcvtsd2ss xmm0, xmm0, xmm0
                      vucomiss xmm0, dword_103F4DF8
                    }
                    if ( v157 | v39 ) /*0x10037e04*/
                    {
                      if ( byte_103F4DE4 == 1 ) /*0x10037e11*/
                      {
                        v321 = &xmmword_103F4DFC; /*0x10037e17*/
                        switch ( v387 ) /*0x10037e1c*/
                        {
                          case 0: /*0x10037e1c*/
                            goto LABEL_231;
                          case 1: /*0x10037e1c*/
                            v321 = &xmmword_103F4E0C; /*0x10037e23*/
                            goto LABEL_231; /*0x10037e28*/
                          case 2: /*0x10037e1c*/
                            v384 = sub_1002DFA0(a1: &xmmword_103F4E3C); /*0x10037e3e*/
                            __asm /*0x10037e41*/
                            {
                              vxorps xmm0, xmm0, xmm0
                              vmovaps xmmword ptr [esi+0D0h], xmm0
                            }
                            v419 = 0; /*0x10037e4d*/
                            v420 = 15; /*0x10037e57*/
                            v471[5] = 20; /*0x10037e67*/
                            v323 = sub_10168120(a1: v298); /*0x10037e72*/
                            v324 = (char *)sub_10168500(a1: v323); /*0x10037e7e*/
                            sub_1002BB30(a1: &v417, Str: v324); /*0x10037e8a*/
                            goto LABEL_234; /*0x10037e8f*/
                          case 3: /*0x10037e1c*/
                            v321 = &xmmword_103F4E1C; /*0x10037e94*/
                            goto LABEL_231; /*0x10037e94*/
                          case 4: /*0x10037e1c*/
                            v321 = &xmmword_103F4E2C; /*0x10037e2a*/
LABEL_231:
                            v384 = sub_1002DFA0(a1: v321); /*0x10037e99*/
                            __asm /*0x10037eae*/
                            {
                              vxorps xmm0, xmm0, xmm0
                              vmovaps xmmword ptr [esi+0D0h], xmm0
                            }
                            v419 = 0; /*0x10037eba*/
                            v420 = 15; /*0x10037ec4*/
                            v326 = (&off_101855F8)[6 * v308 / 8u]; /*0x10037ece*/
                            v471[5] = 20; /*0x10037ed0*/
                            sub_100387C0(_ECX: v454, _XMM0, String: v326); /*0x10037ee1*/
                            v327 = v454; /*0x10037eed*/
                            if ( v456[1] >= 8u ) /*0x10037ef3*/
                              v327 = (_DWORD *)v454[0]; /*0x10037ef5*/
                            v471[5] = 22; /*0x10037f04*/
                            sub_100388A0(a1: &v417, a2: (int)v327, a3: (int)v327 + 2 * v456[0]); /*0x10037f16*/
                            sub_1000BCE0(a1: v454); /*0x10037f21*/
LABEL_234:
                            __asm /*0x10037f26*/
                            {
                              vmovss xmm0, dword ptr [esi+58h]
                              vmovss dword ptr [esi+140h], xmm0
                              vmovss xmm0, dword ptr [esi+0B8h]
                              vmovss dword ptr [esi+144h], xmm0
                              vmovss xmm0, dword ptr [esi+38h]
                              vmovss dword ptr [esi+148h], xmm0
                              vmovss xmm0, dword ptr [esi+34h]
                              vmovss dword ptr [esi+14Ch], xmm0
                            }
                            v471[5] = 20; /*0x10037f5d*/
                            sub_100385F0(Src: &v417); /*0x10037f74*/
                            v459 = v384; /*0x10037f7c*/
                            v471[5] = 21; /*0x10037f82*/
                            sub_1003D950(a1: v454); /*0x10037f99*/
                            sub_10038770(a1: v454); /*0x10037c5d*/
                            sub_1000BC90(a1: &v417); /*0x10037c68*/
                            break; /*0x10037f9e*/
                          default:
                            goto LABEL_205;
                        }
                      }
                    }
                  }
                }
                break; /*0x10037f9e*/
              default:
                break;
            }
          }
        }
      }
    }
LABEL_205:
    v295 = (int)v385 + 1; /*0x10037c70*/
    v471[5] = -1; /*0x10037c74*/
  }
  v332 = off_1029E014; /*0x10037fa3*/
  v333 = *(_DWORD *)off_1029E014; /*0x10037fa9*/
  v334 = *(_DWORD *)(*(_DWORD *)off_1029E014 + 4); /*0x10037fab*/
  v39 = *(_BYTE *)(v334 + 13) == 0; /*0x10037fae*/
  v385 = off_1029E014; /*0x10037fb2*/
  if ( !v39 ) /*0x10037fb5*/
  {
    v384 = 0; /*0x10037fb7*/
    v335 = v334; /*0x10037fbe*/
    v336 = v333; /*0x10037fc0*/
    if ( *(_BYTE *)(v333 + 13) == 0 ) /*0x10037fc6*/
      goto LABEL_243; /*0x10037fc6*/
LABEL_244:
    v39 = *((_DWORD *)v332 + 1) == 0x7FFFFFF; /*0x10038003*/
    v471[5] = -1; /*0x1003800a*/
    if ( !v39 ) /*0x10038014*/
    {
      _EAX = sub_10180946(Size: 0x20u); /*0x1003801c*/
      __asm /*0x10038024*/
      {
        vmovss xmm0, ds:dword_101858C0
        vmovups xmmword ptr [eax+10h], xmm0
      }
      *(_DWORD *)_EAX = v333; /*0x10038031*/
      *(_DWORD *)(_EAX + 4) = v333; /*0x10038033*/
      *(_DWORD *)(_EAX + 8) = v333; /*0x10038036*/
      *(_WORD *)(_EAX + 12) = 0; /*0x10038039*/
      v336 = sub_10018900(a1: v335, a2: v384, a3: _EAX); /*0x10038047*/
      goto LABEL_246; /*0x10038047*/
    }
LABEL_260:
    sub_10018B20(); /*0x100382bf*/
  }
  v336 = v333; /*0x10037fca*/
  do /*0x10037fe7*/
  {
    v335 = v334; /*0x10037fd0*/
    v337 = *(_DWORD *)(v334 + 16); /*0x10037fd2*/
    v338 = 0; /*0x10037fd5*/
    if ( v337 >= 3 ) /*0x10037fda*/
      v336 = v335; /*0x10037fda*/
    LOBYTE(v338) = v337 < 3; /*0x10037fdd*/
    v334 = *(_DWORD *)(v335 + 8 * v338); /*0x10037fe0*/
  }
  while ( *(_BYTE *)(v334 + 13) == 0 ); /*0x10037fe7*/
  v384 = v337 >= 3; /*0x10037ff1*/
  v332 = v385; /*0x10037ff4*/
  if ( *(_BYTE *)(v336 + 13) != 0 ) /*0x10037ffb*/
    goto LABEL_244; /*0x10037ffb*/
LABEL_243:
  if ( *(int *)(v336 + 16) >= 4 ) /*0x10038001*/
    goto LABEL_244; /*0x10038001*/
LABEL_246:
  v341 = *(__int128 **)(v336 + 20); /*0x1003804c*/
  result = *(_DWORD *)(v336 + 24); /*0x1003804f*/
  v396 = result; /*0x10038052*/
  v342 = v386; /*0x10038057*/
  if ( v341 != (__int128 *)result ) /*0x1003805a*/
  {
    v471[5] = -1; /*0x10038060*/
    do /*0x1003808f*/
    {
      v385 = v341; /*0x1003808f*/
      v344 = *(_DWORD *)v341; /*0x10038092*/
      v345 = *(int (__thiscall **)(_DWORD))(**(_DWORD **)v341 + 44); /*0x10038096*/
      v471[5] = -1; /*0x10038099*/
      _EAX = v345(a1: v344); /*0x100380a5*/
      __asm /*0x100380a7*/
      {
        vmovss xmm0, dword ptr [eax]
        vmovss dword ptr [esi+1Ch], xmm0
        vmovss xmm0, dword ptr [eax+4]
        vmovss dword ptr [esi+2Ch], xmm0
        vmovss xmm0, dword ptr [eax+8]
        vmovss dword ptr [esi+3Ch], xmm0
      }
      *(_DWORD *)v398 = 0; /*0x100380c4*/
      *(_DWORD *)v416 = 0; /*0x100380cb*/
      v391[0] = 0; /*0x100380d5*/
      v390 = 0; /*0x100380dc*/
      __asm /*0x100380e3*/
      {
        vmovss xmm0, ds:dword_10184380
        vmulss xmm1, xmm0, dword ptr xmmword_103F4E2C
        vmovss dword ptr [esi+28h], xmm1
        vmulss xmm1, xmm0, dword ptr xmmword_103F4E2C+4
        vmovss dword ptr [esi+30h], xmm1
        vmulss xmm1, xmm0, dword ptr xmmword_103F4E2C+8
        vmovss dword ptr [esi+40h], xmm1
        vmulss xmm0, xmm0, dword ptr xmmword_103F4E2C+0Ch
        vmovss dword ptr [esi+44h], xmm0
      }
      result = sub_1002E000(a1: v344, a2: v398, a3: v416, a4: v391, a5: &v390); /*0x10038133*/
      if ( (_BYTE)result != 0 ) /*0x1003813a*/
      {
        result = (*(int (__thiscall **)(int))(*(_DWORD *)v342 + 44))(a1: v342); /*0x10038144*/
        __asm /*0x10038147*/
        {
          vmovss xmm0, dword ptr [esi+3Ch]
          vsubss xmm0, xmm0, dword ptr [eax+8]
          vmovss xmm1, dword ptr [esi+2Ch]
          vsubss xmm1, xmm1, dword ptr [eax+4]
          vmovss xmm2, dword ptr [esi+1Ch]
          vsubss xmm2, xmm2, dword ptr [eax]
          vmulss xmm1, xmm1, xmm1
          vfmadd231ss xmm1, xmm2, xmm2
          vfmadd231ss xmm1, xmm0, xmm0
          vcvtss2sd xmm0, xmm1, xmm1
          vsqrtsd xmm0, xmm0, xmm0
          vcvtsd2ss xmm0, xmm0, xmm0
          vucomiss xmm0, dword_103F4DF8
        }
        if ( v157 | v39 ) /*0x10038186*/
        {
          __asm /*0x1003818c*/
          {
            vcvttss2si eax, dword ptr [esi+40h]
            vcvttss2si ecx, dword ptr [esi+44h]
          }
          v369 = (_ECX << 24) | ((unsigned __int8)_EAX << 16); /*0x1003819f*/
          __asm { vcvttss2si ecx, dword ptr [esi+30h] } /*0x100381a1*/
          v371 = v369 | ((unsigned __int8)_ECX << 8); /*0x100381ac*/
          __asm { vcvttss2si eax, dword ptr [esi+28h] } /*0x100381ae*/
          v373 = v371 | (unsigned __int8)_EAX; /*0x100381b6*/
          v419 = 10; /*0x100381b8*/
          v420 = 15; /*0x100381c2*/
          __asm /*0x100381cc*/
          {
            vmovsd xmm0, ds:qword_101858B0
            vmovups xmmword ptr [esi+0D0h], xmm0
          }
          LOWORD(v418[1]) = 28271; /*0x100381dc*/
          __asm /*0x100381e5*/
          {
            vmovss xmm0, dword ptr [esi+58h]
            vmovss dword ptr [esi+140h], xmm0
            vmovss xmm0, dword ptr [esi+0B8h]
            vmovss dword ptr [esi+144h], xmm0
            vmovss xmm0, dword ptr [esi+38h]
            vmovss dword ptr [esi+148h], xmm0
            vmovss xmm0, dword ptr [esi+34h]
            vmovss dword ptr [esi+14Ch], xmm0
          }
          v457 = 10; /*0x1003821c*/
          v458 = 15; /*0x10038226*/
          __asm { vmovups xmm0, xmmword ptr [esi+0D0h] } /*0x10038230*/
          _ECX = v456; /*0x10038238*/
          __asm { vmovups xmmword ptr [ecx], xmm0 } /*0x1003823e*/
          v459 = v373; /*0x10038242*/
          v471[5] = 23; /*0x10038248*/
          sub_1003D950(a1: v454); /*0x1003825f*/
          result = v458; /*0x10038264*/
          v342 = v386; /*0x1003826d*/
          if ( v458 >= 0x10 ) /*0x10038270*/
          {
            v343 = (void *)v456[0]; /*0x10038276*/
            if ( v458 + 1 >= 0x1000 ) /*0x10038285*/
            {
              if ( (unsigned int)(v456[0] - 4 - *(_DWORD *)(v456[0] - 4)) >= 0x20 ) /*0x10038296*/
                goto LABEL_36; /*0x10038296*/
              v343 = *(void **)(v456[0] - 4); /*0x10038071*/
            }
            result = sub_10180976(Block: v343); /*0x10038075*/
          }
        }
      }
      v341 = (__int128 *)((char *)v385 + 4); /*0x10038083*/
    }
    while ( (__int128 *)((char *)v385 + 4) != (__int128 *)v396 ); /*0x1003808f*/
  }
  return result; /*0x100382a1*/
}