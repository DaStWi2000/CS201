#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "diary_io.h"
#include "database_search.h"

char* user_login_screen();
void main_menu(char*);

//All Ncurses code is influenced by the Ncurses programming guide by X. Li
//found at http://www.cs.ukzn.ac.za/~hughm/os/notes/ncurses.html
