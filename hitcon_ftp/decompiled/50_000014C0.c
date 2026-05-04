void *_do_global_dtors_aux()
{
  void *result; // rax

  if ( !completed_7697 )
  {
    if ( &__cxa_finalize )
      _cxa_finalize(_dso_handle);
    result = deregister_tm_clones();
    completed_7697 = 1;
  }
  return result;
}
