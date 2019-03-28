#include "StorageTrie.h"
#include "database_io.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

StorageTrie* initialize_database(unsigned*);
void free_database(StorageTrie*, unsigned);
void initialize_node(StorageTrie*);
void add_id_to_trie(FoodItem*, StorageTrie*);
FoodItem** return_child_items(FoodItem**, unsigned*, unsigned*, unsigned, StorageTrie*);
StorageTrie* find_trie_entry(char* searchTerm, StorageTrie* root);
FoodItem** intersection(FoodItem**, FoodItem**, unsigned, unsigned, unsigned*);
void free_trie(StorageTrie*);
