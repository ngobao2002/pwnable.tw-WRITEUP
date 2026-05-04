__int64 __fastcall sub_1276(const char *a1, int a2, int a3)
{
  __int64 result; // rax
  int i; // [rsp+14h] [rbp-Ch]
  int j; // [rsp+14h] [rbp-Ch]
  char *dest; // [rsp+18h] [rbp-8h]

  result = qword_2050C8;
  if ( qword_2050C8 )
  {
    for ( i = 0; i <= 9; ++i )
    {
      if ( *(_QWORD *)(8LL * i + qword_2050C8) && !strcmp(*(const char **)(8LL * i + qword_2050C8), a1) )
      {
        result = *(_QWORD *)(8LL * i + qword_2050C8);
        *(_DWORD *)(result + 36) += a2;
        return result;
      }
    }
    for ( j = 0; ; ++j )
    {
      if ( j > 9 )
        return sub_11C0((__int64)"Too many ingredient", 0);
      if ( !*(_QWORD *)(8LL * j + qword_2050C8) )
        break;
    }
    dest = (char *)realloc(0LL, 0x28uLL);
    if ( !dest )
      sub_11C0((__int64)"Allocate failed !", 1);
    strncpy(dest, a1, 0x1FuLL);
    *((_DWORD *)dest + 8) = a3;
    *((_DWORD *)dest + 9) = a2;
    *(_QWORD *)(qword_2050C8 + 8LL * j) = dest;
    return (unsigned int)++dword_20506C;
  }
  return result;
}
