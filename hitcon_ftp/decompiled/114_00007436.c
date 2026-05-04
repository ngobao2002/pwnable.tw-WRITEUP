__int64 __fastcall msgpack_zone_push_finalizer(__int64 a1, __int64 a2, __int64 a3)
{
  _QWORD *v4; // [rsp+28h] [rbp-8h]

  v4 = *(_QWORD **)(a1 + 24);
  if ( v4 == *(_QWORD **)(a1 + 32) )
    return msgpack_zone_push_finalizer_expand(a1, a2, a3);
  *v4 = a2;
  v4[1] = a3;
  *(_QWORD *)(a1 + 24) += 16LL;
  return 1LL;
}
