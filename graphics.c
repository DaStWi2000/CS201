#include "graphics.h"

void display_food_items(WINDOW*, FoodItem**, unsigned, unsigned);
void display_food_facts(WINDOW*, FoodItem*);
void display_error(WINDOW*, char*);

char* get_window_input(WINDOW* window, int y, int x, int size)
{
  unsigned counter = 0;
  char* data = malloc(size);
  data[size-1] = 0;
  char input = 0;
  while (input != 10)
  {
    input = mvwgetch(window, y, x);
    if (input == 127 && counter > 0)
    {
      mvwaddch(window, y, --x, ' ');
      data[--counter] = 0;
    }
    else if (input == 10 && counter == 0)
    {
      input = 0;
    }
    else if (((input >= 'a' && input <= 'z')
        || (input >= 'A' && input <= 'Z')
        || (input >= '0' && input <= '9')
        || (input == ' '))
        && counter < size-1)
    {
      mvwaddch(window, y, x, input);
      x++;
      data[counter++] = input;
    }
  }
  data[counter] = 0;
  return data;
}

char* user_login_screen()
{
  WINDOW* win = newwin(3, 60, 3, 10);
  WINDOW* info = newwin(5, 60, 7, 10);
  mvwaddstr(win, 1, 1, "Username: ");
  wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
  wrefresh(win);
  waddstr(info, "Welcome to my portfolio project!\n"
                "To continue, please type your name (case sensitive).\n"
                "Only letters and numbers can be in a username.\n"
                "To quit, type 'quit'");
  wrefresh(info);
  char* userName = get_window_input(win, 1, 11, 48);
  if (strlen(userName) + 4 > 48)
  {
    userName = realloc(userName, strlen(userName) + 4);
  }
  unsigned length = strlen(userName);
  userName[length] = '.';
  userName[length + 1] = 'l';
  userName[length + 2] = 'o';
  userName[length + 3] = 'g';
  userName[length + 4] = 0;
  refresh();
  delwin(win);
  delwin(info);
  return userName;
}

void display_food_items(WINDOW* win, FoodItem** list, unsigned length, unsigned start)
{
  char id[9];
  char name[32] = "";
  name[31] = 0;
  char maker[27] = "";
  maker[26] = 0;
  char line[68] = "";
  line[67] = 0;
  wclear(win);
  wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
  for (unsigned i = start; i < length && i - start < 18; i++)
  {
    name[0] = 0;
    maker[0] = 0;
    line[0] = 0;
    strcpy(id, (*(list + i))->product_id);
    strncpy(name, (*(list + i))->product_name, 31);
    strncpy(maker, (*(list + i))->manufacturer, 26);
    if (strlen(name) < 31)
    {
      memset(name + strlen(name), ' ', 31 - strlen(name));
    }
    if (strlen(maker) < 26)
    {
      memset(maker + strlen(maker), ' ', 26 - strlen(maker));
    }
    strcat(line, id);
    strcat(line, "|");
    strcat(line, name);
    strcat(line, "|");
    strcat(line, maker);
    wmove(win, i - start + 1, 1);
    waddstr(win, line);
  }
  wrefresh(win);
}

