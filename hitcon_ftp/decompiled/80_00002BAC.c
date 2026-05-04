__int64 __fastcall clean_req(__int64 a1)
{
  __int64 result; // rax

  if ( *(_QWORD *)(a1 + 272) )
  {
    fclose(*(FILE **)(a1 + 272));
    *(_QWORD *)(a1 + 272) = 0LL;
  }
  result = a1;
  *(_BYTE *)a1 = -1;
  return result;
}
