__int64 __fastcall msgpack_unpacker_next(_QWORD *a1, __int64 *a2)
{
  unsigned int v3; // [rsp+1Ch] [rbp-4h]

  v3 = unpacker_next(a1, a2);
  if ( v3 == 2 )
    msgpack_unpacker_reset((__int64)a1);
  return v3;
}
