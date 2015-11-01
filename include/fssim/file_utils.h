#ifndef FSSIM__FILE_UTILS_H
#define FSSIM__FILE_UTILS_H

#include "fssim/common.h"
#include <sys/time.h>
#include <time.h>

int32_t fs_utils_gettime();
int fs_utils_fsize(FILE* file);
char** fs_utils_splitpath(const char* input, unsigned* size);
int fs_utils_secs2str(int32_t secs, char* buf, int n);
int fs_utils_fsize2str(int32_t secs, char* buf, int n);

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

static inline unsigned char* serialize_uint32_t(unsigned char* buffer,
                                                uint32_t value)
{
  buffer[0] = value >> 24;
  buffer[1] = value >> 16;
  buffer[2] = value >> 8;
  buffer[3] = value;

  return buffer + 4;
}

static inline unsigned char* serialize_int32_t(unsigned char* buffer,
                                               int32_t value)
{
  buffer[0] = value >> 24;
  buffer[1] = value >> 16;
  buffer[2] = value >> 8;
  buffer[3] = value;

  return buffer + 4;
}

static inline unsigned char* serialize_uint8_t(unsigned char* buffer,
                                               uint8_t value)
{
  buffer[0] = value;
  return buffer + 1;
}

static inline uint8_t deserialize_uint8_t(unsigned char* buffer)
{
  uint32_t value = 0;

  value = buffer[0];
  return value;
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

static inline int32_t deserialize_int32_t(unsigned char* buffer)
{
  uint32_t value = 0;

  value |= buffer[0] << 24;
  value |= buffer[1] << 16;
  value |= buffer[2] << 8;
  value |= buffer[3];

  return value;
}

#endif
