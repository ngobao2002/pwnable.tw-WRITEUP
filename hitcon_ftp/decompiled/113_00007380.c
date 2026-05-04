unsigned __int64 __fastcall msgpack_zone_malloc(unsigned __int64 *a1, __int64 a2)
{
  unsigned __int64 v3; // [rsp+18h] [rbp-18h]
  unsigned __int64 v4; // [rsp+20h] [rbp-10h]
  __int64 v5; // [rsp+28h] [rbp-8h]

  v3 = (a1[1] + 7) & 0xFFFFFFFFFFFFFFF8LL;
  v4 = v3 - a1[1] + a2;
  if ( v4 > *a1 )
  {
    v5 = msgpack_zone_malloc_expand(a1, a2 + 7);
    if ( v5 )
      return v5 & 0xFFFFFFFFFFFFFFF8LL;
    else
      return 0LL;
  }
  else
  {
    *a1 -= v4;
    a1[1] += v4;
    return v3;
  }
}
