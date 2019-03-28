#include <stdio.h>
#include <stdlib.h>
#include "DiaryEntry.h"
#include "database_search.h"

void write_item(FoodItem*, DiaryEntry*, unsigned);
void write_diary(char*, DiaryEntry*);
DiaryEntry* read_diary(char*, StorageTrie*);
void delete_item(FoodItem*, DiaryEntry*);
void free_diary(DiaryEntry*);
