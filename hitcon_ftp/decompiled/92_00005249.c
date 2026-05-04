__int64 __fastcall msgpack_pack_int64(_QWORD *a1, __int64 a2)
{
  uint32_t netlong[2]; // [rsp+0h] [rbp-30h] BYREF
  _QWORD *v4; // [rsp+8h] [rbp-28h]
  unsigned __int64 v5; // [rsp+10h] [rbp-20h]
  char v6; // [rsp+1Fh] [rbp-11h] BYREF
  unsigned __int64 v7; // [rsp+20h] [rbp-10h]
  unsigned __int64 v8; // [rsp+28h] [rbp-8h]

  v4 = a1;
  *(_QWORD *)netlong = a2;
  v8 = __readfsqword(0x28u);
  if ( a2 >= -32 )
  {
    if ( *(__int64 *)netlong > 127 )
    {
      if ( *(__int64 *)netlong > 0xFFFF )
      {
        if ( *(__int64 *)netlong > 0xFFFFFFFFLL )
        {
          v6 = -49;
          v5 = _bswap_64(*(unsigned __int64 *)netlong);
          v7 = v5;
          return ((__int64 (__fastcall *)(_QWORD, char *, __int64))v4[1])(*v4, &v6, 9LL);
        }
        else
        {
          v6 = -50;
          LODWORD(v5) = ntohl(netlong[0]);
          LODWORD(v7) = v5;
          return ((__int64 (__fastcall *)(_QWORD, char *, __int64))v4[1])(*v4, &v6, 5LL);
        }
      }
      else if ( *(__int64 *)netlong > 255 )
      {
        v6 = -51;
        LOWORD(v5) = ntohs(netlong[0]);
        LOWORD(v7) = v5;
        return ((__int64 (__fastcall *)(_QWORD, char *, __int64))v4[1])(*v4, &v6, 3LL);
      }
      else
      {
        v6 = -52;
        LOBYTE(v7) = netlong[0];
        return ((__int64 (__fastcall *)(_QWORD, char *, __int64))v4[1])(*v4, &v6, 2LL);
      }
    }
    else
    {
      return ((__int64 (__fastcall *)(_QWORD, uint32_t *, __int64))v4[1])(*v4, netlong, 1LL);
    }
  }
  else if ( *(__int64 *)netlong >= -32768 )
  {
    if ( *(__int64 *)netlong >= -128 )
    {
      v6 = -48;
      LOBYTE(v7) = netlong[0];
      return ((__int64 (__fastcall *)(_QWORD, char *, __int64))v4[1])(*v4, &v6, 2LL);
    }
    else
    {
      v6 = -47;
      LOWORD(v5) = ntohs(netlong[0]);
      LOWORD(v7) = v5;
      return ((__int64 (__fastcall *)(_QWORD, char *, __int64))v4[1])(*v4, &v6, 3LL);
    }
  }
  else if ( *(__int64 *)netlong >= (__int64)0xFFFFFFFF80000000LL )
  {
    v6 = -46;
    LODWORD(v5) = ntohl(netlong[0]);
    LODWORD(v7) = v5;
    return ((__int64 (__fastcall *)(_QWORD, char *, __int64))v4[1])(*v4, &v6, 5LL);
  }
  else
  {
    v6 = -45;
    v5 = _bswap_64(*(unsigned __int64 *)netlong);
    v7 = v5;
    return ((__int64 (__fastcall *)(_QWORD, char *, __int64))v4[1])(*v4, &v6, 9LL);
  }
}
