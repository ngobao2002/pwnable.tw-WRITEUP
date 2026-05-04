__int64 __fastcall msgpack_zone_push_finalizer_expand(__int64 a1, __int64 a2, __int64 a3)
{
  __int64 v5; // [rsp+20h] [rbp-20h]
  __int64 v6; // [rsp+28h] [rbp-18h]
  __int64 v7; // [rsp+30h] [rbp-10h]
  char *v8; // [rsp+38h] [rbp-8h]

  v6 = a1 + 24;
  v7 = (__int64)(*(_QWORD *)(a1 + 32) - *(_QWORD *)(a1 + 40)) >> 4;
  if ( v7 )
    v5 = 2 * v7;
  else
    v5 = 4LL;
  v8 = (char *)realloc(*(void **)(a1 + 40), 16 * v5);
  if ( !v8 )
    return 0LL;
  *(_QWORD *)(a1 + 40) = v8;
  *(_QWORD *)(a1 + 32) = &v8[16 * v5];
  *(_QWORD *)v6 = &v8[16 * v7];
  **(_QWORD **)v6 = a2;
  *(_QWORD *)(*(_QWORD *)v6 + 8LL) = a3;
  *(_QWORD *)v6 += 16LL;
  return 1LL;
}
