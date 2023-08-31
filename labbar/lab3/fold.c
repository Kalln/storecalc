#include <stdio.h>


typedef int(*int_fold_func)(int, int);


int foldl_int_int(int numbers[], int numbers_siz, int_fold_func f) {
  int result = 0;

  // Loopa över arrayen och för varje element e utför result = f(result, e)
  for (int i = 0; i < numbers_siz; ++i) {
    result = f(result, numbers[i]);
  }

  return result;
}


long sum(int numbers[], int numbers_siz) {
  long result = 0;

  for (int i = 0; i < numbers_siz; ++i) {
    result += numbers[i];
  }

  return result;
}

int main(void) {
    int s[4];
    s[0] = 3;
    s[1] = 4;
    s[2] = 5;
    s[3] = 10;
    long res = sum(s, 4);
    printf("%ld", res);
    return 0;
}