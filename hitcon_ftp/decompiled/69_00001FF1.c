void __fastcall msgpack_sbuffer_destroy(__int64 a1)
{
  free(*(void **)(a1 + 8));
}
