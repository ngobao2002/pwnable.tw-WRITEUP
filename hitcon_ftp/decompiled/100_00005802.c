__int64 __fastcall msgpack_pack_str_0(_QWORD *a1, unsigned __int64 a2)
{
  uint32_t v3; // [rsp+1Ch] [rbp-14h] BYREF
  char v4; // [rsp+23h] [rbp-Dh] BYREF
  uint32_t v5; // [rsp+24h] [rbp-Ch]
  unsigned __int64 v6; // [rsp+28h] [rbp-8h]

  v6 = __readfsqword(0x28u);
  if ( a2 > 0x1F )
  {
    if ( a2 > 0xFF )
    {
      if ( a2 > 0xFFFF )
      {
        v4 = -37;
        v3 = ntohl(a2);
        v5 = v3;
        return ((__int64 (__fastcall *)(_QWORD, char *, __int64))a1[1])(*a1, &v4, 5LL);
      }
      else
      {
        v4 = -38;
        LOWORD(v3) = ntohs(a2);
        LOWORD(v5) = v3;
        return ((__int64 (__fastcall *)(_QWORD, char *, __int64))a1[1])(*a1, &v4, 3LL);
      }
    }
    else
    {
      v4 = -39;
      LOBYTE(v5) = a2;
      return ((__int64 (__fastcall *)(_QWORD, char *, __int64))a1[1])(*a1, &v4, 2LL);
    }
  }
  else
  {
    LOBYTE(v3) = a2 | 0xA0;
    return ((__int64 (__fastcall *)(_QWORD, uint32_t *, __int64))a1[1])(*a1, &v3, 1LL);
  }
}
