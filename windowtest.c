#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>

int main()
{
   initscr();
   clear();
   noecho();
   cbreak();

   //keypad(stdscr,1);

   //time_t t;
   //srand((unsigned) time(&t));

   int l;
   int h;
   int i, j;
   int score;
   char c;
   int ran = 1;
   int increment = 0;
   char pos = 'm';
   char * result;

   getmaxyx(stdscr, h, l);
  
   WINDOW * startup = newwin(h, l, 0, 0);
   /*
   char * middleJet = "\n                                          `==\\/\\==`\n                                ____________\\/__\\____________\n                               \\/____________________________\\\n                                 __||__||__\\/.--.\\__||__||__\n                                \\/__|___|___( >< )___|___|__\\\n                                          _\\/`--`\\_\n                                         (\\/------\\)";
   char * leftJet =  "\n           `==\\/\\==`\n ____________\\/__\\____________\n\\/____________________________\\\n  __||__||__\\/.--.\\__||__||__\n \\/__|___|___( >< )___|___|__\\\n           _\\/`--`\\_\n          (\\/------\\)";
   char * rightJet = "\n                                                                         `==\\/\\==`\n                                                               ____________\\/__\\____________\n                                                              \\/____________________________\\\n                                                                __||__||__\\/.--.\\__||__||__\n                                                               \\/__|___|___( >< )___|___|__\\\n                                                                         _\\/`--`\\_\n                                                                        (\\/------\\)";

   char * leftBird = "              \\\\\n             (o>\n          \\\\_//)\n           \\_/_)\n            _|_";
   char * middleBird = "                                              \\\\\n                                              (o>\n                                           \\\\_//)\n                                            \\_/_)\n                                             _|_";
   char * rightBird = "                                                                              \\\\\n                                                                              (o>\n                                                                           \\\\_//)\n                                                                            \\_/_)\n                                                                             _|_";
   */

   //mvaddstr(1, COLS/2 - 7, "--WindyRoad--");
   //mvprintw(1, ((2.5 * (COLS / 3)) - 2), "Score: %d", score);
  
   
   for(i=l/3; i<l; i+=((l / 3) + 1))
   {
   		for(j=0; j< LINES; j++)
   		{
   			mvwaddch(startup, j, i, ACS_VLINE | WA_BOLD);
   		}
   }
   wrefresh(startup);
   
   
   //border(ACS_VLINE, ACS_VLINE, ACS_S1, ACS_S1, ACS_BULLET, ACS_BULLET, ACS_BULLET, ACS_BULLET);



   WINDOW * leftjet = newwin(10, (int)(l /3), h-10, 0);
   //WINDOW * jetarea = newwin(10, ((COLS / 3) - 2), COLS - 10, 1);

   waddstr(leftjet, "\n           `==\\/\\==`\n ____________\\/__\\____________\n\\/____________________________\\\n  __||__||__\\/.--.\\__||__||__\n \\/__|___|___( >< )___|___|__\\\n           _\\/`--`\\_\n          (\\/------\\)");
   overlay(leftjet, startup);
   wrefresh(startup);
   //wrefresh(startup);
   
   //WINDOW * the_box = newwin(LINES, COLS, 0, 0);

   //overlay(the_box, setup);

  // wrefresh(setup);

   wgetch(startup);
   endwin();
}