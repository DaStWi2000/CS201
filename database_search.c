#include "database_search.h"

void add_item_to_node(FoodItem*, StorageTrie*);
void add_item_to_trie(FoodItem*, StorageTrie*);
void add_tags_to_trie(FoodItem*, StorageTrie*);
StorageTrie* make_trie_entry(char*, StorageTrie*);

//Initializes a trie node
void initialize_node(StorageTrie* node)
{
  //Sets the next node to 0 for every next node
  for (unsigned i = 0; i < 36; i++)
  {
    node->next[i] = 0;
  }

  //Sets capacity and length to 0.
  node->allEntriesLength = 0;
  node->allEntriesCapacity = 0;
}

//Takes the intersection of two sets
//Based on code from "https://www.geeksforgeeks.org/union-and-intersection-of-two-sorted-arrays-2/"
FoodItem** intersection(FoodItem** set1, FoodItem** set2, unsigned l1, unsigned l2, unsigned* l)
{
  //sets counter 1 and counter 2 to 0
  unsigned c1 = 0;
  unsigned c2 = 0;

  //If either set is NULL
  if (!l1 || !l2)
  {
    //The intersection of set A and 0 is 0, so NULL is returned
    return NULL;
  }

  //Length of intersect set is initialized to 0
  *l = 0;

  //Temp variable to store
  int result;

  //The returned set's capacity at the most is the length of the smaller set,
  //by the property of intersection in Set Theory
  FoodItem** returnSet = malloc(sizeof(FoodItem*) * fmin(l1, l2));

  //While there are entries left to look at in either array
  while (c1 < l1 && c2 < l2)
  {
    result = strtol((*(set1 + c1))->product_id, NULL, 10) - strtol((*(set2 + c2))->product_id, NULL, 10);
    if (result < 0)
    {
      c1++;
    }
    else if (result > 0)
    {
      c2++;
    }
    else
    {
      *(returnSet + *l) = *(set1 + c1);
      *l = 1 + *l;
      c1++;
    }
  }

  //Returns the intersecting set
  return returnSet;
}

//Initializes the database item id searching and tag searching
StorageTrie* initialize_database(unsigned* size)
{
  //Initializes the initial root of the trie
  StorageTrie* root = malloc(sizeof(StorageTrie));
  initialize_node(root);
  //Stores array of all database entries in the root node
  //This reduces memory fragmentation
  root->AllEntries = malloc(sizeof(FoodItem*));

  //Reads the database into the array
  FoodItem* entryList = read_database("food_database.csv", size);

  //If the file was unsucceful in being read
  if (!entryList)
  {
    //NULL is returned
    free(root);
    return NULL;
  }

  //Adds each item id to the trie
  for (unsigned i = 0; i < *size; i++)
  {
    add_item_to_trie(entryList + i,root);
  }

  //Sets root's AllEntries list pointer to the pointer to all entries
  *(root->AllEntries) = entryList;
  root->allEntriesCapacity = 1;
  root->allEntriesLength = 1;

  //Initializes list of items sorted by ID
  FoodItem** id_list;
  unsigned id_list_length;
  unsigned id_list_capacity;

  for (unsigned i = 0; i < 10; i++)
  {
    //Gets all child items from next[i]
    id_list_length = 0;
    id_list_capacity = 0;
    id_list = return_child_items(id_list, &id_list_length, &id_list_capacity, 10, root->next[i]);

    //For each item id returned, the food item's name and manufacturer are tagged in the tree
    for (int j = 0; j < id_list_length; j++)
    {
      add_tags_to_trie(*(id_list + j), root);
    }

    //Frees the resulting id from memory
    free(id_list);
  }

  //Returns the root of the trie
  return root;
}

//Frees the database from memory
void free_database(StorageTrie* data, unsigned size)
{
  //Frees each food entry recursively
  for (int i = 0; i < size; i++)
  {
    free_food_entry(*(data->AllEntries)+i);
  }

  //Frees the array of all food items
  free(*(data->AllEntries));

  //Frees the trie from memory
  free_trie(data);
}

