unsigned __int64 __fastcall send_err(__int64 a1)
{
  unsigned __int64 v1; // rax
  __int64 v2; // rax
  int *v3; // rax
  char *v4; // rax
  _QWORD v6[2]; // [rsp+10h] [rbp-30h] BYREF
  size_t n[3]; // [rsp+20h] [rbp-20h] BYREF
  unsigned __int64 v8; // [rsp+38h] [rbp-8h]

  v8 = __readfsqword(0x28u);
  msgpack_sbuffer_init(n);
  msgpack_packer_init(v6, (__int64)n, (__int64)msgpack_sbuffer_write);
  msgpack_pack_array(v6, 3uLL);
  msgpack_pack_uint32(v6, 5u);
  msgpack_pack_uint32(v6, *(_DWORD *)(a1 + 316));
  v1 = strlen((const char *)(a1 + 320));
  msgpack_pack_str(v6, v1);
  v2 = strlen((const char *)(a1 + 320));
  msgpack_pack_str_body((__int64)v6, a1 + 320, v2);
  if ( sendto(sockfd, (const void *)n[1], n[0], 0, (const struct sockaddr *)(a1 + 296), *(_DWORD *)(a1 + 312)) == -1 )
  {
    v3 = __errno_location();
    v4 = strerror(*v3);
    fprintf(stderr, "sendto failed. [%s]\n", v4);
  }
  msgpack_sbuffer_destroy((__int64)n);
  return __readfsqword(0x28u) ^ v8;
}
