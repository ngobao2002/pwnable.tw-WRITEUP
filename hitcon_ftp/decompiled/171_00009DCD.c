_QWORD *__fastcall msgpack_zone_clear(_QWORD *a1)
{
  clear_finalizer_array(a1 + 3);
  return clear_chunk_list(a1, a1[6]);
}
