#include "fssim/filesystem.h"
#include <time.h>

#define FS_FNAME "/tmp/fssim-exp-0mb"
#define FNAME_1MB "/tmp/fs-1MB"
#define FNAME_10MB "/tmp/fs-10MB"
#define FNAME_30MB "/tmp/fs-30MB"
#define FNAME_50MB "/tmp/fs-50MB"

static const char* HELP =
    "USAGE:\n"
    "   $ ./experiments <state>\n"
    "\n"
    "STATES\n"
    "   1.    Empty filesystem\n"
    "   2.    Filesystem w/ 10MB in use\n"
    "   3.    Filesystem w/ 50MB in use\n"
    "\n"
    "EXPERIMENTS\n"
    "   1.    Copy of a single 1MB file into '/'\n"
    "   2.    Copy of a single 10MB file into '/'\n"
    "   3.    Copy of a single 30MB file into '/'\n"
    "   4.    Removal of a single 1MB file into '/'\n"
    "   5.    Removal of a single 10MB file into '/'\n"
    "   6.    Removal of a single 30MB file into '/'\n"
    "   7.    Removal of an entire 30-level deep tree of\n"
    "         directories consisting of a single directory\n"
    "   8.    Removal of an entire 30-level deep tree of\n"
    "         directories consisting of a lots and lots of files\n"
    "\n"
    "OUTPUT\n"
    "   The ouput consists of a CSV w/out header:\n"
    "     <exp_name>,<exp_time>\n"
    "\n"
    "EXAMPLE:\n"
    "   $ ./experiments 2\n";

fs_filesystem_t* g_fs = NULL;
clock_t g_start;
clock_t g_end;
int g_state;

#define TICK()                                                                 \
  do {                                                                         \
    g_start = clock();                                                         \
  } while (0)

#define TOCK()                                                                 \
  do {                                                                         \
    g_end = clock();                                                           \
    fprintf(stderr, "%s,%f\n", __func__,                                       \
            (g_end - g_start) / (double)CLOCKS_PER_SEC);                       \
  } while (0)

void SETUP()
{
  fs_utils_fdelete(FS_FNAME);
  g_fs = fs_filesystem_create(FS_BLOCKS_NUM);
  fs_filesystem_mount(g_fs, FS_FNAME);
}

void TEAR_DOWN() { fs_filesystem_destroy(g_fs); }

void e1()
{
  SETUP();
  TICK();

  fs_filesystem_cp(g_fs, FNAME_10MB, "/fs-1MB");

  TOCK();
  TEAR_DOWN();
}

void e2()
{
  SETUP();
  TICK();

  fs_filesystem_cp(g_fs, FNAME_10MB, "/fs-10MB");

  TOCK();
  TEAR_DOWN();
}

void e3()
{
  SETUP();
  TICK();

  fs_filesystem_cp(g_fs, FNAME_30MB, "/fs-30MB");

  TOCK();
  TEAR_DOWN();
}

void e4()
{
  SETUP();
  fs_filesystem_cp(g_fs, FNAME_1MB, "/fs-1MB");

  TICK();

  fs_filesystem_rm(g_fs, "/fs-1MB");

  TOCK();
  TEAR_DOWN();
}

void e5()
{
  SETUP();
  fs_filesystem_cp(g_fs, FNAME_1MB, "/fs-10MB");

  TICK();

  fs_filesystem_rm(g_fs, "/fs-10MB");

  TOCK();
  TEAR_DOWN();
}

void e6()
{
  SETUP();
  fs_filesystem_cp(g_fs, FNAME_1MB, "/fs-30MB");

  TICK();

  fs_filesystem_rm(g_fs, "/fs-30MB");

  TOCK();
  TEAR_DOWN();
}

int main(int argc, char* argv[])
{
  if (argc < 2) {
    fprintf(stderr, "%s", HELP);
    exit(1);
  }

  switch (argv[1][0]) {
    case '1':
      g_state = 1;
      break;
    case '2':
      g_state = 2;
      break;
    case '3':
      g_state = 3;
      break;
    default:
      fprintf(stderr, "%s", HELP);
      fprintf(stderr, "\nERROR: Unknown state %c", argv[1][0]);
      exit(1);
  }

  e1();
  e2();
  e3();
  e4();
  e5();
  e6();
  /* e7(); */
  /* e8(); */

  return 0;
}
