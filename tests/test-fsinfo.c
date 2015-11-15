#include "fssim/common.h"
#include "fssim/fsinfo.h"

void test1()
{
  fs_fsinfo_t info = { 0 };

  fs_file_t* dir = fs_file_create("/", FS_FILE_DIRECTORY, NULL);
  fs_file_t* file = fs_file_create("hue.scm", FS_FILE_REGULAR, NULL);
  fs_file_t* file2 = fs_file_create("other.scm", FS_FILE_REGULAR, NULL);
  fs_file_t* dir_tmp = fs_file_create("/tmp", FS_FILE_DIRECTORY, NULL);
  fs_file_t* dir_whoa = fs_file_create("/whoa", FS_FILE_DIRECTORY, NULL);
  fs_file_t* file3 = fs_file_create("file3.scm", FS_FILE_REGULAR, NULL);

  fs_file_addchild(dir, file);
  fs_file_addchild(dir, file2);
  fs_file_addchild(dir, dir_tmp);
  fs_file_addchild(dir_tmp, file3);
  fs_file_addchild(dir_tmp, dir_whoa);

  fs_fsinfo_calculate(&info, dir);

  ASSERT(info.files == 3, "Expected 3, got %d", info.files);
  ASSERT(info.directories == 2, "Expected 1, got %d", info.directories);

  fs_file_destroy(dir);
}

int main(int argc, char* argv[])
{
  TEST(test1, "calculates files and directories ok");

  return 0;
}
