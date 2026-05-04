__int64 __fastcall msgpack_unpacker_execute(_QWORD *a1)
{
  unsigned int v2; // [rsp+14h] [rbp-Ch]
  unsigned __int64 v3; // [rsp+18h] [rbp-8h]

  v3 = a1[3];
  v2 = template_execute(a1[7], *a1, a1[1], a1 + 3);
  if ( v3 < a1[3] )
    a1[4] += a1[3] - v3;
  return v2;
}
