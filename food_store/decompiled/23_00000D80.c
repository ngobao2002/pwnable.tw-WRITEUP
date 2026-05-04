// attributes: thunk
int setvbuf(FILE *stream, char *buf, int modes, size_t n)
{
  return __imp_setvbuf(stream, buf, modes, n);
}
