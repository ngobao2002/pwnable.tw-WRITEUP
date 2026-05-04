unsigned __int64 init()
{
  setvbuf(stdout, 0LL, 2, 0LL);
  close(0);
  signal(14, (__sighandler_t)handler);
  alarm(0x3Cu);
  return sandbox();
}
