__int64 __fastcall process_send(__int64 a1)
{
  int *v1; // rax
  char *v2; // rax
  unsigned __int8 v4; // [rsp+13h] [rbp-Dh]
  int v5; // [rsp+14h] [rbp-Ch]
  void *ptr; // [rsp+18h] [rbp-8h]

  v4 = 0;
  *__errno_location() = 0;
  *(_QWORD *)(a1 + 8) = *(unsigned __int8 *)(a1 + 288) + time(0LL);
  ++*(_WORD *)(a1 + 290);
  ptr = calloc(1uLL, *(unsigned __int16 *)(a1 + 280));
  if ( !ptr )
  {
    fwrite("out of memory\n", 1uLL, 0xEuLL, stderr);
    exit(-1);
  }
  if ( *(_QWORD *)(a1 + 272) )
  {
    v5 = fread(ptr, 1uLL, *(unsigned __int16 *)(a1 + 280), *(FILE **)(a1 + 272));
    if ( *__errno_location() )
    {
      *(_DWORD *)(a1 + 316) = 4;
      strcpy((char *)(a1 + 320), "illegal packet");
      send_err(a1);
      v1 = __errno_location();
      v2 = strerror(*v1);
      fprintf(stderr, "fread error. [%s]\n", v2);
      clean_req(a1);
    }
    else if ( (unsigned int)send_data(a1, (__int64)ptr, v5) == -1 )
    {
      clean_req(a1);
    }
    else
    {
      if ( v5 < *(unsigned __int16 *)(a1 + 280) )
      {
        clean_req(a1);
        printf("transfer %s finish\n", (const char *)(a1 + 16));
      }
      v4 = 1;
    }
  }
  else
  {
    puts("transfer is not ready");
    clean_req(a1);
  }
  free(ptr);
  return v4;
}