//Returns the child items of the node
FoodItem** return_child_items(FoodItem** list, unsigned* listLength,
                              unsigned* listCapacity, unsigned range,
                              StorageTrie* node)
{
  //If the node isn't valid
  if (!node)
  {
    //No items are returned
    return NULL;
  }

  //If the capacity is 0, make the capacity 1
  if (*listCapacity == 0)
  {
    list = malloc(sizeof(FoodItem*));
    *listCapacity = 1;
  }

  //If there are entries in this node,
  if (node->allEntriesLength)
  {
    //If the capacity is too small, double it until the list is large enough
    while (*listLength + node->allEntriesLength >= *listCapacity)
    {
      *listCapacity *= 2;
      list = realloc(list, *listCapacity * sizeof(FoodItem*));
    }

    //Copy the child items to the overall list
    for (unsigned i = 0; i < node->allEntriesLength; i++)
    {
      *(list + i + *listLength) = *(node->AllEntries + i);
    }

    //Update the length of the list
    *listLength = *listLength + node->allEntriesLength;
  }

  //Loops through all the children
  for (unsigned i = 0; i < range; i++)
  {
    //If child node
    if (node->next[i] != NULL)
    {
      //Append child items to list
      list = return_child_items(list, listLength, listCapacity, range, node->next[i]);
    }
  }

  //Returns the list
  return list;
}

//Adds item to the trie
void add_item_to_trie(FoodItem* item, StorageTrie* root)
{
  //Temporary search term to get modified
  char tmp[9];
  strcpy(tmp, item->product_id);

  //Adds the item to the correct node
  add_item_to_node(item, make_trie_entry(tmp, root));
}

//Adds the keywords in each item to the trie
void add_tags_to_trie(FoodItem* item, StorageTrie* root)
{
  //The string that stores the information will be the length of the greater string
  char* tmp = malloc(round(fmax(strlen(item->product_name),strlen(item->manufacturer)))+1);
  char* tokens;

  //Processes the product name
  strcpy(tmp, item->product_name);
  //Search terms can only be alphanumeric
  tokens = strtok(tmp, " ,./<>?;\':\"[]\\{}|`~!@#$%^&*()-=_+\t\n\r");
  //While there are still search terms
  while (tokens != NULL)
  {
    //If the search term is not a NULL string, it gets added to the trie
    if (*tokens != 0)
      add_item_to_node(item, make_trie_entry(tokens, root));

    //Next item
    tokens = strtok(NULL, " ,./<>?;\':\"[]\\{}|`~!@#$%^&*()-=_+\t\n\r");
  }

  //Processes the manufacturer name
  strcpy(tmp, item->manufacturer);
  //Search terms can only be alphanumeric
  tokens = strtok(tmp, " ,./<>?;\':\"[]\\{}|`~!@#$%^&*()-=_+\t\n\r");
  //While there are still search terms
  while (tokens != NULL)
  {
    //If the search term is not a NULL string, it gets added to the trie
    if (*tokens != 0)
      add_item_to_node(item, make_trie_entry(tokens, root));

    //Next item
    tokens = strtok(NULL, " ,./<>?;\':\"[]\\{}|`~!@#$%^&*()-=_+\t\n\r");
  }

  //Frees the tmp variable
  free(tmp);
}

//Adds the specific item to the specific node
void add_item_to_node(FoodItem* item, StorageTrie* node)
{
  //If the length == capacity
  if (node->allEntriesLength == node->allEntriesCapacity)
  {
    //If the capacity of the node is not initialized
    if (!node->allEntriesCapacity)
    {
      //The capacity becomes 1
      node->AllEntries = malloc(sizeof(FoodItem*));
      node->allEntriesCapacity = 1;
    }

    //The capacity is doubled
    else
    {
      node->allEntriesCapacity *= 2;
      node->AllEntries = realloc(node->AllEntries, sizeof(FoodItem*) * node->allEntriesCapacity);
    }
  }

  //If there are no entries, or the product id is not in the node already
  if (!node->allEntriesLength ||
      strcmp(item->product_id, (*(node->AllEntries + node->allEntriesLength - 1))->product_id))
  {
    //The above condition works because this function is called by ascending ID number,
    //So the last entry's ID will always be <= the current item's ID
    *(node->AllEntries + node->allEntriesLength++) = item;
  }
}

