void __fastcall msgpack_unpacker_free(void *a1)
{
  msgpack_unpacker_destroy((__int64)a1);
  free(a1);
}
