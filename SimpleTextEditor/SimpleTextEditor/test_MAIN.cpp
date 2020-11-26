#include "Header.h"


int main()
{


	/*WINDOW* local_win;
	initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	printw("Hello World !!!");
	attroff(COLOR_PAIR(1));
	mvaddstr(6, 32, "Test");
	refresh();
	local_win = create_newwin(10, 15, 5, 5);
	getch();
	destroy_win(local_win);
	getch();
	endwin();*/

   /* std::ifstream fout; 
    fout.open("test file2.txt", std::ios_base::out);
    STD::MyString s = "4545\n   7878 \n  \n 1212";
    fout >> s;
    fout.close();*/

    WindowModel Model;
    ConsoleView View(&Model);
    Controller Controller(&Model, &View);
    Controller.start();
    getch();
    return 0;
}


