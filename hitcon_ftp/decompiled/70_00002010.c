__int64 __fastcall msgpack_sbuffer_write(__int64 a1, const void *a2, size_t a3)
{
  __int64 v3; // rax
  void *v4; // rax
  unsigned __int64 size; // [rsp+20h] [rbp-20h]

  if ( a3 > *(_QWORD *)(a1 + 16) - *(_QWORD *)a1 )
  {
    if ( *(_QWORD *)(a1 + 16) )
      v3 = 2LL * *(_QWORD *)(a1 + 16);
    else
      v3 = 0x2000LL;
    for ( size = v3; size < *(_QWORD *)a1 + a3; size *= 2LL )
    {
      if ( 2 * size <= size )
      {
        size = *(_QWORD *)a1 + a3;
        v4 = realloc(*(void **)(a1 + 8), size);
        goto LABEL_11;
      }
    }
    v4 = realloc(*(void **)(a1 + 8), size);
LABEL_11:
    if ( !v4 )
      return 0xFFFFFFFFLL;
    *(_QWORD *)(a1 + 8) = v4;
    *(_QWORD *)(a1 + 16) = size;
  }
  memcpy((void *)(*(_QWORD *)(a1 + 8) + *(_QWORD *)a1), a2, a3);
  *(_QWORD *)a1 += a3;
  return 0LL;
}
