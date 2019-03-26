#include "graphics.h"

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
    strcat(line, " ");
    strcat(line, name);
    strcat(line, " ");
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

void main_menu(char* fileName)
{
  unsigned size;
  StorageTrie* data = initialize_database(&size);
  unsigned* diaryEntries = malloc(sizeof(unsigned));
  unsigned* diaryCapacity = malloc(sizeof(unsigned));
  DiaryEntry* diary = read_diary(fileName, diaryEntries, diaryCapacity, data);
  //tester(data, diary, fileName, diaryEntries);
  WINDOW* search_box = newwin(3, 70, 0, 5);
  wborder(search_box, 0, 0, 0, 0, 0, 0, 0, 0);
  WINDOW* display = newwin(20, 70, 4, 5);
  wborder(display, 0, 0, 0, 0, 0, 0, 0, 0);
  WINDOW* error = newwin(1, 70, 3, 5);
  wrefresh(search_box);
  wrefresh(display);
  char* line = malloc(1);
  char* command = 0;
  char* args = 0;
  unsigned foodLength;
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
      char* argument;
      argument = strtok(args, " \n");
      foodLength = 0;
      FoodItem** food = search_items(argument, &foodLength, data);
      display_food_items(display, food, foodLength, 0);
      free(food);
    }
    else if (strcmp(command, "help") == 0)
    {

    }
    else if (strcmp(command, "info") == 0)
    {
      char* argument;
      argument = strtok(args, " \n");
      if (strlen(argument) == 8 || strtol(argument, NULL, 10) != 0L)
      {
        foodLength = 0;
        FoodItem** food = search_items(argument, &foodLength, data);
        if (*food)
          display_food_facts(display, *food);
        free(food);
      }
    }
    else
    {

    }
  }
  while (strcmp(command, "quit") != 0);
  free_diary(diary, diaryEntries, diaryCapacity);
  free_database(data, size);
  free(fileName);
  delwin(search_box);
  delwin(display);
  delwin(error);
}
