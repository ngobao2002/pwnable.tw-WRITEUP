__int64 __fastcall msgpack_object_print_buffer(
        char *a1,
        size_t a2,
        __int64 a3,
        __int64 a4,
        __int64 a5,
        __int64 a6,
        unsigned int a7,
        __int64 a8,
        __int64 a9)
{
  __int64 result; // rax
  int v10; // eax
  int v11; // r8d
  int v12; // r9d
  int v13; // r8d
  int v14; // r9d
  int v15; // r8d
  int v16; // r9d
  int v17; // r8d
  int v18; // r9d
  int v19; // r8d
  int v20; // r9d
  int v21; // r8d
  int v22; // r9d
  int v23; // [rsp+2Ch] [rbp-34h]
  int v24; // [rsp+2Ch] [rbp-34h]
  int v25; // [rsp+2Ch] [rbp-34h]
  int v26; // [rsp+2Ch] [rbp-34h]
  int v27; // [rsp+2Ch] [rbp-34h]
  int v28; // [rsp+2Ch] [rbp-34h]
  int v29; // [rsp+2Ch] [rbp-34h]
  int v30; // [rsp+2Ch] [rbp-34h]
  int v31; // [rsp+2Ch] [rbp-34h]
  int v32; // [rsp+2Ch] [rbp-34h]
  int v33; // [rsp+2Ch] [rbp-34h]
  int v34; // [rsp+2Ch] [rbp-34h]
  int v35; // [rsp+2Ch] [rbp-34h]
  int v36; // [rsp+2Ch] [rbp-34h]
  int v37; // [rsp+2Ch] [rbp-34h]
  int v38; // [rsp+2Ch] [rbp-34h]
  int v39; // [rsp+2Ch] [rbp-34h]
  int v40; // [rsp+2Ch] [rbp-34h]
  int v41; // [rsp+2Ch] [rbp-34h]
  int v42; // [rsp+2Ch] [rbp-34h]
  int v43; // [rsp+2Ch] [rbp-34h]
  int v44; // [rsp+2Ch] [rbp-34h]
  int v45; // [rsp+2Ch] [rbp-34h]
  int v46; // [rsp+2Ch] [rbp-34h]
  int v47; // [rsp+2Ch] [rbp-34h]
  int v48; // [rsp+2Ch] [rbp-34h]
  int v49; // [rsp+2Ch] [rbp-34h]
  int v50; // [rsp+2Ch] [rbp-34h]
  int v51; // [rsp+2Ch] [rbp-34h]
  char *s; // [rsp+30h] [rbp-30h]
  char *sa; // [rsp+30h] [rbp-30h]
  char *sb; // [rsp+30h] [rbp-30h]
  char *sc; // [rsp+30h] [rbp-30h]
  char *sd; // [rsp+30h] [rbp-30h]
  char *se; // [rsp+30h] [rbp-30h]
  char *sf; // [rsp+30h] [rbp-30h]
  char *sg; // [rsp+30h] [rbp-30h]
  char *sh; // [rsp+30h] [rbp-30h]
  char *si; // [rsp+30h] [rbp-30h]
  char *sj; // [rsp+30h] [rbp-30h]
  char *sk; // [rsp+30h] [rbp-30h]
  int maxlen; // [rsp+38h] [rbp-28h]
  size_t maxlena; // [rsp+38h] [rbp-28h]
  size_t maxlenb; // [rsp+38h] [rbp-28h]
  size_t maxlenc; // [rsp+38h] [rbp-28h]
  size_t maxlend; // [rsp+38h] [rbp-28h]
  size_t maxlene; // [rsp+38h] [rbp-28h]
  size_t maxlenf; // [rsp+38h] [rbp-28h]
  size_t maxleng; // [rsp+38h] [rbp-28h]
  size_t maxlenh; // [rsp+38h] [rbp-28h]
  size_t maxleni; // [rsp+38h] [rbp-28h]
  size_t maxlenj; // [rsp+38h] [rbp-28h]
  size_t maxlenk; // [rsp+38h] [rbp-28h]
  size_t maxlenl; // [rsp+38h] [rbp-28h]
  size_t maxlenm; // [rsp+38h] [rbp-28h]
  size_t maxlenn; // [rsp+38h] [rbp-28h]
  size_t maxleno; // [rsp+38h] [rbp-28h]
  _QWORD *i; // [rsp+40h] [rbp-20h]
  _QWORD *j; // [rsp+48h] [rbp-18h]

  switch ( a7 )
  {
    case 0u:
      v23 = snprintf(a1, a2, "nil");
      if ( v23 > 0 && v23 <= (int)a2 )
      {
        maxlen = a2 - v23;
        goto LABEL_114;
      }
      result = 0LL;
      break;
    case 1u:
      if ( (_BYTE)a8 )
        v10 = snprintf(a1, a2, "true");
      else
        v10 = snprintf(a1, a2, "false");
      if ( v10 > 0 && v10 <= (int)a2 )
      {
        maxlen = a2 - v10;
        goto LABEL_114;
      }
      result = 0LL;
      break;
    case 2u:
      v24 = snprintf(a1, a2, "%lu", a8);
      if ( v24 > 0 && v24 <= (int)a2 )
      {
        maxlen = a2 - v24;
        goto LABEL_114;
      }
      result = 0LL;
      break;
    case 3u:
      v25 = snprintf(a1, a2, "%li", a8);
      if ( v25 > 0 && v25 <= (int)a2 )
      {
        maxlen = a2 - v25;
        goto LABEL_114;
      }
      result = 0LL;
      break;
    case 4u:
    case 0xAu:
      v26 = snprintf(a1, a2, "%f", *(double *)&a8);
      if ( v26 > 0 && v26 <= (int)a2 )
      {
        maxlen = a2 - v26;
        goto LABEL_114;
      }
      result = 0LL;
      break;
    case 5u:
      v27 = snprintf(a1, a2, "\"");
      if ( v27 > 0 && v27 <= (int)a2 )
      {
        s = &a1[v27];
        maxlena = a2 - v27;
        v28 = snprintf(s, maxlena, "%.*s", a8, (const char *)a9);
        if ( v28 > 0 && v28 <= (int)maxlena )
        {
          maxlenb = maxlena - v28;
          v29 = snprintf(&s[v28], maxlenb, "\"");
          if ( v29 > 0 && v29 <= (int)maxlenb )
          {
            maxlen = maxlenb - v29;
            goto LABEL_114;
          }
          result = 0LL;
        }
        else
        {
          result = 0LL;
        }
      }
      else
      {
        result = 0LL;
      }
      break;
    case 6u:
      v37 = snprintf(a1, a2, "[");
      if ( v37 <= 0 || v37 > (int)a2 )
        return 0LL;
      sd = &a1[v37];
      maxlenh = a2 - v37;
      if ( (_DWORD)a8 )
      {
        v38 = msgpack_object_print_buffer(
                (_DWORD)sd,
                maxlenh,
                (_DWORD)sd,
                maxlenh,
                v11,
                v12,
                *(_QWORD *)a9,
                *(_QWORD *)(a9 + 8),
                *(_QWORD *)(a9 + 16));
        if ( v38 <= 0 || v38 > (int)maxlenh )
          return 0LL;
        sd += v38;
        maxlenh -= v38;
        for ( i = (_QWORD *)(a9 + 24); (unsigned __int64)i < a9 + 24 * (unsigned __int64)(unsigned int)a8; i += 3 )
        {
          v39 = snprintf(sd, maxlenh, ", ");
          if ( v39 <= 0 || v39 > (int)maxlenh )
            return 0LL;
          se = &sd[v39];
          maxleni = maxlenh - v39;
          v40 = msgpack_object_print_buffer((_DWORD)se, maxleni, (_DWORD)se, maxleni, v13, v14, *i, i[1], i[2]);
          if ( v40 <= 0 || v40 > (int)maxleni )
            return 0LL;
          sd = &se[v40];
          maxlenh = maxleni - v40;
        }
      }
      v41 = snprintf(sd, maxlenh, "]");
      if ( v41 > 0 && v41 <= (int)maxlenh )
      {
        maxlen = maxlenh - v41;
        goto LABEL_114;
      }
      result = 0LL;
      break;
    case 7u:
      v42 = snprintf(a1, a2, "{");
      if ( v42 <= 0 || v42 > (int)a2 )
        return 0LL;
      sf = &a1[v42];
      maxlenj = a2 - v42;
      if ( (_DWORD)a8 )
      {
        v43 = msgpack_object_print_buffer(
                (_DWORD)sf,
                maxlenj,
                (_DWORD)sf,
                maxlenj,
                v15,
                v16,
                *(_QWORD *)a9,
                *(_QWORD *)(a9 + 8),
                *(_QWORD *)(a9 + 16));
        if ( v43 <= 0 || v43 > (int)maxlenj )
          return 0LL;
        sg = &sf[v43];
        maxlenk = maxlenj - v43;
        v44 = snprintf(sg, maxlenk, "=>");
        if ( v44 <= 0 || v44 > (int)maxlenk )
          return 0LL;
        sh = &sg[v44];
        maxlenl = maxlenk - v44;
        v45 = msgpack_object_print_buffer(
                (_DWORD)sh,
                maxlenl,
                (_DWORD)sh,
                maxlenl,
                v17,
                v18,
                *(_QWORD *)(a9 + 24),
                *(_QWORD *)(a9 + 32),
                *(_QWORD *)(a9 + 40));
        if ( v45 <= 0 || v45 > (int)maxlenl )
          return 0LL;
        sf = &sh[v45];
        maxlenj = maxlenl - v45;
        for ( j = (_QWORD *)(a9 + 48); (unsigned __int64)j < a9 + 48 * (unsigned __int64)(unsigned int)a8; j += 6 )
        {
          v46 = snprintf(sf, maxlenj, ", ");
          if ( v46 <= 0 || v46 > (int)maxlenj )
            return 0LL;
          si = &sf[v46];
          maxlenm = maxlenj - v46;
          v47 = msgpack_object_print_buffer((_DWORD)si, maxlenm, (_DWORD)si, maxlenm, v19, v20, *j, j[1], j[2]);
          if ( v47 <= 0 || v47 > (int)maxlenm )
            return 0LL;
          sj = &si[v47];
          maxlenn = maxlenm - v47;
          v48 = snprintf(sj, maxlenn, "=>");
          if ( v48 <= 0 || v48 > (int)maxlenn )
            return 0LL;
          sk = &sj[v48];
          maxleno = maxlenn - v48;
          v49 = msgpack_object_print_buffer((_DWORD)sk, maxleno, (_DWORD)sk, maxleno, v21, v22, j[3], j[4], j[5]);
          if ( v49 <= 0 || v49 > (int)maxleno )
            return 0LL;
          sf = &sk[v49];
          maxlenj = maxleno - v49;
        }
      }
      v50 = snprintf(sf, maxlenj, "}");
      if ( v50 > 0 && v50 <= (int)maxlenj )
      {
        maxlen = maxlenj - v50;
        goto LABEL_114;
      }
      result = 0LL;
      break;
    case 8u:
      v30 = snprintf(a1, a2, "\"");
      if ( v30 > 0 && v30 <= (int)a2 )
      {
        sa = &a1[v30];
        maxlenc = a2 - v30;
        v31 = msgpack_object_bin_print_buffer(sa, maxlenc, a9, (unsigned int)a8);
        if ( v31 > 0 && v31 <= (int)maxlenc )
        {
          maxlend = maxlenc - v31;
          v32 = snprintf(&sa[v31], maxlend, "\"");
          if ( v32 > 0 && v32 <= (int)maxlend )
          {
            maxlen = maxlend - v32;
            goto LABEL_114;
          }
          result = 0LL;
        }
        else
        {
          result = 0LL;
        }
      }
      else
      {
        result = 0LL;
      }
      break;
    case 9u:
      v33 = snprintf(a1, a2, "(ext: %i)", (unsigned int)(char)a8);
      if ( v33 > 0 && v33 <= (int)a2 )
      {
        sb = &a1[v33];
        maxlene = a2 - v33;
        v34 = snprintf(sb, maxlene, "\"");
        if ( v34 > 0 && v34 <= (int)maxlene )
        {
          sc = &sb[v34];
          maxlenf = maxlene - v34;
          v35 = msgpack_object_bin_print_buffer(sc, maxlenf, a9, HIDWORD(a8));
          if ( v35 > 0 && v35 <= (int)maxlenf )
          {
            maxleng = maxlenf - v35;
            v36 = snprintf(&sc[v35], maxleng, "\"");
            if ( v36 > 0 && v36 <= (int)maxleng )
            {
              maxlen = maxleng - v36;
              goto LABEL_114;
            }
            result = 0LL;
          }
          else
          {
            result = 0LL;
          }
        }
        else
        {
          result = 0LL;
        }
      }
      else
      {
        result = 0LL;
      }
      break;
    default:
      v51 = snprintf(a1, a2, "#<UNKNOWN %i %lu>", a7, a8);
      if ( v51 > 0 && v51 <= (int)a2 )
      {
        maxlen = a2 - v51;
LABEL_114:
        result = (unsigned int)(a2 - maxlen);
      }
      else
      {
        result = 0LL;
      }
      break;
  }
  return result;
}
