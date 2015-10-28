#include "fssim/llist.h"
#include "fssim/common.h"
#include "fssim/file.h"

void test1() {
  int value = 2;
  fs_llist_t* list = fs_llist_create(&value);

  fs_llist_destroy(list, NULL);
}

void test2() {
  int value1 = 1;
  int value2 = 2;
  int value3 = 3;
  int value4 = 4;

  fs_llist_t* list1 = fs_llist_create(&value1);
  fs_llist_t* list2 = fs_llist_create(&value2);
  fs_llist_t* list3 = fs_llist_create(&value3);
  fs_llist_t* list4 = fs_llist_create(&value4);

  fs_llist_append(list1, list2); // l1 -> l2 -> nul
  fs_llist_append(list3, list4); // l3 -> l4 -> null
  fs_llist_append(list1, list3); // l1 -> l2 -> l3 -> l4 - > nul

  ASSERT(list1->next == list2, "");
  ASSERT(list2->next == list3, "");
  ASSERT(list3->next == list4, "");
  ASSERT(list4->next == NULL, "");

  fs_llist_destroy(list1, NULL);
}

void file_destructor(void* data)
{
  fs_file_t* file = (fs_file_t*)data;

  fs_file_destroy(file);
}

void test3() {
  fs_file_t* file1 = fs_file_create("file1", FS_FILE_REGULAR, NULL);
  fs_file_t* file2 = fs_file_create("file2", FS_FILE_REGULAR, NULL);
  fs_file_t* file3 = fs_file_create("file3", FS_FILE_REGULAR, NULL);

  fs_llist_t* list1 = fs_llist_create(file1);
  fs_llist_t* list2 = fs_llist_create(file2);
  fs_llist_t* list3 = fs_llist_create(file3);

  fs_llist_append(list1, list2); // l1 -> l2 -> nul
  fs_llist_append(list1, list3); // l1 -> l2 -> l3 -> nul

  fs_llist_destroy(list1, file_destructor);
}

int main(int argc, char *argv[]) { 
  TEST(test1, "creation w/ single stacked data") ;
  TEST(test2, "append multiple stacked data") ;
  TEST(test3, "destroy with destructor") ;
  
  return 0; }
