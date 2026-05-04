__int64 (**sub_E00())(void)
{
  __int64 (**result)(void); // rax

  result = (__int64 (**)(void))(&unk_205017 - &unk_205010);
  if ( (unsigned __int64)(&unk_205017 - &unk_205010) > 0xE )
  {
    result = &ITM_deregisterTMCloneTable;
    if ( &ITM_deregisterTMCloneTable )
      return (__int64 (**)(void))ITM_deregisterTMCloneTable();
  }
  return result;
}
