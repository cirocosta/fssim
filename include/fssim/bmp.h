#ifndef FSSIM__BMP_H
#define FSSIM__BMP_H

#include "fssim/common.h"
#include "fssim/file_utils.h"

typedef struct fs_bmp_t {
  size_t size;
  size_t num_blocks;
  uint32_t last_block;
  uint8_t* mapping;
} fs_bmp_t;

/**
 * Creates a bitmap that maps <size> blocks.
 */
fs_bmp_t* fs_bmp_create(size_t size);

/**
 * Destroys the bitmap
 */
void fs_bmp_destroy(fs_bmp_t* bmp);

/**
 * Frees the given space.
 */
void fs_bmp_free(fs_bmp_t* bmp, uint32_t block);

/**
 * Searches for free space  w/ a next-fit
 * strategy, sets the bit (now used) and returns
 * the block ref
 */
uint32_t fs_bmp_alloc(fs_bmp_t* bmp);

fs_bmp_t* fs_bmp_load(unsigned char* buf, size_t blocks);

// TODO
void fs_bmp_serialize(fs_bmp_t* bmp, unsigned char* buf, int n);

#define FS_BMP_IS_ON_(__bmp, __pos)                                            \
  (CHECK_LBIT(__bmp->mapping[(__pos / 8)], (__pos % 8)))

#define FS_BMP_FLIP_(__bmp, __pos)                                             \
  do {                                                                         \
    SET_LBIT(__bmp->mapping[(__pos / 8)], (__pos % 8));                        \
  } while (0);

#endif
