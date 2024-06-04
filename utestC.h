#ifndef UTESTC
#define UTESTC

#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/**
#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_error(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define assertf(A, M, ...) if(!(A)) {log_error(M, ##__VA_ARGS__); assert(A); }

#define assertEq(ACTUAL, EXPECTED, MSG)                                         \
  _Generic((ACTUAL),                                                           \
      int: assertEq_int,                                                   \
      float: assertEq_float,                                               \
      double: assertEq_double,                                             \
      char: assertEq_char,                                                 \
      default: assertEq_ptr)(ACTUAL, EXPECTED, MSG, __func__, __FILE__, __LINE__)
**/

#define assertf(EXPR, MSG) handleAssert(EXPR, MSG, __func__, __FILE__, __LINE__);
#define assertEq_int(ACTUAL, EXPECTED, MSG) assertEqual_int(ACTUAL, EXPECTED, MSG, __func__, __FILE__, __LINE__)
#define assertEq_float(ACTUAL, EXPECTED, MSG) assertEqual_float(ACTUAL, EXPECTED, MSG, __func__, __FILE__, __LINE__)
#define assertEq_double(ACTUAL, EXPECTED, MSG) assertEqual_double(ACTUAL, EXPECTED, MSG, __func__, __FILE__, __LINE__)
#define assertEq_char(ACTUAL, EXPECTED, MSG) assertEqual_char(ACTUAL, EXPECTED, MSG, __func__, __FILE__, __LINE__)
#define assertEq_ptr(ACTUAL, EXPECTED, MSG) assertEqual_ptr(ACTUAL, EXPECTED, MSG, __func__, __FILE__, __LINE__)

#define INITIAL_SIZE 64

#define EPSILON 0.000000001

typedef enum {INT, FLOAT, DOUBLE, CHAR, PTR} type;

typedef void (*testFunction)();

typedef struct {
  char func[32];
  char file[64];
  int line;
  void* expected;
  void* actual;
  type type;
} assertInfo;

typedef struct {
  char msg[64];
  char errn;
  assertInfo* info;
} testLog;

typedef struct {
  testLog* testLogs;
  size_t n;
  size_t size;
} testLogs;

typedef struct {
  testFunction* functions;
  size_t n;
  size_t size;
} testFunctions;

typedef struct {
  char testFile[64];
  testFunctions* testFunctions;
  testLogs* failed;
  testLogs* passed;
} tests;

tests* makeTests(char file[64]);
void freeTests(tests* tests);
testLog* addLogTo(testLogs* testLogs, testLog* testLog);
void printLogs();
void addTestFunc(testFunction);
void runTests();

testLog* handleAssert(const char expr, const char* msg, const char* func, const char* file, const int line);
testLog* assertEqual_int(const int actual, const int expected, const char* msg, const char* func, const char* file, const int line);
testLog* assertEqual_float(const float actual, const float expected, const char* msg, const char* func, const char* file, const int line);
testLog* assertEqual_double(const double actual, const double expected, const char* msg, const char* func, const char* file, const int line);
testLog* assertEqual_char(const char actual, const char expected, const char* msg, const char* func, const char* file, const int line);
testLog* assertEqual_ptr(const void* actual, const void* expected, const char* msg, const char* func, const char* file, const int line);

#endif

