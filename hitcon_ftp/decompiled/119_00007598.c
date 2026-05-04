__int64 __fastcall template_callback_uint32(__int64 a1, unsigned int a2, __int64 a3)
{
  *(_DWORD *)a3 = 2;
  *(_QWORD *)(a3 + 8) = a2;
  return 0LL;
}
