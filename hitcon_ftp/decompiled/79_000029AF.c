__int64 __fastcall send_oack(__int64 a1)
{
  unsigned __int64 v1; // rax
  __int64 v2; // rax
  int *v3; // rax
  char *v4; // rax
  unsigned __int8 v6; // [rsp+1Bh] [rbp-35h]
  int i; // [rsp+1Ch] [rbp-34h]
  _QWORD v8[2]; // [rsp+20h] [rbp-30h] BYREF
  size_t n[4]; // [rsp+30h] [rbp-20h] BYREF

  n[3] = __readfsqword(0x28u);
  v6 = 1;
  msgpack_sbuffer_init(n);
  msgpack_packer_init(v8, (__int64)n, (__int64)msgpack_sbuffer_write);
  msgpack_pack_array(v8, 2uLL);
  msgpack_pack_uint8(v8, 6);
  msgpack_pack_map(v8, opt_cnt);
  for ( i = 0; i < opt_cnt; ++i )
  {
    v1 = strlen((const char *)&oack_opts + 66 * i);
    msgpack_pack_str(v8, v1);
    v2 = strlen((const char *)&oack_opts + 66 * i);
    msgpack_pack_str_body((__int64)v8, (__int64)&oack_opts + 66 * i, v2);
    msgpack_pack_uint16(v8, word_20D1A0[33 * i]);
  }
  if ( sendto(sockfd, (const void *)n[1], n[0], 0, (const struct sockaddr *)(a1 + 296), *(_DWORD *)(a1 + 312)) == -1
    || *__errno_location() )
  {
    v3 = __errno_location();
    v4 = strerror(*v3);
    fprintf(stderr, "sendto failed. [%s]\n", v4);
    v6 = 0;
  }
  msgpack_sbuffer_destroy((__int64)n);
  return v6;
}
