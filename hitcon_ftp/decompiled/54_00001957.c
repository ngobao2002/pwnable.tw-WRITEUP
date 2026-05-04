__int64 __fastcall msgpack_pack_uint32(_QWORD *a1, uint32_t a2)
{
  uint32_t netlong; // [rsp+4h] [rbp-2Ch] BYREF
  _QWORD *v4; // [rsp+8h] [rbp-28h]
  uint32_t v5; // [rsp+1Ch] [rbp-14h]
  char v6; // [rsp+23h] [rbp-Dh] BYREF
  uint32_t v7; // [rsp+24h] [rbp-Ch]
  unsigned __int64 v8; // [rsp+28h] [rbp-8h]

  v4 = a1;
  netlong = a2;
  v8 = __readfsqword(0x28u);
  if ( a2 > 0xFF )
  {
    if ( netlong > 0xFFFF )
    {
      v6 = -50;
      v5 = ntohl(netlong);
      v7 = v5;
      return ((__int64 (__fastcall *)(_QWORD, char *, __int64))v4[1])(*v4, &v6, 5LL);
    }
    else
    {
      v6 = -51;
      LOWORD(v5) = ntohs(netlong);
      LOWORD(v7) = v5;
      return ((__int64 (__fastcall *)(_QWORD, char *, __int64))v4[1])(*v4, &v6, 3LL);
    }
  }
  else if ( netlong > 0x7F )
  {
    v6 = -52;
    LOBYTE(v7) = netlong;
    return ((__int64 (__fastcall *)(_QWORD, char *, __int64))v4[1])(*v4, &v6, 2LL);
  }
  else
  {
    return ((__int64 (__fastcall *)(_QWORD, uint32_t *, __int64))v4[1])(*v4, &netlong, 1LL);
  }
}
