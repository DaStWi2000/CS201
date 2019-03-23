#ifndef FOODITEM
#define FOODITEM

typedef struct FoodItem FoodItem;

struct FoodItem
{
  char product_id[9];
  char product_name[50];
  char manufacturer[50];
  float energy;
  float carbs;
  float fat;
  float protein;
  float serving_size;
  char serving_size_units[20];
  FoodItem* previous;
  FoodItem* next;
};
#endif
