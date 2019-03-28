#include "graphics.h"

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
  free(fileName);
  endwin();
  return 0;
}
