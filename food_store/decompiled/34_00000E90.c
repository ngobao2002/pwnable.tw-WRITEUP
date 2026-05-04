__int64 (**sub_E90())(void)
{
  __int64 (**result)(void); // rax

  if ( !byte_205048 )
  {
    if ( &__cxa_finalize )
      _cxa_finalize(off_205008);
    result = sub_E00();
    byte_205048 = 1;
  }
  return result;
}
