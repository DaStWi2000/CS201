#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "DiaryEntry.h"
#include "database_search.h"

void write_item(FoodItem*, DiaryEntry*);
DiaryEntry* get_day_items(time_t, DiaryEntry*, unsigned);
void write_diary(char*, DiaryEntry*, unsigned);
DiaryEntry* read_diary(char*, unsigned*, unsigned*, StorageTrie*);
void free_diary(DiaryEntry*, unsigned*, unsigned*);
