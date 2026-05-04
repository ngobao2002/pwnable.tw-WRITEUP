time_t sub_2149()
{
  time_t result; // rax

  qword_205080 = (__int64)realloc(0LL, 0x50uLL);
  if ( !qword_205080 )
    sub_11C0((__int64)"Allocate failed !", 1);
  sub_141B("Egg", 2, 40);
  sub_141B("Pineapple", 0, 217);
  sub_141B("Flouir", 1, 127);
  sub_141B("Suger", 1, 40);
  sub_141B("Apple", 1, 80);
  sub_141B("Beef", 1, 400);
  sub_141B("Salt", 1, 30);
  sub_141B("Pork", 1, 300);
  sub_141B("Scallion", 1, 20);
  result = time(0LL);
  dword_205068 = result;
  return result;
}
