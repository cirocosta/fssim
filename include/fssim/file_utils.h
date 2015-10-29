#ifndef FSSIM__FILE_UTILS_H
#define FSSIM__FILE_UTILS_H

#include "fssim/common.h"

static int fs_utils_fexists(const char* fname)
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

static inline unsigned char* serialize_uint32_t(unsigned char* buffer,
                                                uint32_t value)
{
  buffer[0] = value >> 24;
  buffer[1] = value >> 16;
  buffer[2] = value >> 8;
  buffer[3] = value;

  return buffer + 4;
}

static inline uint32_t deserialize_uint32_t(unsigned char* buffer)
{
  uint32_t value = 0;

  value |= buffer[0] << 24;
  value |= buffer[1] << 16;
  value |= buffer[2] << 8;
  value |= buffer[3];

  return value;
}

#endif
