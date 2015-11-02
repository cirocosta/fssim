#include "fssim/filesystem.h"

fs_filesystem_t* fs_filesystem_create(size_t blocks)
{
  fs_filesystem_t* fs = malloc(sizeof(*fs));
  PASSERT(fs, FS_ERR_MALLOC);

  fs->blocks_num = blocks;
  fs->block_size = FS_BLOCK_SIZE;
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
  fs->root->fblock = fs_fat_addfile(fs->fat);
}

fs_file_t* fs_filesystem_touch(fs_filesystem_t* fs, const char* fname)
{
  // TODO concat fname to fs->cwd name till here
  /* if (fs_utils_fexists(fname)) { */
  /* } */

  fs_file_t* f = fs_file_create(fname, FS_FILE_REGULAR, fs->cwd);

  fs_file_addchild(fs->cwd, f);
  f->parent = fs->cwd;
  f->fblock = fs_fat_addfile(fs->fat);

  return f;
}

// FIXME horrible
void fs_filesystem_ls(fs_filesystem_t* fs, const char* abspath, char* buf,
                      size_t n)
{
  unsigned num_path_comp;
  char** path = fs_utils_splitpath(abspath, &num_path_comp);
  int written = 0;
  char mtime_buf[FS_DATE_FORMAT_SIZE] = { 0 };
  char fsize_buf[FS_FSIZE_FORMAT_SIZE] = { 0 };
  fs_llist_t* child = fs->cwd->children;

  if (num_path_comp) { // we're not in the root
    // go to the correct location and then proceed
    // TODO
    ASSERT(0, "not supported.");
  }

  fs_utils_fsize2str(fs->cwd->attrs.size * FS_BLOCK_SIZE, fsize_buf,
                     FS_FSIZE_FORMAT_SIZE);
  fs_utils_secs2str(fs->cwd->attrs.mtime, mtime_buf, FS_DATE_FORMAT_SIZE);

  written += snprintf(buf + written, n - written, FS_LS_FORMAT,
                      fs->cwd->attrs.is_directory == 1 ? 'd' : 'f', fsize_buf,
                      mtime_buf, ".");

  written += snprintf(buf + written, n - written, FS_LS_FORMAT,
                      fs->cwd->attrs.is_directory == 1 ? 'd' : 'f', fsize_buf,
                      mtime_buf, "..");

  while (child) {
    // TODO separete this into a function (in file.h)
    fs_file_t* file = (fs_file_t*)child->data;

    fs_utils_fsize2str(file->attrs.size * FS_BLOCK_SIZE, fsize_buf,
                       FS_FSIZE_FORMAT_SIZE);
    fs_utils_secs2str(file->attrs.mtime, mtime_buf, FS_DATE_FORMAT_SIZE);

    written += snprintf(buf + written, n, FS_LS_FORMAT,
                        file->attrs.is_directory == 1 ? 'd' : 'f', fsize_buf,
                        mtime_buf, file->attrs.fname);

    child = child->next;
  }

  free(path);
}

int fs_filesystem_serialize_superblock(fs_filesystem_t* fs, unsigned char* buf,
                                       int n)
{
  ASSERT(n >= 8, "`buf` must have at least 8 bytes remaining");
  serialize_uint32_t(buf, fs->block_size);
  serialize_uint32_t(buf + 4, fs->blocks_num);

  return 8;
}

void fs_filesystem_serialize(fs_filesystem_t* fs, unsigned char* buf, int n)
{
  int written = 0;

  written += fs_filesystem_serialize_superblock(fs, buf, n);
  written += fs_fat_serialize(fs->fat, buf + written, n - written);
  written += fs_file_serialize_dir(fs->root, buf + written, n - written);

  // TODO serialize files and stuff
}

fs_filesystem_t* fs_filesystem_load(unsigned char* buf)
{
  uint32_t block_size = deserialize_uint32_t(buf);
  uint32_t blocks = deserialize_uint32_t(buf + 4);

  fs_filesystem_t* fs = fs_filesystem_create(blocks);

  fs->block_size = block_size;
  fs->fat = fs_fat_load(buf + 8, blocks);

  // FIXME maybe we could pass &written in each 'load' .. which would be
  //       MUCH better than relying on some macros like this.
  fs->root = fs_file_load(buf + FS_FAT_SERIALIZE_SIZE(fs->fat) + 8);

  return fs;
}

static fs_llist_t* _find_file(fs_filesystem_t* fs, const char* fname)
{
  fs_llist_t* child = fs->cwd->children;

  while (child) {
    if (!strcmp(((fs_file_t*)child->data)->attrs.fname, fname))
      return child;

    child = child->next;
  }

  return NULL;
}

fs_file_t* fs_filesystem_find(fs_filesystem_t* fs, const char* root,
                              const char* fname)
{
  fs_llist_t* child = _find_file(fs, fname);
  fs_file_t* curr_file = NULL;

  // TODO split 'root' path, get there (if exists)
  //      and then perform the lookup

  if (!child) 
    return NULL;

  return (fs_file_t*)child->data;
}

/* fs_file_t* fs_filesystem_cp(fs_filesystem_t* fs, const char* src, */
/*                             const char* dest) */
/* { */
// open src
// calculate size (fseek & ftell)
// calculate # of blocks needed
// assert we have space for that
// grab those blocks
// (loop): read BUFSIZE, persist bytes readed
// assert we handled all bytes accordingly
// return fs_file_t* corresponding to the file
/* } */

int fs_filesystem_rm(fs_filesystem_t* fs, const char* path)
{
  // TODO split path and get to the directory
  fs_llist_t* child = _find_file(fs, path);

  if (!child)
    return 0;

  fs_llist_remove(fs->cwd->children, child);
  fs_llist_destroy(child, fs_file_destructor);

  return 1;
}
