_QWORD *__fastcall clear_chunk_list(_QWORD *a1, __int64 a2)
{
  _QWORD *result; // rax
  _QWORD *ptr; // [rsp+10h] [rbp-10h]
  _QWORD *v4; // [rsp+18h] [rbp-8h]

  for ( ptr = (_QWORD *)a1[2]; ; ptr = v4 )
  {
    v4 = (_QWORD *)*ptr;
    if ( !*ptr )
      break;
    free(ptr);
  }
  a1[2] = ptr;
  *(_QWORD *)a1[2] = 0LL;
  *a1 = a2;
  result = a1;
  a1[1] = a1[2] + 8LL;
  return result;
}
