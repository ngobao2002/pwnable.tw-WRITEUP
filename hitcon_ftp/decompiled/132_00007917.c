__int64 __fastcall template_callback_map(unsigned __int64 ***a1, unsigned int a2, __int64 a3)
{
  unsigned __int64 **v3; // rbx

  *(_DWORD *)a3 = 7;
  *(_DWORD *)(a3 + 8) = 0;
  if ( !**a1 )
  {
    v3 = *a1;
    *v3 = (unsigned __int64 *)msgpack_zone_new(0x2000LL);
    if ( !**a1 )
      return 4294967294LL;
  }
  *(_QWORD *)(a3 + 16) = msgpack_zone_malloc(**a1, 48LL * a2);
  if ( *(_QWORD *)(a3 + 16) )
    return 0LL;
  else
    return 4294967294LL;
}
