__int64 __fastcall send_ack(__int64 a1)
{
  int *v1; // rax
  char *v2; // rax
  unsigned int v4; // [rsp+1Ch] [rbp-34h]
  _QWORD v5[2]; // [rsp+20h] [rbp-30h] BYREF
  size_t n[4]; // [rsp+30h] [rbp-20h] BYREF

  n[3] = __readfsqword(0x28u);
  msgpack_sbuffer_init(n);
  msgpack_packer_init(v5, (__int64)n, (__int64)msgpack_sbuffer_write);
  msgpack_pack_array(v5, 2uLL);
  msgpack_pack_uint8(v5, 4);
  msgpack_pack_uint16(v5, *(_WORD *)(a1 + 290));
  v4 = sendto(sockfd, (const void *)n[1], n[0], 0, (const struct sockaddr *)(a1 + 296), *(_DWORD *)(a1 + 312));
  if ( v4 == -1 || *__errno_location() )
  {
    v1 = __errno_location();
    v2 = strerror(*v1);
    fprintf(stderr, "sendto failed. [%s]\n", v2);
  }
  msgpack_sbuffer_destroy((__int64)n);
  return v4;
}
