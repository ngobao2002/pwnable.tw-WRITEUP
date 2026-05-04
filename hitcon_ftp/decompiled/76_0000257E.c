__int64 __fastcall send_data(__int64 a1, __int64 a2, unsigned __int64 a3)
{
  int *v3; // rax
  char *v4; // rax
  unsigned int v7; // [rsp+24h] [rbp-3Ch]
  int i; // [rsp+28h] [rbp-38h]
  unsigned int v9; // [rsp+2Ch] [rbp-34h]
  _QWORD v10[2]; // [rsp+30h] [rbp-30h] BYREF
  size_t n[4]; // [rsp+40h] [rbp-20h] BYREF

  n[3] = __readfsqword(0x28u);
  msgpack_sbuffer_init(n);
  msgpack_packer_init(v10, (__int64)n, (__int64)msgpack_sbuffer_write);
  msgpack_pack_array(v10, 4uLL);
  msgpack_pack_uint8(v10, 3);
  msgpack_pack_uint16(v10, *(_WORD *)(a1 + 290));
  v7 = -1;
  for ( i = 1; a3 > i; ++i )
    v7 = (v7 >> 8) ^ crc32_tab[(unsigned __int8)(v7 ^ *(_BYTE *)(i + a2))];
  msgpack_pack_uint32(v10, ~v7);
  msgpack_pack_bin(v10, a3);
  msgpack_pack_bin_body((__int64)v10, a2, a3);
  v9 = sendto(sockfd, (const void *)n[1], n[0], 0, (const struct sockaddr *)(a1 + 296), *(_DWORD *)(a1 + 312));
  if ( v9 == -1 || *__errno_location() )
  {
    v3 = __errno_location();
    v4 = strerror(*v3);
    fprintf(stderr, "sendto failed. [%s]\n", v4);
  }
  msgpack_sbuffer_destroy((__int64)n);
  return v9;
}
