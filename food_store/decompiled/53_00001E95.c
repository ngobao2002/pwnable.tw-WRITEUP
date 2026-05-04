__int64 sub_1E95()
{
  __int64 result; // rax
  __int64 i; // [rsp+0h] [rbp-10h]
  __int64 j; // [rsp+0h] [rbp-10h]
  char *v3; // [rsp+8h] [rbp-8h]
  char *v4; // [rsp+8h] [rbp-8h]

  qword_2050C8 = (__int64)realloc(0LL, 0x50uLL);
  if ( !qword_2050C8 )
    sub_11C0((__int64)"Allocate failed !", 1);
  sub_1276("Egg", 3, 40);
  sub_1276("Pineapple", 0, 217);
  sub_1276("Flouir", 2, 127);
  sub_1276("Suger", 3, 40);
  sub_1276("Beef", 2, 400);
  v3 = (char *)realloc(0LL, 0x88uLL);
  if ( !v3 )
    sub_11C0((__int64)"Allocate Error !", 1);
  strcpy(v3, "Pineapple cake");
  sub_1721(v3, *(_QWORD *)qword_2050C8);
  sub_1721(v3, *(_QWORD *)(qword_2050C8 + 8));
  sub_1721(v3, *(_QWORD *)(qword_2050C8 + 16));
  sub_1721(v3, *(_QWORD *)(qword_2050C8 + 16));
  sub_1721(v3, *(_QWORD *)(qword_2050C8 + 24));
  for ( i = qword_2050D0; i && *(_QWORD *)(i + 128); i = *(_QWORD *)(i + 128) )
    ;
  if ( qword_2050D0 )
    *(_QWORD *)(i + 128) = v3;
  else
    qword_2050D0 = (__int64)v3;
  v4 = (char *)realloc(0LL, 0x88uLL);
  if ( !v4 )
    sub_11C0((__int64)"Allocate Error !", 1);
  strcpy(v4, "Beef noodles");
  sub_1721(v4, *(_QWORD *)(qword_2050C8 + 16));
  sub_1721(v4, *(_QWORD *)(qword_2050C8 + 32));
  for ( j = qword_2050D0; j && *(_QWORD *)(j + 128); j = *(_QWORD *)(j + 128) )
    ;
  result = j;
  *(_QWORD *)(j + 128) = v4;
  return result;
}
