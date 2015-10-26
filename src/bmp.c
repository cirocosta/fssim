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

