__int64 __fastcall msgpack_pack_uint8(_QWORD *a1, char a2)
{
  char v3[4]; // [rsp+4h] [rbp-1Ch] BYREF
  _QWORD *v4; // [rsp+8h] [rbp-18h]
  char v5[2]; // [rsp+16h] [rbp-Ah] BYREF
  unsigned __int64 v6; // [rsp+18h] [rbp-8h]

  v4 = a1;
  v3[0] = a2;
  v6 = __readfsqword(0x28u);
  if ( a2 >= 0 )
    return ((__int64 (__fastcall *)(_QWORD, char *, __int64))v4[1])(*v4, v3, 1LL);
  v5[0] = -52;
  v5[1] = v3[0];
  return ((__int64 (__fastcall *)(_QWORD, char *, __int64))v4[1])(*v4, v5, 2LL);
}
