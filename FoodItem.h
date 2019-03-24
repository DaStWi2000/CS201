#ifndef FOODITEM
#define FOODITEM

typedef struct FoodItem FoodItem;

struct FoodItem
{
  char* product_id;
  char* product_name;
  char* manufacturer;
  float energy;
  float carbs;
  float fat;
  float protein;
  float serving_size;
  char* serving_size_units;
};
#endif
