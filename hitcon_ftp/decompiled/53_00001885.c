__int64 __fastcall msgpack_pack_uint16(_QWORD *a1, uint16_t a2)
{
  uint16_t v3; // [rsp+4h] [rbp-1Ch] BYREF
  _QWORD *v4; // [rsp+8h] [rbp-18h]
  uint16_t v5; // [rsp+12h] [rbp-Eh]
  char v6; // [rsp+15h] [rbp-Bh] BYREF
  uint16_t v7; // [rsp+16h] [rbp-Ah]
  unsigned __int64 v8; // [rsp+18h] [rbp-8h]

  v4 = a1;
  v3 = a2;
  v8 = __readfsqword(0x28u);
  if ( a2 <= 0x7Fu )
    return ((__int64 (__fastcall *)(_QWORD, uint16_t *, __int64))v4[1])(*v4, &v3, 1LL);
  if ( v3 > 0xFFu )
  {
    v6 = -51;
    v5 = ntohs(v3);
    v7 = v5;
    return ((__int64 (__fastcall *)(_QWORD, char *, __int64))v4[1])(*v4, &v6, 3LL);
  }
  else
  {
    v6 = -52;
    LOBYTE(v7) = v3;
    return ((__int64 (__fastcall *)(_QWORD, char *, __int64))v4[1])(*v4, &v6, 2LL);
  }
}
