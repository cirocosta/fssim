#include "fssim/bmp.h"
#include "fssim/common.h"

void test1()
{
  size_t blocks = 1024;
  fs_bmp_t* bmp = fs_bmp_create(blocks);

  fs_bmp_destroy(bmp);
}

int main(int argc, char *argv[])
{
  TEST(test1, "creation and deletion");
  
  return 0;
}

