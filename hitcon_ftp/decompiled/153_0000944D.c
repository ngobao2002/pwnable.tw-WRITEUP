__int64 __fastcall msgpack_unpacker_flush_zone(__int64 *a1)
{
  if ( *(_BYTE *)(a1[7] + 8) )
  {
    if ( (unsigned __int8)msgpack_zone_push_finalizer(a1[5], (__int64)decr_count, *a1) != 1 )
      return 0LL;
    *(_BYTE *)(a1[7] + 8) = 0;
    incr_count((volatile signed __int32 *)*a1);
  }
  return 1LL;
}
