volatile signed __int32 *__fastcall incr_count(volatile signed __int32 *a1)
{
  volatile signed __int32 *result; // rax

  result = a1;
  _InterlockedAdd(a1, 1u);
  return result;
}
