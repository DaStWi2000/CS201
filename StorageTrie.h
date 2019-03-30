#ifndef STORAGETRIE
#define STORAGETRIE

#include "FoodItem.h"

typedef struct StorageTrie StorageTrie;
struct StorageTrie
{
  //My database uses a trie (https://en.wikipedia.org/wiki/Trie) for cataloging
  //food items. This allows lookup times to be decreased because lookup of a term
  //is not proportional to the number of items (n), but instead proportional to the
  //length of the search term (for these purposes, k). This makes lookup for a
  //valid id (k = 8) = O(8) = O(1). It makes lookup of search terms O(k*m), where
  //m = number of search terms. While the initial setup has long runtime (O(n)),
  //And tagging has O(m*n) runtime, it's not as big of an issue considering how
  //the database is static in nature, and search time is a much bigger priority
  //than setup time. Tries also allow easy 'tagging' of items so that search results
  //can be more precise. This uses more space, but tradeoff between storage complexity
  //and search time makes it worth it when discussing a database.
  FoodItem** AllEntries;
  unsigned allEntriesLength;
  unsigned allEntriesCapacity;
  StorageTrie* next[36];
};

#endif
