__int64 __fastcall msgpack_unpacker_init(__int64 a1, unsigned __int64 a2)
{
  size_t size; // [rsp+0h] [rbp-20h]
  void *ptr; // [rsp+10h] [rbp-10h]
  void *v5; // [rsp+18h] [rbp-8h]

  size = a2;
  if ( a2 <= 3 )
    size = 4LL;
  ptr = malloc(size);
  if ( !ptr )
    return 0LL;
  v5 = malloc(0x820uLL);
  if ( v5 )
  {
    *(_QWORD *)a1 = ptr;
    *(_QWORD *)(a1 + 8) = 4LL;
    *(_QWORD *)(a1 + 16) = size - *(_QWORD *)(a1 + 8);
    *(_QWORD *)(a1 + 24) = 4LL;
    *(_QWORD *)(a1 + 32) = 0LL;
    *(_QWORD *)(a1 + 48) = size;
    *(_QWORD *)(a1 + 40) = 0LL;
    *(_QWORD *)(a1 + 56) = v5;
    init_count(*(_DWORD **)a1);
    template_init(*(_QWORD *)(a1 + 56));
    **(_QWORD **)(a1 + 56) = a1 + 40;
    *(_BYTE *)(*(_QWORD *)(a1 + 56) + 8LL) = 0;
    return 1LL;
  }
  else
  {
    free(ptr);
    return 0LL;
  }
}
