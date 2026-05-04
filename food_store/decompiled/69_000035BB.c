unsigned __int16 *__fastcall sub_35BB(unsigned __int16 *a1, __int64 a2)
{
  unsigned __int16 *result; // rax
  unsigned __int16 *i; // [rsp+8h] [rbp-18h]

  for ( i = a1; ; i += 4 )
  {
    result = i;
    if ( i >= &a1[4 * a2] )
      break;
    printf(
      "{ code=%u,jt=%u,jf=%u,k=%u },\n",
      *i,
      *((unsigned __int8 *)i + 2),
      *((unsigned __int8 *)i + 3),
      *((unsigned int *)i + 1));
  }
  return result;
}
