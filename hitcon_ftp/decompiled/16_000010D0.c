// attributes: thunk
ssize_t sendto(int fd, const void *buf, size_t n, int flags, const struct sockaddr *addr, socklen_t addr_len)
{
  return sendto(fd, buf, n, flags, addr, addr_len);
}
