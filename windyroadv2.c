#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include "windyroad.h"
#include <unistd.h>

//To-do
//1 Redraw on window change.
//2 Refactor methods that are repeated.
//3 If color is enabled, add support for color.
//4 Study winexample.c and do #3.
//5 Avoid having parameters of WINDOW * that are defined in main.


void restore_jet(WINDOW * win, int h)
{
	
	mvwaddstr(win, h-10, 1 ,"\n           `==\\/\\==`\n ____________\\/__\\____________\n\\/____________________________\\\n  __||__||__\\/.--.\\__||__||__\n \\/__|___|___( >< )___|___|__\\\n           _\\/`--`\\_\n          (\\/------\\)");
}

void restore_bird(char c, int n, WINDOW * bird)
{
		
	mvwaddstr(bird, 0, 0, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\n  _|_");

}

void overlay_restored_bird(char c, WINDOW * bird, WINDOW * left_chunk, WINDOW * middle_chunk, WINDOW * right_chunk)
{
	if(c == 'l')
	{
		overwrite(bird, left_chunk);
	}

	if(c == 'm')
	{
		overwrite(bird, middle_chunk);
	}

	if(c == 'r')
	{
		overwrite(bird, right_chunk);
	}
}

void overlay_restored_jet(char c, WINDOW * left_jet, WINDOW * middle_jet, WINDOW * right_jet, WINDOW * left_chunk, WINDOW * middle_chunk, WINDOW * right_chunk, WINDOW * setup)
{
	if(c == 'l')
	{
		overwrite(left_jet, left_chunk);
		
	}

	if(c == 'm')
	{
		overwrite(middle_jet, middle_chunk);		
	}

	if(c == 'r')
	{
		overwrite(right_jet, right_chunk);
	}
}

void clear_corresponding(char c, WINDOW * left_chunk, WINDOW * middle_chunk, WINDOW * right_chunk, WINDOW * setup)
{
	if(c == 'l')
	{
		werase(left_chunk);
		wrefresh(left_chunk);
		overwrite(left_chunk, setup);
		wrefresh(setup);
	}

	if(c == 'm')
	{
		werase(middle_chunk);
		wrefresh(middle_chunk);
		overwrite(middle_chunk, setup);
		wrefresh(setup);
	}

	if(c == 'r')
	{
		werase(right_chunk);
		wrefresh(right_chunk);
		overwrite(right_chunk, setup);
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

WINDOW * find_correspondingJet(char c, WINDOW * left_jet, WINDOW * middle_jet, WINDOW * right_jet, WINDOW * setup)
{
	if(c == 'l')
	{
		return left_jet;
	}
	if(c == 'm')
	{
		return middle_jet;
	}
	if(c == 'r')
	{
		return right_jet;
	}
	return NULL;
}

void lock_in_position(char c, int l, int h, WINDOW * setup)
{
	int i, j;

	if(c == 'l')
	{
		for(i=l/3; i< (2 * l) / 3; i+=((l / 3) + 1))
   		{
   			for(j=0; j< h; j++)
   			{
   				mvwaddch(setup, j, i, ACS_VLINE | A_BLINK);
   			}
   		}
	}

	if(c == 'm')
	{
		for(i=l/3; i<= ((2 * l) / 3) + 1; i+=((l / 3) + 1))
   		{
   			for(j=0; j< h; j++)
   			{
   				mvwaddch(setup, j, i, ACS_VLINE | A_BLINK);
   			}
   		}
	}

	if(c == 'r')
	{
		for(i=((2 * l) / 3); i< l; i+=((l / 3) + 1))
   		{
   			for(j=0; j< h; j++)
   			{
   				mvwaddch(setup, j, i, ACS_VLINE | A_BLINK);
   			}
   		}
	}
}

void restore_position(char c, int l, int h, WINDOW * setup)
{
	int i, j;

	if(c == 'l')
	{
		for(i=l/3; i< (2 * l) / 3; i+=((l / 3) + 1))
   		{
   			for(j=0; j< h; j++)
   			{
   				mvwaddch(setup, j, i, ACS_VLINE);
   			}
   		}
	}

	if(c == 'm')
	{
		for(i=l/3; i<= ((2 * l) / 3) + 1; i+=((l / 3) + 1))
   		{
   			for(j=0; j< h; j++)
   			{
   				mvwaddch(setup, j, i, ACS_VLINE);
   			}
   		}
	}

	if(c == 'r')
	{
		for(i=((2 * l) / 3); i< l; i+=((l / 3) + 1))
   		{
   			for(j=0; j< h; j++)
   			{
   				mvwaddch(setup, j, i, ACS_VLINE);
   			}
   		}
	}
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
   curs_set(0);

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
   char * middle_jet = "\n                                          `==\\/\\==`\n                                ____________\\/__\\____________\n                               \\/____________________________\\\n                                 __||__||__\\/.--.\\__||__||__\n                                \\/__|___|___( >< )___|___|__\\\n                                          _\\/`--`\\_\n                                         (\\/------\\)";
   char * left_jet =  "\n           `==\\/\\==`\n ____________\\/__\\____________\n\\/____________________________\\\n  __||__||__\\/.--.\\__||__||__\n \\/__|___|___( >< )___|___|__\\\n           _\\/`--`\\_\n          (\\/------\\)";
   char * right_jet = "\n                                                                         `==\\/\\==`\n                                                               ____________\\/__\\____________\n                                                              \\/____________________________\\\n                                                                __||__||__\\/.--.\\__||__||__\n                                                               \\/__|___|___( >< )___|___|__\\\n                                                                         _\\/`--`\\_\n                                                                        (\\/------\\)";

   char * leftBird = "              \\\\\n             (o>\n          \\\\_//)\n           \\_/_)\n            _|_";
   char * middleBird = "                                              \\\\\n                                              (o>\n                                           \\\\_//)\n                                            \\_/_)\n                                             _|_";
   char * rightBird = "                                                                              \\\\\n                                                                              (o>\n                                                                           \\\\_//)\n                                                                            \\_/_)\n                                                                             _|_";
   */

   WINDOW * setup = newwin(LINES, COLS, 0, 0);


   getmaxyx(stdscr, h, l);

   //Window Initialization
   WINDOW * left_chunk = newwin(h, l /3, 1, 0);
   WINDOW * middle_chunk = newwin(h, l/3, 1, l/3 + 1);
   WINDOW * right_chunk = newwin(h, l/3, 1, ((2 * l) / 3) + 2);
   
   WINDOW * left_jet = newwin(10, l /3, h-9, 0);
   WINDOW * middle_jet = newwin(10, l/3, h-9, l/3 + 1);
   WINDOW * right_jet = newwin(10, l/3, h-9, ((2 * l) / 3) + 2);

   WINDOW * left_bird1 = newwin(18, 7, 1, 0);
   WINDOW * left_bird2 = newwin(18, 7, 7, 0);
   WINDOW * left_bird3 = newwin(18, 7, 13, 0);

   WINDOW * middle_bird1 = newwin(18, 7, 1, l/3 + 1);
   WINDOW * middle_bird2 = newwin(18, 7, 6, l/3 + 1);
   WINDOW * middle_bird3 = newwin(18, 7, 11, l/3 + 1);
   
   WINDOW * right_bird1 = newwin(18, 7, 1, ((2 * l) / 3) + 2);
   WINDOW * right_bird2 = newwin(18, 7, 6, ((2 * l) / 3) + 2);
   WINDOW * right_bird3 = newwin(18, 7, 11, ((2 * l) / 3) + 2);
  

   waddstr(left_jet, "\n           `==\\/\\==`\n ____________\\/__\\____________\n\\/____________________________\\\n  __||__||__\\/.--.\\__||__||__\n \\/__|___|___( >< )___|___|__\\\n           _\\/`--`\\_\n          (\\/------\\)");
   waddstr(middle_jet, "\n           `==\\/\\==`\n ____________\\/__\\____________\n\\/____________________________\\\n  __||__||__\\/.--.\\__||__||__\n \\/__|___|___( >< )___|___|__\\\n           _\\/`--`\\_\n          (\\/------\\)");
   waddstr(right_jet, "\n           `==\\/\\==`\n ____________\\/__\\____________\n\\/____________________________\\\n  __||__||__\\/.--.\\__||__||__\n \\/__|___|___( >< )___|___|__\\\n           _\\/`--`\\_\n          (\\/------\\)");

   waddstr(left_bird1, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\n  _|_");
   waddstr(left_bird2, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\n  _|_");
   waddstr(left_bird3, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\n  _|_");
   waddstr(middle_bird1, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\n  _|_");
   waddstr(middle_bird2, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\n  _|_");
   waddstr(middle_bird3, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\n  _|_");
   waddstr(right_bird1, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\n  _|_");
   waddstr(right_bird2, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\n  _|_");
   waddstr(right_bird3, "   \\\\\n   (o>\n\\\\_//)\n \\_/_)\n  _|_");

   

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
   overlay(left_jet, left_chunk);
   overlay(middle_jet, middle_chunk);
   overlay(right_jet, right_chunk);
   
   overlay(middle_chunk, setup);
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
			    			werase(middle_chunk);
			    			wrefresh(middle_chunk);
			    			overwrite(middle_chunk, setup);
			    			wrefresh(setup);
			    			overwrite(left_chunk, setup);
			    			wrefresh(setup);
			    			pos = 'l';
			    			restore_jet(middle_chunk, h);
			    		}
			    		


			    		if(pos == 'r')
			    		{
			    			werase(right_chunk);
			    			wrefresh(right_chunk);
			    			overwrite(right_chunk, setup);
			    			wrefresh(setup);
			    			overwrite(middle_chunk, setup);
			    			wrefresh(setup);
			    			pos = 'm';
			    			restore_jet(right_chunk, h);
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
			    			werase(middle_chunk);
			    			wrefresh(middle_chunk);
			    			overwrite(middle_chunk, setup);
			    			wrefresh(setup);
			    			overwrite(right_chunk, setup);
			    			wrefresh(setup);
			    			pos = 'r';
			    			restore_jet(middle_chunk, h);
			    		}

			    		if(pos == 'l')
			    		{
			    			werase(left_chunk);
			    			wrefresh(left_chunk);
			    			overwrite(left_chunk, setup);
			    			wrefresh(setup);
			    			overwrite(middle_chunk, setup);
			    			wrefresh(setup);
			    			pos = 'm';
			    			restore_jet(left_chunk, h);
			    		}
			    	}
			    }

			 	else
			 	{
		    		lock_in_position(pos, l, h, setup);

		    		ran = (rand() % 3) + 1;
		    		if(ran == 1)
		    		{
		    			
		    			if(find_corresponding(ran) != pos)
		    			{
		    				werase(left_chunk);
		    				wrefresh(left_chunk);
		    				overwrite(left_chunk, setup);
		    				wrefresh(setup);
		    			}
		    			

		    			overlay_restored_bird('l', left_bird1, left_chunk, middle_chunk, right_chunk);
		    			overwrite(left_bird1, left_chunk);
		    			overwrite(left_chunk, setup);
		    			wrefresh(setup);
		    			sleep(1);
		    			werase(left_bird1);
		    			overwrite(left_bird1, left_chunk);
		    			overwrite(left_chunk, setup);
		    			wrefresh(setup);

		    			
		    			overlay_restored_bird('l', left_bird2, left_chunk, middle_chunk, right_chunk);
		    			overwrite(left_bird2, left_chunk);
		    			overwrite(left_chunk, setup);
		    			wrefresh(setup);
		    			sleep(1);
		    			werase(left_bird2);
		    			overwrite(left_bird2, left_chunk);
		    			overwrite(left_chunk, setup);
		    			wrefresh(setup);
		    			
		    			overlay_restored_bird('l', left_bird3, left_chunk, middle_chunk, right_chunk);
		    			overwrite(left_bird3, left_chunk);
		    			overlay(left_chunk, setup);
		    			wrefresh(setup);
		    			werase(left_bird3);
		    			overwrite(left_bird3, left_chunk);
		    			overlay(left_chunk, setup);
		    			wrefresh(setup);
		    			pos1 = 'l';
		    		}

		    		if(ran == 2)
		    		{
		    			
		    			if(find_corresponding(ran) != pos)
		    			{
		    				werase(middle_chunk);
		    				wrefresh(middle_chunk);
		    				overwrite(middle_chunk, setup);
		    				wrefresh(setup);
		    			}
		    	
		    			

		    			overlay_restored_bird('m', middle_bird1, left_chunk, middle_chunk, right_chunk);
		    			overwrite(middle_bird1, middle_chunk);
		    			overwrite(middle_chunk, setup);
		    			wrefresh(setup);
		    			sleep(1);
		    			
		    			werase(middle_bird1);
		    			overwrite(middle_bird1, middle_chunk);
		    			overwrite(middle_chunk, setup);
		    			wrefresh(setup);
		    			
		    			
		    			overlay_restored_bird('m', middle_bird2, left_chunk, middle_chunk, right_chunk);	
		    			overwrite(middle_bird2, middle_chunk);
		    			overwrite(middle_chunk, setup);
		    			wrefresh(setup);
		    			sleep(1);
		    			
		    			werase(middle_bird2);
		    			overwrite(middle_bird2, middle_chunk);
		    			overwrite(middle_chunk, setup);
		    			wrefresh(setup);
		    			
		    			
		    			overlay_restored_bird('m', middle_bird3, left_chunk, middle_chunk, right_chunk);	
		    			overwrite(middle_bird3, middle_chunk);
		    			overlay(middle_chunk, setup);
		    			wrefresh(setup);
		    			
		    			werase(middle_bird3);
		    			overwrite(middle_bird3, middle_chunk);
		    			overlay(middle_chunk, setup);
		    			wrefresh(setup);
		    			
		    			pos1= 'm';

		    			
		    		}
		    		if(ran == 3)
		    		{
		    			
		    			if(find_corresponding(ran) != pos)
		    			{
		    				werase(right_chunk);
		    				wrefresh(right_chunk);
		    				overwrite(right_chunk, setup);
		    				wrefresh(setup);
		    			}


		    			overlay_restored_bird('r', right_bird1, left_chunk, middle_chunk, right_chunk);
		    			overwrite(right_bird1, right_chunk);
		    			overwrite(right_chunk, setup);
		    			wrefresh(setup);
		    			sleep(1);
		    			
		    			werase(right_bird1);
		    			overwrite(right_bird1, right_chunk);
		    			overwrite(right_chunk, setup);
		    			wrefresh(setup);
		    			

		    			
		    			overlay_restored_bird('r', right_bird2, left_chunk, middle_chunk, right_chunk);
		    			overwrite(right_bird2, right_chunk);
		    			overwrite(right_chunk, setup);
		    			wrefresh(setup);
		    			sleep(1);
		    			
		    			werase(right_bird2);
		    			overwrite(right_bird2, right_chunk);
		    			overwrite(right_chunk, setup);
		    			wrefresh(setup);
		    			
		    			
		    			overlay_restored_bird('r', right_bird3, left_chunk, middle_chunk, right_chunk);
		    			overwrite(right_bird3, right_chunk);
		    			overlay(right_chunk, setup);
		    			wrefresh(setup);
		    			
		    			werase(right_bird3);
		    			overwrite(right_bird3, right_chunk);
		    			overlay(right_chunk, setup);
		    			wrefresh(setup);
		    			
		    			pos1 = 'r';
		    		}

		    		sleep(1);

		    		clear_corresponding(pos1, left_chunk, middle_chunk, right_chunk, setup);
		    		restore_jet(find_correspondingJet(pos1, left_jet, middle_jet, right_jet, setup), h);
		    		overlay_restored_jet(pos1, left_jet, middle_jet, right_jet, left_chunk, middle_chunk, right_chunk, setup);
		    		restore_position(pos, l, h, setup);

		    		
		    		restore_bird('l', 1, left_bird1);
		    	    restore_bird('l', 2, left_bird2);
		    	    restore_bird('l', 3, left_bird3);
		    	    restore_bird('m', 1, middle_bird1);
		    	    restore_bird('m', 2, middle_bird2);
		    	    restore_bird('m', 2, middle_bird3);
		    		restore_bird('r', 1, right_bird1);
		    	    restore_bird('r', 2, right_bird2);
		    	    restore_bird('r', 3, right_bird3);
		    	    
		    		
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
