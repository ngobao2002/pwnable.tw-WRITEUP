void __fastcall __noreturn main(const char *a1, char **a2, char **a3)
{
  sub_FFC();
  sub_F00();
  sub_1E95(a1, a2);
  sub_316C();
  sub_2149();
  while ( 1 )
  {
    sub_1DF4();
    switch ( sub_1134(a1, a2) )
    {
      case 1LL:
        sub_1CA9(a1, (__int64)a2);
        break;
      case 2LL:
        sub_2B62();
        break;
      case 3LL:
        sub_1D5A();
        break;
      case 4LL:
        sub_30E3(a1, (__int64)a2);
        break;
      case 5LL:
        sub_2259();
        break;
      case 6LL:
        sub_2A12();
        break;
      case 7LL:
        if ( stream )
          fclose(stream);
        exit(0);
      default:
        continue;
    }
  }
}
