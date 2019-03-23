#include "database_search.h"

//Searches for search_term in entry, if found returns 1. Else returns 0.
FoodItem* find_search_term(char* search_term, FoodItem* entry, unsigned* size_of_list)
{
  FoodItem* returnEntry = NULL;
  FoodItem* current = returnEntry;

  //Converts search term to uppercase
  char* focus = search_term;
  while (*focus)
  {
    *focus = toupper(*focus);
    focus++;
  }

  //Cycles through list of food items looking for the search term
  for (int i = 0; i < *size_of_list; i++)
  {
    //Looks in the product id
    char* test = NULL;
    test = strstr(entry->product_id, search_term);

    //If match is found, the node and the previous node are linked
    if (test != NULL)
    {
      link(current, entry);
      if (returnEntry == NULL)
      {
        returnEntry = entry;
      }
      current = entry;
    }
    else
    {
      //Looks in the product name
      test = strstr(entry->product_name, search_term);

      //If match is found, the node and the previous node are linked
      if (test != NULL)
      {
        link(current, entry);
        if (returnEntry == NULL)
        {
          returnEntry = entry;
        }
        current = entry;
      }
      else
      {
        //Looks in the manufacturer
        test = strstr(entry->manufacturer, search_term);

        //If match is found, the node and the previous node are linked
        if (test != NULL)
        {
          link(current, entry);
          if (returnEntry == NULL)
          {
            returnEntry = entry;
          }
          current = entry;
        }
      }
    }
    entry++;
  }
  return returnEntry;
}

//Implementation of Binary Search used to find item associated with id
FoodItem* find_product_id(char* search_term, FoodItem* entry, unsigned* size_of_list)
{
  unsigned begin = 0;
  unsigned end = *size_of_list-1;
  unsigned index;
  unsigned result;
  while (begin <= end)
  {
    unsigned index = (begin + end)/2;
    result = strcmp((entry+index)->product_id, search_term);
    if (result == 0)
    {
      return entry;
    }
    else if (result < 0)
    {
      begin = index + 1;
    }
    else
    {
      end = index - 1;
    }
  }
  return NULL;
}

//links two nodes together
void link(FoodItem* first, FoodItem* second)
{
  second->next = NULL;
  second->previous = first;
  if (first != NULL)
    first->next = second;
}
