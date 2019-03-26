#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "diary_io.h"
#include "database_search.h"

char* get_window_input(WINDOW*, int, int, int);
char* user_login_screen();
void display_food_items(WINDOW*, FoodItem**, unsigned, unsigned);
void main_menu(char*);
