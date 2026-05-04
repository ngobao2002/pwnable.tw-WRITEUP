__int64 __fastcall msgpack_unpack(__int64 a1, unsigned __int64 a2, unsigned __int64 *a3, __int64 a4, __int64 a5)
{
  _QWORD *v6; // rbx
  __int64 v7; // rdx
  __int64 v8[5]; // [rsp+0h] [rbp-8A0h] BYREF
  _QWORD *v9; // [rsp+28h] [rbp-878h]
  __int64 v10; // [rsp+30h] [rbp-870h] BYREF
  unsigned __int64 *v11; // [rsp+38h] [rbp-868h]
  unsigned __int64 v12; // [rsp+40h] [rbp-860h]
  __int64 v13; // [rsp+48h] [rbp-858h]
  unsigned int v14; // [rsp+54h] [rbp-84Ch]
  unsigned __int64 v15; // [rsp+58h] [rbp-848h] BYREF
  __int64 *v16; // [rsp+60h] [rbp-840h] BYREF
  char v17; // [rsp+68h] [rbp-838h]
  unsigned __int64 v18; // [rsp+888h] [rbp-18h]

  v13 = a1;
  v12 = a2;
  v11 = a3;
  v10 = a4;
  v9 = (_QWORD *)a5;
  v18 = __readfsqword(0x28u);
  v15 = 0LL;
  if ( a3 )
    v15 = *v11;
  if ( v12 <= v15 )
    return 0LL;
  template_init((__int64)&v16);
  v16 = &v10;
  v17 = 0;
  v14 = template_execute((__int64)&v16, v13, v12, &v15);
  if ( (v14 & 0x80000000) != 0 )
    return v14;
  if ( v11 )
    *v11 = v15;
  if ( !v14 )
    return 0LL;
  v6 = v9;
  template_data(v8, &v16);
  v7 = v8[1];
  *v6 = v8[0];
  v6[1] = v7;
  v6[2] = v8[2];
  if ( v12 <= v15 )
    return 2LL;
  else
    return 1LL;
}
