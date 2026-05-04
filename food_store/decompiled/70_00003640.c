void __fastcall init(unsigned int a1, __int64 a2, __int64 a3)
{
  signed __int64 v4; // rbp
  __int64 i; // rbx

  v4 = &off_204CF8 - off_204CF0;
  init_proc();
  if ( v4 )
  {
    for ( i = 0LL; i != v4; ++i )
      ((void (__fastcall *)(_QWORD, __int64, __int64))off_204CF0[i])(a1, a2, a3);
  }
}
