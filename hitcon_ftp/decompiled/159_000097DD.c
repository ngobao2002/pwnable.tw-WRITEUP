__int64 __fastcall msgpack_unpack_next(_QWORD *a1, __int64 a2, unsigned __int64 a3, unsigned __int64 *a4)
{
  _QWORD *v5; // rbx
  __int64 v6; // rdx
  __int64 v7[4]; // [rsp+0h] [rbp-890h] BYREF
  unsigned __int64 *v8; // [rsp+20h] [rbp-870h]
  unsigned __int64 v9; // [rsp+28h] [rbp-868h]
  __int64 v10; // [rsp+30h] [rbp-860h]
  _QWORD *v11; // [rsp+38h] [rbp-858h]
  unsigned int v12; // [rsp+44h] [rbp-84Ch]
  unsigned __int64 v13; // [rsp+48h] [rbp-848h] BYREF
  _QWORD *v14; // [rsp+50h] [rbp-840h] BYREF
  char v15; // [rsp+58h] [rbp-838h]
  unsigned __int64 v16; // [rsp+878h] [rbp-18h]

  v11 = a1;
  v10 = a2;
  v9 = a3;
  v8 = a4;
  v16 = __readfsqword(0x28u);
  v13 = 0LL;
  msgpack_unpacked_destroy_0(a1);
  if ( v8 )
    v13 = *v8;
  if ( v9 <= v13 )
    return 0LL;
  template_init((__int64)&v14);
  v14 = v11;
  v15 = 0;
  v12 = template_execute((__int64)&v14, v10, v9, &v13);
  if ( v8 )
    *v8 = v13;
  if ( (v12 & 0x80000000) == 0 )
  {
    if ( v12 )
    {
      v5 = v11;
      template_data(v7, &v14);
      v6 = v7[1];
      v5[1] = v7[0];
      v5[2] = v6;
      v5[3] = v7[2];
      return 2LL;
    }
    else
    {
      return 0LL;
    }
  }
  else
  {
    msgpack_zone_free(*v11);
    *v11 = 0LL;
    return v12;
  }
}
