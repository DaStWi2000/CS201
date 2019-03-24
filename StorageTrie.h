#ifndef STORAGETRIE
#define STORAGETRIE

#include "FoodItem.h"

typedef struct StorageTrie StorageTrie;
struct StorageTrie
{
  FoodItem** AllEntries;
  unsigned allEntriesLength;
  unsigned allEntriesCapacity;
  StorageTrie* next[36];
};

#endif
