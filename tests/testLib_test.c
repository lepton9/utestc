#include "../utestC.h"
#include <stdlib.h>


void test_assertEqInt() {
  assertEq_int(4, 4, "Should be true");
  assertEq_int(5, 4, "Should not be true");
}
void test_assertEqFloat() {
  assertEq_float(4.0, 4.0, "Should be true");
  assertEq_float(5.0, 4.0, "Should not be true");
}
void test_assertEqDouble() {
  assertEq_float(4.0, 4.0, "Should be true");
  assertEq_float(5.0, 4.0, "Should not be true");
}
void test_assertEqChar() {
  assertEq_char('g', 'g', "Should be true");
  assertEq_char('f', 'g', "Should not be true");
}
void test_assertEqPtr() {
  int* a = malloc(sizeof(int));
  int* b = malloc(sizeof(int));
  assertEq_ptr(a, a, "Should be true");
  assertEq_ptr(a, b, "Should not be true");
}


int main(int argc, char** argv) {
  makeTests(argv[0]);
  addTestFunc(test_assertEqInt);
  addTestFunc(test_assertEqFloat);
  addTestFunc(test_assertEqDouble);
  addTestFunc(test_assertEqChar);
  addTestFunc(test_assertEqPtr);
  runTests();
  return 0;
}

