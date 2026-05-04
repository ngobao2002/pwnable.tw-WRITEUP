__int64 __fastcall call_finalizer_array(__int64 *a1)
{
  __int64 result; // rax
  __int64 i; // [rsp+18h] [rbp-8h]

  for ( i = *a1; ; i -= 16LL )
  {
    result = a1[2];
    if ( i == result )
      break;
    (*(void (__fastcall **)(_QWORD))(i - 16))(*(_QWORD *)(i - 16 + 8));
  }
  return result;
}
