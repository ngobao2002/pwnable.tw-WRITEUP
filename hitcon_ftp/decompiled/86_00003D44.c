_QWORD *__fastcall add_request(__int64 a1)
{
  _QWORD *result; // rax
  int i; // [rsp+1Ch] [rbp-4h]

  for ( i = 0; i <= 31; ++i )
  {
    if ( !init_reqs[i] )
    {
      result = init_reqs;
      init_reqs[i] = a1;
      return result;
    }
  }
  return (_QWORD *)fwrite("reach max connection\n", 1uLL, 0x15uLL, stderr);
}
