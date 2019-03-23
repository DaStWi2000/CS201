#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "DiaryEntry.h"

void write_item(FoodItem*, DiaryEntry*);
DiaryEntry* get_day_items(time_t, DiaryEntry*, unsigned);
void write_diary(DiaryEntry*, unsigned);
