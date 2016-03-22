#ifndef CU_H
#define CU_H

#define run_test(test) do { \
  int success = test(); \
  tests_run++; \
  success && tests_pass++; \
  char *result_string = success ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m"; \
  printf("\x1b[36m[TEST]\x1b[0m %s: %s\n", #test, result_string); \
} while(0)

#define test_result() (tests_run == tests_pass)

int tests_run = 0;
int tests_pass = 0;

#endif
