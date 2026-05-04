int __cdecl main(int argc, const char **argv, const char **envp)
{
  int *v3; // rax
  char *v4; // rax
  uint16_t v5; // ax
  int *v6; // rax
  char *v7; // rax
  int *v8; // rax
  char *v9; // rax
  unsigned __int64 v11; // rax
  size_t v12; // r12
  const void *v13; // rbx
  void *v14; // rax
  int v15; // r8d
  int v16; // r9d
  __int64 request; // [rsp+20h] [rbp-480h]
  void *dest; // [rsp+28h] [rbp-478h]
  struct timeval timeout; // [rsp+30h] [rbp-470h] BYREF
  __int64 v20; // [rsp+40h] [rbp-460h]
  _QWORD *v21; // [rsp+48h] [rbp-458h]
  __int64 v22; // [rsp+50h] [rbp-450h] BYREF
  int v23; // [rsp+58h] [rbp-448h]
  __int64 v24; // [rsp+60h] [rbp-440h]
  _QWORD *v25; // [rsp+68h] [rbp-438h]
  _QWORD v26[8]; // [rsp+70h] [rbp-430h] BYREF
  struct sockaddr addr; // [rsp+B0h] [rbp-3F0h] BYREF
  fd_set readfds; // [rsp+C0h] [rbp-3E0h] BYREF
  unsigned __int8 s[290]; // [rsp+140h] [rbp-360h] BYREF
  unsigned __int16 v30; // [rsp+262h] [rbp-23Eh]
  unsigned int v31; // [rsp+264h] [rbp-23Ch]
  struct sockaddr v32; // [rsp+268h] [rbp-238h] BYREF
  socklen_t v33; // [rsp+278h] [rbp-228h] BYREF
  unsigned int v34; // [rsp+27Ch] [rbp-224h] BYREF
  char v35[520]; // [rsp+280h] [rbp-220h] BYREF
  unsigned __int64 v36; // [rsp+488h] [rbp-18h]

  v36 = __readfsqword(0x28u);
  sockfd = socket(2, 2, 17);
  if ( sockfd == -1 )
  {
    v3 = __errno_location();
    v4 = strerror(*v3);
    fprintf(stderr, "socket failed. [%s]\n", v4);
    exit(-1);
  }
  addr.sa_family = 2;
  *(_DWORD *)&addr.sa_data[2] = inet_addr("0.0.0.0");
  if ( argc <= 1 )
    v5 = 69;
  else
    v5 = strtoul(argv[1], 0LL, 10);
  *(_WORD *)addr.sa_data = htons(v5);
  if ( bind(sockfd, &addr, 0x10u) < 0 )
  {
    v6 = __errno_location();
    v7 = strerror(*v6);
    fprintf(stderr, "bind failed. [%s]\n", v7);
    exit(-1);
  }
  datain = calloc(1uLL, 0xFFC5uLL);
  if ( !datain )
  {
    fwrite("out of memory\n", 1uLL, 0xEuLL, stderr);
    exit(-1);
  }
  while ( 1 )
  {
    timeout.tv_sec = 1LL;
    timeout.tv_usec = 0LL;
    memset(&readfds, 0, sizeof(readfds));
    readfds.fds_bits[sockfd / 64] |= 1LL << (sockfd % 64);
    if ( select(sockfd + 1, &readfds, 0LL, 0LL, &timeout) == -1 )
      break;
    if ( (readfds.fds_bits[sockfd / 64] & (1LL << (sockfd % 64))) != 0 )
    {
      memset(s, 0, 0x340uLL);
      v33 = 16;
      *__errno_location() = 0;
      v31 = recvfrom(sockfd, datain, 0xFFC5uLL, 0, &v32, &v33);
      if ( v31 <= 2 || *__errno_location() )
      {
        puts("illegal packet");
      }
      else
      {
        msgpack_unpacker_init(v26, 65477LL);
        v11 = msgpack_unpacker_buffer_capacity((__int64)v26);
        if ( v11 < (int)v31 && (unsigned __int8)msgpack_unpacker_reserve_buffer((__int64)v26, (int)v31) )
        {
          fwrite("out of memory\n", 1uLL, 0xEuLL, stderr);
          exit(-1);
        }
        v12 = (int)v31;
        v13 = datain;
        v14 = (void *)msgpack_unpacker_buffer(v26);
        memcpy(v14, v13, v12);
        msgpack_unpacker_buffer_consumed((__int64)v26, (int)v31);
        msgpack_unpacked_init(&v22);
        if ( (unsigned int)msgpack_unpacker_next(v26, &v22) != 2 )
        {
          puts("parse packet failed");
          goto LABEL_94;
        }
        if ( v23 != 6 || (unsigned int)v24 <= 1 || *(_DWORD *)v25 != 2 )
        {
LABEL_39:
          v34 = 4;
          strcpy(v35, "illegal packet");
          send_err((__int64)s);
          goto LABEL_94;
        }
        v20 = v24;
        v21 = v25;
        s[0] = v25[1];
        request = find_request(*(__int64 *)&v32.sa_family);
        if ( request )
        {
          if ( s[0] == 3 || (int)v31 <= 512 )
          {
            switch ( s[0] )
            {
              case 1u:
              case 2u:
                if ( (unsigned int)v20 <= 2
                  || *(_DWORD *)v21 != 2
                  || *((_DWORD *)v21 + 6) != 5
                  || *((_DWORD *)v21 + 12) != 5
                  || (_DWORD)v20 == 4 && *((_DWORD *)v21 + 18) != 7 )
                {
                  goto LABEL_62;
                }
                *(_BYTE *)request = s[0];
                process_new((unsigned __int8 *)request, v20, v21);
                break;
              case 3u:
                if ( *(_BYTE *)request == 2 )
                {
                  if ( (_DWORD)v20 != 4
                    || *(_DWORD *)v21 != 2
                    || *((_DWORD *)v21 + 6) != 2
                    || *((_DWORD *)v21 + 12) != 2
                    || *((_DWORD *)v21 + 18) != 8 && *((_DWORD *)v21 + 18) != 5 )
                  {
                    goto LABEL_62;
                  }
                  v30 = v21[4];
                  if ( (int)v31 <= (unsigned __int64)*(unsigned __int16 *)(request + 280) + 13 )
                  {
                    if ( !check_crc32((const char *)&v34, v21[7], v21[7], (int)&v34, v15, v16, v21[9], v21[10], v21[11]) )
                    {
                      *(_DWORD *)(request + 316) = 4;
                      strcpy((char *)(request + 320), "crc32 check failed");
                      send_err(request);
                      clean_req(request);
                    }
                    else if ( v30 == *(unsigned __int16 *)(request + 290) + 1
                           && (unsigned __int8)process_recv(request, *((_DWORD *)v21 + 20), (const void *)v21[11]) != 1 )
                    {
                      puts("process recving failed");
                    }
                  }
                  else
                  {
                    *(_DWORD *)(request + 316) = 4;
                    strcpy((char *)(request + 320), "illegal packet");
                    send_err(request);
                    puts("incoming packet too large");
                    clean_req(request);
                  }
                }
                break;
              case 4u:
                if ( *(_BYTE *)request == 1 )
                {
                  if ( (_DWORD)v20 != 2 || *(_DWORD *)v21 != 2 || *((_DWORD *)v21 + 6) != 2 )
                    goto LABEL_62;
                  v30 = v21[4];
                  if ( v30 == *(_WORD *)(request + 290) )
                  {
                    *(_DWORD *)(request + 292) = v31;
                    if ( (unsigned __int8)process_send(request) != 1 )
                      puts("process sending failed");
                  }
                }
                break;
              case 5u:
                if ( (_DWORD)v20 == 3 && *(_DWORD *)v21 == 2 && *((_DWORD *)v21 + 6) == 2 && *((_DWORD *)v21 + 12) == 5 )
                {
                  *(_DWORD *)(request + 316) = v21[4];
                  memset((void *)(request + 320), 0, 0x200uLL);
                  strncpy((char *)(request + 320), (const char *)v21[8], *((unsigned int *)v21 + 14));
                  printf("error %i at client, %s\n", *(unsigned int *)(request + 316), (const char *)(request + 320));
                  clean_req(request);
                }
                else
                {
LABEL_62:
                  *(_DWORD *)(request + 316) = 4;
                  strcpy((char *)(request + 320), "illegal packet");
                  send_err(request);
                }
                break;
              default:
                *(_DWORD *)(request + 316) = 4;
                strcpy((char *)(request + 320), "illegal packet");
                send_err(request);
                printf("unexpected opcode %u\n", s[0]);
                clean_req(request);
                break;
            }
          }
          else
          {
            *(_DWORD *)(request + 316) = 4;
            strcpy((char *)(request + 320), "illegal packet");
            send_err(request);
            puts("incoming packet too large");
          }
        }
        else
        {
          if ( (int)v31 > 512 )
          {
            v34 = 4;
            strcpy(v35, "illegal packet");
            send_err((__int64)s);
            puts("incoming packet too large");
            goto LABEL_94;
          }
          if ( !s[0] )
          {
LABEL_50:
            v34 = 5;
            strcpy(v35, "unknown transfer id");
            send_err((__int64)s);
            puts(v35);
            goto LABEL_94;
          }
          if ( s[0] <= 2u )
          {
            if ( (unsigned int)v20 <= 2
              || *(_DWORD *)v21 != 2
              || *((_DWORD *)v21 + 6) != 5
              || *((_DWORD *)v21 + 12) != 5
              || (_DWORD)v20 == 4 && *((_DWORD *)v21 + 18) != 7 )
            {
              goto LABEL_39;
            }
            if ( (unsigned __int8)process_new(s, v24, v25) )
            {
              dest = calloc(1uLL, 0x340uLL);
              if ( !dest )
              {
                fwrite("out of memory\n", 1uLL, 0xEuLL, stderr);
                exit(-1);
              }
              memcpy(dest, s, 0x340uLL);
              add_request((__int64)dest);
            }
          }
          else
          {
            if ( s[0] != 5 )
              goto LABEL_50;
            if ( (_DWORD)v20 != 3 || *(_DWORD *)v21 != 2 || *((_DWORD *)v21 + 6) != 2 || *((_DWORD *)v21 + 12) != 5 )
              goto LABEL_39;
            v34 = v21[4];
            strncpy(v35, (const char *)v21[8], *((unsigned int *)v21 + 14));
            printf("error %i at client, %s\n", v34, v35);
          }
        }
LABEL_94:
        msgpack_unpacker_destroy(v26);
        msgpack_unpacked_destroy(&v22);
      }
    }
  }
  v8 = __errno_location();
  v9 = strerror(*v8);
  fprintf(stderr, "select failed. [%s]\n", v9);
  return -1;
}
