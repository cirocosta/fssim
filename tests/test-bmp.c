#include "fssim/bmp.h"
#include "fssim/common.h"

void test1()
{
  {
    size_t blocks = 1024;
    fs_bmp_t* bmp = fs_bmp_create(blocks);

    ASSERT(bmp->num_blocks == 1024, "");
    ASSERT(bmp->size == 128,
           "The size of the bmp must equal the # of uint8 entries it has");

    fs_bmp_destroy(bmp);
  }
  {
    size_t blocks = 1025;
    fs_bmp_t* bmp = fs_bmp_create(blocks);

    ASSERT(bmp->num_blocks == 1025, "");
    ASSERT(bmp->size == 129,
           "The size of the bmp must equal the # of uint8 entries it has");

    fs_bmp_destroy(bmp);
  }
}

void test2()
{
  size_t blocks = 16;
  fs_bmp_t* bmp = fs_bmp_create(blocks);

  fs_bmp_destroy(bmp);
}

int main(int argc, char* argv[])
{
  TEST(test1, "creation and deletion");
  TEST(test2, "bit flipping and retrieval");

  return 0;
}
