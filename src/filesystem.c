#include "fssim/filesystem.h"

fs_filesystem_t* fs_filesystem_create(size_t blocks)
{
  fs_filesystem_t* fs = malloc(sizeof(*fs));
  PASSERT(fs, FS_ERR_MALLOC);

  fs->blocks_num = blocks;
  fs->fat = NULL;
  fs->root = NULL;
  fs->cwd = NULL;
  fs->file = NULL;

  return fs;
}

static FILE* _create_system_file(const char* fname)
{
  FILE* file;

  if (!fs_utils_fexists(fname)) {
    PASSERT(file = fopen(fname, "w+"), "fopen(w+) failed:");
    return file;
  }

  PASSERT(file = fopen(fname, "r+"), "fopen(r+) failed:");

  return file;
}

void fs_filesystem_destroy(fs_filesystem_t* fs)
{
  if (fs->fat) {
    fs_fat_destroy(fs->fat);
  }

  if (fs->file) {
    PASSERT(fclose(fs->file) == 0, "fclose error:");
    fs->file = NULL;
  }

  if (fs->root) {
    fs_file_destroy(fs->root);
  }

  free(fs);
}

void fs_filesystem_mount(fs_filesystem_t* fs, const char* fname)
{
  fs_file_t* parent = NULL;
  fs->file = _create_system_file(fname);
  
  // TODO read from existing file
  fs->fat = fs_fat_create(fs->blocks_num);
  fs->root = fs_file_create("/", FS_FILE_DIRECTORY, parent);
  fs->cwd = fs->root;
}

