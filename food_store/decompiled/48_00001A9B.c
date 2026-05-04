int __fastcall sub_1A9B(int a1)
{
  int result; // eax
  int j; // [rsp+14h] [rbp-Ch]
  __int64 i; // [rsp+18h] [rbp-8h]

  result = qword_2050D0;
  if ( qword_2050D0 )
  {
    for ( i = qword_2050D0; i; i = *(_QWORD *)(i + 128) )
    {
      puts("-------------------------------------------");
      _printf_chk();
      if ( a1 )
      {
        puts("Ingredient :");
        for ( j = 0; j <= 12; ++j )
        {
          if ( *(_QWORD *)(i + 8 * (j + 2LL) + 8) )
            _printf_chk();
        }
      }
    }
    return puts("-------------------------------------------");
  }
  return result;
}
