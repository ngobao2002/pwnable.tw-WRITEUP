__int64 __fastcall msgpack_pack_object(
        _QWORD *a1,
        __int64 a2,
        __int64 a3,
        __int64 a4,
        __int64 a5,
        __int64 a6,
        int a7,
        unsigned __int64 a8,
        _QWORD *a9)
{
  __int64 result; // rax
  float v10; // xmm0_4
  int v11; // r8d
  int v12; // r9d
  int v13; // ecx
  int v14; // r8d
  int v15; // r9d
  int v16; // ecx
  int v17; // ecx
  int v18; // r8d
  int v19; // r9d
  int v20; // [rsp+1Ch] [rbp-34h]
  int v21; // [rsp+20h] [rbp-30h]
  int v22; // [rsp+24h] [rbp-2Ch]
  int v23; // [rsp+24h] [rbp-2Ch]
  int v24; // [rsp+24h] [rbp-2Ch]
  int v25; // [rsp+28h] [rbp-28h]
  int v26; // [rsp+28h] [rbp-28h]
  int v27; // [rsp+2Ch] [rbp-24h]
  _QWORD *v28; // [rsp+30h] [rbp-20h]
  _QWORD *v29; // [rsp+38h] [rbp-18h]

  switch ( a7 )
  {
    case 0:
      result = msgpack_pack_nil((__int64)a1);
      break;
    case 1:
      if ( (_BYTE)a8 )
        result = msgpack_pack_true((__int64)a1);
      else
        result = msgpack_pack_false((__int64)a1);
      break;
    case 2:
      result = msgpack_pack_uint64(a1, a8);
      break;
    case 3:
      result = msgpack_pack_int64(a1, a8);
      break;
    case 4:
      result = msgpack_pack_double((__int64)a1, *(double *)&a8);
      break;
    case 5:
      v27 = msgpack_pack_str_0(a1, (unsigned int)a8);
      if ( v27 >= 0 )
        result = msgpack_pack_str_body_0((__int64)a1, (__int64)a9, (unsigned int)a8);
      else
        result = (unsigned int)v27;
      break;
    case 6:
      v25 = msgpack_pack_array_0(a1, (unsigned int)a8);
      if ( v25 >= 0 )
      {
        v28 = a9;
        v13 = a8;
        while ( v28 != &a9[3 * (unsigned int)a8] )
        {
          v26 = msgpack_pack_object((_DWORD)a1, a8, (_DWORD)a1, v13, v11, v12, *v28, v28[1], v28[2]);
          if ( v26 < 0 )
            return (unsigned int)v26;
          v28 += 3;
        }
        result = 0LL;
      }
      else
      {
        result = (unsigned int)v25;
      }
      break;
    case 7:
      v22 = msgpack_pack_map_0(a1, (unsigned int)a8);
      if ( v22 >= 0 )
      {
        v29 = a9;
        v16 = a8;
        while ( v29 != &a9[6 * (unsigned int)a8] )
        {
          v23 = msgpack_pack_object((_DWORD)a1, a8, (_DWORD)a1, v16, v14, v15, *v29, v29[1], v29[2]);
          if ( v23 < 0 )
            return (unsigned int)v23;
          v24 = msgpack_pack_object((_DWORD)a1, a8, (_DWORD)a1, v17, v18, v19, v29[3], v29[4], v29[5]);
          if ( v24 < 0 )
            return (unsigned int)v24;
          v29 += 6;
        }
        result = 0LL;
      }
      else
      {
        result = (unsigned int)v22;
      }
      break;
    case 8:
      v21 = msgpack_pack_bin_0(a1, (unsigned int)a8);
      if ( v21 >= 0 )
        result = msgpack_pack_bin_body_0((__int64)a1, (__int64)a9, (unsigned int)a8);
      else
        result = (unsigned int)v21;
      break;
    case 9:
      v20 = msgpack_pack_ext(a1, HIDWORD(a8), a8);
      if ( v20 >= 0 )
        result = msgpack_pack_ext_body((__int64)a1, (__int64)a9, HIDWORD(a8));
      else
        result = (unsigned int)v20;
      break;
    case 10:
      v10 = *(double *)&a8;
      result = msgpack_pack_float((__int64)a1, v10);
      break;
    default:
      result = 0xFFFFFFFFLL;
      break;
  }
  return result;
}
