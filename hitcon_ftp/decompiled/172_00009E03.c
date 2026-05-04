__int64 __fastcall msgpack_zone_init(_QWORD *a1, __int64 a2)
{
  a1[6] = a2;
  if ( (unsigned __int8)init_chunk_list(a1, a2) != 1 )
    return 0LL;
  init_finalizer_array(a1 + 3);
  return 1LL;
}
