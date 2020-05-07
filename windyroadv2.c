#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include "windyroad.h"
#include <unistd.h>

//To-do
//0. Use all lowercase function names rather than camelCase.
//1. Blink the column once locked-in.
//2. Delete cursor.
//3 Redraw on window change.
//4 See if I can only have one bird show at a time.
//5 Refactor methods that are repeated.
//6 If color is enabled, add support for color.
//7 Study winexample.c and do #3.
//8 Avoid having parameters of WINDOW * that are defined in main.


void restore_jet(WINDOW * win, int h)
{
	
	mvwaddstr(win, h-10, 1 ,"\n           `==\\/\\==`\n ____________\\/__\\____________\n\\/____________________________\\\n  __||__||__\\/.--.\\__||__||__\n \\/__|___|___( >< )___|___|__\\\n           _\\/`--`\\_\n          (\\/------\\)");
}

void overlay_restored_jet(char c, WINDOW * leftJet, WINDOW * middleJet, WINDOW * rightJet, WINDOW * leftChunk, WINDOW * middleChunk, WINDOW * rightChunk, WINDOW * setup)
{
	if(c == 'l')
	{
		overwrite(leftJet, leftChunk);
		
	}

	if(c == 'm')
	{
		overwrite(middleJet, middleChunk);		
	}

	if(c == 'r')
	{
		overwrite(rightJet, rightChunk);
	}
}

void clear_corresponding(char c, WINDOW * leftChunk, WINDOW * middleChunk, WINDOW * rightChunk, WINDOW * setup)
{
	if(c == 'l')
	{
		werase(leftChunk);
		wrefresh(leftChunk);
		overwrite(leftChunk, setup);
		wrefresh(setup);
	}

	if(c == 'm')
	{
		werase(middleChunk);
		wrefresh(middleChunk);
		overwrite(middleChunk, setup);
		wrefresh(setup);
	}

	if(c == 'r')
	{
		werase(rightChunk);
		wrefresh(rightChunk);
		overwrite(rightChunk, setup);
		wrefresh(setup);
	}
}

static int assert_dimension()
{
   if((LINES < 30) || (COLS < 95))
   {
   		return 1;
   		
   }
   return 0;
}


char find_corresponding(int r)
{
	if(r == 1)
	{
		return 'l';
	}
	if(r == 2)
	{
		return 'm';
	}

	if(r == 3)
	{
		return 'r';
	}

	return '\0';
}

WINDOW * find_correspondingJet(char c, WINDOW * leftJet, WINDOW * middleJet, WINDOW * rightJet, WINDOW * setup)
{
	if(c == 'l')
	{
		return leftJet;
	}
	if(c == 'm')
	{
		return middleJet;
	}
	if(c == 'r')
	{
		return rightJet;
	}
	return NULL;
}

