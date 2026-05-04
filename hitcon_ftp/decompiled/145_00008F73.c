void __fastcall msgpack_unpacker_destroy(__int64 a1)
{
  msgpack_zone_free(*(_QWORD *)(a1 + 40));
  free(*(void **)(a1 + 56));
  decr_count(*(void **)a1);
}
