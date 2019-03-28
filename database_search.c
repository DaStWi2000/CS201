#include "database_search.h"

void add_item_to_node(FoodItem*, StorageTrie*);
void add_item_to_trie(FoodItem*, StorageTrie*);
void add_tags_to_trie(FoodItem*, StorageTrie*);
StorageTrie* make_trie_entry(char*, StorageTrie*);


void initialize_node(StorageTrie* node)
{
  for (unsigned i = 0; i < 36; i++)
  {
    node->next[i] = 0;
  }
  node->allEntriesLength = 0;
  node->allEntriesCapacity = 0;
}

FoodItem** intersection(FoodItem** set1, FoodItem** set2, unsigned l1, unsigned l2, unsigned* l)
{
  unsigned c1 = 0;
  unsigned c2 = 0;
  if (!l1 || !l2)
  {
    return NULL;
  }
  *l = 0;
  int result;
  FoodItem** returnSet = malloc(sizeof(FoodItem*) * fmax(l1, l2));
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
  return returnSet;
}

StorageTrie* initialize_database(unsigned* size)
{
  StorageTrie* root = malloc(sizeof(StorageTrie));
  initialize_node(root);
  root->AllEntries = malloc(sizeof(FoodItem*));
  FoodItem* entryList = read_database("database", size);
  for (unsigned i = 0; i < *size; i++)
  {
    add_item_to_trie(entryList + i,root);
  }
  *(root->AllEntries) = entryList;
  root->allEntriesCapacity = 1;
  root->allEntriesLength = 1;
  FoodItem** id_list;
  unsigned id_list_length;
  unsigned id_list_capacity;
  for (unsigned i = 0; i < 10; i++)
  {
    id_list_length = 0;id_list_capacity = 0;
    id_list = return_child_items(id_list, &id_list_length, &id_list_capacity, 10, root->next[i]);
    for (int j = 0; j < id_list_length; j++)
    {
      add_tags_to_trie(*(id_list + j), root);
    }
    free(id_list);
  }
  return root;
}

void free_database(StorageTrie* data, unsigned size)
{
  for (int i = 0; i < size; i++)
  {
    free_food_entry(*(data->AllEntries)+i);
  }
  free(*(data->AllEntries));
  free_trie(data);
}

FoodItem** return_child_items(FoodItem** list, unsigned* listLength,
                              unsigned* listCapacity, unsigned range,
                              StorageTrie* node)
{
  if (!node)
  {
    return NULL;
  }
  if (*listCapacity == 0)
  {
    list = malloc(sizeof(FoodItem*) * 2);
    *listCapacity = 2;
  }
  if (node->allEntriesLength)
  {
    while (*listLength + node->allEntriesLength >= *listCapacity)
    {
      *listCapacity *= 2;
      list = realloc(list, *listCapacity * sizeof(FoodItem*));
    }
    for (unsigned i = 0; i < node->allEntriesLength; i++)
    {
      *(list + i + *listLength) = *(node->AllEntries + i);
    }
    *listLength = *listLength + node->allEntriesLength;
  }
  for (unsigned i = 0; i < range; i++)
  {
    if (node->next[i] != NULL)
    {
      list = return_child_items(list, listLength, listCapacity, range, node->next[i]);
    }
  }
  return list;
}

void add_item_to_trie(FoodItem* item, StorageTrie* root)
{
  char tmp[9];
  strcpy(tmp, item->product_id);
  add_item_to_node(item, make_trie_entry(tmp, root));
}

void add_tags_to_trie(FoodItem* item, StorageTrie* root)
{
  char* tmp = malloc(round(fmax(strlen(item->product_name),strlen(item->manufacturer)))+1);
  char* tokens;
  strcpy(tmp, item->product_name);
  tokens = strtok(tmp, " ,./<>?;\':\"[]\\{}|`~!@#$%^&*()-=_+\t\n\r");
  while (tokens != NULL)
  {
    if (*tokens != 0)
      add_item_to_node(item, make_trie_entry(tokens, root));
    tokens = strtok(NULL, " ,./<>?;\':\"[]\\{}|`~!@#$%^&*()-=_+\t\n\r");
  }
  strcpy(tmp, item->manufacturer);
  tokens = strtok(tmp, " ,./<>?;\':\"[]\\{}|`~!@#$%^&*()-=_+\t\n\r");
  while (tokens != NULL)
  {
    if (*tokens != 0)
      add_item_to_node(item, make_trie_entry(tokens, root));
    tokens = strtok(NULL, " ,./<>?;\':\"[]\\{}|`~!@#$%^&*()-=_+\t\n\r");
  }
  free(tmp);
}

