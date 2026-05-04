void __fastcall destroy_finalizer_array(__int64 a1)
{
  call_finalizer_array((__int64 *)a1);
  free(*(void **)(a1 + 16));
}
