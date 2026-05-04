// attributes: thunk
void *realloc(void *ptr, size_t size)
{
  return __imp_realloc(ptr, size);
}
