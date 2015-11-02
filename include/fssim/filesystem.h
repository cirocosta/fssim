#ifndef FSSIM__FILESYSTEM_H
#define FSSIM__FILESYSTEM_H

#include "fssim/common.h"
#include "fssim/fat.h"
#include "fssim/file.h"
#include "fssim/file_utils.h"

typedef struct fs_filesystem_t {
  size_t blocks_num;
  size_t block_size;

  fs_fat_t* fat;
  fs_file_t* root;
  fs_file_t* cwd;
  FILE* file;
  uint8_t* buf;

} fs_filesystem_t;

fs_filesystem_t* fs_filesystem_create(size_t blocks);
void fs_filesystem_load(fs_filesystem_t* fs, unsigned char* buf);
void fs_filesystem_destroy(fs_filesystem_t* fs);

void fs_filesystem_mount(fs_filesystem_t* fs, const char* fname);
void fs_filesystem_ls(fs_filesystem_t* fs, const char* abspath, char* buf,
                      size_t n);
fs_file_t* fs_filesystem_find(fs_filesystem_t* fs, const char* root,
                              const char* fname);
fs_file_t* fs_filesystem_cp(fs_filesystem_t* fs, const char* src,
                            const char* dest);
fs_file_t* fs_filesystem_touch(fs_filesystem_t* fs, const char* fname);
int fs_filesystem_rm(fs_filesystem_t* fs, const char* path);

int fs_filesystem_serialize(fs_filesystem_t* fs, unsigned char* buf, int n);
void fs_filesystem_unmount(fs_filesystem_t* fs);

// private
int fs_filesystem_serialize_superblock(fs_filesystem_t* fs, unsigned char* buf,
                                       int n);

#endif
