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
  int b = 0;
  int e = entries - 1;
  int condition;
  int i;
  while (b <= e)
  {
    i = (b + e)/2;
    condition = difftime((entryList + i)->date, date);
    if (condition == 0)
    {
      return (entryList + i);
    }
    else if (condition < 0)
    {
      b = i + 1;
    }
    else
    {
      e = i - 1;
    }
  }
  return NULL;
}

void write_diary(char* userName, DiaryEntry* entryList, unsigned entries)
{
  FILE* diary = fopen(userName, "w");
  struct tm* timeinfo;
  char date[11];
  for (unsigned i = 0; i < entries; i++)
  {
    timeinfo = gmtime(&((entryList + i)->date));
    strftime(date, 11, "%Y,%m,%d", timeinfo);
    for (unsigned j = 0; j < (entryList + i)->numEntries; j++)
    {
      fprintf(diary, "%s,%s\n", date, (*((entryList + i)->entries)+j)->product_id);
    }
  }
  fclose(diary);
}

DiaryEntry* read_diary(char* userName, unsigned* length, unsigned* capacity, StorageTrie* search)
{
  FILE* diary = fopen(userName, "r");
  *capacity = 2;
  *length = 0;
  DiaryEntry* entries = malloc(sizeof(DiaryEntry) * *capacity);

  if (diary)
  {
    //Temporary cstring for reading lines in
    char* line = malloc(50);
    char* token;
    DiaryEntry* tmp;
    struct tm timeinfo = {0};

    FoodItem** food;

    unsigned* l = malloc(sizeof(unsigned));

    //While there are still lines left to read in the file
    while (fgets(line, 50, diary) != NULL)
    {
      token = strtok(line, ",\n");
      timeinfo.tm_year = atoi(token) - 1900;
      token = strtok (NULL, ",\n");
      timeinfo.tm_mon = atoi(token) - 1;
      token = strtok (NULL, ",\n");
      timeinfo.tm_mday = atoi(token);
      token = strtok (NULL, ",\n");
      tmp = get_day_items(mktime(&timeinfo), entries, *length);
      if (!tmp)
      {
        tmp = (entries + *(length));
        *length = *length + 1;
        tmp->entries = malloc(sizeof(FoodItem*)*2);
        tmp->numEntries = 0;
        tmp->capacity = 2;
      }
      if (*length == *capacity)
      {
        *capacity *= 2;
        entries = realloc(entries, sizeof(DiaryEntry) * *capacity);
      }
      tmp->date = mktime(&timeinfo);
      food = search_items(token, l, search);
      write_item(*food, tmp);
      free(food);
    }
    free(line);
    free(l);
    fclose(diary);
  }
  return entries;
}

void free_diary(DiaryEntry* diary, unsigned* diaryCapacity, unsigned* diaryEntries)
{
  free (diaryEntries);
  free (diaryCapacity);
  free (diary->entries);
  free (diary);
}
