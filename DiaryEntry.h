#ifndef DIARYENTRY
#define DIARYENTRY

#include "FoodItem.h"

typedef struct DiaryEntry DiaryEntry;

//Stores the diary. Includes FoodItem pointer array and unsigned array
//with capacities and lengths of each array
struct DiaryEntry
{
  //The items and quantities are stored in dynamic arrays.
  //While this makes add and delete operate in O(n) time,
  //Find operates in O(lg n) time, while Modify operates in O(1) time.
  //Because modify depends on find, the total runtime of modify = O(lg n) + O(1) = O(lg n)
  //If this were a linked list, Add, Delete, and Modify operate in O(1) time,
  //but find operates in O(n) time. And since add, delete, and modify depend
  //on find, all these operations would operate in O(n) time, making an array a better
  //structure for this purpose.
  FoodItem** entries;
  unsigned* quantities;
  unsigned numEntries;
  unsigned capacity;
};

#endif
