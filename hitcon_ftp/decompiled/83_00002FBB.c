__int64 __fastcall process_new(unsigned __int8 *a1, unsigned int a2, _QWORD *a3)
{
  const char *v3; // rax
  int *v4; // rax
  char *v5; // rax
  const char *v6; // rax
  int v7; // eax
  int *v8; // rax
  char *v9; // rax
  unsigned __int8 v12; // [rsp+23h] [rbp-10Dh]
  signed int i; // [rsp+24h] [rbp-10Ch]
  char *s1; // [rsp+28h] [rbp-108h]
  size_t n; // [rsp+30h] [rbp-100h]
  char *src; // [rsp+38h] [rbp-F8h]
  __int64 v17; // [rsp+40h] [rbp-F0h]
  __int64 v18; // [rsp+48h] [rbp-E8h]
  struct stat v19; // [rsp+50h] [rbp-E0h] BYREF
  char dest[8]; // [rsp+E0h] [rbp-50h] BYREF
  __int64 v21; // [rsp+E8h] [rbp-48h]
  __int64 v22; // [rsp+F0h] [rbp-40h]
  __int64 v23; // [rsp+F8h] [rbp-38h]
  __int64 v24; // [rsp+100h] [rbp-30h]
  __int64 v25; // [rsp+108h] [rbp-28h]
  __int64 v26; // [rsp+110h] [rbp-20h]
  __int64 v27; // [rsp+118h] [rbp-18h]
  unsigned __int64 v28; // [rsp+128h] [rbp-8h]

  v28 = __readfsqword(0x28u);
  v12 = 0;
  *__errno_location() = 0;
  *((_WORD *)a1 + 145) = 0;
  *((_WORD *)a1 + 140) = 512;
  a1[288] = 3;
  *((_QWORD *)a1 + 1) = a1[288] + time(0LL);
  n = a3[4];
  src = (char *)a3[5];
  if ( !(_DWORD)n || (unsigned int)n > 0x100 )
  {
    *((_DWORD *)a1 + 79) = 0;
    strcpy((char *)a1 + 320, "invalid filename");
    send_err((__int64)a1);
    goto LABEL_61;
  }
  memset(a1 + 16, 0, 0x100uLL);
  strncpy((char *)a1 + 16, src, (unsigned int)n);
  if ( a1[16] == 47 )
  {
    *((_DWORD *)a1 + 79) = 2;
    strcpy((char *)a1 + 320, "absolute path forbidden");
    send_err((__int64)a1);
    goto LABEL_61;
  }
  if ( strnstr() )
  {
    *((_DWORD *)a1 + 79) = 2;
    snprintf((char *)a1 + 320, 0x200uLL, "file (%s) access violation", (const char *)a1 + 16);
    send_err((__int64)a1);
    goto LABEL_61;
  }
  s1 = (char *)a3[8];
  if ( strncasecmp(s1, "netascii", 8uLL) && strncasecmp(s1, "octet", 5uLL) )
  {
    if ( !strncasecmp(s1, "mail", 4uLL) )
    {
      *((_DWORD *)a1 + 79) = 0;
      strcpy((char *)a1 + 320, "mail mode is not supported");
    }
    else
    {
      *((_DWORD *)a1 + 79) = 0;
      strcpy((char *)a1 + 320, "unknown mode");
    }
    send_err((__int64)a1);
    goto LABEL_61;
  }
  v17 = a3[10];
  v18 = a3[11];
  memset(&oack_opts, 0, 0x210uLL);
  opt_cnt = 0;
  if ( a2 > 3 )
  {
    for ( i = 0; (unsigned int)v17 > i; ++i )
    {
      if ( *(_DWORD *)(v18 + 48LL * i) == 5
        && *(_DWORD *)(v18 + 48LL * i + 8) <= 0x3Fu
        && *(_DWORD *)(v18 + 48LL * i + 24) == 2 )
      {
        if ( !strncasecmp(*(const char **)(v18 + 48LL * i + 16), "blksize", 7uLL) )
        {
          *((_WORD *)a1 + 140) = *(_QWORD *)(v18 + 48LL * i + 32);
          if ( *((_WORD *)a1 + 140) <= 0x1FFu )
            *((_WORD *)a1 + 140) = 512;
          if ( *((_WORD *)a1 + 140) > 0xFFB8u )
            *((_WORD *)a1 + 140) = -72;
          add_oack_opt("blksize", *((_WORD *)a1 + 140));
        }
        else if ( !strncasecmp(*(const char **)(v18 + 48LL * i + 16), "tsize", 5uLL) )
        {
          *((_DWORD *)a1 + 71) = *(_QWORD *)(v18 + 48LL * i + 32);
          add_oack_opt("tsize", *((_DWORD *)a1 + 71));
        }
        else if ( !strncasecmp(*(const char **)(v18 + 48LL * i + 16), "timeout", 7uLL) )
        {
          a1[288] = *(_QWORD *)(v18 + 48LL * i + 32);
          if ( !a1[288] )
            a1[288] = 1;
          add_oack_opt("timeout", a1[288]);
        }
        else
        {
          *(_QWORD *)dest = 0LL;
          v21 = 0LL;
          v22 = 0LL;
          v23 = 0LL;
          v24 = 0LL;
          v25 = 0LL;
          v26 = 0LL;
          v27 = 0LL;
          strncpy(dest, *(const char **)(v18 + 48LL * i + 16), *(unsigned int *)(v18 + 48LL * i + 8));
          add_oack_opt(dest, *(_QWORD *)(v18 + 48LL * i + 32));
        }
      }
    }
  }
  if ( *((_QWORD *)a1 + 34) )
    fclose(*((FILE **)a1 + 34));
  if ( *a1 == 1 )
  {
    if ( !strncasecmp(s1, "octet", 5uLL) )
      v3 = "rb";
    else
      v3 = "rt";
    *((_QWORD *)a1 + 34) = fopen((const char *)a1 + 16, v3);
    if ( !*((_QWORD *)a1 + 34) || *__errno_location() )
    {
LABEL_42:
      *((_DWORD *)a1 + 79) = 1;
      strcpy((char *)a1 + 320, "invalid path or no access");
      send_err((__int64)a1);
      v4 = __errno_location();
      v5 = strerror(*v4);
      fprintf(stderr, "fopen failed. [%s]\n", v5);
      goto LABEL_61;
    }
  }
  else
  {
    fwrite("disable uploading\n", 1uLL, 0x12uLL, stderr);
    strcpy((char *)a1 + 16, "/dev/null");
    if ( !strncasecmp(s1, "octet", 5uLL) )
      v6 = "wb";
    else
      v6 = "wt";
    *((_QWORD *)a1 + 34) = fopen((const char *)a1 + 16, v6);
    if ( !*((_QWORD *)a1 + 34) || *__errno_location() )
      goto LABEL_42;
  }
  setvbuf(*((FILE **)a1 + 34), 0LL, 0, *((unsigned __int16 *)a1 + 140));
  if ( opt_cnt )
  {
    if ( *a1 == 1 )
    {
      if ( (unsigned int)stat((char *)a1 + 16, &v19) == -1 )
      {
        *((_DWORD *)a1 + 79) = 2;
        strcpy((char *)a1 + 320, "invalid path or no access");
        send_err((__int64)a1);
        v8 = __errno_location();
        v9 = strerror(*v8);
        fprintf(stderr, "fseek failed. [%s]\n", v9);
        goto LABEL_61;
      }
      *((_DWORD *)a1 + 71) = v19.st_size;
      add_oack_opt("tsize", *((_DWORD *)a1 + 71));
    }
    send_oack((__int64)a1);
    goto LABEL_60;
  }
  v7 = *a1;
  if ( v7 == 1 )
  {
    v12 = process_send((__int64)a1);
    goto LABEL_61;
  }
  if ( v7 != 2 || (unsigned int)send_ack((__int64)a1) != -1 )
LABEL_60:
    v12 = 1;
LABEL_61:
  if ( v12 != 1 )
    clean_req((__int64)a1);
  return v12;
}
