#include "graphics.h"

//Private Functions
void display_food_items(WINDOW*, FoodItem**, unsigned*, unsigned, unsigned);
void display_food_facts(WINDOW*, FoodItem*);
void display_error(WINDOW*, char*);
char* get_window_input(WINDOW*, unsigned, unsigned, unsigned);
void display_error(WINDOW*, char*);
void display_help(WINDOW*);
void display_page_info(WINDOW*, unsigned, unsigned);

//Input Function. Takes user keyboard input and displays it on the screen.
//When the enter key is pressed, the input is returned as a cstring
char* get_window_input(WINDOW* window, unsigned y, unsigned x, unsigned size)
{
  //Keeps track of the number of keys typed
  unsigned counter = 0;

  //Makes and initializes a dynamic array based on the window size given in parameters
  char* data = malloc(size);
  data[size-1] = 0;

  //Temporary variable used to store the currently typed key
  char input = 0;

  //While the key pressed is not ASCII 10 (Enter key), the loop gets user input
  while (input != 10)
  {
    //Gets Key Press
    input = mvwgetch(window, y, x);

    //If the user types a backspace
    if (input == 127 && counter > 0)
    {
      mvwaddch(window, y, --x, ' ');
      data[--counter] = 0;
    }

    //If the user types an enter key and there's nothing that's been typed
    else if (input == 10 && counter == 0)
    {
      input = 0;
    }

    //If the user types an Alphanumeric key
    else if (((input >= 'a' && input <= 'z')
        || (input >= 'A' && input <= 'Z')
        || (input >= '0' && input <= '9')
        || (input == ' '))
        && counter < size-1)
    {
      //Displays the character and moves the cursor over
      mvwaddch(window, y, x, input);
      x++;
      data[counter++] = input;
    }

    //If the user presses a key that is short for an escape sequence
    else if (input == 0x1B)
    {
      mvwgetch(window, y, x);
      mvwgetch(window, y, x);
    }

    //All other input is ignored
  }

  //Sets the end of the character array after the last letter typed.
  data[counter] = 0;

  //Returns the user input
  return data;
}

//Shows the user login screen. This is where the user types the name of the file
//they want their diet log to be stored.
char* user_login_screen()
{
  //Typing box
  WINDOW* win = newwin(3, 60, 3, 10);

  //Information box
  WINDOW* info = newwin(5, 60, 7, 10);

  //Displays the word 'Username' in the username box
  mvwaddstr(win, 1, 1, "Username: ");

  //Draws a border around the username box
  wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
  wrefresh(win);

  //Outputs the below information to the information box
  waddstr(info, "Welcome to my portfolio project!\n"
                "To continue, please type the name of your diary file.\n"
                "Only letters and numbers can be used as input.\n"
                "To quit, type 'quit'");
  wrefresh(info);

  //Gets the username from the user
  char* userName = get_window_input(win, 1, 11, 48);

  //If the capacity of the cstring is exceeded when '.log' is appended
  if (strlen(userName) + 4 > 48)
  {
    //The cstring is resized
    userName = realloc(userName, strlen(userName) + 4);
  }

  //Appends '.log' to the end of the user input
  unsigned length = strlen(userName);
  userName[length] = '.';
  userName[length + 1] = 'l';
  userName[length + 2] = 'o';
  userName[length + 3] = 'g';
  userName[length + 4] = 0;

  //Cleans up the windows
  refresh();
  delwin(win);
  delwin(info);

  //Returns the name
  return userName;
}

