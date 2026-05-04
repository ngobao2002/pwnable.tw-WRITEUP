unsigned __int64 __fastcall sub_1CA9(const char *a1, __int64 a2)
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
          sub_16A4();
          result = sub_1134(a1, a2);
          if ( result != 2 )
            break;
          if ( (unsigned int)dword_2050A8 <= 2 )
            goto LABEL_9;
          sub_1B90();
        }
        if ( result > 2 )
          break;
        if ( result != 1 )
          goto LABEL_14;
        if ( (unsigned int)dword_2050A8 > 1 )
        {
          sub_17DC();
        }
        else
        {
LABEL_9:
          a1 = "Your level is too low !";
          puts("Your level is too low !");
        }
      }
      if ( result != 3 )
        break;
      a1 = (_BYTE *)(&dword_0 + 1);
      sub_1A9B(1);
    }
    if ( result == 4 )
      return result;
LABEL_14:
    a1 = "Invalid Choice";
    puts("Invalid Choice");
  }
}
