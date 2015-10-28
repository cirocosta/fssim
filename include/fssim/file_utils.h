#ifndef FSSIM__FILE_UTILS_H
#define FSSIM__FILE_UTILS_H 

#include "fssim/common.h"

inline static int _fexists(const char* fname)
{
  if (!access(fname, F_OK))
    return 1;
  return 0;
}

#endif
