#ifndef FSSIM__FSINFO_H
#define FSSIM__FSINFO_H

#include "fssim/common.h"
#include "fssim/file.h"

typedef struct fs_fsinfo_t {
  uint32_t usedspace;   // B
  uint32_t wastedspace; // B
  uint16_t files;       // count
  uint16_t directories; // count
} fs_fsinfo_t;

/**
 * Calculates file infomation given a root dir
 * and a properly initialized fsinfo structure.
 */
static void fs_fsinfo_calculate(fs_fsinfo_t* info, fs_file_t* root)
{
  fs_file_t* f = NULL;
  fs_llist_t* l = root->children;

  while (l) {
    f = (fs_file_t*)l->data;

    if (f->attrs.is_directory) {
      info->directories += 1;
      info->usedspace += FS_BLOCK_SIZE;
      fs_fsinfo_calculate(info, f);
    } else {
      info->files += 1;
      info->usedspace += f->attrs.size;
      info->wastedspace += f->attrs.size % FS_BLOCK_SIZE;
    }

    l = l->next;
  }
}

#endif
