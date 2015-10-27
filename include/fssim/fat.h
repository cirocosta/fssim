#ifndef FSSIM__FAT_H
#define FSSIM__FAT_H 

#include "fssim/common.h"
#include "fssim/constants.h"
#include "fssim/bmp.h"

/**
 * FAT - File Allocation Table
 *
 * phys  entry
 * ----  -----
 *  0      0    // end
 *  1      2    // points to 2
 *  x      1    // points to 1
 *  2      2    // end
 *
 *  say file->block = x. Then x->1->2 corresponds
 *  to the physical blocks of the file.
 */

typedef struct fs_fat_t {
  size_t length;
  uint32_t* blocks;
  fs_bmp_t* bmp;
} fs_fat_t;

fs_fat_t* fs_fat_create(size_t length);
void fs_fat_destroy(fs_fat_t* fat);

void fs_fat_removefile(fs_fat_t* fat, uint32_t file_pos);
uint32_t fs_fat_addfile(fs_fat_t* fat);
uint32_t fs_file_addblock(fs_fat_t* fat, uint32_t file_pos);

#endif
