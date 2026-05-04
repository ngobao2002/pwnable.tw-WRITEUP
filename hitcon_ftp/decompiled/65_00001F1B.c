__int64 __fastcall msgpack_unpacker_buffer_consumed(__int64 a1, __int64 a2)
{
  __int64 result; // rax

  *(_QWORD *)(a1 + 8) += a2;
  result = a1;
  *(_QWORD *)(a1 + 16) -= a2;
  return result;
}
