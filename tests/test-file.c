#include "fssim/common.h"
#include "fssim/file.h"

void test1()
{
  fs_file_t* file = fs_file_create("/", FS_FILE_DIRECTORY, NULL);

  ASSERT(file->attrs.is_directory == 1, "");
  ASSERT(!strcmp(file->attrs.fname, "/"), "");
  ASSERT(file->parent == file, "");
  // TODO maybe this should actually be an empty linked list...
  ASSERT(file->children == NULL, "");

  fs_file_destroy(file);
}

void test2()
{
  fs_file_t* dir = fs_file_create("/", FS_FILE_DIRECTORY, NULL);
  fs_file_t* file = fs_file_create("hue.scm", FS_FILE_DIRECTORY, NULL);
  fs_file_t* file2 = fs_file_create("hue.scm", FS_FILE_DIRECTORY, NULL);

  fs_file_addchild(dir, file);

  ASSERT(dir->children != NULL, "");
  ASSERT(dir->children->data == file, "");

  fs_file_addchild(dir, file2);
  ASSERT(dir->children->data == file2, "");
  ASSERT(dir->children->next->data == file, "");

  fs_file_destroy(dir);
}

int main(int argc, char* argv[])
{
  TEST(test1, "file creation and deletion");
  TEST(test2, "adding files to directory");
  return 0;
}
