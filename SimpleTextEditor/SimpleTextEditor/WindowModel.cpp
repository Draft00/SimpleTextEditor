#include "Header.h"


WINDOW* WindowModel::create_newwin(int height, int width, int starty, int startx)
{
	WINDOW* local_win;

	local_win = newwin(height, width, starty, startx);
	wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(local_win);
	return local_win;
}
void WindowModel::destroy_win(WINDOW* local_win)
{
	wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(local_win);
	delwin(local_win);
}


WindowModel::WindowModel()
{

	initscr();
	resize_term(MAX_NLINES, MAX_NCOLS); //int nlines, int ncols
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_CYAN);
	create_cmd_line();
	create_line_stat();
	//m_local_win = create_newwin(MAX_NLINES, MAX_NCOLS, 0, 0);
}

WindowModel::~WindowModel()
{
	//destroy_win(m_local_win);
	endwin();
}

void WindowModel::create_line_stat()
{
	int x = 0, y = 0;
	mvprintw(MAX_NLINES - 2, 0, mode[curr_status].c_str());
	refresh();
	getyx(stdscr, y, x);
	mvprintw(y, x + 1, m_filename.c_str());
	refresh();
	getyx(stdscr, y, x);
	mvprintw(y, x + 1, "Line: %d/%d", m_num_curr_line, m_num_lines);
	refresh();
	move(0, 0);
	refresh();
}

void WindowModel::create_cmd_line()
{
	attron(COLOR_PAIR(1));
	mvhline(MAX_NLINES - 1, 0, ' ', MAX_NCOLS);
	attroff(COLOR_PAIR(1));
	refresh();
}