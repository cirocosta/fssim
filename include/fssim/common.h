#ifndef FSSIM_COMMON_H
#define FSSIM_COMMON_H

#define KGRN "\x1B[32m"
#define KBRED "\x1B[31;1m"
#define KRESET "\033[0m"

#define CHECK_RBIT(__var, __pos) ((__var) & (1 << (__pos)))
#define CHECK_LBIT(__var, __pos) ((__var) & (128 >> (__pos)))
#define SET_LBIT(__var, __pos)                                                 \
  do {                                                                         \
    __var ^= (128 >> __pos);                                                   \
  } while (0);
#define SET_RBIT(__var, __pos)                                                 \
  do {                                                                         \
    __var ^= (1 << __pos);                                                     \
  } while (0);

#include "fssim/constants.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <valgrind/valgrind.h>

#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define FREE(__ptr)                                                            \
  do {                                                                         \
    free(__ptr);                                                               \
    __ptr = 0;                                                                 \
  } while (0)

#define FREE_ARR(__ptr, __count)                                               \
  do {                                                                         \
    unsigned __i = 0;                                                          \
    while (__i < __count)                                                      \
      free(__ptr[__i++]);                                                      \
    free(__ptr);                                                               \
    __ptr = 0;                                                                 \
  } while (0)

#define LOG(__msg, ...)                                                        \
  do {                                                                         \
    fprintf(stdout, "\n(LOG)\t");                                              \
    fprintf(stdout, __msg, ##__VA_ARGS__);                                     \
  } while (0)

#define LOGERR(__msg, ...)                                                     \
  do {                                                                         \
    fprintf(stderr, "\n(LOGERR)\t");                                           \
    fprintf(stderr, __msg, ##__VA_ARGS__);                                     \
  } while (0)

#define ASSERT(__cond, __msg, ...)                                             \
  do {                                                                         \
    if (!(__cond)) {                                                           \
      VALGRIND_PRINTF_BACKTRACE("\nStack trace @ %s(), %d", __func__,          \
                                __LINE__);                                     \
      fprintf(stderr, "\n" __BASE_FILE__ ": %2d\n", __LINE__);                 \
      fprintf(stderr, KBRED "Assertion `%s` failed\n" KRESET, #__cond);        \
      fprintf(stderr, "\t" __msg "\n", ##__VA_ARGS__);                         \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

#define PASSERT(condition, message, ...)                                       \
  do {                                                                         \
    if (!(condition)) {                                                        \
      VALGRIND_PRINTF_BACKTRACE("\nStack trace @ %s(), %d", __func__,          \
                                __LINE__);                                     \
      fprintf(stderr, "\n" __BASE_FILE__ ": %2d\n", __LINE__);                 \
      fprintf(stderr, message, ##__VA_ARGS__);                                 \
      fprintf(stderr, "%s\n", strerror(errno));                                \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

#define TEST(__test, ...)                                                      \
  do {                                                                         \
    LOG("RUN\t%s: %s:" #__VA_ARGS__, __FILENAME__, #__test);                   \
    fflush(stdout);                                                            \
    __test();                                                                  \
    LOG("OK\t\t%s: %s%s%s:" #__VA_ARGS__, __FILENAME__, KGRN, #__test,         \
        KRESET);                                                               \
    fflush(stdout);                                                            \
  } while (0)

#define STRNCMP(__a, __b)                                                      \
  ASSERT(!strncmp(__a, __b, strlen(__b)), "`%s` != `%s`", __a, __b)

#ifndef NDEBUG
#define DASSERT(__cond, __msg, ...) ASSERT(__cond, __msg, #__VA_ARGS__)
#else
#define DASSERT(__cond, __msg, ...)                                            \
  do {                                                                         \
  } while (0)
#endif

#endif // ! FSSIM_COMMON_H
