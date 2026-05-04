// attributes: thunk
size_t fwrite(const void *ptr, size_t size, size_t n, FILE *s)
{
  return __imp_fwrite(ptr, size, n, s);
}
