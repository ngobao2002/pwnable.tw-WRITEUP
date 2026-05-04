// attributes: thunk
int snprintf(char *s, size_t maxlen, const char *format, ...)
{
  return __imp_snprintf(s, maxlen, format);
}
