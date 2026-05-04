__int64 __fastcall unpacker_next(_QWORD *a1, __int64 *a2)
{
  __int64 *v3; // rbx
  __int64 v4; // rdx
  __int64 v5[4]; // [rsp+0h] [rbp-50h] BYREF
  __int64 *v6; // [rsp+20h] [rbp-30h]
  _QWORD *v7; // [rsp+28h] [rbp-28h]
  unsigned int v8; // [rsp+34h] [rbp-1Ch]
  unsigned __int64 v9; // [rsp+38h] [rbp-18h]

  v7 = a1;
  v6 = a2;
  v9 = __readfsqword(0x28u);
  msgpack_unpacked_destroy_0(a2);
  v8 = msgpack_unpacker_execute(a1);
  if ( (v8 & 0x80000000) == 0 )
  {
    if ( v8 )
    {
      *v6 = msgpack_unpacker_release_zone((__int64)v7);
      v3 = v6;
      msgpack_unpacker_data(v5, (__int64)v7);
      v4 = v5[1];
      v3[1] = v5[0];
      v3[2] = v4;
      v3[3] = v5[2];
      return 2LL;
    }
    else
    {
      return 0LL;
    }
  }
  else
  {
    *v6 = 0LL;
    memset(v6 + 1, 0, 0x18uLL);
    return v8;
  }
}
