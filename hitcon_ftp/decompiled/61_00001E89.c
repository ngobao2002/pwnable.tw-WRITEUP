_QWORD *__fastcall msgpack_packer_init(_QWORD *a1, __int64 a2, __int64 a3)
{
  _QWORD *result; // rax

  *a1 = a2;
  result = a1;
  a1[1] = a3;
  return result;
}
