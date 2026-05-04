int sub_25E1()
{
  int result; // eax
  int v1; // [rsp+4h] [rbp-Ch]
  __int64 v2; // [rsp+8h] [rbp-8h]

  v1 = 0;
  result = qword_205060;
  if ( qword_205060 )
  {
    v2 = qword_205060;
    do
    {
      puts("*****************************************");
      _printf_chk();
      _printf_chk();
      _printf_chk();
      _printf_chk();
      v2 = *(_QWORD *)(v2 + 40);
      ++v1;
    }
    while ( v2 != qword_205060 );
    return puts("*****************************************");
  }
  return result;
}
