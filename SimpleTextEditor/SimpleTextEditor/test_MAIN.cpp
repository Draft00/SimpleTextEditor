#include "Header.h"

/*
WINDOW* create_newwin(int height, int width, int starty, int startx)
{
	WINDOW* local_win;

	local_win = newwin(height, width, starty, startx);
	wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(local_win);
	return local_win;
}

void destroy_win(WINDOW* local_win)
{
	wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(local_win);
	delwin(local_win);
}
*/
int main()
{
    /*initscr();
    noecho();
    curs_set(FALSE);
    cbreak();

    if (has_colors() == FALSE)
    {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);

    attron(COLOR_PAIR(1));
    mvprintw(1, 1, "1. Start");
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(2));
    mvprintw(2, 1, "2. Options");
    attroff(COLOR_PAIR(2));

    attron(COLOR_PAIR(3));
    mvprintw(3, 1, "3. Options");
    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(4));
    mvprintw(4, 1, "4. Exit");
    attroff(COLOR_PAIR(4));

    char ch = NULL;
    while (1) {
        ch = getch();
        if (ch == '1') {
            clear();
            mvprintw(0, 0, "Press any key ...");
            break;
        }
        else if (ch == '4') {
            endwin();
            exit(1);
        }
    }

    refresh();
    getch();
    endwin();
    return 0;
    */

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
	
    WindowModel Model;
    ConsoleView View(&Model);
    Controller Controller(&Model);
    Controller.start();
    getch();
    return 0;
}

