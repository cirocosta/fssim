#include "fssim/common.h"
#include "fssim/fat.h"

void test1()
{
  fs_fat_t* fat = fs_fat_create(10);

  ASSERT(fat->blocks[0] == 0, "0->NIL");
  ASSERT(fat->blocks[9] == 9, "9->NIL");

  fs_fat_destroy(fat);
}

void test2()
{
  fs_fat_t* fat = fs_fat_create(10);

  uint32_t file_entry0 = fs_fat_addfile(fat);
  ASSERT(file_entry0 == 0, "");
  ASSERT(fat->blocks[file_entry0] == 0,
         "0->NIL but internally 0 is marked used (bmp should take care)");

  uint32_t file_entry1 = fs_fat_addfile(fat);
  ASSERT(file_entry1 == 1, "");
  ASSERT(fat->blocks[file_entry1] == 1, "1->NIL should be received. 0 is "
                                        "already taken (even though it points "
                                        "to NIL)");
  fs_fat_destroy(fat);
}

int main(int argc, char* argv[])
{
  TEST(test1, "creation and deletion");
  TEST(test2, "file add");

  return 0;
}
