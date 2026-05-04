__int64 __fastcall template_callback_int32(__int64 a1, int a2, __int64 a3)
{
  if ( a2 < 0 )
    *(_DWORD *)a3 = 3;
  else
    *(_DWORD *)a3 = 2;
  *(_QWORD *)(a3 + 8) = a2;
  return 0LL;
}
