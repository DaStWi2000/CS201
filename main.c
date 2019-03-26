//#include <math.h>
#include "graphics.h"
//#include "database_io.h"
//#include "database_search.h"
//#include "diary_io.h"

/*void tester(StorageTrie* data, DiaryEntry* diary, char* fileName, unsigned* diaryEntries)
{
  unsigned* temp = malloc(sizeof(unsigned));
  FoodItem** food = search_items("45001542", temp, data);
  write_item(*food,diary);
  write_diary(fileName, diary, *diaryEntries);
  free(temp);
  free(food);
}*/

int main()
{
  initscr();
  noecho();
  char* fileName = user_login_screen();
  if (strcmp(fileName, "quit.log"))
  {
    main_menu(fileName);
  }
  endwin();
  return 0;
}
