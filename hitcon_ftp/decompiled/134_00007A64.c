__int64 __fastcall template_callback_str(__int64 a1, __int64 a2, __int64 a3, int a4, __int64 a5)
{
  _QWORD *v5; // rbx

  if ( !**(_QWORD **)a1 )
  {
    v5 = *(_QWORD **)a1;
    *v5 = msgpack_zone_new(0x2000LL);
    if ( !**(_QWORD **)a1 )
      return 4294967294LL;
  }
  *(_DWORD *)a5 = 5;
  *(_QWORD *)(a5 + 16) = a3;
  *(_DWORD *)(a5 + 8) = a4;
  *(_BYTE *)(a1 + 8) = 1;
  return 0LL;
}
