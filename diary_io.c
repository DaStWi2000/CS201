#include "diary_io.h"

void write_item(FoodItem* food, DiaryEntry* entry)
{
  if (entry->capacity == entry->numEntries)
  {
    entry->capacity *= 2;
    entry->entries = realloc(entry->entries, sizeof(DiaryEntry*)*entry->capacity);
  }
  *(entry->entries + entry->numEntries) = food;
  entry->numEntries++;
}

DiaryEntry* get_day_items(time_t date, DiaryEntry* entryList, unsigned entries)
{
  for (unsigned i = 0; i < entries; i++)
  {
    if (difftime((entryList + i)->date, date) == 0)
    {
      return (entryList + i);
    }
  }
  return NULL;
}

void write_diary(DiaryEntry* entryList, unsigned entries)
{
  FILE* diary;
  char filename[7];
  struct tm* time_format;
  for (unsigned i = 0; i < entries; i++)
  {
    time_format = localtime(&((entryList + i)->date));
    strftime(filename, 7, "%C%m%d", time_format);
    diary = fopen(filename, "w");
    for (unsigned j = 0; j < (entryList + i)->numEntries; j++)
    {
      fprintf(diary, "%s\n", (*((entryList + i)->entries + j))->product_id);
    }
    fclose(diary);
  }
}

DiaryEntry* read_diary()
{
  
}