//Displays the food items/quantities given
void display_food_items(WINDOW* win, FoodItem** list, unsigned* quantities, unsigned length, unsigned start)
{
  //Sets the max size of the name and manufacturer displayed
  unsigned nameLength = 34;
  unsigned makerLength = 29;

  //If there are quantities given,
  if (quantities)
  {
    //These sizes are shrunk so that the quantities can also be displayed
    nameLength -= 2;
    makerLength -= 2;
  }

  //Initializes the displayed information (ID, Product Name, Manufacturer, Quantity)
  char id[9];
  char* name = malloc(nameLength + 1);
  name[nameLength] = 0;
  char* maker = malloc(makerLength + 1);
  maker[makerLength] = 0;
  char quantity[4];
  char line[74] = "";

  //Resets the window
  wclear(win);
  wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);

  //Displays the items on page (start - 1). 17 items/page
  for (unsigned i = start * 17; i < length && i - start*17 < 17; i++)
  {
    //Resets the cstrings so that the next line is written
    name[0] = 0;
    maker[0] = 0;
    quantity[0] = 0;
    line[0] = 0;

    //Adds the info in the format ID|NAME|MAKER,
    //making sure each one is formatted with the correct number of spaces
    strcpy(id, (*(list + i))->product_id);
    strncpy(name, (*(list + i))->product_name, nameLength);
    strncpy(maker, (*(list + i))->manufacturer, makerLength);

    //If there are spaces that need to be used to pad the table
    if (strlen(name) < nameLength)
    {
      memset(name + strlen(name), ' ', nameLength - strlen(name));
    }
    if (strlen(maker) < makerLength)
    {
      memset(maker + strlen(maker), ' ', makerLength - strlen(maker));
    }

    //Combines ID, Name, and Manufacturer
    strcat(line, id);
    strcat(line, "|");
    strcat(line, name);
    strcat(line, "|");
    strcat(line, maker);

    //If quantities need to be displayed
    if (quantities)
    {
      //They get displayed
      strcat(line, "|");
      snprintf(quantity, 4, "%3u", *(quantities + i));
      strcat(line, quantity);
    }

    //Moves to the empty line and prints out the info
    wmove(win, i - start*17 + 1, 1);
    waddstr(win, line);
  }

  //Frees cstrings and refreshes the window
  free(name);
  free(maker);
  wrefresh(win);
}

//Displays the nutrition of a specific item
void display_food_facts(WINDOW* win, FoodItem* item)
{
  //Temp variable for storing food information
  char tmp[10];

  //Initializes display window
  wclear(win);
  wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);

  //Prints ID
  wmove(win, 1, 1);
  waddstr(win, "ID: ");
  waddstr(win, item->product_id);

  //Prints Product Name (or however much of it can fit in the window)
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

  //Prints the Manufacturer (or however much of it can fit in the window)
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

  //Prints the nutrition of the item (calories, fat, protein, carbs)
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

  //Prints the serving size
  wmove(win, 8, 1);
  sprintf(tmp, "%.1f", roundf(item->serving_size * 10)/10);
  waddstr(win, "Serving Size: ");
  waddstr(win, tmp);
  waddstr(win, " ");
  waddstr(win, item->serving_size_units);
  wrefresh(win);
}

//Displays the error given
void display_error(WINDOW* win, char* error)
{
  //Resets the window
  wclear(win);
  wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
  wmove(win, 1, 1);

  //Displays the error
  waddstr(win, "ERROR: ");
  waddstr(win, error);
  wrefresh(win);
}

//Displays the help menu
void display_help(WINDOW* win)
{
  wclear(win);
  wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
  wmove(win, 1, 1);
  waddstr(win, "Commands\tSyntax\t\t\tDescription");
  wmove(win, 2, 1);
  waddstr(win, "'search'\tterm1 ... termn\t\tSearches the data for the terms.");
  wmove(win, 3, 40);
  waddstr(win, "Returns the intersection of the");
  wmove(win, 4, 40);
  waddstr(win, "searches.");
  wmove(win, 5, 1);
  waddstr(win, "'info'\t\tproduct_id\t\tDisplays the information for the");
  wmove(win, 6, 40);
  waddstr(win, "given product ID.");
  wmove(win, 7, 1);
  waddstr(win, "'page'\t\tpage_number\t\tSelects the page from the previous");
  wmove(win, 8, 40);
  waddstr(win, "search to be shown.");
  wmove(win, 9, 1);
  waddstr(win, "'diary view'\t\t\t\tShows the first page of the diary.");
  wmove(win, 10, 1);
  waddstr(win, "'diary page'\tpage_number\t\tSelects the diary page to be shown.");
  wmove(win, 11, 1);
  waddstr(win, "'diary write'\tproduct_id [quantity]\tWrites the quantity of product_id");
  wmove(win, 12, 40);
  waddstr(win, "to the diary. If no quantity is");
  wmove(win, 13, 40);
  waddstr(win, "given, 1 is assumed. Can be used to");
  wmove(win, 14, 40);
  waddstr(win, "modify an entry's quantity.");
  wmove(win, 15, 1);
  waddstr(win, "'diary delete'\tproduct_id\t\tDeletes the product_id from the");
  wmove(win, 16, 40);
  waddstr(win, "diary.");
  wmove(win, 17, 1);
  waddstr(win, "'quit'\t\t\t\t\tQuits the program.");
  wrefresh(win);
}

