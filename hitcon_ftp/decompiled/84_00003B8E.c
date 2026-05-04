bool __fastcall check_crc32(
        const char *a1,
        __int64 a2,
        __int64 a3,
        int a4,
        int a5,
        int a6,
        __int64 a7,
        __int64 a8,
        __int64 a9)
{
  unsigned int v10; // [rsp+20h] [rbp-18h]
  int i; // [rsp+24h] [rbp-14h]

  msgpack_object_print_buffer((_DWORD)a1, a8 + 1, a8 + 1, a4, a5, a6, a7, a8, a9);
  v10 = -1;
  for ( i = 1; i < strlen(a1); ++i )
    v10 = (v10 >> 8) ^ crc32_tab[(unsigned __int8)(v10 ^ a1[i])];
  return a2 == ~v10;
}
