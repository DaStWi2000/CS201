#include "StorageTrie.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

void initialize_node(StorageTrie*);
FoodItem** search_items(char*, unsigned*, StorageTrie*);
FoodItem** return_child_items(FoodItem**, unsigned*, unsigned*, StorageTrie*);
void add_item_to_trie(FoodItem*, StorageTrie*);
void add_item_to_node(FoodItem*, StorageTrie*);
StorageTrie* find_trie_node(char*, StorageTrie*);
void free_trie(StorageTrie*);
