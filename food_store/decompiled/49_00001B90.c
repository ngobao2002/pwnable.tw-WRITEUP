unsigned __int64 sub_1B90()
{
  char *s1; // [rsp+10h] [rbp-30h]
  char *v2; // [rsp+18h] [rbp-28h]
  char s[24]; // [rsp+20h] [rbp-20h] BYREF
  unsigned __int64 v4; // [rsp+38h] [rbp-8h]

  v4 = __readfsqword(0x28u);
  v2 = 0LL;
  memset(s, 0, sizeof(s));
  _printf_chk();
  sub_11FF((__int64)s);
  if ( qword_2050D0 )
  {
    for ( s1 = (char *)qword_2050D0; s1; s1 = (char *)*((_QWORD *)s1 + 16) )
    {
      if ( !strcmp(s1, s) )
      {
        if ( v2 )
          *((_QWORD *)v2 + 16) = *((_QWORD *)s1 + 16);
        else
          qword_2050D0 = *((_QWORD *)s1 + 16);
        realloc(s1, 0LL);
        return __readfsqword(0x28u) ^ v4;
      }
      v2 = s1;
    }
  }
  return __readfsqword(0x28u) ^ v4;
}
