int sub_15C0()
{
  int result; // eax
  int i; // [rsp+Ch] [rbp-4h]

  result = qword_2050C8;
  if ( qword_2050C8 )
  {
    puts(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    for ( i = 0; i <= 9; ++i )
    {
      if ( *(_QWORD *)(8LL * i + qword_2050C8) )
        _printf_chk();
    }
    return puts(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  }
  return result;
}
