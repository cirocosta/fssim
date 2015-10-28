#ifndef FSSIM__FILE_UTILS_H
#define FSSIM__FILE_UTILS_H

#include "fssim/common.h"

inline static int fs_utils_fexists(const char* fname)
{
  if (!access(fname, F_OK))
    return 1;
  return 0;
}

inline static int fs_utils_fsize(FILE* file)
{
  int size;
  int last_pos;

  PASSERT(~fseek(file, 0, SEEK_SET), "");
  last_pos = ftell(file);
  PASSERT(~fseek(file, 0, SEEK_END), "");
  size = ftell(file);
  PASSERT(~fseek(file, last_pos, SEEK_SET), "");

  return size;
}

inline static void fs_utils_fdelete(const char* fname)
{
  if (fs_utils_fexists(fname)) {
    PASSERT(!unlink(fname), "fdelete:");
  }
}

#endif
