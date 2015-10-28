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

int main(int argc, char* argv[])
{
  TEST(test1, "file creation and deletion");
  return 0;
}
