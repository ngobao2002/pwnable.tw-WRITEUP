unsigned __int64 __fastcall sub_30E3(const char *a1, __int64 a2)
{
  unsigned __int64 result; // rax

  while ( 1 )
  {
    while ( 1 )
    {
      while ( 1 )
      {
        while ( 1 )
        {
          sub_26C7();
          result = sub_1134(a1, a2);
          if ( result != 2 )
            break;
          sub_2744();
        }
        if ( result > 2 )
          break;
        if ( result != 1 )
          goto LABEL_12;
        sub_2F40((__int64)a1, a2);
      }
      if ( result != 3 )
        break;
      if ( (unsigned int)dword_2050A8 > 3 )
        sub_2895();
    }
    if ( result == 4 )
      return result;
LABEL_12:
    a1 = "Invalid Choice";
    puts("Invalid Choice");
  }
}
