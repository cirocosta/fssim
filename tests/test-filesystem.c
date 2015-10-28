#include "fssim/common.h"
#include "fssim/filesystem.h"

void test1()
{
  fs_filesystem_t* fs = fs_filesystem_create(10);

  fs_filesystem_destroy(fs);
}

int main(int argc, char *argv[])
{
  TEST(test1, "creation and deletion");
  
  return 0;
}
