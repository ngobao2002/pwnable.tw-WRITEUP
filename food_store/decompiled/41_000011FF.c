_BYTE *__fastcall sub_11FF(__int64 a1)
{
  _BYTE *result; // rax
  int chk; // [rsp+1Ch] [rbp-4h]

  chk = _read_chk();
  if ( chk <= 0 )
  {
    puts("read error");
    exit(1);
  }
  result = (_BYTE *)*(unsigned __int8 *)(chk - 1LL + a1);
  if ( (_BYTE)result == 10 )
  {
    result = (_BYTE *)(chk - 1LL + a1);
    *result = 0;
  }
  return result;
}
