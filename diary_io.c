#include "diary_io.h"

FoodItem** find_item(FoodItem*, DiaryEntry*);

//Writes the given FoodItem to the Diary
void write_item(FoodItem* food, DiaryEntry* entry, unsigned quantity)
{
  //If there are no more entries able to be written
  if (entry->capacity == entry->numEntries)
  {
    //The array is initialized
    if (entry->capacity == 0)
    {
      entry->capacity = 1;
      entry->entries = malloc(sizeof(FoodItem*));
      entry->quantities = malloc(sizeof(unsigned));
    }

    //The array is resized
    else
    {
      entry->capacity *= 2;
      entry->entries = realloc(entry->entries, sizeof(FoodItem*) * entry->capacity);
      entry->quantities = realloc(entry->quantities, sizeof(unsigned) * entry->capacity);
    }
  }

  //If the food item is not found in the diary,
  if (!find_item(food, entry))
  {
    //The item is added in the position that makes the diary organized by id
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

  //Otherwise
  else
  {
    //The quantity is updated in the entry
    unsigned i = find_item(food, entry) - entry->entries;
    *(entry->quantities + i) = quantity;
  }
}

//Deletes the given FoodItem from the Diary
void delete_item(FoodItem* food, DiaryEntry* entry)
{
  //If the item is found
  if (find_item(food, entry))
  {
    //Item is deleted and items after get shifted down an index
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

//Finds food item in diary entry
FoodItem** find_item(FoodItem* food, DiaryEntry* entry)
{
  //Implementation of binary search, which runs in O(lg n)
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

  //If item not found
  return NULL;
}

//Writes the diary to a file
void write_diary(char* userName, DiaryEntry* entryList)
{
  //If a stream to the file is successfully opened
  FILE* diary = fopen(userName, "w");
  if (diary)
  {
    //Each line gets written to the username specified
    for (unsigned i = 0; i < entryList->numEntries; i++)
    {
      fprintf(diary, "%s,%u\n", (*(entryList->entries+i))->product_id, *(entryList->quantities + i));
    }
    fclose(diary);
  }
}

//Reads the diary file into the program's data structure
DiaryEntry* read_diary(char* userName, StorageTrie* search)
{
  //Attempts to open the file
  FILE* diary = fopen(userName, "r");
  DiaryEntry* entries = malloc(sizeof(DiaryEntry));
  entries->numEntries = 0;
  entries->capacity = 0;

  //If the diary is successfully opened
  if (diary)
  {
    //Temporary cstring for reading lines in
    char line[13] = "";
    char* token;

    //Stores the temporary address of a node described by the id
    StorageTrie* result;

    //While there are still lines left to read in the file
    while (fgets(line, 12, diary) != NULL)
    {
      //Parses out the id and quantities
      token = strtok(line, ",");
      result = find_trie_entry(token, search);

      //If the ID is found
      if (result)
      {
        //Writes the item and quantity to the diary
        token = strtok(NULL, "\n");
        write_item(*(result->AllEntries), entries, strtol(token, NULL, 10));
      }
    }

    //Closes file strem
    fclose(diary);
  }

  //Returns the diary entry
  return entries;
}

//Frees the diary from memory
void free_diary(DiaryEntry* diary)
{
  //Frees diary storage pointers
  if (diary->capacity)
  {
    free(diary->quantities);
    free(diary->entries);
  }

  //Frees diary
  free(diary);
}