void add_item_to_node(FoodItem* item, StorageTrie* node)
{
  if (node->allEntriesLength == node->allEntriesCapacity)
  {
    if (!node->allEntriesCapacity)
    {
      node->AllEntries = malloc(sizeof(FoodItem*) * 2);
      node->allEntriesCapacity = 2;
    }
    else
    {
      node->allEntriesCapacity *= 2;
      node->AllEntries = realloc(node->AllEntries, sizeof(FoodItem*) * node->allEntriesCapacity);
    }
  }
  if (!node->allEntriesLength ||
      strcmp(item->product_id, (*(node->AllEntries + node->allEntriesLength - 1))->product_id))
  {
    *(node->AllEntries + node->allEntriesLength++) = item;
  }
}

StorageTrie* make_trie_entry(char* search, StorageTrie* node)
{
  unsigned i = 0;
  while (search[0])
  {
    if (search[0] <= '9' && search[0] >= '0')
    {
      if (!(node->next)[search[0]-'0'])
      {
        node->next[search[0]-'0'] = malloc(sizeof(StorageTrie));
        initialize_node(node->next[search[0]-'0']);
      }
      node = (node->next)[search[0]-'0'];
    }
    else if (search[0] <= 'Z' && search[0] >= 'A')
    {
      if (!(node->next)[search[0]-'A'+10])
      {
        node->next[search[0]-'A'+ 10] = malloc(sizeof(StorageTrie));
        initialize_node(node->next[search[0]-'A'+ 10]);
      }
      node = (node->next)[search[0]-'A'+10];
    }
    else if (search[0] <= 'z' && search[0] >= 'a')
    {
      if (!(node->next)[search[0]-'a'+ 10])
      {
        node->next[search[0]-'a'+ 10] = malloc(sizeof(StorageTrie));
        initialize_node(node->next[search[0]-'a'+ 10]);
      }
      node = (node->next)[search[0]-'a'+10];
    }
    i = 0;
    while (search[i])
    {
      search[i] = search[i+1];
      i++;
    }
  }
  return node;
}

StorageTrie* find_trie_entry(char* searchTerm, StorageTrie* root)
{
  if (!searchTerm)
  {
    return NULL;
  }
  char* search = malloc(strlen(searchTerm)+1);
  strcpy(search, searchTerm);
  unsigned i = 0;
  while (search[0])
  {
    if (search[0] <= '9' && search[0] >= '0')
    {
      if (root->next[search[0]-'0'] == NULL)
      {
        free(search);
        return NULL;
      }
      root = root->next[search[0]-'0'];
    }
    else if (search[0] <= 'Z' && search[0] >= 'A')
    {
      if (root->next[search[0]-'A'+10] == NULL)
      {
        free(search);
        return NULL;
      }
      root = root->next[search[0]-'A'+10];
    }
    else if (search[0] <= 'z' && search[0] >= 'a')
    {
      if (root->next[search[0]-'a'+10] == NULL)
      {
        free(search);
        return NULL;
      }
      root = root->next[search[0]-'a'+10];
    }
    else
    {
      free(search);
      return NULL;
    }
    i = 0;
    while (search[i])
    {
      search[i] = search[i+1];
      i++;
    }
  }
  free(search);
  return root;
}

void free_trie(StorageTrie* root)
{
  if (root->allEntriesCapacity)
    free(root->AllEntries);
  for (unsigned i = 0; i < 36; i++)
  {
    if (root->next[i] != NULL)
    {
      free_trie(root->next[i]);
    }
  }
  free(root);
}
