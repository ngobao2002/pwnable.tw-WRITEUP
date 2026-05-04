__int64 __fastcall find_request(__int64 a1)
{
  char *v1; // rax
  int i; // [rsp+10h] [rbp-10h]
  int v4; // [rsp+14h] [rbp-Ch]
  const char *s1; // [rsp+18h] [rbp-8h]

  s1 = inet_ntoa(*(struct in_addr *)((char *)&a1 + 4));
  v4 = ntohs(WORD1(a1));
  for ( i = 0; i <= 31; ++i )
  {
    if ( init_reqs[i] )
    {
      v1 = inet_ntoa(*(struct in_addr *)(init_reqs[i] + 300LL));
      if ( !strcmp(s1, v1) && v4 == ntohs(*(_WORD *)(init_reqs[i] + 298LL)) )
        return init_reqs[i];
    }
  }
  return 0LL;
}
