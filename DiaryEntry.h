#ifndef DIARYENTRY
#define DIARYENTRY

#include "FoodItem.h"

typedef struct DiaryEntry DiaryEntry;

struct DiaryEntry
{
  FoodItem** entries;
  unsigned* quantities;
  unsigned numEntries;
  unsigned capacity;
};

#endif
