__int64 __fastcall sub_32BA(__int64 a1, __int64 a2, unsigned __int64 a3)
{
  int v4; // eax
  unsigned __int64 i; // [rsp+28h] [rbp-18h]
  __int64 v6; // [rsp+38h] [rbp-8h]

  if ( !a3 || a3 > 0x1000 )
    return 0xFFFFFFFFLL;
  for ( i = 0LL; i < a3; ++i )
  {
    v6 = 8 * (a3 - i - 1) + a2;
    if ( *(_WORD *)v6 == 5 )
    {
      v4 = (*(unsigned __int8 *)(v6 + 2) << 8) | *(unsigned __int8 *)(v6 + 3);
      if ( v4 == 65278 )
      {
        if ( *(_DWORD *)(16LL * *(unsigned int *)(v6 + 4) + a1 + 16) != -1 )
        {
          fprintf(stderr, "Duplicate label use: '%s'\n", *(const char **)(16LL * *(unsigned int *)(v6 + 4) + a1 + 8));
          return 1LL;
        }
        *(_DWORD *)(16LL * *(unsigned int *)(v6 + 4) + a1 + 16) = a3 - i - 1;
        *(_DWORD *)(v6 + 4) = 0;
        *(_BYTE *)(v6 + 2) = 0;
        *(_BYTE *)(v6 + 3) = 0;
      }
      else if ( v4 == 0xFFFF )
      {
        if ( *(_DWORD *)(16LL * *(unsigned int *)(v6 + 4) + a1 + 16) == -1 )
        {
          fprintf(stderr, "Unresolved label: '%s'\n", *(const char **)(16LL * *(unsigned int *)(v6 + 4) + a1 + 8));
          return 1LL;
        }
        *(_DWORD *)(v6 + 4) = *(_DWORD *)(16LL * *(unsigned int *)(v6 + 4) + a1 + 16) - (a3 - i);
        *(_BYTE *)(v6 + 2) = 0;
        *(_BYTE *)(v6 + 3) = 0;
      }
    }
  }
  return 0LL;
}