//Creates a trie entry for the search term
StorageTrie* make_trie_entry(char* search, StorageTrie* node)
{
  //Cycles through search string
  unsigned i = 0;
  while (search[0])
  {
    //If the character is numeric
    if (search[0] <= '9' && search[0] >= '0')
    {
      //If the child node doesn't exist, it's created and initialized
      if (!(node->next)[search[0]-'0'])
      {
        node->next[search[0]-'0'] = malloc(sizeof(StorageTrie));
        initialize_node(node->next[search[0]-'0']);
      }

      //Goes to the child node
      node = (node->next)[search[0]-'0'];
    }

    //If the character is uppercase
    else if (search[0] <= 'Z' && search[0] >= 'A')
    {
      //If the child node doesn't exist, it's created and initialized
      if (!(node->next)[search[0]-'A'+10])
      {
        node->next[search[0]-'A'+ 10] = malloc(sizeof(StorageTrie));
        initialize_node(node->next[search[0]-'A'+ 10]);
      }

      //Goes to the child node
      node = (node->next)[search[0]-'A'+10];
    }

    //If the character is lowercase
    else if (search[0] <= 'z' && search[0] >= 'a')
    {
      //If the child node doesn't exist, it's created and initialized
      if (!(node->next)[search[0]-'a'+ 10])
      {
        node->next[search[0]-'a'+ 10] = malloc(sizeof(StorageTrie));
        initialize_node(node->next[search[0]-'a'+ 10]);
      }

      //Goes to the child node
      node = (node->next)[search[0]-'a'+10];
    }

    //Deletes the first character
    i = 0;
    while (search[i])
    {
      search[i] = search[i+1];
      i++;
    }
  }

  //Returns the node created
  return node;
}

//Finds if trie entry for the search term exists
StorageTrie* find_trie_entry(char* searchTerm, StorageTrie* root)
{
  //If the search term is NULL
  if (!searchTerm)
  {
    return NULL;
  }

  //Sets up search term
  char* search = malloc(strlen(searchTerm)+1);
  strcpy(search, searchTerm);
  unsigned i = 0;

  //While there are characters left to be searched
  while (search[0])
  {
    //If the character is numeric
    if (search[0] <= '9' && search[0] >= '0')
    {
      //If that letter doesn't exist, there are no items with that tag
      if (root->next[search[0]-'0'] == NULL)
      {
        free(search);
        return NULL;
      }

      //Goes to next child
      root = root->next[search[0]-'0'];
    }

    //If the character is a capital letter
    else if (search[0] <= 'Z' && search[0] >= 'A')
    {
      //If that letter doesn't exist, there are no items with that tag
      if (root->next[search[0]-'A'+10] == NULL)
      {
        free(search);
        return NULL;
      }

      //Goes to next child
      root = root->next[search[0]-'A'+10];
    }

    //If the letter is lower case
    else if (search[0] <= 'z' && search[0] >= 'a')
    {
      //If that letter doesn't exist, there are no items with that tag
      if (root->next[search[0]-'a'+10] == NULL)
      {
        free(search);
        return NULL;
      }

      //Goes to next child
      root = root->next[search[0]-'a'+10];
    }

    //Otherwise, the trie doesn't support special characters
    else
    {
      //Search returns NULL
      free(search);
      return NULL;
    }

    //Deletes first character of the string
    i = 0;
    while (search[i])
    {
      search[i] = search[i+1];
      i++;
    }
  }

  //Returns the entry with the key search term
  free(search);
  return root;
}

//Frees trie from memory
void free_trie(StorageTrie* root)
{
  //Frees all entries array
  if (root->allEntriesCapacity)
    free(root->AllEntries);

  //Frees child nodes
  for (unsigned i = 0; i < 36; i++)
  {
    if (root->next[i] != NULL)
    {
      free_trie(root->next[i]);
    }
  }

  //Frees the root node
  free(root);
}
