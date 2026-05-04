// attributes: thunk
ssize_t recvfrom(int fd, void *buf, size_t n, int flags, struct sockaddr *addr, socklen_t *addr_len)
{
  return recvfrom(fd, buf, n, flags, addr, addr_len);
}
