__int64 __fastcall sub_1721(const char *a1, __int64 a2)
{
  int i; // [rsp+1Ch] [rbp-4h]

  memset(qword_205050, 0, 0x80uLL);
  for ( i = 0; i <= 12; ++i )
  {
    if ( !*(_QWORD *)&a1[8 * i + 24] )
    {
      *(_QWORD *)&a1[8 * i + 24] = a2;
      return 1LL;
    }
  }
  snprintf(qword_205050, 0x7FuLL, "Can not add more ingredient to %s", a1);
  sub_11C0((__int64)qword_205050, 0);
  return 0LL;
}