//Displays the page number the user is currently looking at
void display_page_info(WINDOW* win, unsigned currentPage, unsigned totalPages)
{
  //Determines the string equivalent of the page number arguments
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

  //Outputs the page number
  char output[73] = "Page ";

  //If the page numbers are too large
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

  //If it is not
  else
  {
    strncat(output, currentString, 67);
  }

  //Shows the page number info
  wclear(win);
  waddstr(win, output);
  wrefresh(win);

  //Cleans up memory
  free(currentString);
  free(totalString);
}

//Main Menu Function
void main_menu(char* fileName)
{
  //Size of array storing whole database
  unsigned size;

  //If the database is successfully initialized, the pointer to the root of the
  //trie is returned
  StorageTrie* data = initialize_database(&size);

  //If the the data from the file is successfully parsed and indexed
  if (data)
  {
    //Reads the diary
    DiaryEntry* diary = read_diary(fileName, data);

    //Creates and displays the windows
    WINDOW* search_box = newwin(3, 76, 0, 2);
    wborder(search_box, 0, 0, 0, 0, 0, 0, 0, 0);
    WINDOW* display = newwin(19, 76, 4, 2);
    wborder(display, 0, 0, 0, 0, 0, 0, 0, 0);
    WINDOW* page = newwin(1, 76, 23, 2);
    wrefresh(search_box);
    wrefresh(display);

    //Creates variables used to parse command line
    char* line = malloc(1);
    char* command = 0;
    char* args = 0;

    //Stores info about the food lists
    unsigned foodLength;
    unsigned capacity;
    unsigned previousFoodLength;
    FoodItem** food = 0;
    FoodItem** previous = 0;

    //While the user has not typed 'quit'
    do
    {
      //Gets the command + arguments from the user
      free(line);
      line = get_window_input(search_box, 1, 1, 68);

      //Clears the search box
      wclear(search_box);
      wborder(search_box, 0, 0, 0, 0, 0, 0, 0, 0);
      wrefresh(search_box);

      //Parses the command into base command and arguments
      command = strtok(line, " \n");
      args = strtok(NULL, "\n");

      //User typed the search command
      if (!strcmp(command, "search"))
      {
        //If a previous iteration of food is still in memory
        if (food)
        {
          //It gets cleared
          free(food);
          food = 0;
        }

        //Parses arguments into an array of cstrings
        char** arguments = malloc(sizeof(char*));
        //Capacity and length of cstring array
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

        //Resets the previously stored food information
        previousFoodLength = 0;
        previous = 0;

        //While there are search terms to be evaluated
        for (unsigned i = 0; i < l; i++)
        {
          //Gets the current search term's results
          foodLength = 0;
          capacity = 0;
          food = return_child_items(food, &foodLength, &capacity, 36,
                                    find_trie_entry(*(arguments + i), data));

          //If there is more than one item that is searched for
          if (l > 1)
          {
            //If this is the inital iteration
            if (i == 0)
            {
              //It finds the next items values
              previousFoodLength = foodLength;
              previous = food;
              foodLength = 0;
              capacity = 0;
              food = 0;
              food = return_child_items(food, &foodLength, &capacity, 36,
                                        find_trie_entry(*(arguments + 1), data));
              i++;
            }

            //Takes the intersection of the current and previous sets
            food = intersection(food, previous, foodLength, previousFoodLength, &foodLength);
            free(previous);

            //Sets the intersection as the previous set
            previous = food;
            previousFoodLength = foodLength;
          }
        }

        //If there are items returned from the searches
        if (food && *food)
        {
          //The items and #pages are displayed
          display_food_items(display, food, NULL, foodLength, 0);
          display_page_info(page, 1, (foodLength - 1)/17 + 1);
        }
        else
        {
          //Display product finding error and clears page numbers
          display_error(display, "No products found.");
          wclear(page);
          wrefresh(page);
        }

        //Clears arguments from user
        free(arguments);
      }

      //User typed the help command
      else if (!strcmp(command, "help"))
      {
        //Clears previous food and page info from memory
        if (food)
        {
          free(food);
          food = 0;
        }
        wclear(page);
        wrefresh(page);

        //Displays help options
        display_help(display);
      }

      //User typed the info command
      else if (!strcmp(command, "info"))
      {
        //Clears previous food and page info from memory
        if (food)
        {
          free(food);
          food = 0;
        }
        wclear(page);
        wrefresh(page);

        //If it's a valid product id
        if (args && strlen(args) == 8 && strtoul(args, NULL, 10) != 0L)
        {
          //Resets previous food information
          foodLength = 0;
          capacity = 0;
          food = return_child_items(food, &foodLength, &capacity, 10,
                                    find_trie_entry(args, data));

          //If the product was found
          if (food && *food)
            display_food_facts(display, *food);

          //If the product was not found
          else
            display_error(display, "Product ID not found.");

          //Cleans up memory
          free(food);
          food = 0;
        }

        //If it's not a valid product ID
        else
        {
          //Display error and clear page box
          display_error(display, "Invalid product ID.");
          wclear(page);
          wrefresh(page);
        }
      }

      //User typed the diary command
      else if (!strcmp(command, "diary"))
      {
        //If a previous iteration of food is still in memory
        if (food)
        {
          //It gets cleared
          free(food);
          food = 0;
        }

        //Parses arguments into an array of cstrings
        char** arguments = malloc(sizeof(char*));
        //Capacity and length of cstring array
        unsigned c = 1;
        unsigned l = 1;
        *arguments = strtok(args, " ");

        //If there are no arguments
        if (!*arguments)
        {
          //Displays the error and clears page info
          display_error(display, "Invalid diary command");
          wclear(page);
          wrefresh(page);
        }

        //If there are arguments
        else
        {
          //Parses the arguments into seperate arguments
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

          //If the argument to diary is 'view'
          if (!strcmp(*arguments, "view"))
          {
            wclear(page);
            wrefresh(page);

            //Shows the first page of entries in the diary
            display_food_items(display, diary->entries, diary->quantities, diary->numEntries, 0);
            if (diary->numEntries)
              display_page_info(page, 1, (diary->numEntries-1)/17 + 1);
          }

          //If the argument to diary is 'write'
          else if (!strcmp(*arguments, "write"))
          {
            if (l != 2 && l != 3)
            {
              display_error(display, "Invalid Use of 'diary write'");
              wclear(page);
              wrefresh(page);
            }
            else if (strlen(*(arguments + 1)) == 8 && strtoul(*(arguments + 1), NULL, 10) != 0L)
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
                display_page_info(page, 1, (diary->numEntries-1)/17 + 1);
              }
              else if (food && *food && l == 3 && strtoul(*(arguments + 2), NULL, 10) != 0L
                       && strtoul(*(arguments + 2), NULL, 10) < 1000)
              {
                write_item(*food, diary, strtoul(*(arguments + 2), NULL, 10));
                display_food_items(display, diary->entries, diary->quantities, diary->numEntries, 0);
                display_page_info(page, 1, (diary->numEntries-1)/17 + 1);
              }
              else
              {
                display_error(display, "Invalid Product ID or Quantity.");
                wclear(page);
                wrefresh(page);
              }
              free(food);
              food = 0;
            }
            else
            {
              display_error(display, "Invalid Product ID.");
              wclear(page);
              wrefresh(page);
            }
          }

          //If the argument to diary is 'delete'
          else if (!strcmp(*arguments, "delete"))
          {
            //Clears page info
            wclear(page);
            wrefresh(page);

            //If there aren't exactly two arguments
            if (l != 2)
            {
              //Displays error and clears page info
              display_error(display, "Invalid Use of 'diary delete'");
            }

            //If there are two arguments, with the other agument being a valid product ID
            else if (strlen(*(arguments + 1)) == 8 && strtoul(*(arguments + 1), NULL, 10) != 0L)
            {
              //Resets food information
              foodLength = 0;
              capacity = 0;

              //Finds the product id and returns a pointer to the node entry
              food = return_child_items(food, &foodLength, &capacity, 10,
                                        find_trie_entry(*(arguments + 1), data));

              //If the item is found
              if (food && *food)
              {
                //The item is deleted and the first page of the diary is shown
                delete_item(*food, diary);
                display_food_items(display, diary->entries, diary->quantities, diary->numEntries, 0);
                if (diary->numEntries)
                  display_page_info(page, 1, (diary->numEntries-1)/17 + 1);
              }

              //If the item is not found
              else
              {
                //Error is shown and page info is cleared
                display_error(display, "Product ID not found.");
                wclear(page);
                wrefresh(page);
              }
              free(food);
              food = 0;
            }
          }

          //If the argument to diary is 'page'
          else if (!strcmp(*arguments, "page"))
          {
            //If there aren't exactly two arguments
            if (l != 2)
            {
              //Error is displayed and page info cleared
              display_error(display, "Invalid Use of 'diary page'");
              wclear(page);
              wrefresh(page);
            }

            //Otherwise
            else
            {
              //Converts the page number into an unsigned
              unsigned pageNumber = strtoul(*(arguments + 1), NULL, 10);

              //Clears page information
              wclear(page);
              wrefresh(page);

              //If it's a valid page number
              if (pageNumber && pageNumber <= (diary->numEntries-1)/17 + 1)
              {
                display_food_items(display, diary->entries, diary->quantities, diary->numEntries, pageNumber - 1);
                if (diary->numEntries)
                  display_page_info(page, pageNumber, (diary->numEntries-1)/17 + 1);
              }

              //If it's not a valid page number
              else
              {
                //Error is displayed and page info is cleared
                display_error(display, "Invalid Diary Page Number");
              }
            }
          }

          //If the argument to diary is invalid
          else
          {
            //Displays the error and clears page info
            display_error(display, "Invalid diary command");
            wclear(page);
            wrefresh(page);
          }
        }

        //Clears arguments
        free(arguments);
      }

      //User typed the page command
      else if (!strcmp(command, "page"))
      {
        if (args)
        {
          //Converts 2nd argument to a long
          unsigned pageNumber = strtoul(args, NULL, 10);

          //If a valid page number and previous food array is defined
          if (pageNumber && pageNumber <= ((foodLength - 1)/17 + 1) && food)
          {
            //Displays the page of food items specified by the user
            display_food_items(display, food, NULL, foodLength, pageNumber - 1);
            display_page_info(page, pageNumber, (foodLength - 1)/17 + 1);
          }

          //If there is not previous food array defined
          else if (!food)
          {
            //Displays the error and erases page info
            display_error(display, "Please search for an item to select the page number");
            wclear(page);
            wrefresh(page);
          }
          //If it is not a valid page number
          else
          {
            //Displays the error and erases page info
            display_error(display, "Invalid Page Number");
            wclear(page);
            wrefresh(page);
          }        
        }
        //If it is not a valid page number
        else
        {
          //Displays the error and erases page info
          display_error(display, "Invalid Page Number");
          wclear(page);
          wrefresh(page);
        }
      }
      else
      {
        //If the user didn't type quit
        if (strcmp(command, "quit"))
        {
          //It's an invalid command
          display_error(display, "Invalid Command. Type 'help' for a list of commands.");
          wclear(page);
          wrefresh(page);
        }
      }
    }
    while (strcmp(command, "quit") != 0);

    //Frees the command line cstring
    free(line);

    //Writes the diary to the file
    write_diary(fileName, diary);

    //Cleans up the program
    free_diary(diary);
    free_database(data, size);
    delwin(search_box);
    delwin(page);
    delwin(display);
  }
}
