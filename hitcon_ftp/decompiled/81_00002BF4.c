__int64 __fastcall process_recv(__int64 a1, unsigned int a2, const void *a3)
{
  int *v4; // rax
  char *v5; // rax

  *__errno_location() = 0;
  *(_QWORD *)(a1 + 8) = *(unsigned __int8 *)(a1 + 288) + time(0LL);
  ++*(_WORD *)(a1 + 290);
  if ( (unsigned int)send_ack(a1) == -1 )
  {
    clean_req(a1);
    return 0LL;
  }
  else if ( fwrite(a3, a2, 1uLL, *(FILE **)(a1 + 272)) == 1 && !*__errno_location() )
  {
    if ( a2 < *(unsigned __int16 *)(a1 + 280) )
    {
      printf("file %s, %u blocks received\n", (const char *)(a1 + 16), *(unsigned __int16 *)(a1 + 290));
      clean_req(a1);
    }
    return 1LL;
  }
  else
  {
    *(_DWORD *)(a1 + 316) = 3;
    strcpy((char *)(a1 + 320), "disk full or allocation exceeded");
    send_err(a1);
    v4 = __errno_location();
    v5 = strerror(*v4);
    fprintf(stderr, "fwrite error. [%s]\n", v5);
    clean_req(a1);
    return 0LL;
  }
}
