void *__fastcall msgpack_unpacked_destroy_0(_QWORD *a1)
{
  void *result; // rax

  result = (void *)*a1;
  if ( *a1 )
  {
    msgpack_zone_free(*a1);
    *a1 = 0LL;
    return memset(a1 + 1, 0, 0x18uLL);
  }
  return result;
}
