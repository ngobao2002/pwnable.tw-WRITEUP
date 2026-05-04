__int64 __fastcall msgpack_pack_double(__int64 a1, double a2)
{
  char v3; // [rsp+2Fh] [rbp-11h] BYREF
  unsigned __int64 v4; // [rsp+30h] [rbp-10h]
  unsigned __int64 v5; // [rsp+38h] [rbp-8h]

  v5 = __readfsqword(0x28u);
  v3 = -53;
  v4 = _bswap_64(*(unsigned __int64 *)&a2);
  return (*(__int64 (__fastcall **)(_QWORD, char *, __int64))(a1 + 8))(*(_QWORD *)a1, &v3, 9LL);
}
