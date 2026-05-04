void __fastcall destroy_chunk_list(__int64 a1)
{
  _QWORD *ptr; // [rsp+10h] [rbp-10h]
  _QWORD *v2; // [rsp+18h] [rbp-8h]

  for ( ptr = *(_QWORD **)(a1 + 16); ; ptr = v2 )
  {
    v2 = (_QWORD *)*ptr;
    free(ptr);
    if ( !v2 )
      break;
  }
}
