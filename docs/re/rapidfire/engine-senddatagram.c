int __thiscall sub_101B9B30(_DWORD *this, _DWORD *a2)
{
  _DWORD *v2; // edi
  int v4; // ebx
  int v5; // eax
  int v6; // edi
  const char *v7; // eax
  int v9; // ebx
  int v10; // eax
  int v11; // eax
  int v12; // edi
  int i; // eax
  int v14; // ecx
  bool v15; // zf
  unsigned __int16 v16; // ax
  int v17; // edi
  int v18; // eax
  __int64 v19; // rax
  int v20; // eax
  char v21; // al
  int v22; // eax
  char v23; // al
  float v24; // edi
  double v25; // st7
  double v26; // st6
  double v27; // st7
  int v28; // esi
  int v29; // [esp+0h] [ebp-128h]
  int v30; // [esp+4h] [ebp-124h]
  __int64 v31; // [esp+8h] [ebp-120h]
  __int64 v32; // [esp+Ch] [ebp-11Ch]
  int v33; // [esp+10h] [ebp-118h]
  int v34; // [esp+10h] [ebp-118h]
  int v35; // [esp+38h] [ebp-F0h]
  int v36; // [esp+40h] [ebp-E8h] BYREF
  int v37; // [esp+48h] [ebp-E0h]
  int v38; // [esp+4Ch] [ebp-DCh]
  float v39; // [esp+58h] [ebp-D0h]
  char v40[4]; // [esp+5Ch] [ebp-CCh] BYREF
  unsigned __int8 v41; // [esp+63h] [ebp-C5h]
  char v42[128]; // [esp+64h] [ebp-C4h] BYREF
  char Buffer[64]; // [esp+E4h] [ebp-44h] BYREF

  v2 = a2; /*0x101b9b46*/
  *(_DWORD *)v40 = a2; /*0x101b9b4b*/
  v4 = sub_101C9FE0(); /*0x101b9b56*/
  v35 = v4; /*0x101b9b61*/
  if ( *(_DWORD *)(dword_10696B1C + 48) != 0 && a2 != nullptr ) /*0x101b9b6b*/
  {
    v5 = (a2[3] + 7) >> 3; /*0x101b9b73*/
    if ( v5 > 0 ) /*0x101b9b78*/
      (*(void (__cdecl **)(const char *, _DWORD, int))(g_pVCR + 144))(a1: "datagram", a2: *a2, a3: v5 - 1); /*0x101b9b92*/
  }
  if ( *(this + 35) == 0 ) /*0x101b9b9e*/
  {
    v6 = *(_DWORD *)(dword_106B2CB4 + 48); /*0x101b9ba8*/
    if ( v6 != (*(int (__thiscall **)(_DWORD *))(*this + 280))(a1: this) ) /*0x101b9bb7*/
      (*(void (__thiscall **)(_DWORD *, _DWORD))(*this + 276))(a1: this, a2: *(_DWORD *)(dword_106B2CB4 + 48)); /*0x101b9bcd*/
    v2 = *(_DWORD **)v40; /*0x101b9bcf*/
  }
  if ( sub_10290650() != 0 )
  {
    if ( *((_WORD *)this + 8491) != 0 ) /*0x101b9c13*/
    {
      sub_101B9240(a1: 0, a2: this + 8); /*0x101b9c1d*/
      sub_101B9240(a1: 1, a2: this + 17); /*0x101b9c2a*/
      sub_101B9240(a1: 2, a2: this + 26); /*0x101b9c37*/
    }
    if ( *((_BYTE *)this + 48) != 0 ) /*0x101b9c40*/
    {
      v7 = (const char *)sub_102904F0(a1: 0); /*0x101b9c4a*/
      ConMsg(a1: "%s:send reliable stream overflow\n", v7); /*0x101b9c55*/
      sub_101CA000(a1: v4); /*0x101b9c5c*/
      return 0; /*0x101b9c76*/
    }
    if ( (int)*(this + 11) > 0 ) /*0x101b9c7d*/
    {
      sub_101B9720(a1: this + 8, a2: 0); /*0x101b9c87*/
      sub_1028F5C0(a1: this + 8); /*0x101b9c8e*/
      v2 = *(_DWORD **)v40; /*0x101b9c93*/
    }
    sub_1028FCA0(a1: "CNetChan_TransmitBits->send", a2: v4, a3: 228016, a4: -1); /*0x101b9cac*/
    v34 = *(this + 2); /*0x101b9cb4*/
    v41 = 0; /*0x101b9cbb*/
    sub_10290330(a1: v34); /*0x101b9cc2*/
    sub_10290330(a1: *(this + 3)); /*0x101b9cd1*/
    sub_1028FBF0(a1: 0); /*0x101b9d26*/
    if ( (unsigned __int8)sub_101C95E0() != 0 ) /*0x101b9d32*/
      sub_10290310(a1: 0); /*0x101b9d3c*/
    v9 = (v38 + 7) >> 3; /*0x101b9d54*/
    sub_1028FBF0(a1: *(this + 6)); /*0x101b9d57*/
    v10 = *(this + 7); /*0x101b9d5c*/
    if ( v10 > 0 ) /*0x101b9d61*/
    {
      v41 = 16; /*0x101b9d6d*/
      sub_1028FBF0(a1: (unsigned __int8)v10); /*0x101b9d74*/
    }
    if ( (unsigned __int8)sub_101B60D0(a1: &v36) != 0 ) /*0x101b9d89*/
      v41 |= 1u; /*0x101b9d8b*/
    if ( v2 != nullptr )
    {
      if ( v2[3] >= v37 - v38 )
        ConDMsg(a1: "CNetChan::SendDatagram:  data would overfow, ignoring\n");
      else
        sub_1028F690(a1: *v2, a2: v2[3]); /*0x101b9db3*/
    }
    if ( *(this + 20) >= v37 - v38 )
      ConDMsg(a1: "CNetChan::SendDatagram:  Unreliable would overfow, ignoring\n");
    else
      sub_1028F690(a1: *(this + 17), a2: *(this + 20)); /*0x101b9de6*/
    sub_1028F5C0(a1: this + 17); /*0x101b9dfe*/
    v11 = *(this + 29); /*0x101b9e03*/
    if ( v11 > 0 && v11 < v37 - v38 ) /*0x101b9e18*/
    {
      sub_1028F690(a1: *(this + 26), a2: *(this + 29)); /*0x101b9e25*/
      sub_1028F5C0(a1: this + 26); /*0x101b9e2d*/
    }
    v12 = 16; /*0x101b9e39*/
    if ( *(this + 35) == 1 ) /*0x101b9e3e*/
      v12 = *(_DWORD *)(dword_106B248C + 48); /*0x101b9e45*/
    for ( i = v38; (v38 + 7) >> 3 < v12; i = v38 ) /*0x101b9e56*/
      sub_10061340(a1: 0, a2: 6, a3: 1); /*0x101b9e64*/
    if ( (unsigned int)(i % 8 - 1) <= 1 ) /*0x101b9e89*/
      sub_10061340(a1: 0, a2: 6, a3: 1); /*0x101b9e97*/
    v14 = v38 % 8; /*0x101b9eae*/
    if ( v38 % 8 > 0 ) /*0x101b9eb1*/
    {
      v41 |= 32 * (8 - v14); /*0x101b9ebf*/
      if ( 8 - v14 > 0 ) /*0x101b9ec7*/
        sub_10061340(a1: dword_10427C20[(8 - v14) & 0x1F] - 1, a2: 8 - v14, a3: 1); /*0x101b9ede*/
    }
    v15 = *(_DWORD *)(dword_106B23B4 + 48) == 0; /*0x101b9ee9*/
    v40[0] = 0; /*0x101b9eed*/
    if ( !v15 ) /*0x101b9ef4*/
      v40[0] = (v38 + 7) >> 3 >= *(_DWORD *)(dword_106B2444 + 48); /*0x101b9f0d*/
    LODWORD(v39) = v41; /*0x101b9f22*/
    sub_1028FBF0(a1: v41); /*0x101b9f28*/
    if ( (unsigned __int8)sub_101C95E0() != 0 ) /*0x101b9f34*/
    {
      v16 = sub_101B1930(a1: v9 + v36, a2: ((v38 + 7) >> 3) - v9); /*0x101b9f4e*/
      sub_10061340(a1: v16, a2: 16, a3: 1); /*0x101b9f64*/
    }
    v17 = sub_101CE860( /*0x101b9f9c*/
            a1: (int)this,
            a2: *(this + 35),
            a3: (Concurrency::details::WorkItem *)(this + 38),
            a4: v36,
            a5: (v38 + 7) >> 3,
            a6: 0,
            a7: v40[0],
            a8: 0);
    sub_1028F5C0(a1: this + 26); /*0x101b9faa*/
    v18 = *(_DWORD *)(dword_106B263C + 48); /*0x101b9fb5*/
    if ( v18 != 0
      && v18 != 2
      && (*(_DWORD *)(dword_106B25F4 + 48) == 0
       || (unsigned __int8)sub_102905E0(a1: this + 38) == 0
       && !Concurrency::details::WorkItem::IsContext(this: (Concurrency::details::WorkItem *)(this + 38))) )
    {
      memset(Buffer, 0, sizeof(Buffer)); /*0x101b9ffb*/
      if ( *(_DWORD *)(dword_106B23B4 + 48) != 0 && v17 != 0 ) /*0x101ba015*/
      {
        *(_DWORD *)v40 = (v38 + 7) >> 3; /*0x101ba023*/
        if ( v17 < *(int *)v40 ) /*0x101ba02b*/
          sub_10284980(Buffer, BufferCount: 0x40u, Format: " compression=%5u [%5.2f %%]", ArgList: v17); /*0x101ba051*/
      }
      v19 = ((__int64 (__thiscall *)(int *, _DWORD *))*(_DWORD *)(*off_10432404 + 28))(a1: off_10432404, a2: this + 38); /*0x101ba065*/
      *(_DWORD *)v40 = -14336; /*0x101ba06d*/
      if ( v19 != 0 ) /*0x101ba077*/
      {
        v40[0] = -1; /*0x101ba079*/
        v40[1] = -1; /*0x101ba07f*/
        v40[3] = -1; /*0x101ba085*/
        v40[2] = 100; /*0x101ba08b*/
        v32 = v19; /*0x101ba099*/
        v20 = sub_102904F0(a1: 0); /*0x101ba0a8*/
        v21 = (*(int (__thiscall **)(_DWORD *, int, _DWORD, _DWORD))*this)(a1: this, a2: v20, a3: v32, a4: HIDWORD(v32)); /*0x101ba0b4*/
        sub_10284980(Buffer: v42, BufferCount: 0x80u, Format: "%12.12s %21.21s s(%llx)", ArgList: v21); /*0x101ba0c8*/
      }
      else
      {
        v22 = sub_102904F0(a1: 0); /*0x101ba0da*/
        v23 = (*(int (__thiscall **)(_DWORD *, int))*this)(a1: this, a2: v22); /*0x101ba0e6*/
        sub_10284980(Buffer: v42, BufferCount: 0x80u, Format: "%12.12s %21.21s", ArgList: v23); /*0x101ba0fa*/
      }
      ConColorMsg(
        a1: (const struct Color *)v40,
        a2: "UDP -> %s: sz=%5i seq=%5i ack=%5i rel=%1i tm=%8.3f%s\n",
        v42,
        (v38 + 7) >> 3,
        *(this + 2) & 0x3F,
        *(this + 3) & 0x3F,
        LOBYTE(v39) & 1,
        dbl_106B2B48,
        Buffer);
    }
    LODWORD(v24) = v17 + 28; /*0x101ba15c*/
    v31 = (unsigned int)*(this + 7); /*0x101ba162*/
    v30 = *(this + 3); /*0x101ba163*/
    v29 = *(this + 2); /*0x101ba164*/
    v39 = v24; /*0x101ba169*/
    sub_101B14C0(a1: 0, a2: v29, a3: v30, a4: v31, a5: HIDWORD(v31), a6: LODWORD(v24)); /*0x101ba16f*/
    sub_101B1670(a1: 0, a2: LODWORD(v24)); /*0x101ba179*/
    if ( dbl_106B2B48 > *((double *)this + 23) ) /*0x101ba18e*/
      *((double *)this + 23) = dbl_106B2B48; /*0x101ba190*/
    v25 = (double)SLODWORD(v39) / (double)(int)*(this + 44) + *((double *)this + 23); /*0x101ba1a6*/
    *((double *)this + 23) = v25; /*0x101ba1ac*/
    v39 = *(float *)(dword_106B275C + 44); /*0x101ba1c4*/
    if ( v39 > 0.0 ) /*0x101ba1cc*/
    {
      v26 = v25; /*0x101ba1da*/
      v27 = v39 + dbl_106B2B48; /*0x101ba1da*/
      if ( v26 > v27 ) /*0x101ba1de*/
        *((double *)this + 23) = v27; /*0x101ba1e0*/
    }
    *(this + 7) = 0; /*0x101ba1f0*/
    v33 = v35; /*0x101ba1f7*/
  }
  else
  {
    *((double *)this + 23) = 0.0; /*0x101b9bef*/
    *(this + 7) = 0; /*0x101b9bf5*/
    sub_1028F5C0(a1: this + 8); /*0x101b9bf8*/
    sub_1028F5C0(a1: this + 17); /*0x101b9c00*/
    v33 = v4; /*0x101b9c05*/
  }
  v28 = ++*(this + 2); /*0x101ba1fb*/
  sub_101CA000(a1: v33); /*0x101ba1fe*/
  return v28 - 1; /*0x101b9c64*/
}