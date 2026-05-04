__int64 __fastcall msgpack_pack_ext(_QWORD *a1, unsigned __int64 a2, char a3)
{
  __int64 result; // rax
  char v5; // [rsp+32h] [rbp-Eh] BYREF
  uint32_t v6; // [rsp+33h] [rbp-Dh]
  char v7; // [rsp+37h] [rbp-9h]
  unsigned __int64 v8; // [rsp+38h] [rbp-8h]

  v8 = __readfsqword(0x28u);
  switch ( a2 )
  {
    case 1uLL:
      v5 = -44;
      LOBYTE(v6) = a3;
      result = ((__int64 (__fastcall *)(_QWORD, char *, __int64))a1[1])(*a1, &v5, 2LL);
      break;
    case 2uLL:
      v5 = -43;
      LOBYTE(v6) = a3;
      result = ((__int64 (__fastcall *)(_QWORD, char *, __int64))a1[1])(*a1, &v5, 2LL);
      break;
    case 4uLL:
      v5 = -42;
      LOBYTE(v6) = a3;
      result = ((__int64 (__fastcall *)(_QWORD, char *, __int64))a1[1])(*a1, &v5, 2LL);
      break;
    case 8uLL:
      v5 = -41;
      LOBYTE(v6) = a3;
      result = ((__int64 (__fastcall *)(_QWORD, char *, __int64))a1[1])(*a1, &v5, 2LL);
      break;
    case 0x10uLL:
      v5 = -40;
      LOBYTE(v6) = a3;
      result = ((__int64 (__fastcall *)(_QWORD, char *, __int64))a1[1])(*a1, &v5, 2LL);
      break;
    default:
      if ( a2 > 0xFF )
      {
        if ( a2 > 0xFFFF )
        {
          v5 = -55;
          v6 = ntohl(a2);
          v7 = a3;
          result = ((__int64 (__fastcall *)(_QWORD, char *, __int64))a1[1])(*a1, &v5, 6LL);
        }
        else
        {
          v5 = -56;
          LOWORD(v6) = ntohs(a2);
          BYTE2(v6) = a3;
          result = ((__int64 (__fastcall *)(_QWORD, char *, __int64))a1[1])(*a1, &v5, 4LL);
        }
      }
      else
      {
        v5 = -57;
        LOBYTE(v6) = a2;
        BYTE1(v6) = a3;
        result = ((__int64 (__fastcall *)(_QWORD, char *, __int64))a1[1])(*a1, &v5, 3LL);
      }
      break;
  }
  return result;
}
