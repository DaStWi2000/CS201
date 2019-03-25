#include <math.h>
#include <ncurses.h>
#include "database_io.h"
#include "database_search.h"
#include "diary_io.h"

void print_facts(FoodItem* entry)
{
  printf("ID:\t\t%s\n", entry->product_id);
  printf("Product Name:\t%s\n", entry->product_name);
  printf("Manufacturer:\t%s\n", entry->manufacturer);
  printf("Calories:\t%.1f g\n", roundf(entry->energy * 10)/10);
  printf("Carbohydrates:\t%.1f g\n", roundf(entry->carbs * 10)/10);
  printf("Total Fat\t%.1f g\n", roundf(entry->fat * 10)/10);
  printf("Protein:\t%.1f g\n", roundf(entry->protein * 10)/10);
  printf("Serving Size:\t%.1f %s\n", entry->serving_size, entry->serving_size_units);
}

StorageTrie* initialize_database(unsigned* size)
{
  StorageTrie* root = malloc(sizeof(StorageTrie));
  initialize_node(root);
  root->AllEntries = malloc(sizeof(FoodItem*));
  FoodItem* entryList = read_database("database", size);
  for (unsigned i = 0; i < *size; i++)
  {
    add_item_to_trie(entryList + i,root);
  }
  *(root->AllEntries) = entryList;
  root->allEntriesCapacity = 1;
  root->allEntriesLength = 1;
}

void free_database(StorageTrie* data, unsigned* size)
{
  for (int i = 0; i < *size; i++)
  {
    free_food_entry(*(data->AllEntries)+i);
  }
  free(size);
  free(*(data->AllEntries));
  free_trie(data);
}

int main()
{
  unsigned* size = malloc(sizeof(unsigned));
  StorageTrie* data = initialize_database(size);
  unsigned* diaryEntries = malloc(sizeof(unsigned));
  unsigned* diaryCapacity = malloc(sizeof(unsigned));
  unsigned* temp = malloc(sizeof(unsigned));
  DiaryEntry* diary = read_diary("daniel.log", diaryEntries, diaryCapacity, data);
  FoodItem** food = search_items("45001542", temp, data);
  write_item(*food,diary);
  write_diary("daniel.log", diary, *diaryEntries);
  free_database(data, size);
  free(temp);
  free(food);
  return 0;
}
