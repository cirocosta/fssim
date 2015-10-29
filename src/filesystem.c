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
    PASSERT(file = fopen(fname, "w+b"), "fopen(w+) failed:");
    return file;
  }

  PASSERT(file = fopen(fname, "r+b"), "fopen(r+) failed:");

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

fs_file_t* fs_filesystem_touch(fs_filesystem_t* fs, const char* fname)
{
  // TODO concat fname to fs->cwd name till here
  /* if (fs_utils_fexists(fname)) { */
  /* } */

  fs_file_t* f = fs_file_create(fname, FS_FILE_REGULAR, fs->cwd);
  fs_file_addchild(fs->cwd, f);

  return f;
}

void fs_filesystem_ls(fs_filesystem_t* fs, const char* abspath, char* buf,
                      size_t n)
{
  unsigned num_path_comp;
  char** path = fs_utils_splitpath(abspath, &num_path_comp);
  int written = 0;

  if (num_path_comp) { // we're not in the root
    // go to the correct location and then proceed
    // TODO
    ASSERT(0, "not supported.");
  }

  written += snprintf(buf + written, n, "%c %u %s .\n",
                      fs->cwd->attrs.is_directory == 1 ? 'd' : 'f',
                      fs->cwd->attrs.size, "TEMPO");
  written += snprintf(buf + written, n - written, "%c %u %s ..\n",
                      fs->cwd->parent->attrs.is_directory == 1 ? 'd' : 'f',
                      fs->cwd->parent->attrs.size, "TEMPO");

  // TODO
  // parse directory file references by parsing
  // the payload and inspecting the content.

  free(path);
}
