#include "fssim/common.h"
#include "fssim/fat.h"

void test1()
{
  fs_fat_t* fat = fs_fat_create(10);

  ASSERT(fat->blocks[0] == 0, "0->NIL");
  ASSERT(fat->blocks[9] == 9, "9->NIL");

  fs_fat_destroy(fat);
}

int main(int argc, char *argv[])
{
  TEST(test1, "creation and deletion");
  
  return 0;
}
