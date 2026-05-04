__int64 sub_1134()
{
  char nptr[24]; // [rsp+10h] [rbp-20h] BYREF
  unsigned __int64 v2; // [rsp+28h] [rbp-8h]

  v2 = __readfsqword(0x28u);
  if ( (int)_read_chk() <= 0 )
  {
    puts("Error");
    exit(-1);
  }
  return atoll(nptr);
}
