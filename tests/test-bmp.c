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

  SET_LBIT(bmp->mapping[0], 0); // 0b10000000
  ASSERT(bmp->mapping[0] == 128, "");
  SET_LBIT(bmp->mapping[0], 7); // 0b10000001
  ASSERT(bmp->mapping[0] == 129, "");

  ASSERT(FS_BMP_IS_ON_(bmp, 0), "");
  ASSERT(FS_BMP_IS_ON_(bmp, 7), "");

  // 0b1000001
  // 0b0000001
  SET_LBIT(bmp->mapping[1], 7);
  LOGERR("bmp->mapping[1] = %d", bmp->mapping[1]);
  ASSERT(FS_BMP_IS_ON_(bmp, 15), "");

  fs_bmp_destroy(bmp);
}

void test3()
{
  size_t blocks = 16;
  fs_bmp_t* bmp = fs_bmp_create(blocks);

  // 0b10000000
  // 0b00000000
  FS_BMP_FLIP_(bmp, 0);
  // 0b10000100
  // 0b00000000
  FS_BMP_FLIP_(bmp, 5);
  // 0b10000100
  // 0b00000001
  FS_BMP_FLIP_(bmp, 15);

  ASSERT(FS_BMP_IS_ON_(bmp, 0), "");
  ASSERT(FS_BMP_IS_ON_(bmp, 5), "");
  ASSERT(FS_BMP_IS_ON_(bmp, 15), "");

  fs_bmp_destroy(bmp);
}

void test4()
{
  size_t blocks = 8;
  fs_bmp_t* bmp = fs_bmp_create(blocks);

  uint32_t b0 = fs_bmp_alloc(bmp);
  uint32_t b1 = fs_bmp_alloc(bmp);
  uint32_t b2 = fs_bmp_alloc(bmp);

  ASSERT(b0 == 0, "");
  ASSERT(b1 == 1, "");
  ASSERT(b2 == 2, "");
  //   !!
  // 0b11100000

  ASSERT(FS_BMP_IS_ON_(bmp, 0), "");
  ASSERT(FS_BMP_IS_ON_(bmp, 1), "");
  ASSERT(FS_BMP_IS_ON_(bmp, 2), "");

  //      !!!
  // 0b11111100
  FS_BMP_FLIP_(bmp, 3);
  FS_BMP_FLIP_(bmp, 4);
  FS_BMP_FLIP_(bmp, 5);

  //         !!
  // 0b11111110
  uint32_t b6 = fs_bmp_alloc(bmp);
  uint32_t b7 = fs_bmp_alloc(bmp);

  ASSERT(b6 == 6, "");
  ASSERT(b7 == 7, "");
  ASSERT(FS_BMP_IS_ON_(bmp, 6), "");
  ASSERT(FS_BMP_IS_ON_(bmp, 7), "");

  //     !!
  // 0b11001111
  FS_BMP_FLIP_(bmp, 2);
  FS_BMP_FLIP_(bmp, 3);
  ASSERT(!FS_BMP_IS_ON_(bmp, 2), "");
  ASSERT(!FS_BMP_IS_ON_(bmp, 3), "");

  //     !!
  // 0b11111111
  b2 = fs_bmp_alloc(bmp);
  uint32_t b3 = fs_bmp_alloc(bmp);
  ASSERT(b2 == 2, "");
  ASSERT(b3 == 3, "");
  ASSERT(FS_BMP_IS_ON_(bmp, 2), "");
  ASSERT(FS_BMP_IS_ON_(bmp, 3), "");

  fs_bmp_destroy(bmp);
}

void test5()
{
  size_t blocks = 16;
  fs_bmp_t* bmp = fs_bmp_create(blocks);

  //         !
  // 0b0000001
  // 0b0000000
  FS_BMP_FLIP_(bmp, 7);
  bmp->last_block = 7;

  ASSERT(FS_BMP_IS_ON_(bmp, 7), "");
  ASSERT(!FS_BMP_IS_ON_(bmp, 8), "");

  //         !
  // 0b0000001
  //   !
  // 0b0000000
  uint32_t b8 = fs_bmp_alloc(bmp);
  ASSERT(FS_BMP_IS_ON_(bmp, 8), "");

  fs_bmp_destroy(bmp);
}

void test6()
{
  const size_t BUFSIZE = 512;
  const size_t BLOCKS = 16;
  unsigned char* buf = calloc(BUFSIZE, sizeof(*buf));
  fs_bmp_t* bmp = fs_bmp_create(BLOCKS);

  PASSERT(buf, FS_ERR_MALLOC);

  // 0b11100000
  // 0b11100000
  fs_bmp_alloc(bmp);
  fs_bmp_alloc(bmp);
  fs_bmp_alloc(bmp);
  bmp->last_block = 7;
  fs_bmp_alloc(bmp);
  fs_bmp_alloc(bmp);
  fs_bmp_alloc(bmp);

  fs_bmp_serialize(bmp, buf, BUFSIZE);

  ASSERT(deserialize_uint8_t(buf) == bmp->mapping[0], "");
  ASSERT(deserialize_uint8_t(buf + 1) == bmp->mapping[1], "");

  fs_bmp_destroy(bmp);
  free(buf);
}

void test7()
{
  const size_t BUFSIZE = 512;
  const size_t BLOCKS = 16;

  unsigned char* buf = calloc(BUFSIZE, sizeof(*buf));
  fs_bmp_t* bmp = fs_bmp_create(BLOCKS);

  PASSERT(buf, FS_ERR_MALLOC);

  // 0b11100000
  // 0b11100000
  fs_bmp_alloc(bmp);
  fs_bmp_alloc(bmp);
  fs_bmp_alloc(bmp);
  bmp->last_block = 7;
  fs_bmp_alloc(bmp);
  fs_bmp_alloc(bmp);
  fs_bmp_alloc(bmp);

  fs_bmp_serialize(bmp, buf, BUFSIZE);
  fs_bmp_t* bmp2 = fs_bmp_load(buf, BLOCKS);

  ASSERT(bmp2->mapping[0] == bmp->mapping[0], "");
  ASSERT(bmp2->mapping[1] == bmp->mapping[1], "");

  fs_bmp_destroy(bmp);
  fs_bmp_destroy(bmp2);
  free(buf);
}

int main(int argc, char* argv[])
{
  TEST(test1, "creation and deletion");
  TEST(test2, "bit value checker");
  TEST(test3, "bit flipper");
  TEST(test4, "block alloc");
  TEST(test5, "block alloc - multiple rows");
  TEST(test6, "persistence - serialize");
  TEST(test7, "persistence - load");

  return 0;
}
