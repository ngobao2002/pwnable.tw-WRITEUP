__int64 __fastcall msgpack_unpacker_reset(__int64 a1)
{
  __int64 result; // rax

  template_init(*(_QWORD *)(a1 + 56));
  result = a1;
  *(_QWORD *)(a1 + 32) = 0LL;
  return result;
}
