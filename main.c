#include "graphics.h"

int main()
{
  //Intializes Ncurses
  initscr();
  noecho();

  //Gets file name
  char* fileName = user_login_screen();

  //As long as the user didn't say quit, the main menu opens
  if (strcmp(fileName, "quit.log"))
  {
    main_menu(fileName);
  }

  //Ends program
  free(fileName);
  endwin();
  return 0;
}
