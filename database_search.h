#include "StorageTrie.h"
#include "database_io.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

StorageTrie* initialize_database(unsigned*);
void free_database(StorageTrie*, unsigned);
void initialize_node(StorageTrie*);
FoodItem** search_items(char*, unsigned*, StorageTrie*);
FoodItem** return_child_items(FoodItem**, unsigned*, unsigned*, StorageTrie*);
void add_item_to_trie(FoodItem*, StorageTrie*);
void add_item_to_node(FoodItem*, StorageTrie*);
StorageTrie* find_trie_node(char*, StorageTrie*);
void free_trie(StorageTrie*);
