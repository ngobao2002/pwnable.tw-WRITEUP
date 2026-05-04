unsigned __int64 sub_F00()
{
  __int16 v1; // [rsp+0h] [rbp-10F0h] BYREF
  char *v2; // [rsp+8h] [rbp-10E8h]
  char v3[208]; // [rsp+10h] [rbp-10E0h] BYREF
  char v4[4104]; // [rsp+E0h] [rbp-1010h] BYREF
  unsigned __int64 v5; // [rsp+10E8h] [rbp-8h]

  v5 = __readfsqword(0x28u);
  memset(v4, 0, sizeof(v4));
  qmemcpy(v3, &unk_36E0, 0xC8uLL);
  sub_32BA(v4, v3, 25LL);
  v1 = 25;
  v2 = v3;
  prctl(38, 1LL, 0LL, 0LL, 0LL);
  prctl(22, 2LL, &v1);
  return __readfsqword(0x28u) ^ v5;
}
