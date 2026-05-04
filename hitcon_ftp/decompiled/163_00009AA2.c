_QWORD *__fastcall msgpack_zone_malloc_expand(_QWORD *a1, unsigned __int64 a2)
{
  unsigned __int64 i; // [rsp+18h] [rbp-28h]
  _QWORD *v4; // [rsp+30h] [rbp-10h]

  for ( i = a1[6]; i < a2; i *= 2LL )
  {
    if ( 2 * i <= i )
    {
      i = a2;
      break;
    }
  }
  v4 = malloc(i + 8);
  if ( !v4 )
    return 0LL;
  *v4 = a1[2];
  a1[2] = v4;
  *a1 = i - a2;
  a1[1] = (char *)v4 + a2 + 8;
  return v4 + 1;
}
