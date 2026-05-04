void __fastcall _libc_csu_init(unsigned int a1, __int64 a2, __int64 a3)
{
  signed __int64 v4; // rbp
  __int64 i; // rbx

  v4 = &_do_global_dtors_aux_fini_array_entry - &_frame_dummy_init_array_entry;
  init_proc();
  if ( v4 )
  {
    for ( i = 0LL; i != v4; ++i )
      ((void (__fastcall *)(_QWORD, __int64, __int64))*(&_frame_dummy_init_array_entry + i))(a1, a2, a3);
  }
}
