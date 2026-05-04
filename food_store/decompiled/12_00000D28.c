// attributes: thunk
ssize_t read(int fd, void *buf, size_t nbytes)
{
  return __imp_read(fd, buf, nbytes);
}
