__int64 __fastcall msgpack_pack_bin(_QWORD *a1, unsigned __int64 a2)
{
  char v3; // [rsp+23h] [rbp-Dh] BYREF
  uint32_t v4; // [rsp+24h] [rbp-Ch]
  unsigned __int64 v5; // [rsp+28h] [rbp-8h]

  v5 = __readfsqword(0x28u);
  if ( a2 > 0xFF )
  {
    if ( a2 > 0xFFFF )
    {
      v3 = -58;
      v4 = ntohl(a2);
      return ((__int64 (__fastcall *)(_QWORD, char *, __int64))a1[1])(*a1, &v3, 5LL);
    }
    else
    {
      v3 = -59;
      LOWORD(v4) = ntohs(a2);
      return ((__int64 (__fastcall *)(_QWORD, char *, __int64))a1[1])(*a1, &v3, 3LL);
    }
  }
  else
  {
    v3 = -60;
    LOBYTE(v4) = a2;
    return ((__int64 (__fastcall *)(_QWORD, char *, __int64))a1[1])(*a1, &v3, 2LL);
  }
}
