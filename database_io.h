#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "FoodItem.h"

char line_to_entry(char*, FoodItem*);
FoodItem* read_database(char*, unsigned*);
