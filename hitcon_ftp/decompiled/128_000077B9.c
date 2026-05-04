__int64 __fastcall template_callback_true(__int64 a1, __int64 a2)
{
  *(_DWORD *)a2 = 1;
  *(_BYTE *)(a2 + 8) = 1;
  return 0LL;
}
