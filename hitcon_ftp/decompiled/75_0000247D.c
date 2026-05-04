unsigned __int64 __fastcall add_oack_opt(const char *a1, __int16 a2)
{
  unsigned __int64 result; // rax
  int i; // [rsp+1Ch] [rbp-4h]

  result = (unsigned int)opt_cnt;
  if ( opt_cnt <= 7 )
  {
    for ( i = 0; i < opt_cnt; ++i )
    {
      if ( !strcmp((const char *)&oack_opts + 66 * i, a1) )
      {
        result = (unsigned __int64)word_20D1A0;
        word_20D1A0[33 * i] = a2;
        return result;
      }
    }
    strncpy((char *)&oack_opts + 66 * opt_cnt, a1, 0x3FuLL);
    word_20D1A0[33 * opt_cnt] = a2;
    return (unsigned int)++opt_cnt;
  }
  return result;
}
