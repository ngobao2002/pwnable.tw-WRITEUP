_BOOL8 __fastcall msgpack_zone_is_empty(__int64 a1)
{
  return *(_QWORD *)a1 == *(_QWORD *)(a1 + 48)
      && !**(_QWORD **)(a1 + 16)
      && *(_QWORD *)(a1 + 24) == *(_QWORD *)(a1 + 40);
}
