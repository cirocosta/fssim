#ifndef FSSIM__FILE_UTILS_H
#define FSSIM__FILE_UTILS_H

#include "fssim/common.h"

inline static int fs_utils_fexists(const char* fname)
{
  return !access(fname, F_OK) ? 1 : 0;
}

inline static void fs_utils_fdelete(const char* fname)
{
  if (fs_utils_fexists(fname)) {
    PASSERT(!unlink(fname), "fdelete:");
  }
}

int fs_utils_fsize(FILE* file);
char** fs_utils_splitpath(const char* input, unsigned* size);

#endif
