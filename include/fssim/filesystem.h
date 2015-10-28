#ifndef FSSIM__FILESYSTEM_H
#define FSSIM__FILESYSTEM_H

#include "fssim/common.h"
#include "fssim/fat.h"
#include "fssim/file.h"
#include "fssim/file_utils.h"

/* typedef struct fs_filesystem_superblock_t { */
/*   qtd_dirs; */
/*   qtd_arquivos; */
/*   wasted_space; */
/* } fs_filesystem_superblock_t; */

typedef struct fs_filesystem_t {
  size_t blocks_num;
  fs_fat_t* fat;
  fs_file_t* root;
  fs_file_t* cwd;
  FILE* file;
} fs_filesystem_t;

fs_filesystem_t* fs_filesystem_create(size_t blocks);
void fs_filesystem_destroy(fs_filesystem_t* fs);

void fs_filesystem_mount(fs_filesystem_t* fs, const char* fname);
void fs_filesystem_ls(fs_filesystem_t* fs, const char* abspath, char* buf,
                      size_t n);
fs_file_t* fs_filesystem_touch(fs_filesystem_t* fs, const char* fname);

// TODO
void fs_filesystem_unmount(fs_filesystem_t* fs);

#endif
