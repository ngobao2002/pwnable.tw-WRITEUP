void *__fastcall msgpack_unpacker_new(unsigned __int64 a1)
{
  void *ptr; // [rsp+18h] [rbp-8h]

  ptr = malloc(0x40uLL);
  if ( !ptr )
    return 0LL;
  if ( (unsigned __int8)msgpack_unpacker_init((__int64)ptr, a1) == 1 )
    return ptr;
  free(ptr);
  return 0LL;
}
