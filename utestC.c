#include "testLib.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

static tests* allTests;

tests* makeTests(char file[32]) {
  tests* t = (tests*)malloc(sizeof(tests));

  t->testFunctions = (testFunctions*)malloc(sizeof(testFunctions));
  t->testFunctions->functions = malloc(INITIAL_SIZE * sizeof(testFunction));
  t->testFunctions->size = INITIAL_SIZE;
  t->testFunctions->n = 0;

  t->failed = (testLogs*)malloc(sizeof(testLogs));
  t->failed->testLogs = malloc(INITIAL_SIZE * sizeof(testLog));
  t->passed = (testLogs*)malloc(sizeof(testLogs));
  t->passed->testLogs = malloc(INITIAL_SIZE * sizeof(testLog));

  t->failed->size = INITIAL_SIZE;
  t->failed->n = 0;
  t->passed->size = INITIAL_SIZE;
  t->passed->n = 0;

  strcpy(t->testFile, file); 
  allTests = t;
  return t;
}

void freeLog(testLog* l) {
  if (l->info->expected && l->info->type != PTR) free(l->info->expected);
  if (l->info->actual&& l->info->type != PTR) free(l->info->actual);
  free(l->info);
  // free(l);
}

void freeTests(tests* tests) {
  for (int i = 0; i < tests->passed->n; i++) {
    freeLog(&tests->passed->testLogs[i]);
  }
  for (int i = 0; i < tests->failed->n; i++) {
    freeLog(&tests->failed->testLogs[i]);
  }
  free(tests->passed);
  free(tests->failed);
  free(tests->testFunctions);
  free(tests);
}

testLog* addLogTo(testLogs* testLogs, testLog* l) {
  if (testLogs->n == testLogs->size) {
    testLogs->size *= 2;
    testLogs->testLogs = realloc(testLogs->testLogs, testLogs->size * sizeof(testLog));
    if (testLogs->testLogs == NULL) {
      perror("Realloc for testLogs failed");
      exit(EXIT_FAILURE);
    }
  }
  testLogs->testLogs[testLogs->n++] = *l;
  return &testLogs->testLogs[testLogs->n - 1];
}

testLog* handleAssert(const char expr, const char* errMsg, const char* func, const char* file, const int line) {
  testLog l;
  strcpy(l.msg, errMsg);
  l.errn = errno;
  l.info = malloc(sizeof(assertInfo));
  strncpy(l.info->func, func, sizeof(l.info->func)/sizeof(char));
  strncpy(l.info->file, file, sizeof(l.info->file)/sizeof(char));
  l.info->line = line;

  if (expr) return addLogTo(allTests->passed, &l);
  else return addLogTo(allTests->failed, &l);
}

testLog* assertEqual_int(const int actual, const int expected, const char* msg, const char* func, const char* file, const int line) {
  char eq = actual == expected;
  testLog* l = handleAssert(eq, msg, func, file, line);
  l->info->type = INT;
  l->info->actual= (int*)malloc(sizeof(int));
  l->info->expected = (int*)malloc(sizeof(int));
  memcpy(l->info, &actual, sizeof(int));
  memcpy(l->info, &expected, sizeof(int));
  if (!eq) sprintf(l->msg + strlen(l->msg), ", %d != %d", actual, expected);
  return l;
}
testLog* assertEqual_float(const float actual, const float expected, const char* msg, const char* func, const char* file, const int line) {
  char eq = fabs(actual - expected) < EPSILON;
  testLog* l = handleAssert(eq, msg, func, file, line);
  l->info->type = FLOAT;
  l->info->actual= (float*)malloc(sizeof(float));
  l->info->expected = (float*)malloc(sizeof(float));
  memcpy(l->info->actual, &actual, sizeof(float));
  memcpy(l->info->expected, &expected, sizeof(float));
  if (!eq) sprintf(l->msg + strlen(l->msg), ", %f != %f", actual, expected);
  return l;
}
testLog* assertEqual_double(const double actual, const double expected, const char* msg, const char* func, const char* file, const int line) {
  char eq = fabs(actual - expected) < EPSILON;
  testLog* l = handleAssert(eq, msg, func, file, line);
  l->info->type = DOUBLE;
  l->info->actual= (double*)malloc(sizeof(double));
  l->info->expected = (double*)malloc(sizeof(double));
  memcpy(l->info->actual, &actual, sizeof(double));
  memcpy(l->info->expected, &expected, sizeof(double));
  if (!eq) sprintf(l->msg + strlen(l->msg), ", %f != %f", actual, expected);
  return l;
}
testLog* assertEqual_char(const char actual, const char expected, const char* msg, const char* func, const char* file, const int line) {
  char eq = actual == expected;
  testLog* l = handleAssert(eq, msg, func, file, line);
  l->info->type = CHAR;
  l->info->actual= (char*)malloc(sizeof(char));
  l->info->expected = (char*)malloc(sizeof(char));
  memcpy(l->info->actual, &actual, sizeof(char));
  memcpy(l->info->expected, &expected, sizeof(char));
  if (!eq) sprintf(l->msg + strlen(l->msg), ", '%c' != '%c'", actual, expected);
  return l;
}
testLog* assertEqual_ptr(const void* actual, const void* expected, const char* msg, const char* func, const char* file, const int line) {
  char eq = actual == expected;
  testLog* l = handleAssert(eq, msg, func, file, line);
  l->info->type = PTR;
  l->info->actual= (void*)actual;
  l->info->expected = (void*)expected;
  if (!eq) sprintf(l->msg + strlen(l->msg), ", %p != %p", actual, expected);
  return l;
}


void addTestFunc(testFunction tf) {
  if (allTests->testFunctions->n == allTests->testFunctions->size) {
    allTests->testFunctions->size *= 2;
    allTests->testFunctions->functions = realloc(allTests->testFunctions->functions, allTests->testFunctions->size * sizeof(testFunction));
    if (allTests->testFunctions->functions == NULL) {
      perror("Realloc for testFunctions failed");
      exit(EXIT_FAILURE);
    }
  }
  allTests->testFunctions->functions[allTests->testFunctions->n++] = tf;
}

void printtestLogs() {
  printf("\n");
  printf("Testing %s\n", allTests->testFile);
  if (allTests->passed->n > 0) printf(" \033[42;30mPASSED\033[0m: %d ", (int)allTests->passed->n);
  if (allTests->failed->n > 0) printf(" \033[41;30mFAILED\033[0m: %d ", (int)allTests->failed->n);
  printf("\n");

  //printf("Tests \033[42;30mPASSED\033[0m: %d \033[41;30mFAILED\033[0m: %d\n", (int)allTests->passed->n, (int)allTests->failed->n);

  for (int i = 0; i < allTests->failed->n; ++i) {
    testLog l = allTests->failed->testLogs[i];
    fprintf(stderr, "\n  \033[41;30m[ERROR]\033[0m %s\n", l.msg);
    fprintf(stderr, "          %s() in %s, line: %d\n", l.info->func, l.info->file, l.info->line);
  }
}

void runTests() {
  for (int i = 0; i < allTests->testFunctions->n; i++) {
    // Print the function name being tested at the moment
    allTests->testFunctions->functions[i] ();
  }
  printtestLogs();
  freeTests(allTests);
}

