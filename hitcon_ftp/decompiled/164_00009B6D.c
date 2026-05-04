_QWORD *__fastcall init_finalizer_array(_QWORD *a1)
{
  _QWORD *result; // rax

  *a1 = 0LL;
  a1[1] = 0LL;
  result = a1;
  a1[2] = 0LL;
  return result;
}
