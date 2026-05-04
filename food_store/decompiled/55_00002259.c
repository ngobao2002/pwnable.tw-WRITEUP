unsigned __int64 sub_2259()
{
  char *v0; // rax
  __int64 v1; // rax
  int v3; // [rsp+8h] [rbp-48h]
  int i; // [rsp+Ch] [rbp-44h]
  char *s1; // [rsp+10h] [rbp-40h]
  __int64 j; // [rsp+18h] [rbp-38h]
  char *dest; // [rsp+28h] [rbp-28h]
  char s2[24]; // [rsp+30h] [rbp-20h] BYREF
  unsigned __int64 v9; // [rsp+48h] [rbp-8h]

  v9 = __readfsqword(0x28u);
  v3 = 0;
  sub_1A9B(0);
  _printf_chk();
  sub_11FF((__int64)s2);
  if ( qword_2050D0 )
  {
    for ( s1 = (char *)qword_2050D0; s1; s1 = (char *)*((_QWORD *)s1 + 16) )
    {
      if ( !strcmp(s1, s2) )
      {
        if ( (unsigned __int64)qword_2050B8 > 9 )
        {
          qword_2050B8 -= 10LL;
        }
        else
        {
          v0 = qword_205050;
          *(_QWORD *)qword_205050 = 0x6576616820756F59LL;
          strcpy(v0 + 8, " no power !");
          sub_11C0((__int64)qword_205050, 1);
        }
        for ( i = 0; i <= 12; ++i )
        {
          if ( *(_QWORD *)&s1[8 * i + 24] )
          {
            if ( *(_DWORD *)(*(_QWORD *)&s1[8 * i + 24] + 36LL) )
            {
              v1 = *(_QWORD *)&s1[8 * i + 24];
              --*(_DWORD *)(v1 + 36);
              v3 += *(_DWORD *)(*(_QWORD *)&s1[8 * i + 24] + 32LL);
            }
            else
            {
              snprintf(qword_205050, 0x7FuLL, "Run out of %s\n", *(const char **)&s1[8 * i + 24]);
              sub_11C0((__int64)qword_205050, 1);
            }
          }
        }
        if ( (unsigned __int64)(qword_2050B0 + 60) <= 0x63 )
        {
          qword_2050B0 += 60LL;
        }
        else
        {
          ++dword_2050A8;
          qword_2050B0 -= 40LL;
        }
        dest = (char *)realloc(0LL, 0x38uLL);
        *((_DWORD *)dest + 6) = (int)((double)v3 * 1.217);
        *((_QWORD *)dest + 4) = v3 / 0x1Cu;
        strncpy(dest, s1, 0x18uLL);
        if ( qword_205060 )
        {
          for ( j = qword_205060; *(_QWORD *)(j + 40) != qword_205060; j = *(_QWORD *)(j + 40) )
            ;
          *(_QWORD *)(j + 40) = dest;
          *((_QWORD *)dest + 5) = qword_205060;
          *((_QWORD *)dest + 6) = j;
          *(_QWORD *)(qword_205060 + 48) = dest;
        }
        else
        {
          qword_205060 = (__int64)dest;
          *((_QWORD *)dest + 5) = dest;
          *((_QWORD *)dest + 6) = dest;
        }
        puts("Done !");
        return __readfsqword(0x28u) ^ v9;
      }
    }
  }
  return __readfsqword(0x28u) ^ v9;
}
