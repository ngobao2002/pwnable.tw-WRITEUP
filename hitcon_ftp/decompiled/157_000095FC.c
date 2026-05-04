__int64 __fastcall msgpack_unpacker_next_with_size(_QWORD *a1, __int64 *a2, _QWORD *a3)
{
  unsigned int v5; // [rsp+2Ch] [rbp-4h]

  v5 = unpacker_next(a1, a2);
  if ( v5 == 2 || !v5 )
    *a3 = a1[4];
  if ( v5 == 2 )
    msgpack_unpacker_reset((__int64)a1);
  return v5;
}
