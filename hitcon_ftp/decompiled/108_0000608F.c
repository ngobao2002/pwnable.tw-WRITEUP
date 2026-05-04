int __fastcall msgpack_object_print(
        FILE *a1,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        unsigned int a7,
        size_t size,
        char *ptr)
{
  int result; // eax
  int v10; // r8d
  int v11; // r9d
  int v12; // ecx
  int v13; // r8d
  int v14; // r9d
  int v15; // r8d
  int v16; // r9d
  int v17; // ecx
  int v18; // r8d
  int v19; // r9d
  int v20; // ecx
  int v21; // r8d
  int v22; // r9d
  int v23; // ecx
  int v24; // r8d
  int v25; // r9d
  char *i; // [rsp+10h] [rbp-20h]
  char *j; // [rsp+18h] [rbp-18h]

  switch ( a7 )
  {
    case 0u:
      result = fwrite("nil", 1uLL, 3uLL, a1);
      break;
    case 1u:
      if ( (_BYTE)size )
        result = fprintf(a1, "true");
      else
        result = fprintf(a1, "false");
      break;
    case 2u:
      result = fprintf(a1, "%lu", size);
      break;
    case 3u:
      result = fprintf(a1, "%li", size);
      break;
    case 4u:
    case 0xAu:
      result = fprintf(a1, "%f", *(double *)&size);
      break;
    case 5u:
      fputc(34, a1);
      fwrite(ptr, (unsigned int)size, 1uLL, a1);
      result = fputc(34, a1);
      break;
    case 6u:
      fputc(91, a1);
      if ( (_DWORD)size )
      {
        msgpack_object_print(
          (int)a1,
          (int)a1,
          (int)a1,
          size,
          v10,
          v11,
          *(_QWORD *)ptr,
          *((_QWORD *)ptr + 1),
          *((void **)ptr + 2));
        for ( i = ptr + 24; i < &ptr[24 * (unsigned int)size]; i += 24 )
        {
          fwrite(", ", 1uLL, 2uLL, a1);
          msgpack_object_print((int)a1, 1, (int)a1, v12, v13, v14, *(_QWORD *)i, *((_QWORD *)i + 1), *((void **)i + 2));
        }
      }
      result = fputc(93, a1);
      break;
    case 7u:
      fputc(123, a1);
      if ( (_DWORD)size )
      {
        msgpack_object_print(
          (int)a1,
          (int)a1,
          (int)a1,
          size,
          v15,
          v16,
          *(_QWORD *)ptr,
          *((_QWORD *)ptr + 1),
          *((void **)ptr + 2));
        fwrite("=>", 1uLL, 2uLL, a1);
        msgpack_object_print(
          (int)a1,
          1,
          (int)a1,
          v17,
          v18,
          v19,
          *((_QWORD *)ptr + 3),
          *((_QWORD *)ptr + 4),
          *((void **)ptr + 5));
        for ( j = ptr + 48; j < &ptr[48 * (unsigned int)size]; j += 48 )
        {
          fwrite(", ", 1uLL, 2uLL, a1);
          msgpack_object_print((int)a1, 1, (int)a1, v20, v21, v22, *(_QWORD *)j, *((_QWORD *)j + 1), *((void **)j + 2));
          fwrite("=>", 1uLL, 2uLL, a1);
          msgpack_object_print(
            (int)a1,
            1,
            (int)a1,
            v23,
            v24,
            v25,
            *((_QWORD *)j + 3),
            *((_QWORD *)j + 4),
            *((void **)j + 5));
        }
      }
      result = fputc(125, a1);
      break;
    case 8u:
      fputc(34, a1);
      msgpack_object_bin_print(a1, (__int64)ptr, (unsigned int)size);
      result = fputc(34, a1);
      break;
    case 9u:
      fprintf(a1, "(ext: %i)", (unsigned int)(char)size);
      fputc(34, a1);
      msgpack_object_bin_print(a1, (__int64)ptr, HIDWORD(size));
      result = fputc(34, a1);
      break;
    default:
      result = fprintf(a1, "#<UNKNOWN %i %lu>", a7, size);
      break;
  }
  return result;
}
