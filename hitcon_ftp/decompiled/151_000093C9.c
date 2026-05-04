__int64 __fastcall msgpack_unpacker_release_zone(__int64 a1)
{
  __int64 v2; // [rsp+18h] [rbp-8h]

  v2 = *(_QWORD *)(a1 + 40);
  if ( !v2 )
    return 0LL;
  if ( (unsigned __int8)msgpack_unpacker_flush_zone(a1) != 1 )
    return 0LL;
  *(_QWORD *)(a1 + 40) = 0LL;
  **(_QWORD **)(a1 + 56) = a1 + 40;
  return v2;
}
