unsigned int sub_FFC()
{
  unsigned int v0; // eax
  int fd; // [rsp+Ch] [rbp-4h]

  setvbuf(stdin, 0LL, 2, 0LL);
  setvbuf(stdout, 0LL, 2, 0LL);
  setvbuf(stderr, 0LL, 2, 0LL);
  v0 = time(0LL);
  srand(v0);
  stream = fopen("/home/food_store/flag", "r");
  if ( !stream )
  {
    puts("Open failed !");
    exit(-2);
  }
  qword_205050 = (char *)realloc(0LL, 0x80uLL);
  fd = open("/dev/urandom", 0);
  if ( fd < 0 )
  {
    puts("error");
    exit(-1);
  }
  read(fd, &unk_2050D8, 8uLL);
  close(fd);
  signal(14, (__sighandler_t)handler);
  return alarm(0x3Cu);
}
