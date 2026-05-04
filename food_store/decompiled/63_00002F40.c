int __fastcall sub_2F40(__int64 a1, __int64 a2)
{
  unsigned int v3; // [rsp+4h] [rbp-Ch]
  unsigned int v4; // [rsp+8h] [rbp-8h]
  unsigned int v5; // [rsp+Ch] [rbp-4h]

  sub_2DCA();
  v3 = sub_1134(a1, a2);
  if ( v3 > 9 )
    return sub_11C0((__int64)"Out of bound\n", 11);
  if ( !*(_QWORD *)(8LL * v3 + qword_205080) )
    return sub_11C0((__int64)"Not found !", 0);
  _printf_chk();
  v4 = sub_1134(1LL, "Quantity :");
  if ( *(_DWORD *)(*(_QWORD *)(8LL * v3 + qword_205080) + 36LL) < v4 )
    return puts("You can not buy so much !");
  v5 = v4 * *(_DWORD *)(*(_QWORD *)(8LL * v3 + qword_205080) + 32LL);
  if ( qword_2050C0 < (unsigned __int64)v5 )
    return puts("You do not have enough money :(");
  qword_2050C0 -= v5;
  sub_1276(*(const char **)(8LL * v3 + qword_205080), v4, *(_DWORD *)(*(_QWORD *)(8LL * v3 + qword_205080) + 32LL));
  *(_DWORD *)(*(_QWORD *)(8LL * v3 + qword_205080) + 36LL) -= v4;
  return puts("Success !");
}
