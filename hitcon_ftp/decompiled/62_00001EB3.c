__int64 __fastcall msgpack_unpacker_reserve_buffer(__int64 a1, unsigned __int64 a2)
{
  if ( a2 > *(_QWORD *)(a1 + 16) )
    return msgpack_unpacker_expand_buffer(a1, a2);
  else
    return 1LL;
}
