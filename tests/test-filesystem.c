#include "fssim/common.h"
#include "fssim/filesystem.h"

#define FS_TEST_FNAME "/tmp/test-fssim"

void test1()
{
  fs_filesystem_t* fs = fs_filesystem_create(10);

  fs_filesystem_destroy(fs);
}

void test2()
{
  fs_filesystem_t* fs = fs_filesystem_create(10); // 40 KB

  fs_utils_fdelete(FS_TEST_FNAME);
  ASSERT(fs_utils_fexists(FS_TEST_FNAME) == 0, "No previous file");

  fs_filesystem_mount(fs, FS_TEST_FNAME);

  ASSERT(fs_utils_fexists(FS_TEST_FNAME) == 1,
         "must create a file that represents the FS");
  // TODO check the size as well??

  fs_filesystem_destroy(fs);
}

void test3()
{
  //
  // is_dir size last_modified name
  const char* expected = "d 4096 0 .\n"
                         "d 4096 0 ..\n";
  char* buf = calloc(strlen(expected) + 1, sizeof(*buf));
  memset(buf, '\0', strlen(expected) + 1);

  fs_filesystem_t* fs = fs_filesystem_create(10); // 40 KB
  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  /* fs_filesystem_ls(fs, "/", buf, strlen(expected) + 1); */

  /* ASSERT(!strcmp(buf, expected), ""); */

  fs_filesystem_destroy(fs);
  free(buf);
}

int main(int argc, char* argv[])
{
  TEST(test1, "creation and deletion");
  TEST(test2, "mounting w/out previous mount");
  TEST(test3, "listing");

  return 0;
}
