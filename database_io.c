#include "database_io.h"

char line_to_entry(char*, FoodItem*);

//Reads information from file into an array of entries
FoodItem* read_database(char* filename, unsigned* size_of_list)
{
  //Initializes file
  FILE* database = fopen(filename, "r");

  //Sets initial conditions for dynamic array
  unsigned capacity = 2;
  unsigned index_of_list = 0;

  //Allocates array with enough entries to support database
  FoodItem* entryList = malloc(sizeof(FoodItem)*capacity);

  //Temporary cstring for reading lines in
  char line[500];

  //While there are still lines left to read in the file
  while (fgets(line, 500, database) != NULL)
  {
    if (index_of_list == capacity)
    {
      capacity *= 2;
      entryList = realloc(entryList, sizeof(FoodItem)*capacity);
    }
    //If there is successful conversion of line to FoodItem,
    if (line_to_entry(line, (entryList + index_of_list)))
    {
      //The loop moves to populate the next item in the array
      index_of_list++;
    }
  }
  //Sets the size of the list so that the main function knows
  *size_of_list = index_of_list;

  //Closes the file
  fclose(database);

  //Returns the array
  return entryList;
}

//Parses the line into the FoodItem
char line_to_entry(char* line, FoodItem* entry)
{
  //Stores number of grams/mL per serving
  float serving_size_gml;

  //Array of cstrings that store parsed info
  char* items[12];

  //Starts the parsing
  int i = 0;
  char* tmp;
  items[i] = strtok(line, "~\n");
  while(items[i] != NULL)
  {
    i++;
    items[i] = strtok(NULL, "~\n");
  }

  //If the information was successfully parsed
  if (i == 11 && entry != NULL)
  {
    //Temporary variable
    float tmp;

    //Multiplier used to find nutrition per serving vs per 100 g/mL
    float multiplier;

    //Copies all the information to the array, multiplying the multiplier by the nutrition
    entry->product_id = malloc(strlen(items[0]) + 1);
    entry->product_name = malloc(strlen(items[1]) + 1);
    entry->manufacturer = malloc(strlen(items[2]) + 1);
    strcpy(entry->product_id, items[0]);
    strcpy(entry->product_name, items[1]);
    strcpy(entry->manufacturer, items[2]);
    multiplier = atof(items[7]);
    multiplier /= 100.0;
    tmp = atof(items[3]);
    entry->energy = multiplier * tmp;
    tmp = atof(items[4]);
    entry->carbs = multiplier * tmp;
    tmp = atof(items[5]);
    entry->fat = multiplier * tmp;
    tmp = atof(items[6]);
    entry->protein = multiplier * tmp;
    tmp = atof(items[9]);
    entry->serving_size = tmp;
    entry->serving_size_units = malloc(strlen(items[10]) + 1);
    strcpy(entry->serving_size_units, items[10]);
    return 1;
  }

  //Returns 0 if unsuccessful conversion
  else
  {
    return 0;
  }
}

void free_food_entry(FoodItem* item)
{
  free(item->product_id);
  free(item->product_name);
  free(item->manufacturer);
  free(item->serving_size_units);
}