void display_food_facts(WINDOW* win, FoodItem* item)
{
  char tmp[10];
  wclear(win);
  wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
  wmove(win, 1, 1);
  waddstr(win, "ID: ");
  waddstr(win, item->product_id);
  wmove(win, 2, 1);
  char templen[55];
  waddstr(win, "Product Name: ");
  if (strlen(item->product_name) >= 54)
  {
    memcpy(templen, item->product_name, 54);
    templen[54] = 0;
    waddstr(win, templen);
  }
  else
  {
    waddstr(win, item->product_name);
  }
  wmove(win, 3, 1);
  waddstr(win, "Manufacturer: ");
  if (strlen(item->manufacturer) >= 54)
  {
    memcpy(templen, item->manufacturer, 54);
    templen[54] = 0;
    waddstr(win, templen);
  }
  else
  {
    waddstr(win, item->manufacturer);
  }
  wmove(win, 4, 1);
  sprintf(tmp, "%.1f", roundf(item->energy * 10)/10);
  waddstr(win, "Calories: ");
  waddstr(win, tmp);
  waddstr(win, " kcal");
  wmove(win, 5, 1);
  sprintf(tmp, "%.1f", roundf(item->carbs * 10)/10);
  waddstr(win, "Carbohydrates: ");
  waddstr(win, tmp);
  waddstr(win, " g");
  wmove(win, 6, 1);
  sprintf(tmp, "%.1f", roundf(item->fat * 10)/10);
  waddstr(win, "Fat: ");
  waddstr(win, tmp);
  waddstr(win, " g");
  wmove(win, 7, 1);
  sprintf(tmp, "%.1f", roundf(item->protein * 10)/10);
  waddstr(win, "Protein: ");
  waddstr(win, tmp);
  waddstr(win, " g");
  wmove(win, 8, 1);
  sprintf(tmp, "%.1f", roundf(item->serving_size * 10)/10);
  waddstr(win, "Serving Size: ");
  waddstr(win, tmp);
  waddstr(win, " ");
  waddstr(win, item->serving_size_units);
  wrefresh(win);
}

void display_error(WINDOW* win, char* error)
{
  wclear(win);
  wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
  wmove(win, 1, 1);
  waddstr(win, "ERROR: ");
  waddstr(win, error);
  wrefresh(win);
}

void display_help(WINDOW* win)
{
  wclear(win);
  wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
  wmove(win, 1, 1);
  waddstr(win, "Commands\tSyntax\t\t\tDescription");
  wmove(win, 2, 1);
  waddstr(win, "'search'\tterm1, ..., termn\tSearches the data for the");
  wmove(win, 3, 1);
  waddstr(win, "\t\t\t\t\tterms. Returns the");
  wmove(win, 4, 1);
  waddstr(win, "\t\t\t\t\tintersection of the searches.");
  wmove(win, 5, 1);
  waddstr(win, "'info'\t\tproduct_id\t\tDisplays the information for");
  wmove(win, 6, 1);
  waddstr(win, "\t\t\t\t\tthe given product ID.");
  wrefresh(win);
}

