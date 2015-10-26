#ifndef FSSIM__BMP_H
#define FSSIM__BMP_H 

#include "fssim/common.h"

/**
 * BITMAP
 *
 * bit(0) --> free
 * bit(1) --> occupied
 */

typedef struct fs_bmp_t {
  size_t size;
  size_t num_blocks;
  uint8_t* mapping;
} fs_bmp_t;

fs_bmp_t* fs_bmp_create(size_t size);
void fs_bmp_destroy(fs_bmp_t* bmp);

void fs_bmp_free(fs_bmp_t* bmp, uint32_t block);
uint32_t fs_bmp_alloc(fs_bmp_t* bmp);

#endif
