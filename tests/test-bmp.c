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

  ASSERT(bmp->mapping[0] == 0x00, "Initialily [00000000]");
  ASSERT(bmp->mapping[1] == 0x00, "Initialily [00000000]");

  SET_LBIT(bmp->mapping[0], 0);
  ASSERT(bmp->mapping[0] == 128, "");
  SET_LBIT(bmp->mapping[0], 7);
  ASSERT(bmp->mapping[0] == 129, "");

  ASSERT(fs_bmp_IS_ON_(bmp, 0), "");
  ASSERT(fs_bmp_IS_ON_(bmp, 7), "");

  SET_LBIT(bmp->mapping[1], 7);
  LOGERR("bmp->mapping[1] = %d", bmp->mapping[1]);
  ASSERT(fs_bmp_IS_ON_(bmp, 15), "");

  fs_bmp_destroy(bmp);
}

void test3()
{
  size_t blocks = 16;
  fs_bmp_t* bmp = fs_bmp_create(blocks);

  fs_bmp_FLIP_(bmp, 0);
  fs_bmp_FLIP_(bmp, 5);
  fs_bmp_FLIP_(bmp, 15);

  ASSERT(CHECK_LBIT(bmp->mapping[0], 0), "");
  ASSERT(CHECK_LBIT(bmp->mapping[0], 5), "");
  ASSERT(CHECK_LBIT(bmp->mapping[1], 7), "");

  fs_bmp_destroy(bmp);
}

int main(int argc, char* argv[])
{
  TEST(test1, "creation and deletion");
  TEST(test2, "bit value checker");
  TEST(test3, "bit flipper");

  return 0;
}
