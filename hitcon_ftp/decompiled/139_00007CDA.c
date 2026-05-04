__int64 __fastcall template_execute(__int64 a1, __int64 a2, unsigned __int64 a3, unsigned __int64 *a4)
{
  int v4; // eax
  _QWORD *v5; // rcx
  int v6; // r8d
  int v7; // r9d
  char v8; // cl
  __int64 v9; // rdx
  __int64 v10; // rdx
  __int64 v11; // rdx
  __int64 v12; // rdx
  __int64 v13; // rdx
  __int64 v14; // rdx
  int v15; // eax
  __int64 v16; // rdx
  __int64 v17; // rdx
  __int64 v18; // rdx
  float v20; // [rsp+8h] [rbp-98h]
  double v21; // [rsp+8h] [rbp-98h]
  unsigned int v23; // [rsp+38h] [rbp-68h]
  int v24; // [rsp+3Ch] [rbp-64h]
  unsigned int v25; // [rsp+40h] [rbp-60h]
  int v26; // [rsp+44h] [rbp-5Ch]
  unsigned __int16 netlongd; // [rsp+48h] [rbp-58h]
  unsigned int netlonge; // [rsp+48h] [rbp-58h]
  __int64 netlongf; // [rsp+48h] [rbp-58h]
  __int16 netlongg; // [rsp+48h] [rbp-58h]
  int netlongh; // [rsp+48h] [rbp-58h]
  __int64 netlongi; // [rsp+48h] [rbp-58h]
  uint16_t netlongj; // [rsp+48h] [rbp-58h]
  uint16_t netlongk; // [rsp+48h] [rbp-58h]
  uint16_t netlongl; // [rsp+48h] [rbp-58h]
  uint32_t netlongm; // [rsp+48h] [rbp-58h]
  uint16_t netlong; // [rsp+48h] [rbp-58h]
  unsigned int netlonga; // [rsp+48h] [rbp-58h]
  uint16_t netlongb; // [rsp+48h] [rbp-58h]
  unsigned int netlongc; // [rsp+48h] [rbp-58h]
  unsigned __int8 *v41; // [rsp+50h] [rbp-50h]
  unsigned __int8 *v42; // [rsp+58h] [rbp-48h]
  unsigned __int8 *v43; // [rsp+60h] [rbp-40h]
  _QWORD *v44; // [rsp+68h] [rbp-38h]
  __int64 v45; // [rsp+78h] [rbp-28h]
  char src[8]; // [rsp+80h] [rbp-20h] BYREF
  __int64 v47; // [rsp+88h] [rbp-18h]
  __int64 v48; // [rsp+90h] [rbp-10h]
  unsigned __int64 v49; // [rsp+98h] [rbp-8h]

  v49 = __readfsqword(0x28u);
  if ( a3 < *a4 )
    exit(1);
  v41 = (unsigned __int8 *)(*a4 + a2);
  v43 = (unsigned __int8 *)(a2 + a3);
  v42 = 0LL;
  v23 = *(_DWORD *)(a1 + 20);
  v24 = *(_DWORD *)(a1 + 16);
  v25 = *(_DWORD *)(a1 + 24);
  v44 = (_QWORD *)(a1 + 32);
  if ( v41 == (unsigned __int8 *)(a2 + a3) )
  {
LABEL_147:
    v26 = 0;
    goto LABEL_148;
  }
  while ( 1 )
  {
    if ( v24 )
      goto LABEL_48;
    if ( *v41 > 0x9Fu )
      break;
    if ( *v41 >= 0x90u )
    {
      if ( v25 > 0x1F )
      {
        v26 = -2;
        goto LABEL_148;
      }
      v26 = template_callback_array((unsigned __int64 ***)a1, *v41 & 0xF, (__int64)&v44[8 * (unsigned __int64)v25]);
      if ( v26 < 0 )
        goto LABEL_148;
      if ( (*v41 & 0xF) == 0 )
      {
        v5 = &v44[8 * (unsigned __int64)v25];
        v9 = v5[1];
        *(_QWORD *)src = *v5;
        v47 = v9;
        v48 = v5[2];
        goto LABEL_128;
      }
      LODWORD(v44[8 * (unsigned __int64)v25 + 4]) = 0;
      v44[8 * (unsigned __int64)v25++ + 3] = *v41 & 0xF;
    }
    else
    {
      if ( *v41 <= 0x7Fu )
      {
        v26 = template_callback_uint8(a1, *v41, (__int64)src);
        if ( v26 >= 0 )
          goto LABEL_128;
        goto LABEL_148;
      }
      if ( v25 > 0x1F )
      {
        v26 = -2;
        goto LABEL_148;
      }
      v26 = template_callback_map((unsigned __int64 ***)a1, *v41 & 0xF, (__int64)&v44[8 * (unsigned __int64)v25]);
      if ( v26 < 0 )
        goto LABEL_148;
      if ( (*v41 & 0xF) == 0 )
      {
        v5 = &v44[8 * (unsigned __int64)v25];
        v10 = v5[1];
        *(_QWORD *)src = *v5;
        v47 = v10;
        v48 = v5[2];
        goto LABEL_128;
      }
      LODWORD(v44[8 * (unsigned __int64)v25 + 4]) = 1;
      v44[8 * (unsigned __int64)v25++ + 3] = *v41 & 0xF;
    }
LABEL_140:
    v24 = 0;
    if ( ++v41 == v43 )
      goto LABEL_147;
  }
  if ( *v41 <= 0xDFu )
  {
    if ( *v41 >= 0xC0u )
    {
      switch ( *v41 )
      {
        case 0xC0u:
          v26 = template_callback_nil(a1, src);
          if ( v26 < 0 )
            goto LABEL_148;
          goto LABEL_128;
        case 0xC2u:
          v26 = template_callback_false(a1, (__int64)src);
          if ( v26 < 0 )
            goto LABEL_148;
          goto LABEL_128;
        case 0xC3u:
          v26 = template_callback_true(a1, (__int64)src);
          if ( v26 < 0 )
            goto LABEL_148;
          goto LABEL_128;
        case 0xC4u:
        case 0xC5u:
        case 0xC6u:
          v23 = 1 << (*v41 & 3);
          v24 = *v41 & 0x1F;
          break;
        case 0xC7u:
        case 0xC8u:
        case 0xC9u:
          v23 = 1 << ((*v41 + 1) & 3);
          v24 = *v41 & 0x1F;
          break;
        case 0xCAu:
        case 0xCBu:
        case 0xCCu:
        case 0xCDu:
        case 0xCEu:
        case 0xCFu:
        case 0xD0u:
        case 0xD1u:
        case 0xD2u:
        case 0xD3u:
          v23 = 1 << (*v41 & 3);
          v24 = *v41 & 0x1F;
          break;
        case 0xD4u:
        case 0xD5u:
        case 0xD6u:
        case 0xD7u:
          v8 = *v41 & 3;
          v23 = (1 << v8) + 1;
          if ( 1 << v8 == -1 )
          {
LABEL_97:
            v26 = template_callback_ext(a1, a2, v42, v23, (__int64)src);
            if ( v26 >= 0 )
              goto LABEL_128;
            goto LABEL_148;
          }
          v24 = 34;
          break;
        case 0xD8u:
          v23 = 17;
          v24 = 34;
          break;
        case 0xD9u:
        case 0xDAu:
        case 0xDBu:
          v23 = 1 << ((*v41 & 3) - 1);
          v24 = *v41 & 0x1F;
          break;
        case 0xDCu:
        case 0xDDu:
        case 0xDEu:
        case 0xDFu:
          v23 = 2 << (*v41 & 1);
          v24 = *v41 & 0x1F;
          break;
        default:
          v26 = -1;
          goto LABEL_148;
      }
    }
    else
    {
      v23 = *v41 & 0x1F;
      if ( (*v41 & 0x1F) == 0 )
      {
LABEL_93:
        v26 = template_callback_str(a1, a2, (__int64)v42, v23, (__int64)src);
        if ( v26 >= 0 )
          goto LABEL_128;
        goto LABEL_148;
      }
      v24 = 32;
    }
    while ( 2 )
    {
      ++v41;
LABEL_48:
      if ( v43 - v41 < (unsigned __int64)v23 )
        goto LABEL_147;
      v42 = v41;
      v41 += v23 - 1;
      switch ( v24 )
      {
        case 4:
          v23 = *v42;
          if ( !*v42 )
            goto LABEL_95;
          v24 = 33;
          continue;
        case 5:
          netlongk = ntohs(*(_WORD *)v42);
          v23 = netlongk;
          if ( !netlongk )
            goto LABEL_95;
          v24 = 33;
          continue;
        case 6:
          v23 = ntohl(*(_DWORD *)v42);
          if ( !v23 )
            goto LABEL_95;
          v24 = 33;
          continue;
        case 7:
          v23 = *v42 + 1;
          if ( *v42 == -1 )
            goto LABEL_97;
          v24 = 34;
          continue;
        case 8:
          netlongl = ntohs(*(_WORD *)v42);
          v23 = netlongl + 1;
          if ( netlongl == -1 )
            goto LABEL_97;
          v24 = 34;
          continue;
        case 9:
          netlongm = ntohl(*(_DWORD *)v42);
          v23 = netlongm + 1;
          if ( netlongm == -1 )
            goto LABEL_97;
          v24 = 34;
          continue;
        case 10:
          v20 = COERCE_FLOAT(ntohl(*(_DWORD *)v42));
          v26 = template_callback_float(v20, a1, (__int64)src);
          if ( v26 >= 0 )
            goto LABEL_128;
          goto LABEL_148;
        case 11:
          v21 = COERCE_DOUBLE(_bswap_64_0(*(_QWORD *)v42));
          v26 = template_callback_double(v21, a1, (__int64)src);
          if ( v26 >= 0 )
            goto LABEL_128;
          goto LABEL_148;
        case 12:
          v26 = template_callback_uint8(a1, *v42, (__int64)src);
          if ( v26 >= 0 )
            goto LABEL_128;
          goto LABEL_148;
        case 13:
          netlongd = ntohs(*(_WORD *)v42);
          v26 = template_callback_uint16(a1, netlongd, (__int64)src);
          if ( v26 >= 0 )
            goto LABEL_128;
          goto LABEL_148;
        case 14:
          netlonge = ntohl(*(_DWORD *)v42);
          v26 = template_callback_uint32(a1, netlonge, (__int64)src);
          if ( v26 >= 0 )
            goto LABEL_128;
          goto LABEL_148;
        case 15:
          netlongf = _bswap_64_0(*(_QWORD *)v42);
          v26 = template_callback_uint64(a1, netlongf, (__int64)src);
          if ( v26 >= 0 )
            goto LABEL_128;
          goto LABEL_148;
        case 16:
          v26 = template_callback_int8(a1, *v42, (__int64)src);
          if ( v26 >= 0 )
            goto LABEL_128;
          goto LABEL_148;
        case 17:
          netlongg = ntohs(*(_WORD *)v42);
          v26 = template_callback_int16(a1, netlongg, (__int64)src);
          if ( v26 >= 0 )
            goto LABEL_128;
          goto LABEL_148;
        case 18:
          netlongh = ntohl(*(_DWORD *)v42);
          v26 = template_callback_int32(a1, netlongh, (__int64)src);
          if ( v26 >= 0 )
            goto LABEL_128;
          goto LABEL_148;
        case 19:
          netlongi = _bswap_64_0(*(_QWORD *)v42);
          v26 = template_callback_int64(a1, netlongi, (__int64)src);
          if ( v26 >= 0 )
            goto LABEL_128;
          goto LABEL_148;
        case 20:
          v23 = 2;
          v24 = 34;
          continue;
        case 21:
          v23 = 3;
          v24 = 34;
          continue;
        case 22:
          v23 = 5;
          v24 = 34;
          continue;
        case 23:
          v23 = 9;
          v24 = 34;
          continue;
        case 24:
          v23 = 17;
          v24 = 34;
          continue;
        case 25:
          v23 = *v42;
          if ( !*v42 )
            goto LABEL_93;
          v24 = 32;
          continue;
        case 26:
          netlongj = ntohs(*(_WORD *)v42);
          v23 = netlongj;
          if ( !netlongj )
            goto LABEL_93;
          v24 = 32;
          continue;
        case 27:
          v23 = ntohl(*(_DWORD *)v42);
          if ( !v23 )
            goto LABEL_93;
          v24 = 32;
          continue;
        case 28:
          netlong = ntohs(*(_WORD *)v42);
          if ( v25 > 0x1F )
          {
            v26 = -2;
            goto LABEL_148;
          }
          v26 = template_callback_array((unsigned __int64 ***)a1, netlong, (__int64)&v44[8 * (unsigned __int64)v25]);
          if ( v26 < 0 )
            goto LABEL_148;
          if ( !netlong )
          {
            v5 = &v44[8 * (unsigned __int64)v25];
            v11 = v5[1];
            *(_QWORD *)src = *v5;
            v47 = v11;
            v48 = v5[2];
            goto LABEL_128;
          }
          LODWORD(v44[8 * (unsigned __int64)v25 + 4]) = 0;
          v44[8 * (unsigned __int64)v25++ + 3] = netlong;
          break;
        case 29:
          netlonga = ntohl(*(_DWORD *)v42);
          if ( v25 > 0x1F )
          {
            v26 = -2;
            goto LABEL_148;
          }
          v26 = template_callback_array((unsigned __int64 ***)a1, netlonga, (__int64)&v44[8 * (unsigned __int64)v25]);
          if ( v26 < 0 )
            goto LABEL_148;
          if ( !netlonga )
          {
            v5 = &v44[8 * (unsigned __int64)v25];
            v12 = v5[1];
            *(_QWORD *)src = *v5;
            v47 = v12;
            v48 = v5[2];
            goto LABEL_128;
          }
          LODWORD(v44[8 * (unsigned __int64)v25 + 4]) = 0;
          v44[8 * (unsigned __int64)v25++ + 3] = netlonga;
          goto LABEL_140;
        case 30:
          netlongb = ntohs(*(_WORD *)v42);
          if ( v25 > 0x1F )
          {
            v26 = -2;
            goto LABEL_148;
          }
          v26 = template_callback_map((unsigned __int64 ***)a1, netlongb, (__int64)&v44[8 * (unsigned __int64)v25]);
          if ( v26 < 0 )
            goto LABEL_148;
          if ( !netlongb )
          {
            v5 = &v44[8 * (unsigned __int64)v25];
            v13 = v5[1];
            *(_QWORD *)src = *v5;
            v47 = v13;
            v48 = v5[2];
            goto LABEL_128;
          }
          LODWORD(v44[8 * (unsigned __int64)v25 + 4]) = 1;
          v44[8 * (unsigned __int64)v25++ + 3] = netlongb;
          goto LABEL_140;
        case 31:
          netlongc = ntohl(*(_DWORD *)v42);
          if ( v25 > 0x1F )
          {
            v26 = -2;
            goto LABEL_148;
          }
          v26 = template_callback_map((unsigned __int64 ***)a1, netlongc, (__int64)&v44[8 * (unsigned __int64)v25]);
          if ( v26 < 0 )
            goto LABEL_148;
          if ( !netlongc )
          {
            v5 = &v44[8 * (unsigned __int64)v25];
            v14 = v5[1];
            *(_QWORD *)src = *v5;
            v47 = v14;
            v48 = v5[2];
            goto LABEL_128;
          }
          LODWORD(v44[8 * (unsigned __int64)v25 + 4]) = 1;
          v44[8 * (unsigned __int64)v25++ + 3] = netlongc;
          goto LABEL_140;
        case 32:
          goto LABEL_93;
        case 33:
LABEL_95:
          v26 = template_callback_bin(a1, a2, (__int64)v42, v23, (__int64)src);
          if ( v26 < 0 )
            goto LABEL_148;
          goto LABEL_128;
        case 34:
          goto LABEL_97;
        default:
          v26 = -1;
          goto LABEL_148;
      }
      goto LABEL_140;
    }
  }
  v4 = template_callback_int8(a1, *v41, (__int64)src);
  v26 = v4;
  if ( v4 >= 0 )
  {
LABEL_128:
    while ( v25 )
    {
      v45 = (__int64)&v44[8 * (unsigned __int64)(v25 - 1)];
      v15 = *(_DWORD *)(v45 + 32);
      if ( v15 == 1 )
      {
        v17 = v47;
        *(_QWORD *)(v45 + 40) = *(_QWORD *)src;
        *(_QWORD *)(v45 + 48) = v17;
        *(_QWORD *)(v45 + 56) = v48;
        *(_DWORD *)(v45 + 32) = 2;
        goto LABEL_140;
      }
      if ( v15 )
      {
        if ( v15 != 2 )
        {
          v26 = -1;
          goto LABEL_148;
        }
        v26 = template_callback_map_item(
                a1,
                v45,
                a1,
                v45,
                v6,
                v7,
                *(_QWORD *)(v45 + 40),
                *(_QWORD *)(v45 + 48),
                *(_QWORD *)(v45 + 56),
                src[0]);
        if ( v26 < 0 )
          goto LABEL_148;
        if ( --*(_QWORD *)(v45 + 24) )
        {
          *(_DWORD *)(v45 + 32) = 1;
          goto LABEL_140;
        }
      }
      else
      {
        v26 = template_callback_array_item(a1, v45, v45, (int)v5, v6, v7, src[0]);
        if ( v26 < 0 )
          goto LABEL_148;
        if ( --*(_QWORD *)(v45 + 24) )
          goto LABEL_140;
      }
      LODWORD(v5) = ((v25 - 1) << 6) + (_DWORD)v44;
      v16 = *(_QWORD *)(v45 + 8);
      *(_QWORD *)src = *(_QWORD *)v45;
      v47 = v16;
      v48 = *(_QWORD *)(v45 + 16);
      --v25;
    }
    v18 = v47;
    *v44 = *(_QWORD *)src;
    *(_QWORD *)(a1 + 40) = v18;
    *(_QWORD *)(a1 + 48) = v48;
    ++v41;
    v26 = 1;
  }
LABEL_148:
  *(_DWORD *)(a1 + 16) = v24;
  *(_DWORD *)(a1 + 20) = v23;
  *(_DWORD *)(a1 + 24) = v25;
  *a4 = (unsigned __int64)&v41[-a2];
  return (unsigned int)v26;
}
