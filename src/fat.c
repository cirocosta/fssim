#include "fssim/fat.h"

fs_fat_t* fs_fat_create(size_t length)
{
  fs_fat_t* fat = malloc(sizeof(*fat));
  PASSERT(fat, FS_ERR_MALLOC);

  fat->length = length;
  fat->blocks = calloc(length, sizeof(*fat->blocks));
  PASSERT(fat->blocks, FS_ERR_MALLOC);

  return fat;
}

void fs_fat_destroy(fs_fat_t* fat)
{
  free(fat->blocks);
  free(fat);
}

/* void fs_fat_removefile(fs_fat_t* fat, uint32_t file_pos); */
/* uint32_t fs_fat_addfile(fs_fat_t* fat); */
/* uint32_t fs_file_addblock(fs_fat_t* fat, uint32_t file_pos); */
