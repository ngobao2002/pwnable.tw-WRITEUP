__int64 sub_2DCA()
{
  __int64 result; // rax
  int i; // [rsp+4h] [rbp-Ch]
  unsigned int v2; // [rsp+8h] [rbp-8h]
  unsigned int v3; // [rsp+Ch] [rbp-4h]

  v2 = 0;
  result = qword_205080;
  if ( qword_205080 )
  {
    v3 = time(0LL) - dword_205068;
    if ( v3 > 0xE )
      v2 = v3 / 0xF;
    puts("#######################################################");
    for ( i = 0; i <= 9; ++i )
    {
      if ( *(_QWORD *)(8LL * i + qword_205080) )
      {
        *(_DWORD *)(*(_QWORD *)(8LL * i + qword_205080) + 36LL) += v2;
        _printf_chk();
      }
    }
    puts("#######################################################");
    return _printf_chk();
  }
  return result;
}
