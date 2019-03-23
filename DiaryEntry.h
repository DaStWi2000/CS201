#ifndef DIARYENTRY
#define DIARYENTRY

#include "FoodItem.h"
#include <time.h>

typedef struct DiaryEntry DiaryEntry;

struct DiaryEntry
{
  time_t date;
  FoodItem** entries;
  unsigned numEntries;
  unsigned capacity;
};

#endif
