#ifndef FSSIM__CONSTANTS_H
#define FSSIM__CONSTANTS_H 

#include "fssim/common.h"
#include <stdint.h>

#define FS_KILOBYTE 1024
#define FS_MEGABYTE 1024*FS_KILOBYTE

#define FS_BLOCK_SIZE 4096
#define FS_PARTITION_SIZE 100*FS_MEGABYTE
#define FS_BLOCKS_NUM FS_PARTITION_SIZE/FS_BLOCK_SIZE

#define FS_ERR_MALLOC "Couldn't allocate memory"

#endif
