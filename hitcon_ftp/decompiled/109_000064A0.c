__int64 __fastcall msgpack_object_bin_print_buffer(char *a1, size_t a2, __int64 a3, unsigned __int64 a4)
{
  int v7; // [rsp+24h] [rbp-1Ch]
  int v8; // [rsp+24h] [rbp-1Ch]
  unsigned __int64 i; // [rsp+28h] [rbp-18h]
  size_t maxlen; // [rsp+38h] [rbp-8h]

  maxlen = a2;
  for ( i = 0LL; i < a4; ++i )
  {
    if ( *(_BYTE *)(a3 + i) == 34 )
    {
      v7 = snprintf(a1, maxlen, "\\\"");
      if ( v7 <= 0 || v7 > (int)maxlen )
        return 0LL;
      a1 += v7;
      maxlen -= v7;
    }
    else if ( ((*__ctype_b_loc())[*(unsigned __int8 *)(a3 + i)] & 0x4000) != 0 )
    {
      if ( maxlen )
      {
        *a1++ = *(_BYTE *)(a3 + i);
        --maxlen;
      }
    }
    else
    {
      v8 = snprintf(a1, maxlen, "\\x%02x", *(unsigned __int8 *)(a3 + i));
      if ( v8 <= 0 || v8 > (int)maxlen )
        return 0LL;
      a1 += v8;
      maxlen -= v8;
    }
  }
  return (unsigned int)(a2 - maxlen);
}