void main_menu(char* fileName)
{
  unsigned size;
  StorageTrie* data = initialize_database(&size);
  DiaryEntry* diary = read_diary(fileName, data);
  WINDOW* search_box = newwin(3, 70, 0, 5);
  wborder(search_box, 0, 0, 0, 0, 0, 0, 0, 0);
  WINDOW* display = newwin(20, 70, 4, 5);
  wborder(display, 0, 0, 0, 0, 0, 0, 0, 0);
  wrefresh(search_box);
  wrefresh(display);
  char* line = malloc(1);
  char* command = 0;
  char* args = 0;
  unsigned foodLength;
  unsigned capacity;
  unsigned previousFoodLength;
  unsigned previousCapacity;
  do
  {
    free(line);
    line = get_window_input(search_box, 1, 1, 68);
    wclear(search_box);
    wborder(search_box, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(search_box);
    command = strtok(line, " \n");
    args = strtok(NULL, "\n");
    if (strcmp(command, "search") == 0)
    {
      char** arguments = malloc(sizeof(char*));
      unsigned c = 1;
      unsigned l = 1;
      *arguments = strtok(args, " ");
      while (*(arguments + l - 1) != NULL)
      {
        if (c == l - 1)
        {
          arguments = realloc(arguments, c * 2 *sizeof(char*));
          c *= 2;
        }
        *(arguments + l) = strtok(NULL, " ");
        l++;
      }
      l--;
      previousFoodLength = 0;
      FoodItem** food = 0;
      FoodItem** previous = 0;
      for (int i = 0; i < l; i++)
      {
        foodLength = 0;
        capacity = 0;
        food = return_child_items(food, &foodLength, &capacity, 36,
                                  find_trie_entry(*(arguments + i), data));
        if (l > 1)
        {
          if (i == 0)
          {
            previousFoodLength = foodLength;
            previous = food;
            foodLength = 0;
            capacity = 0;
            food = 0;
            food = return_child_items(food, &foodLength, &capacity, 36,
                                      find_trie_entry(*(arguments + 1), data));
            i++;
          }
          food = intersection(food, previous, foodLength, previousFoodLength, &foodLength);
          free(previous);
          previous = food;
          previousFoodLength = foodLength;
        }
      }
      if (food && *food)
      {
        display_food_items(display, food, foodLength, 0);
      }
      else
      {
        display_error(display, "No products found.");
      }
      free(arguments);
      free(food);
    }
    else if (strcmp(command, "help") == 0)
    {
      display_help(display);
    }
    else if (strcmp(command, "info") == 0)
    {
      char* argument;
      argument = strtok(args, " \n");
      if (strlen(argument) == 8 && strtol(argument, NULL, 10) != 0L)
      {
        foodLength = 0;
        capacity = 0;
        FoodItem** food;
        food = return_child_items(food, &foodLength, &capacity, 10,
                                  find_trie_entry(argument, data));
        if (food && *food)
          display_food_facts(display, *food);
        else
          display_error(display, "Product ID not found.");
        free(food);
      }
      else
      {
        display_error(display, "Invalid product ID.");
      }
    }
    else if (strcmp(command, "diary") == 0)
    {
      char** arguments = malloc(sizeof(char*));
      unsigned c = 1;
      unsigned l = 1;
      *arguments = strtok(args, " ");
      if (!*arguments)
      {
        display_error(display, "Invalid diary command");
      }
      else
      {
        while (*(arguments + l - 1) != NULL)
        {
          if (c == l - 1)
          {
            arguments = realloc(arguments, c * 2 *sizeof(char*));
            c *= 2;
          }
          *(arguments + l) = strtok(NULL, " ");
          l++;
        }
        l--;
        if (!strcmp(*arguments, "view"))
        {
          display_food_items(display, diary->entries, diary->numEntries, 0);
        }
        else if (!strcmp(*arguments, "write"))
        {
          if (l != 2 && l != 3)
          {
            display_error(display, "Invalid Use of 'diary add'");
          }
          else if (strlen(*(arguments + 1)) == 8 && strtol(*(arguments + 1), NULL, 10) != 0L)
          {
            foodLength = 0;
            capacity = 0;
            FoodItem** food;
            food = return_child_items(food, &foodLength, &capacity, 10,
                                      find_trie_entry(*(arguments + 1), data));
            if (food && *food && l == 2)
            {
              write_item(*food, diary, 1);
            }
            else if (food && *food && l == 3 && strtol(*(arguments + 2), NULL, 10) != 0L)
            {
              write_item(*food, diary, strtol(*(arguments + 2), NULL, 10));
            }
            else
            {
              display_error(display, "Product ID not found.");
            }
            free(food);
          }
          else
          {
            display_error(display, "Invalid Product ID");
          }
        }
        else if (!strcmp(*arguments, "delete"))
        {
          if (l != 2)
          {
            display_error(display, "Invalid Use of 'diary delete'");
          }
          else if (strlen(*(arguments + 1)) == 8 && strtol(*(arguments + 1), NULL, 10) != 0L)
          {
            foodLength = 0;
            capacity = 0;
            FoodItem** food;
            food = return_child_items(food, &foodLength, &capacity, 10,
                                      find_trie_entry(*(arguments + 1), data));
            if (food && *food)
              delete_item(*food, diary);
            else
              display_error(display, "Product ID not found.");
            free(food);
          }
        }
      }
      free(arguments);
    }
    else
    {
      display_error(display, "Invalid Command. Type 'help' for a list of commands.");
    }
  }
  while (strcmp(command, "quit") != 0);
  free(line);
  write_diary(fileName, diary);
  free_diary(diary);
  free_database(data, size);
  delwin(search_box);
  delwin(display);
}
