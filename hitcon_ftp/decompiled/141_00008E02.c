void __fastcall decr_count(void *a1)
{
  if ( !_InterlockedDecrement((volatile signed __int32 *)a1) )
    free(a1);
}
