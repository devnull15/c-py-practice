#include <stdio.h>
#include <assert.h>

void pass_by_ref(int timer, int* val) {
  assert(val != NULL);
  *val = timer%2;
  return;
}

int dereference(int* val) {
  assert(val != NULL);
  return *val;
}

char get_char(void) {
  return getchar();
}

int fib_recursive(int n) {
  if(n<=1) { return n; }
  return fib_recursive(n-1) + fib_recursive(n-2);
}

int call_my_function(int (*function)(void)) {
  assert(function != NULL);
  return (*function)();
}
