#ifndef FSSIM__FAT_H
#define FSSIM__FAT_H 

#include "fssim/common.h"
#include "fssim/constants.h"

typedef struct fs_fat_t {
  long blocks[FS_BLOCKS_NUM];
} fs_fat_t;

fs_fat_t* fs_fat_create();
void fs_fat_destroy(fs_fat_t* fat);

void fs_file_add(int file_num, char* data);

#endif
