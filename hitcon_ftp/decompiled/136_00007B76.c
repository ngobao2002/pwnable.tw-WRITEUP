__int64 __fastcall template_callback_ext(__int64 a1, __int64 a2, _BYTE *a3, int a4, __int64 a5)
{
  _QWORD *v6; // rbx

  if ( !a4 )
    return 0xFFFFFFFFLL;
  if ( !**(_QWORD **)a1 )
  {
    v6 = *(_QWORD **)a1;
    *v6 = msgpack_zone_new(0x2000LL);
    if ( !**(_QWORD **)a1 )
      return 4294967294LL;
  }
  *(_DWORD *)a5 = 9;
  *(_BYTE *)(a5 + 8) = *a3;
  *(_QWORD *)(a5 + 16) = a3 + 1;
  *(_DWORD *)(a5 + 12) = a4 - 1;
  *(_BYTE *)(a1 + 8) = 1;
  return 0LL;
}
