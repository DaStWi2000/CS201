#include "database_search.h"

void initialize_node(StorageTrie* node)
{
  for (unsigned i = 0; i < 36; i++)
  {
    node->next[i] = 0;
  }
  node->allEntriesLength = 0;
  node->allEntriesCapacity = 0;
}

FoodItem** search_items(char* searchTerm, unsigned* length, StorageTrie* root)
{
  unsigned i = 0;
  char* search = malloc(strlen(searchTerm)+1);
  strcpy(search, searchTerm);
  while (search[0])
  {
    if (search[0] <= '9' && search[0] >= '0')
    {
      if (root->next[search[0]-'0'] == NULL)
      {
        return NULL;
      }
      root = root->next[search[0]-'0'];
    }
    else if (search[0] <= 'Z' && search[0] >= 'A')
    {
      if (root->next[search[0]-'A'+10] == NULL)
      {
        return NULL;
      }
      root = root->next[search[0]-'A'+10];
    }
    else if (search[0] <= 'z' && search[0] >= 'a')
    {
      if (root->next[search[0]-'a'+10] == NULL)
      {
        return NULL;
      }
      root = root->next[search[0]-'a'+10];
    }
    else
    {
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
  FoodItem** list = malloc(sizeof(FoodItem*)*2);
  unsigned* listCapacity = malloc(sizeof(unsigned));
  *length = 0;
  *listCapacity = 2;
  list = return_child_items(list,length,listCapacity,root);
  free(listCapacity);
  return list;
}

FoodItem** return_child_items(FoodItem** list, unsigned* listLength, unsigned* listCapacity, StorageTrie* node)
{
  if (node->allEntriesLength)
  {
    while (*listLength + node->allEntriesLength > *listCapacity)
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
  for (unsigned i = 0; i < 36; i++)
  {
    if (node->next[i] != NULL)
    {
      list = return_child_items(list, listLength, listCapacity, node->next[i]);
    }
  }
  return list;
}

void add_item_to_trie(FoodItem* item, StorageTrie* root)
{
  char* tmp = malloc(round(fmax(fmax(strlen(item->product_id), strlen(item->product_name)),strlen(item->manufacturer)))+1);
  strcpy(tmp, item->product_id);
  add_item_to_node(item, find_trie_node(tmp, root));
  char* tokens;
  strcpy(tmp, item->product_name);
  tokens = strtok(tmp, " ,./<>?;\':\"[]\\{}|`~!@#$%^&*()-=_+\t\n\r");
  while (tokens != NULL)
  {
    if (*tokens != 0)
      add_item_to_node(item, find_trie_node(tokens, root));
    tokens = strtok(NULL, " ,./<>?;\':\"[]\\{}|`~!@#$%^&*()-=_+\t\n\r");
  }
  strcpy(tmp, item->manufacturer);
  tokens = strtok(tmp, " ,./<>?;\':\"[]\\{}|`~!@#$%^&*()-=_+\t\n\r");
  while (tokens != NULL)
  {
    if (*tokens != 0)
      add_item_to_node(item, find_trie_node(tokens, root));
    tokens = strtok(NULL, " ,./<>?;\':\"[]\\{}|`~!@#$%^&*()-=_+\t\n\r");
  }
  free(tmp);
}

void add_item_to_node(FoodItem* item, StorageTrie* node)
{
  if (node->allEntriesLength == node->allEntriesCapacity)
  {
    if (!node->allEntriesLength)
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
  *(node->AllEntries + node->allEntriesLength++) = item;
}

StorageTrie* find_trie_node(char* search, StorageTrie* node)
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
