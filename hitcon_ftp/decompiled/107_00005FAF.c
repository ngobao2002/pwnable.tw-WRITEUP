unsigned __int64 __fastcall msgpack_object_bin_print(FILE *a1, __int64 a2, unsigned __int64 a3)
{
  unsigned __int64 result; // rax
  unsigned __int64 i; // [rsp+28h] [rbp-8h]

  for ( i = 0LL; ; ++i )
  {
    result = i;
    if ( i >= a3 )
      break;
    if ( *(_BYTE *)(a2 + i) == 34 )
    {
      fwrite("\\\"", 1uLL, 2uLL, a1);
    }
    else if ( ((*__ctype_b_loc())[*(unsigned __int8 *)(a2 + i)] & 0x4000) != 0 )
    {
      fputc(*(char *)(a2 + i), a1);
    }
    else
    {
      fprintf(a1, "\\x%02x", *(unsigned __int8 *)(a2 + i));
    }
  }
  return result;
}
