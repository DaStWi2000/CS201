#include "diary_io.h"

FoodItem** find_item(FoodItem*, DiaryEntry*);

void write_item(FoodItem* food, DiaryEntry* entry, unsigned quantity)
{
  if (entry->capacity == entry->numEntries)
  {
    if (entry->capacity == 0)
    {
      entry->capacity = 1;
      entry->entries = malloc(sizeof(FoodItem*));
      entry->quantities = malloc(sizeof(unsigned));
    }
    else
    {
      entry->capacity *= 2;
      entry->entries = realloc(entry->entries, sizeof(FoodItem*) * entry->capacity);
      entry->quantities = realloc(entry->quantities, sizeof(unsigned) * entry->capacity);
    }
  }
  if (!find_item(food, entry))
  {
    unsigned i = entry->numEntries;
    for (; i > 0 && strcmp(food->product_id, (*(entry->entries + i - 1))->product_id) < 0; i--)
    {
      *(entry->entries + i) = *(entry->entries + i - 1);
      *(entry->quantities + i) = *(entry->quantities + i - 1);
    }
    *(entry->entries + i) = food;
    *(entry->quantities + i) = quantity;
    entry->numEntries++;
  }
  else
  {
    unsigned i = find_item(food, entry) - entry->entries;
    *(entry->quantities + i) = quantity;
  }
}

void delete_item(FoodItem* food, DiaryEntry* entry)
{
  if (find_item(food, entry))
  {
    unsigned i = 0;
    for (; strcmp(food->product_id, (*(entry->entries + i))->product_id) > 0; i++);
    for (; i < entry->numEntries - 1; i++)
    {
      *(entry->entries + i) = *(entry->entries + i + 1);
      *(entry->quantities + i) = *(entry->quantities + i + 1);
    }
    entry->numEntries--;
  }
}

FoodItem** find_item(FoodItem* food, DiaryEntry* entry)
{
  int b = 0;
  int e = entry->numEntries - 1;
  int index = 0;
  while (b <= e)
  {
    index = (b + e)/2;
    if (!strcmp(food->product_id, (*(entry->entries + index))->product_id))
    {
      return entry->entries + index;
    }
    else if (strcmp(food->product_id, (*(entry->entries + index))->product_id) > 0)
    {
      b = index + 1;
    }
    else
    {
      e = index - 1;
    }
  }
  return NULL;
}

void write_diary(char* userName, DiaryEntry* entryList)
{
  FILE* diary = fopen(userName, "w");
  for (unsigned i = 0; i < entryList->numEntries; i++)
  {
    fprintf(diary, "%s,%u\n", (*(entryList->entries+i))->product_id, *(entryList->quantities + i));
  }
  fclose(diary);
}

DiaryEntry* read_diary(char* userName, StorageTrie* search)
{
  FILE* diary = fopen(userName, "r");
  DiaryEntry* entries = malloc(sizeof(DiaryEntry));
  entries->numEntries = 0;
  entries->capacity = 0;
  if (diary)
  {
    //Temporary cstring for reading lines in
    char line[60];
    char* token;

    StorageTrie* result;

    //While there are still lines left to read in the file
    while (fgets(line, 60, diary) != NULL)
    {
      token = strtok(line, ",");
      result = find_trie_entry(token, search);
      if (result)
      {
        token = strtok(NULL, "\n");
        write_item(*(result->AllEntries), entries, strtol(token, NULL, 10));
      }
    }
    fclose(diary);
  }
  return entries;
}

void free_diary(DiaryEntry* diary)
{
  if (diary->capacity)
  {
    free(diary->quantities);
    free(diary->entries);
  }
  free(diary);
}
