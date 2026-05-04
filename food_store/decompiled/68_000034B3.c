__int64 __fastcall sub_34B3(int *a1, const char *a2)
{
  unsigned int v3; // [rsp+1Ch] [rbp-14h]
  int *v4; // [rsp+20h] [rbp-10h]
  int *v5; // [rsp+28h] [rbp-8h]

  v4 = a1 + 2;
  if ( *a1 == 256 )
  {
    fwrite("Too many labels\n", 1uLL, 0x10uLL, stderr);
    exit(1);
  }
  if ( *a1 )
  {
    v5 = &v4[4 * *a1];
    v3 = 0;
    while ( v4 < v5 )
    {
      if ( !strcmp(a2, *(const char **)v4) )
        return v3;
      v4 += 4;
      ++v3;
    }
    *(_QWORD *)v4 = a2;
    v4[2] = -1;
    ++*a1;
    return v3;
  }
  else
  {
    *(_QWORD *)v4 = a2;
    a1[4] = -1;
    ++*a1;
    return 0LL;
  }
}
