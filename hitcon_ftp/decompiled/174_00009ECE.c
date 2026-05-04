void __fastcall msgpack_zone_free(void *a1)
{
  if ( a1 )
  {
    msgpack_zone_destroy((__int64)a1);
    free(a1);
  }
}
