__int64 __fastcall template_callback_double(double a1, __int64 a2, __int64 a3)
{
  *(_DWORD *)a3 = 4;
  *(double *)(a3 + 8) = a1;
  return 0LL;
}
