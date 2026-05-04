__int64 sub_2895()
{
  __int64 result; // rax
  int i; // [rsp+4h] [rbp-Ch]
  _DWORD *s; // [rsp+8h] [rbp-8h]

  if ( (unsigned __int64)qword_2050C0 <= 0x64 )
    return sub_11C0((__int64)"You have no enough money :(\n", 0);
  qword_2050C0 -= 100LL;
  result = qword_2050C8;
  if ( qword_2050C8 )
  {
    for ( i = 0; ; ++i )
    {
      if ( i > 9 )
        return sub_11C0((__int64)"Too many ingredient", 0);
      if ( !*(_QWORD *)(8LL * i + qword_2050C8) )
        break;
    }
    s = realloc(0LL, 0x28uLL);
    if ( !s )
      sub_11C0((__int64)"Allocate failed !", 1);
    puts("NPC : I think you can make your ingredient by yourself");
    _printf_chk();
    puts("NPC : Enjoy it :)");
    memset(s, 0, 0x20uLL);
    sub_11FF((__int64)s);
    s[8] = rand() % 500;
    s[9] = 1;
    result = (__int64)s;
    *(_QWORD *)(qword_2050C8 + 8LL * i) = s;
  }
  return result;
}
