unsigned __int64 sub_17DC()
{
  int v1; // [rsp+4h] [rbp-7Ch]
  int i; // [rsp+8h] [rbp-78h]
  int j; // [rsp+Ch] [rbp-74h]
  const char *v4; // [rsp+10h] [rbp-70h]
  __int64 k; // [rsp+18h] [rbp-68h]
  unsigned __int64 v6; // [rsp+20h] [rbp-60h]
  __int64 v7; // [rsp+28h] [rbp-58h]
  char s[32]; // [rsp+30h] [rbp-50h] BYREF
  char v9[40]; // [rsp+50h] [rbp-30h] BYREF
  unsigned __int64 v10; // [rsp+78h] [rbp-8h]

  v10 = __readfsqword(0x28u);
  k = 0LL;
  v6 = 0LL;
  v7 = 0LL;
  v1 = 0;
  memset(s, 0, sizeof(s));
  memset(v9, 0, 0x20uLL);
  v4 = (const char *)realloc(0LL, 0x88uLL);
  if ( !v4 )
    sub_11C0((__int64)"Allocate Error !", 1);
  for ( i = 0; i <= 7; ++i )
  {
    s[i] = *((_BYTE *)&v4 + i);
    sprintf(&v9[2 * i], "%02X", (unsigned __int8)(s[i] ^ byte_2050D8[i]));
  }
  for ( j = 0; j <= 12; ++j )
    *(_QWORD *)&v4[8 * j + 24] = 0LL;
  _printf_chk();
  _printf_chk();
  _isoc99_scanf();
  getchar();
  sub_15C0();
  do
  {
    _printf_chk();
    v6 = sub_1134(1LL, "Choose ingredient :");
    if ( (unsigned int)dword_20506C <= v6 )
    {
      sub_11C0((__int64)"Out of bound\n", 1);
    }
    else if ( *(_QWORD *)(8 * v6 + qword_2050C8) )
    {
      v1 = sub_1721(v4, *(_QWORD *)(8 * v6 + qword_2050C8));
    }
    if ( !v1 )
      break;
    _printf_chk();
    v7 = sub_1134(1LL, "Add more ingredient ? (1/Yes,2/No) : ");
  }
  while ( v7 == 1 );
  for ( k = qword_2050D0; k && *(_QWORD *)(k + 128); k = *(_QWORD *)(k + 128) )
    ;
  if ( qword_2050D0 )
    *(_QWORD *)(k + 128) = v4;
  else
    qword_2050D0 = (__int64)v4;
  return __readfsqword(0x28u) ^ v10;
}
