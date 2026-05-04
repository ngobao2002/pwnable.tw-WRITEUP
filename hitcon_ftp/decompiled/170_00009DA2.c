void __fastcall msgpack_zone_destroy(__int64 a1)
{
  destroy_finalizer_array(a1 + 24);
  destroy_chunk_list(a1);
}
