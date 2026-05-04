_QWORD *__fastcall template_data(_QWORD *a1, _QWORD *a2)
{
  __int64 v2; // rdx

  v2 = a2[5];
  *a1 = a2[4];
  a1[1] = v2;
  a1[2] = a2[6];
  return a1;
}
