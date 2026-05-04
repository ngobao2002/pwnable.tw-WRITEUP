unsigned __int64 sandbox()
{
  __int16 v1; // [rsp+0h] [rbp-1190h] BYREF
  char *v2; // [rsp+8h] [rbp-1188h]
  char v3[368]; // [rsp+10h] [rbp-1180h] BYREF
  char v4[4104]; // [rsp+180h] [rbp-1010h] BYREF
  unsigned __int64 v5; // [rsp+1188h] [rbp-8h]

  v5 = __readfsqword(0x28u);
  memset(v4, 0, sizeof(v4));
  qmemcpy(v3, &unk_A3C0, 0x168uLL);
  bpf_resolve_jumps(v4, v3, 45LL);
  v1 = 45;
  v2 = v3;
  prctl(38, 1LL, 0LL, 0LL, 0LL);
  prctl(22, 2LL, &v1);
  return __readfsqword(0x28u) ^ v5;
}
