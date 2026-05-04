__int64 __fastcall msgpack_unpacker_expand_buffer(__int64 a1, unsigned __int64 a2)
{
  void *v3; // rax
  char *v4; // rax
  unsigned __int64 size; // [rsp+18h] [rbp-38h]
  unsigned __int64 v6; // [rsp+20h] [rbp-30h]
  size_t n; // [rsp+28h] [rbp-28h]
  char *ptr; // [rsp+38h] [rbp-18h]

  if ( *(_QWORD *)(a1 + 8) == *(_QWORD *)(a1 + 24)
    && (unsigned int)get_count(*(unsigned int **)a1) == 1
    && *(_BYTE *)(*(_QWORD *)(a1 + 56) + 8LL) != 1 )
  {
    *(_QWORD *)(a1 + 16) = *(_QWORD *)(a1 + 16) + *(_QWORD *)(a1 + 8) - 4LL;
    *(_QWORD *)(a1 + 8) = 4LL;
    *(_QWORD *)(a1 + 24) = 4LL;
    if ( a2 <= *(_QWORD *)(a1 + 16) )
      return 1LL;
  }
  if ( *(_QWORD *)(a1 + 24) == 4LL )
  {
    for ( size = 2LL * (*(_QWORD *)(a1 + 8) + *(_QWORD *)(a1 + 16)); size < *(_QWORD *)(a1 + 8) + a2; size *= 2LL )
    {
      if ( 2 * size <= size )
      {
        size = *(_QWORD *)(a1 + 8) + a2;
        v3 = realloc(*(void **)a1, size);
        goto LABEL_13;
      }
    }
    v3 = realloc(*(void **)a1, size);
LABEL_13:
    if ( !v3 )
      return 0LL;
    *(_QWORD *)a1 = v3;
    *(_QWORD *)(a1 + 16) = size - *(_QWORD *)(a1 + 8);
  }
  else
  {
    v6 = *(_QWORD *)(a1 + 48);
    n = *(_QWORD *)(a1 + 8) - *(_QWORD *)(a1 + 24);
    while ( v6 < a2 + n + 4 )
    {
      if ( 2 * v6 <= v6 )
      {
        v6 = a2 + n + 4;
        v4 = (char *)malloc(v6);
        goto LABEL_22;
      }
      v6 *= 2LL;
    }
    v4 = (char *)malloc(v6);
LABEL_22:
    ptr = v4;
    if ( !v4 )
      return 0LL;
    init_count(v4);
    memcpy(ptr + 4, (const void *)(*(_QWORD *)a1 + *(_QWORD *)(a1 + 24)), n);
    if ( *(_BYTE *)(*(_QWORD *)(a1 + 56) + 8LL) )
    {
      if ( (unsigned __int8)msgpack_zone_push_finalizer(*(_QWORD *)(a1 + 40), (__int64)decr_count, *(_QWORD *)a1) != 1 )
      {
        free(ptr);
        return 0LL;
      }
      *(_BYTE *)(*(_QWORD *)(a1 + 56) + 8LL) = 0;
    }
    else
    {
      decr_count(*(void **)a1);
    }
    *(_QWORD *)a1 = ptr;
    *(_QWORD *)(a1 + 8) = n + 4;
    *(_QWORD *)(a1 + 16) = v6 - *(_QWORD *)(a1 + 8);
    *(_QWORD *)(a1 + 24) = 4LL;
  }
  return 1LL;
}
