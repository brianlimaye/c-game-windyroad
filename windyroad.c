#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include "windyroad.h"
#include <unistd.h>

char * findCorresponding(int r, char * leftB, char * middleB, char * rightB)
{
	if(r == 1)
	{
		return leftB;
	}
	if(r == 2)
	{
		return middleB;
	}

	if(r == 3)
	{
		return rightB;
	}

	return NULL;
}

char getEquivalentChar(int dir)
{
	if(dir == 1)
	{
		return 'l';
	}

	if(dir == 2)
	{
		return 'm';
	}

	if(dir == 3)
	{
		return 'r';
	}
	return '\0';
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
   int score;
   char c;
   int ran = 1;
   int increment = 0;
   char pos = 'm';
   char * result;
   char * middleJet = "\n                                          `==\\/\\==`\n                                ____________\\/__\\____________\n                               \\/____________________________\\\n                                 __||__||__\\/.--.\\__||__||__\n                                \\/__|___|___( >< )___|___|__\\\n                                          _\\/`--`\\_\n                                         (\\/------\\)";
   char * leftJet =  "\n           `==\\/\\==`\n ____________\\/__\\____________\n\\/____________________________\\\n  __||__||__\\/.--.\\__||__||__\n \\/__|___|___( >< )___|___|__\\\n           _\\/`--`\\_\n          (\\/------\\)";
   char * rightJet = "\n                                                                         `==\\/\\==`\n                                                               ____________\\/__\\____________\n                                                              \\/____________________________\\\n                                                                __||__||__\\/.--.\\__||__||__\n                                                               \\/__|___|___( >< )___|___|__\\\n                                                                         _\\/`--`\\_\n                                                                        (\\/------\\)";

   char * leftBird = "              \\\\\n             (o>\n          \\\\_//)\n           \\_/_)\n            _|_";
   char * middleBird = "                                              \\\\\n                                              (o>\n                                           \\\\_//)\n                                            \\_/_)\n                                             _|_";
   char * rightBird = "                                                                              \\\\\n                                                                              (o>\n                                                                           \\\\_//)\n                                                                            \\_/_)\n                                                                             _|_";




   mvaddstr(0, COLS/2 - 7, "--WindyRoad--");
   mvprintw(0, 64, "Score: %d", score);
  
   for(i=0; i<COLS; i+=(COLS / 3))
   {
   		for(j=0; j<= LINES; j++)
   		{
   			mvaddch(j, i, ACS_VLINE | WA_BOLD);
   		}
   }

   WINDOW * setup = newwin(LINES, COLS, 0, 0);

   //Set-up				 

    
    struct jet * je;
    struct box * bo;

    //je->xflat = COLS/2;
    //je->yflat = LINES - 10;
  
    move(20, 0);
    printw(middleJet);

    for(i=0; i<COLS; i+=(COLS / 3))
    {
   		for(j=0; j<= LINES; j++)
   		{
   			mvaddch(j, i, ACS_VLINE | WA_BOLD);
   		}
    }

    //Logic :)

    		

    
	    	while(true)
	    	{
		    	keypad(setup, true);
		    	char c = getch();

		    	if(c != (char)KEY_UP)
		    	{
			    	if(c == (char)KEY_BACKSPACE)
			    	{
			    		break;
			    	}

			    	if(c == (char)KEY_LEFT)
			    	{
			    		if(pos == 'l')
			    		{
			    			continue;
			    		}
			    		clear();
			    		mvaddstr(0, COLS/2 - 7, "--WindyRoad--");
			    		mvprintw(0, 64, "Score: %d", score);
		   				move(22, 0);

		   				if(pos == 'm')
		   				{
			    			printw(leftJet);
			    			pos = 'l';
			    		}

			    		if(pos == 'r')
			    		{
			    			printw(middleJet);
			    			pos = 'm';
			    		}
			    		
			    		
			    		for(i=0; i<COLS; i+=(COLS / 3))
		    			{
		   					for(j=0; j<= LINES; j++)
		   					{
		   						mvaddch(j, i, ACS_VLINE | WA_BOLD);
		   					}
		    			}

			    	}

			    	if(c == (char) KEY_RIGHT)
			    	{
			    		if(pos == 'r')
			    		{
			    			continue;
			    		}
			    		clear();
			    		mvaddstr(0, COLS/2 - 7, "--WindyRoad--");
			    		mvprintw(0, 64, "Score: %d", score);
			    		move(22, 0);
			    		if(pos == 'm')
			    		{
			    			printw(rightJet);
			    			pos = 'r';
			    		}

			    		if(pos == 'l')
			    		{
			    			printw(middleJet);
			    			pos = 'm';
			    		}
			    		
			    		
			    		for(i=0; i<COLS; i+=(COLS / 3))
		    			{
		   					for(j=0; j<= LINES; j++)
		   					{
		   						mvaddch(j, i, ACS_VLINE | WA_BOLD);
		   					}
		    			}
			    	}
			    }


			 	else
			 	{
		    		ran = (rand() % 3) + 1;
		    		result = findCorresponding(ran, leftBird, middleBird, rightBird);

		    		

		    		mvprintw(1, 0, result);
		    		for(i=0; i<COLS; i+=(COLS / 3))
	    			{
	   					for(j=0; j<= LINES; j++)
	   					{
	   						mvaddch(j, i, ACS_VLINE | WA_BOLD);
	   					}
	    			}
	    			refresh();
	    			sleep(1);
	    			mvprintw(8, 0, result);
	    			for(i=0; i<COLS; i+=(COLS / 3))
	    			{
	   					for(j=0; j<= LINES; j++)
	   					{
	   						mvaddch(j, i, ACS_VLINE | WA_BOLD);
	   					}
	    			}
	    			refresh();
	    			sleep(1);
	    			mvprintw(15, 0, result);
	    			
	    			for(i=0; i<COLS; i+=(COLS / 3))
	    			{
	   					for(j=0; j<= LINES; j++)
	   					{
	   						mvaddch(j, i, ACS_VLINE | WA_BOLD);
	   					}
	    			}
	    			refresh();

	    			
	    			if(pos == getEquivalentChar(ran))
	    			{
	    				
	    				clear();
	    				mvprintw(10, 36, "Your score was %d", score);
	    				mvprintw(15, 32, "Press any key to continue.");
	    				break;
	    			}
	    			else
	    			{
	    				score += 10;
	    				mvprintw(0, 64, "Score: %d", score);
	    			}
	    		}

	    	}

   getch();

   endwin();
}
