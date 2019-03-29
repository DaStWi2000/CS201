#include "graphics.h"

void display_food_items(WINDOW*, FoodItem**, unsigned*, unsigned, unsigned);
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
    else if (input == 0x1B)
    {
      mvwgetch(window, y, x);
      mvwgetch(window, y, x);
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

void display_food_items(WINDOW* win, FoodItem** list, unsigned* quantities, unsigned length, unsigned start)
{
  unsigned nameLength = 34;
  unsigned makerLength = 29;
  if (quantities)
  {
    nameLength -= 2;
    makerLength -= 2;
  }
  char id[9];
  char* name = malloc(nameLength + 1);
  name[nameLength] = 0;
  char* maker = malloc(makerLength + 1);
  maker[makerLength] = 0;
  char quantity[4];
  char line[74] = "";
  line[73] = 0;
  wclear(win);
  wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
  for (unsigned i = start * 17; i < length && i - start*17 < 17; i++)
  {
    name[0] = 0;
    maker[0] = 0;
    quantity[0] = 0;
    line[0] = 0;
    strcpy(id, (*(list + i))->product_id);
    strncpy(name, (*(list + i))->product_name, nameLength);
    strncpy(maker, (*(list + i))->manufacturer, makerLength);
    if (strlen(name) < nameLength)
    {
      memset(name + strlen(name), ' ', nameLength - strlen(name));
    }
    if (strlen(maker) < makerLength)
    {
      memset(maker + strlen(maker), ' ', makerLength - strlen(maker));
    }
    strcat(line, id);
    strcat(line, "|");
    strcat(line, name);
    strcat(line, "|");
    strcat(line, maker);
    if (quantities)
    {
      strcat(line, "|");
      snprintf(quantity, 4, "%3u", *(quantities + i));
      strcat(line, quantity);
    }

    wmove(win, i - start*17 + 1, 1);
    waddstr(win, line);
  }
  free(name);
  free(maker);
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
  char templen[61];
  waddstr(win, "Product Name: ");
  if (strlen(item->product_name) >= 60)
  {
    memcpy(templen, item->product_name, 60);
    templen[60] = 0;
    waddstr(win, templen);
  }
  else
  {
    waddstr(win, item->product_name);
  }
  wmove(win, 3, 1);
  waddstr(win, "Manufacturer: ");
  if (strlen(item->manufacturer) >= 60)
  {
    memcpy(templen, item->manufacturer, 60);
    templen[60] = 0;
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

void display_page_info(WINDOW* win, unsigned currentPage, unsigned totalPages)
{
  unsigned cLength = 0;
  unsigned tLength = 0;
  unsigned tmp = currentPage;
  while (tmp)
  {
    cLength++;
    tmp /= 10;
  }
  tmp = totalPages;
  while (tmp)
  {
    tLength++;
    tmp /= 10;
  }
  char* currentString = malloc(cLength + 1);
  sprintf(currentString, "%u", currentPage);
  char* totalString = malloc(tLength + 1);
  sprintf(totalString, "%u", totalPages);
  char output[73] = "Page ";
  if (66 - strlen(currentString) > 0)
  {
    strcat(output, currentString);
    strcat(output, "/");
    if (strlen(output) + strlen(totalString) < 73)
    {
      strcat(output, totalString);
    }
    else
    {
      strncat(output, totalString, 72 - strlen(output));
    }
  }
  else
  {
    strncat(output, currentString, 67);
  }
  wclear(win);
  waddstr(win, output);
  wrefresh(win);
  free(currentString);
  free(totalString);
}

void main_menu(char* fileName)
{
  unsigned size;
  StorageTrie* data = initialize_database(&size);
  DiaryEntry* diary = read_diary(fileName, data);
  WINDOW* search_box = newwin(3, 76, 0, 2);
  wborder(search_box, 0, 0, 0, 0, 0, 0, 0, 0);
  WINDOW* display = newwin(19, 76, 4, 2);
  wborder(display, 0, 0, 0, 0, 0, 0, 0, 0);
  WINDOW* page = newwin(1, 76, 23, 2);
  wrefresh(search_box);
  wrefresh(display);
  char* line = malloc(1);
  char* command = 0;
  char* args = 0;
  unsigned foodLength;
  unsigned capacity;
  unsigned previousFoodLength;
  unsigned previousCapacity;
  FoodItem** food = 0;
  FoodItem** previous = 0;
  do
  {
    free(line);
    line = get_window_input(search_box, 1, 1, 68);
    wrefresh(page);
    wclear(search_box);
    wborder(search_box, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(search_box);
    command = strtok(line, " \n");
    args = strtok(NULL, "\n");
    if (!strcmp(command, "search"))
    {
      if (food)
      {
        free(food);
        food = 0;
      }
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
      food = 0;
      previous = 0;
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
        display_food_items(display, food, NULL, foodLength, 0);
        display_page_info(page, 1, foodLength/17 + 1);
      }
      else
      {
        display_error(display, "No products found.");
        wclear(page);
        wrefresh(page);
      }
      free(arguments);
    }
    else if (!strcmp(command, "help"))
    {
      if (food)
      {
        free(food);
        food = 0;
      }
      wclear(page);
      wrefresh(page);
      display_help(display);
    }
    else if (!strcmp(command, "info"))
    {
      if (food)
      {
        free(food);
        food = 0;
      }
      wclear(page);
      wrefresh(page);
      char* argument;
      argument = strtok(args, " ");
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
    else if (!strcmp(command, "diary"))
    {
      if (food)
      {
        free(food);
        food = 0;
      }
      char** arguments = malloc(sizeof(char*));
      unsigned c = 1;
      unsigned l = 1;
      *arguments = strtok(args, " ");
      if (!*arguments)
      {
        display_error(display, "Invalid diary command");
        wclear(page);
        wrefresh(page);
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
          display_food_items(display, diary->entries, diary->quantities, diary->numEntries, 0);
          display_page_info(page, 1, diary->numEntries/17 + 1);
        }
        else if (!strcmp(*arguments, "write"))
        {
          if (l != 2 && l != 3)
          {
            display_error(display, "Invalid Use of 'diary write'");
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
              display_food_items(display, diary->entries, diary->quantities, diary->numEntries, 0);
            }
            else if (food && *food && l == 3 && strtol(*(arguments + 2), NULL, 10) != 0L
                     && strtol(*(arguments + 2), NULL, 10) < 1000)
            {
              write_item(*food, diary, strtol(*(arguments + 2), NULL, 10));
              display_food_items(display, diary->entries, diary->quantities, diary->numEntries, 0);
              display_page_info(page, 1, diary->numEntries/17 + 1);
            }
            else
            {
              display_error(display, "Invalid Product ID or Quantity.");
            }
            free(food);
          }
          else
          {
            display_error(display, "Invalid Product ID.");
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
            {
              delete_item(*food, diary);
              display_food_items(display, diary->entries, diary->quantities, diary->numEntries, 0);
            }
            else
            {
              display_error(display, "Product ID not found.");
            }
            free(food);
          }
        }
        else if (!strcmp(*arguments, "page"))
        {
          if (l != 2)
          {
            display_error(display, "Invalid Use of 'diary page'");
          }
          else
          {
            unsigned pageNumber = strtol(*(arguments + 1), NULL, 10);
            if (pageNumber && pageNumber <= diary->numEntries/17 + 1)
            {
              display_food_items(display, diary->entries, diary->quantities, diary->numEntries, pageNumber - 1);
              display_page_info(page, 1, diary->numEntries/17 + 1);
            }
            else
            {
              display_error(display, "Invalid Diary Page Number");
              wclear(page);
              wrefresh(page);
            }
          }
        }
        else
        {
          display_error(display, "Invalid diary command");
          wclear(page);
          wrefresh(page);
        }
      }
      free(arguments);
    }
    else if (!strcmp(command, "page"))
    {
      char* argument;
      argument = strtok(args, " ");
      unsigned pageNumber = strtol(argument, NULL, 10);
      if (pageNumber && pageNumber <= (foodLength/17 + 1) && food)
      {
        display_food_items(display, food, NULL, foodLength, pageNumber - 1);
        display_page_info(page, pageNumber, foodLength/17 + 1);
      }
      else if (!food)
      {
        display_error(display, "Please search for an item to select the page number");
        wclear(page);
        wrefresh(page);
      }
      else
      {
        display_error(display, "Invalid Page Number");
        wclear(page);
        wrefresh(page);
      }
    }
    else
    {
      display_error(display, "Invalid Command. Type 'help' for a list of commands.");
      wclear(page);
      wrefresh(page);
    }
  }
  while (strcmp(command, "quit") != 0);
  free(line);
  write_diary(fileName, diary);
  free_diary(diary);
  free_database(data, size);
  delwin(search_box);
  delwin(page);
  delwin(display);
}