int main()
{
  
   initscr();
   clear();
   noecho();
   cbreak();

   keypad(stdscr,1);

   time_t t;
   srand((unsigned) time(&t));

   int i;
   int j;
   int h, l;
   int len;
   int height;
   int score;
   char c;
   int ran = 1;
   int increment = 0;
   char pos = 'm';
   char pos1 = ' ';

   /*
   char * middleJet = "\n                                          `==\\/\\==`\n                                ____________\\/__\\____________\n                               \\/____________________________\\\n                                 __||__||__\\/.--.\\__||__||__\n                                \\/__|___|___( >< )___|___|__\\\n                                          _\\/`--`\\_\n                                         (\\/------\\)";
   char * leftJet =  "\n           `==\\/\\==`\n ____________\\/__\\____________\n\\/____________________________\\\n  __||__||__\\/.--.\\__||__||__\n \\/__|___|___( >< )___|___|__\\\n           _\\/`--`\\_\n          (\\/------\\)";
   char * rightJet = "\n                                                                         `==\\/\\==`\n                                                               ____________\\/__\\____________\n                                                              \\/____________________________\\\n                                                                __||__||__\\/.--.\\__||__||__\n                                                               \\/__|___|___( >< )___|___|__\\\n                                                                         _\\/`--`\\_\n                                                                        (\\/------\\)";

   char * leftBird = "              \\\\\n             (o>\n          \\\\_//)\n           \\_/_)\n            _|_";
   char * middleBird = "                                              \\\\\n                                              (o>\n                                           \\\\_//)\n                                            \\_/_)\n                                             _|_";
   char * rightBird = "                                                                              \\\\\n                                                                              (o>\n                                                                           \\\\_//)\n                                                                            \\_/_)\n                                                                             _|_";
   */

   WINDOW * setup = newwin(LINES, COLS, 0, 0);


   getmaxyx(stdscr, h, l);

   //Window Initialization
   WINDOW * leftChunk = newwin(h, l /3, 1, 0);
   WINDOW * middleChunk = newwin(h, l/3, 1, l/3 + 1);
   WINDOW * rightChunk = newwin(h, l/3, 1, ((2 * l) / 3) + 2);
   
   WINDOW * leftJet = newwin(10, l /3, h-9, 0);
   WINDOW * middleJet = newwin(10, l/3, h-9, l/3 + 1);
   WINDOW * rightJet = newwin(10, l/3, h-9, ((2 * l) / 3) + 2);

   WINDOW * leftBird1 = newwin(18, 7, 1, 0);
   WINDOW * leftBird2 = newwin(18, 7, 7, 0);
   WINDOW * leftBird3 = newwin(18, 7, 13, 0);

   WINDOW * middleBird1 = newwin(18, 7, 1, l/3 + 1);
   WINDOW * middleBird2 = newwin(18, 7, 7, l/3 + 1);
   WINDOW * middleBird3 = newwin(18, 7, 13, l/3 + 1);
   
   WINDOW * rightBird1 = newwin(18, 7, 1, ((2 * l) / 3) + 2);
   WINDOW * rightBird2 = newwin(18, 7, 7, ((2 * l) / 3) + 2);
   WINDOW * rightBird3 = newwin(18, 7, 13, ((2 * l) / 3) + 2);
  

   waddstr(leftJet, "\n           `==\\/\\==`\n ____________\\/__\\____________\n\\/____________________________\\\n  __||__||__\\/.--.\\__||__||__\n \\/__|___|___( >< )___|___|__\\\n           _\\/`--`\\_\n          (\\/------\\)");
   waddstr(middleJet, "\n           `==\\/\\==`\n ____________\\/__\\____________\n\\/____________________________\\\n  __||__||__\\/.--.\\__||__||__\n \\/__|___|___( >< )___|___|__\\\n           _\\/`--`\\_\n          (\\/------\\)");
   waddstr(rightJet, "\n           `==\\/\\==`\n ____________\\/__\\____________\n\\/____________________________\\\n  __||__||__\\/.--.\\__||__||__\n \\/__|___|___( >< )___|___|__\\\n           _\\/`--`\\_\n          (\\/------\\)");

   waddstr(leftBird1, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\r\n  _|_");
   waddstr(leftBird2, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\r\n  _|_");
   waddstr(leftBird3, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\r\n  _|_");
   waddstr(middleBird1, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\r\n  _|_");
   waddstr(middleBird2, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\r\n  _|_");
   waddstr(middleBird3, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\r\n  _|_");
   waddstr(rightBird1, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\r\n  _|_");
   waddstr(rightBird2, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\r\n  _|_");
   waddstr(rightBird3, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\r\n  _|_");

   

   mvwprintw(setup, 0, 0, "Press up-arrow to lock position.");
   mvwprintw(setup, 0, l / 2 - 7, "--WindyRoad--");
   mvwprintw(setup, 0, ((5 * l) / 6) - 7, "Score: %d", score);
  
   //Background Initalization
   for(i=l/3; i<l; i+=((l / 3) + 1))
   {
   		for(j=0; j< h; j++)
   		{
   			mvwaddch(setup, j, i, ACS_VLINE);
   		}
   }
   
   //Show Middle Jet before game begins..
   overlay(leftJet, leftChunk);
   overlay(middleJet, middleChunk);
   overlay(rightJet, rightChunk);
   
   overlay(middleChunk, setup);
   wrefresh(setup);

 

    //Logic :)

    		
	    	
	    	while(true)
	    	{

	    		if(assert_dimension())
	    		{
	    			endwin();
	    			puts("Your terminal needs to be at least 95 X 30...");
	    			exit(1);
	    		}
		    	
		    	keypad(setup, true);
		    	char c = wgetch(setup);

		    	if(c != (char)KEY_UP)
		    	{

			    	if(c == (char)KEY_LEFT)
			    	{
			    		if(pos == 'l')
			    		{
			    			continue;
			    		}

		   				if(pos == 'm')
		   				{
			    			werase(middleChunk);
			    			wrefresh(middleChunk);
			    			overwrite(middleChunk, setup);
			    			wrefresh(setup);
			    			overwrite(leftChunk, setup);
			    			wrefresh(setup);
			    			pos = 'l';
			    			restore_jet(middleChunk, h);
			    		}
			    		


			    		if(pos == 'r')
			    		{
			    			werase(rightChunk);
			    			wrefresh(rightChunk);
			    			overwrite(rightChunk, setup);
			    			wrefresh(setup);
			    			overwrite(middleChunk, setup);
			    			wrefresh(setup);
			    			pos = 'm';
			    			restore_jet(rightChunk, h);
			    		}
			    	}

			    	if(c == (char) KEY_RIGHT)
			    	{
			    		if(pos == 'r')
			    		{
			    			continue;
			    		}
			 
			    		if(pos == 'm')
			    		{
			    			werase(middleChunk);
			    			wrefresh(middleChunk);
			    			overwrite(middleChunk, setup);
			    			wrefresh(setup);
			    			overwrite(rightChunk, setup);
			    			wrefresh(setup);
			    			pos = 'r';
			    			restore_jet(middleChunk, h);
			    		}

			    		if(pos == 'l')
			    		{
			    			werase(leftChunk);
			    			wrefresh(leftChunk);
			    			overwrite(leftChunk, setup);
			    			wrefresh(setup);
			    			overwrite(middleChunk, setup);
			    			wrefresh(setup);
			    			pos = 'm';
			    			restore_jet(leftChunk, h);
			    		}
			    	}
			    }

			 	else
			 	{
		    		ran = (rand() % 3) + 1;
		    		if(ran == 1)
		    		{
		    			if(find_corresponding(ran) != pos)
		    			{
		    				werase(leftChunk);
		    				wrefresh(leftChunk);
		    				overwrite(leftChunk, setup);
		    				wrefresh(setup);
		    			}

		    			overlay(leftBird1, leftChunk);
		    			overwrite(leftChunk, setup);
		    			wrefresh(setup);
		    			sleep(1);
		    			overlay(leftBird2, leftChunk);
		    			overwrite(leftChunk, setup);
		    			wrefresh(setup);
		    			sleep(1);
		    			overlay(leftBird3, leftChunk);
		    			overwrite(leftChunk, setup);
		    			wrefresh(setup);
		    			pos1 = 'l';
		    		}

		    		if(ran == 2)
		    		{
		    			if(find_corresponding(ran) != pos)
		    			{
		    				werase(middleChunk);
		    				wrefresh(middleChunk);
		    				overwrite(middleChunk, setup);
		    				wrefresh(setup);
		    			}

		    			overlay(middleBird1, middleChunk);
		    			overwrite(middleChunk, setup);
		    			wrefresh(setup);
		    			sleep(1);
		    			overlay(middleBird2, middleChunk);
		    			overwrite(middleChunk, setup);
		    			wrefresh(setup);
		    			sleep(1);
		    			overlay(middleBird3, middleChunk);
		    			overwrite(middleChunk, setup);
		    			wrefresh(setup);
		    			pos1= 'm';	
		    		}
		    		if(ran == 3)
		    		{
		    			if(find_corresponding(ran) != pos)
		    			{
		    				werase(rightChunk);
		    				wrefresh(rightChunk);
		    				overwrite(rightChunk, setup);
		    				wrefresh(setup);
		    			}

		    			overlay(rightBird1, rightChunk);
		    			overwrite(rightChunk, setup);
		    			wrefresh(setup);
		    			sleep(1);
		    			overlay(rightBird2, rightChunk);
		    			overwrite(rightChunk, setup);
		    			wrefresh(setup);
		    			sleep(1);
		    			overlay(rightBird3, rightChunk);
		    			overwrite(rightChunk, setup);
		    			wrefresh(setup);
		    			pos1 = 'r';
		    		}

		    		sleep(1);
		    		clear_corresponding(pos1, leftChunk, middleChunk, rightChunk, setup);
		    		restore_jet(find_correspondingJet(pos1, leftJet, middleJet, rightJet, setup), h);
		    		overlay_restored_jet(pos1, leftJet, middleJet, rightJet, leftChunk, middleChunk, rightChunk, setup);
		    		
	    			if(pos1 == pos)
	    			{
	    				werase(setup);
	    				mvwprintw(setup, 10, l / 2 - 10, "Your score was %d", score);
	    				mvwprintw(setup, 15, l / 2 - 14, "Press any key to continue.");
	    				break;
	    			}
	    			else
	    			{
	    				score += 10;
	    				mvwprintw(setup, 0, ((5 * l) / 6) - 7, "Score: %d", score);
	    			}
	    		}
	    	}
   wgetch(setup);

   endwin();
}
