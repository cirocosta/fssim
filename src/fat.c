#include "fssim/fat.h"

fs_fat_t* fs_fat_create(size_t length)
{
  fs_fat_t* fat = malloc(sizeof(*fat));
  PASSERT(fat, FS_ERR_MALLOC);

  fat->length = length;
  fat->blocks = calloc(fat->length, sizeof(*fat->blocks));
  PASSERT(fat->blocks, FS_ERR_MALLOC);

  while (length-- > 0)
    fat->blocks[length] = length;

  fat->bmp = fs_bmp_create(fat->length);

  return fat;
}

fs_fat_t* fs_fat_load(unsigned char* buf, size_t blocks)
{
  fs_fat_t* fat = malloc(sizeof(*fat));
  PASSERT(fat, FS_ERR_MALLOC);

  size_t i = 0;
  fat->length = blocks;
  fat->blocks = calloc(fat->length, sizeof(*fat->blocks));
  PASSERT(fat->blocks, FS_ERR_MALLOC);

  for (; i < blocks; i++)
    fat->blocks[i] = deserialize_uint32_t(buf + (i * 4));

  fat->bmp = fs_bmp_load(buf + i*4, blocks);

  return fat;
}

void fs_fat_destroy(fs_fat_t* fat)
{
  fs_bmp_destroy(fat->bmp);
  free(fat->blocks);
  free(fat);
}

uint32_t fs_fat_addfile(fs_fat_t* fat)
{
  uint32_t free_block = fs_bmp_alloc(fat->bmp);

  fat->blocks[free_block] = free_block;
  return free_block;
}

uint32_t fs_fat_addblock(fs_fat_t* fat, uint32_t file_pos)
{
  uint32_t free_block = fs_bmp_alloc(fat->bmp);

  while (fat->blocks[file_pos] != file_pos)
    file_pos = fat->blocks[file_pos];

  fat->blocks[file_pos] = free_block;
  fat->blocks[free_block] = free_block;

  return free_block;
}

void fs_fat_removefile(fs_fat_t* fat, uint32_t file_pos)
{
  uint32_t tmp_pos;

  while (1) {
    tmp_pos = file_pos;
    fs_bmp_free(fat->bmp, file_pos);
    file_pos = fat->blocks[file_pos];

    if (fat->blocks[tmp_pos] == tmp_pos)
      return;

    fat->blocks[tmp_pos] = tmp_pos;
  }
}

void fs_fat_serialize(fs_fat_t* fat, unsigned char* buf, int n)
{
  int i = 0;
  
  // TODO verify if this is correct
  ASSERT(n >= (fat->length + fat->bmp->size),
         "`buf` must at least have %lu bytes remaining",
         fat->length + fat->bmp->size);

  for (; i < fat->length; i++)
    serialize_uint32_t(buf + (4 * i), fat->blocks[i]);

  fs_bmp_serialize(fat->bmp, buf + (i * 4), n - fat->length);
}
