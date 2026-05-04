__int64 __fastcall template_callback_array_item(int a1, __int64 a2, int a3, int a4, int a5, int a6, char src)
{
  memcpy((void *)(*(_QWORD *)(a2 + 16) + 24LL * *(unsigned int *)(a2 + 8)), &src, 0x18uLL);
  ++*(_DWORD *)(a2 + 8);
  return 0LL;
}
