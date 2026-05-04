__int64 *__fastcall clear_finalizer_array(__int64 *a1)
{
  __int64 *result; // rax

  call_finalizer_array(a1);
  result = a1;
  *a1 = a1[2];
  return result;
}
