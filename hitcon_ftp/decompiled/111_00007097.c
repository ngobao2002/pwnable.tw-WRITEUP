char __fastcall msgpack_object_equal(
        int a1,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        double a7,
        double a8,
        double a9,
        double a10,
        double a11,
        double a12,
        double a13,
        double a14,
        int a15,
        double n,
        char *s1,
        int a18,
        __int64 a19,
        char *s2)
{
  char result; // al
  bool v13; // al
  bool v14; // al
  bool v15; // al
  int v16; // edx
  int v17; // ecx
  int v18; // edx
  int v19; // ecx
  int v20; // edx
  int v21; // ecx
  int v22; // r8d
  int v23; // r9d
  char *v24; // [rsp+0h] [rbp-30h]
  char *v25; // [rsp+8h] [rbp-28h]
  char *v26; // [rsp+10h] [rbp-20h]
  char *v27; // [rsp+18h] [rbp-18h]

  if ( a15 != a18 )
    return 0;
  switch ( a15 )
  {
    case 0:
      result = 1;
      break;
    case 1:
      result = LOBYTE(n) == (unsigned __int8)a19;
      break;
    case 2:
      result = *(_QWORD *)&n == a19;
      break;
    case 3:
      result = *(_QWORD *)&n == a19;
      break;
    case 4:
    case 10:
      result = n == *(double *)&a19;
      break;
    case 5:
      v13 = LODWORD(n) == (_DWORD)a19 && !memcmp(s1, s2, LODWORD(n));
      result = v13;
      break;
    case 6:
      if ( LODWORD(n) == (_DWORD)a19 )
      {
        if ( LODWORD(n) )
        {
          v24 = s1;
          v16 = (int)s1;
          v17 = LODWORD(n);
          v25 = s2;
          do
          {
            if ( (unsigned __int8)msgpack_object_equal(
                                    a1,
                                    a2,
                                    v16,
                                    v17,
                                    a5,
                                    a6,
                                    *(_QWORD *)v24,
                                    *((_QWORD *)v24 + 1),
                                    *((void **)v24 + 2),
                                    *(_QWORD *)v25,
                                    *((_QWORD *)v25 + 1),
                                    *((void **)v25 + 2)) != 1 )
              return 0;
            v24 += 24;
            v25 += 24;
          }
          while ( v24 < &s1[24 * LODWORD(n)] );
          result = 1;
        }
        else
        {
          result = 1;
        }
      }
      else
      {
        result = 0;
      }
      break;
    case 7:
      if ( LODWORD(n) == (_DWORD)a19 )
      {
        if ( LODWORD(n) )
        {
          v26 = s1;
          v18 = (int)s1;
          v19 = LODWORD(n);
          v27 = s2;
          do
          {
            if ( (unsigned __int8)msgpack_object_equal(
                                    a1,
                                    a2,
                                    v18,
                                    v19,
                                    a5,
                                    a6,
                                    *(_QWORD *)v26,
                                    *((_QWORD *)v26 + 1),
                                    *((void **)v26 + 2),
                                    *(_QWORD *)v27,
                                    *((_QWORD *)v27 + 1),
                                    *((void **)v27 + 2)) != 1
              || (unsigned __int8)msgpack_object_equal(
                                    a1,
                                    a2,
                                    v20,
                                    v21,
                                    v22,
                                    v23,
                                    *((_QWORD *)v26 + 3),
                                    *((_QWORD *)v26 + 4),
                                    *((void **)v26 + 5),
                                    *((_QWORD *)v27 + 3),
                                    *((_QWORD *)v27 + 4),
                                    *((void **)v27 + 5)) != 1 )
            {
              return 0;
            }
            v26 += 48;
            v27 += 48;
          }
          while ( v26 < &s1[48 * LODWORD(n)] );
          result = 1;
        }
        else
        {
          result = 1;
        }
      }
      else
      {
        result = 0;
      }
      break;
    case 8:
      v14 = LODWORD(n) == (_DWORD)a19 && !memcmp(s1, s2, LODWORD(n));
      result = v14;
      break;
    case 9:
      v15 = HIDWORD(n) == HIDWORD(a19) && LOBYTE(n) == (_BYTE)a19 && !memcmp(s1, s2, HIDWORD(n));
      result = v15;
      break;
    default:
      result = 0;
      break;
  }
  return result;
}
