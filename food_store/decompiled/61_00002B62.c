int sub_2B62()
{
  unsigned int v1; // [rsp+Ch] [rbp-34h]
  unsigned int i; // [rsp+10h] [rbp-30h]
  unsigned int v3; // [rsp+14h] [rbp-2Ch]
  char *s2; // [rsp+18h] [rbp-28h]
  char *s2a; // [rsp+18h] [rbp-28h]
  char *s1; // [rsp+20h] [rbp-20h]
  __int64 v7; // [rsp+30h] [rbp-10h]
  __int64 v8; // [rsp+38h] [rbp-8h]

  v1 = 0;
  if ( !qword_2050D0 )
    return puts("NPC: I am very hungry !");
  for ( s2 = (char *)qword_2050D0; s2; s2 = (char *)*((_QWORD *)s2 + 16) )
    ++v1;
  v3 = rand() % v1;
  s2a = (char *)qword_2050D0;
  for ( i = 0; i < v3; ++i )
    s2a = (char *)*((_QWORD *)s2a + 16);
  _printf_chk();
  _printf_chk();
  if ( sub_1134(1LL, "Your choice (1/Yes,0/No) :") != 1 || !qword_205060 )
    return puts("NPC: So sad :(");
  s1 = (char *)qword_205060;
  while ( strcmp(s1, s2a) )
  {
    s1 = (char *)*((_QWORD *)s1 + 5);
    ++i;
    if ( s1 == (char *)qword_205060 )
      return sub_11C0((__int64)"Can not find the food !", 0);
  }
  if ( (unsigned __int64)(qword_2050B0 + 80) <= 0x63 )
  {
    qword_2050B0 += 80LL;
  }
  else
  {
    ++dword_2050A8;
    qword_2050B0 -= 20LL;
  }
  qword_2050C0 += 434LL;
  v7 = *((_QWORD *)s1 + 6);
  v8 = *((_QWORD *)s1 + 5);
  *(_QWORD *)(v7 + 40) = v8;
  *(_QWORD *)(v8 + 48) = v7;
  if ( s1 == (char *)qword_205060 )
  {
    if ( qword_205060 == v8 )
      qword_205060 = 0LL;
    else
      qword_205060 = v8;
  }
  realloc(s1, 0LL);
  return puts("NPC: Thank you !");
}
