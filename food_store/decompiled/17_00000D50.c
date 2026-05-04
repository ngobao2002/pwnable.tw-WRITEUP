// attributes: thunk
__sighandler_t signal(int sig, __sighandler_t handler)
{
  return __imp_signal(sig, handler);
}
