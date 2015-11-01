#ifndef FSSIM__CONSTANTS_H
#define FSSIM__CONSTANTS_H

#include "fssim/common.h"
#include <stdint.h>

#define FS_KILOBYTE 1024
#define FS_MEGABYTE 1024 * FS_KILOBYTE

#define FS_NAME_MAX 11

#define FS_BLOCK_SIZE 4096
#define FS_PARTITION_SIZE 100 * FS_MEGABYTE
#define FS_BLOCKS_NUM FS_PARTITION_SIZE / FS_BLOCK_SIZE

#define FS_ERR_MALLOC "Couldn't allocate memory"

// 16 characters excluding null
// terminating byte. Eg:  2015-11-01 13:22
#define FS_DATE_FORMAT "%4Y-%2m-%2d %2H:%2M"
#define FS_DATE_FORMAT_SIZE 17

#define FS_FILESIZE_FORMAT "%3.1f%2s"
#define FS_FILESIZE_FORMAT_SIZE 8

static const char* FS_FILESIZE_UNITS[] = { "B", "KB", "MB", "GB" };

typedef enum fs_file_type {
  FS_FILE_DIRECTORY = 1,
  FS_FILE_REGULAR,
  FS_FILE_ROOT_DIR
} fs_file_type;

#endif
