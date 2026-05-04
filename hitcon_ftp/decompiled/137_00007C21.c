unsigned __int64 __fastcall template_init(__int64 a1)
{
  _QWORD *v1; // rbx
  __int64 v2; // rdx
  __int64 v4[5]; // [rsp+0h] [rbp-50h] BYREF
  __int64 v5; // [rsp+28h] [rbp-28h]
  unsigned __int64 v6; // [rsp+38h] [rbp-18h]

  v5 = a1;
  v6 = __readfsqword(0x28u);
  *(_DWORD *)(a1 + 16) = 0;
  *(_DWORD *)(v5 + 20) = 0;
  *(_DWORD *)(v5 + 24) = 0;
  v1 = (_QWORD *)v5;
  template_callback_root(v4);
  v2 = v4[1];
  v1[4] = v4[0];
  v1[5] = v2;
  v1[6] = v4[2];
  return __readfsqword(0x28u) ^ v6;
}
