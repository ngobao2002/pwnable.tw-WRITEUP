__int64 __fastcall init_chunk_list(_QWORD *a1, __int64 a2)
{
  _QWORD *v3; // [rsp+18h] [rbp-8h]

  v3 = malloc(a2 + 8);
  if ( !v3 )
    return 0LL;
  a1[2] = v3;
  *a1 = a2;
  a1[1] = v3 + 1;
  *v3 = 0LL;
  return 1LL;
}
