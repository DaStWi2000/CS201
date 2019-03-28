#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "FoodItem.h"

FoodItem* read_database(char*, unsigned*);
void free_food_entry(FoodItem*);
