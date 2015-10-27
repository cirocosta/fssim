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

void test3()
{
  fs_fat_t* fat = fs_fat_create(10);

  uint32_t file_entry0 = fs_fat_addfile(fat);
  uint32_t file_entry1 = fs_fat_addfile(fat);

  // file0 :  0->2->NIL 
  // file1 :  1->NIL 
  fs_fat_addblock(fat, file_entry0);
  ASSERT(fat->blocks[file_entry0] == 2, "");
  ASSERT(fat->blocks[2] == 2, "");

  // file0 :  0->2->NIL 
  // file1 :  1->3->NIL 
  fs_fat_addblock(fat, file_entry1);
  ASSERT(fat->blocks[file_entry1] == 3, "");
  ASSERT(fat->blocks[3] == 3, "");

  // file0 :  0->2->NIL 
  // file1 :  1->3->4->NIL 
  fs_fat_addblock(fat, file_entry1);
  ASSERT(fat->blocks[3] == 4, "");
  ASSERT(fat->blocks[4] == 4, "");

  fs_fat_destroy(fat);
}

void test4()
{
  fs_fat_t* fat = fs_fat_create(7);

  uint32_t file_entry0 = fs_fat_addfile(fat);
  uint32_t file_entry1 = fs_fat_addfile(fat);

  fs_fat_addblock(fat, file_entry0);
  fs_fat_addblock(fat, file_entry1);
  fs_fat_addblock(fat, file_entry1);
  fs_fat_addblock(fat, file_entry1);
  // file0 :  0->2->NIL 
  // file1 :  1->3->4->5->6->NIL 
  fs_fat_addblock(fat, file_entry1);

  // file0 :  - 
  // file1 :  1->3->4->5->6->NIL 
  fs_fat_removefile(fat, file_entry0);

  // file1 :  1->3->4->5->6->0->NIL 
  //          2->NIL
  fs_fat_addblock(fat, file_entry1);

  // file1 :  1->3->4->5->6->0->2->NIL 
  fs_fat_addblock(fat, file_entry1);

  ASSERT(fat->blocks[0] == 2, "");
  ASSERT(fat->blocks[1] == 3, "");
  ASSERT(fat->blocks[2] == 2, "");
  ASSERT(fat->blocks[3] == 4, "");
  ASSERT(fat->blocks[4] == 5, "");
  ASSERT(fat->blocks[5] == 6, "");
  ASSERT(fat->blocks[6] == 0, "");

  fs_fat_destroy(fat);
}

int main(int argc, char* argv[])
{
  TEST(test1, "creation and deletion");
  TEST(test2, "file add");
  TEST(test3, "add block");
  TEST(test4, "remove file");

  return 0;
}
