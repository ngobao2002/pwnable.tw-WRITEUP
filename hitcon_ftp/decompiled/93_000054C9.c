__int64 __fastcall msgpack_pack_float(__int64 a1, float a2)
{
  char v3; // [rsp+23h] [rbp-Dh] BYREF
  uint32_t v4; // [rsp+24h] [rbp-Ch]
  unsigned __int64 v5; // [rsp+28h] [rbp-8h]

  v5 = __readfsqword(0x28u);
  v3 = -54;
  v4 = ntohl(LODWORD(a2));
  return (*(__int64 (__fastcall **)(_QWORD, char *, __int64))(a1 + 8))(*(_QWORD *)a1, &v3, 5LL);
}
