__int64 __fastcall msgpack_unpacker_reset_zone(__int64 a1)
{
  return msgpack_zone_clear(*(_QWORD *)(a1 + 40));
}
