_QWORD *__fastcall msgpack_zone_new(__int64 a1)
{
  _QWORD *ptr; // [rsp+18h] [rbp-8h]

  ptr = malloc(0x38uLL);
  if ( !ptr )
    return 0LL;
  ptr[6] = a1;
  if ( (unsigned __int8)init_chunk_list(ptr, a1) != 1 )
  {
    free(ptr);
    return 0LL;
  }
  else
  {
    init_finalizer_array(ptr + 3);
    return ptr;
  }
}
