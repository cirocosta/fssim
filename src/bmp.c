#include "fssim/bmp.h"

fs_bmp_t* fs_bmp_create(size_t size)
{
  ASSERT(size, "Size must be at least > 0");

  fs_bmp_t* bmp = malloc(sizeof(*bmp));
  PASSERT(bmp, FS_ERR_MALLOC);

  bmp->num_blocks = size;
  bmp->size = ((size - 1) / 8 | 0) + 1;
  bmp->mapping = calloc(bmp->size, sizeof(*bmp->mapping));
  PASSERT(bmp->mapping, FS_ERR_MALLOC);

  memset(bmp->mapping, 0x00, bmp->size);

  return bmp;
}

void fs_bmp_destroy(fs_bmp_t* bmp)
{
  free(bmp->mapping);
  free(bmp);
}

void fs_bmp_free(fs_bmp_t* bmp, uint32_t block) {}

uint32_t fs_bmp_alloc(fs_bmp_t* bmp) { return 1; }

int fs_bmp_IS_ON_(fs_bmp_t* bmp, int pos)
{
  int offset = pos % 8;
  int index = pos / 8 | 0;

  ASSERT(index < bmp->size, "BMP bounds error: %d is not a valid position",
         pos);

  LOGERR("offset = %d, index = %d", offset, index);

  return CHECK_LBIT(bmp->mapping[index], offset);
}


void fs_bmp_FLIP_(fs_bmp_t* bmp, int pos)
{
  int offset = pos % 8;
  int index = pos / 8 | 0;

  SET_LBIT(bmp->mapping[index], offset);
}

