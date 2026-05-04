int sub_2A12()
{
  int result; // eax
  int v1; // [rsp+0h] [rbp-20h]
  int v2; // [rsp+4h] [rbp-1Ch]
  _QWORD *ptr; // [rsp+8h] [rbp-18h]
  __int64 v4; // [rsp+10h] [rbp-10h]
  __int64 v5; // [rsp+18h] [rbp-8h]

  v1 = 0;
  sub_25E1();
  result = qword_205060;
  if ( qword_205060 )
  {
    _printf_chk();
    v2 = sub_1134(1LL, "What do you want to eat ? :");
    ptr = (_QWORD *)qword_205060;
    while ( v1 != v2 )
    {
      ptr = (_QWORD *)ptr[5];
      ++v1;
      if ( ptr == (_QWORD *)qword_205060 )
        return sub_11C0((__int64)"Can not find the food !", 0);
    }
    qword_2050B8 += ptr[4];
    v4 = ptr[6];
    v5 = ptr[5];
    *(_QWORD *)(v4 + 40) = v5;
    *(_QWORD *)(v5 + 48) = v4;
    realloc(ptr, 0LL);
    if ( ptr == (_QWORD *)qword_205060 )
    {
      if ( qword_205060 == v5 )
        qword_205060 = 0LL;
      else
        qword_205060 = v5;
    }
    return puts("Success !");
  }
  return result;
}
