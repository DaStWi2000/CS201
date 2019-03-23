#include <math.h>
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

int main()
{
  unsigned* size = malloc(sizeof(unsigned));
  FoodItem* entryList = read_database("database", size);
  char search[15] = "barbecue";
  FoodItem* head = find_search_term(search, entryList, size);
  while (head != NULL)
  {
    print_facts(head);
    head = head->next;
  }
  return 0;
}
