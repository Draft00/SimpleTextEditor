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
	init_pair(1, COLOR_BLACK, COLOR_CYAN);
	create_cmd_line();
	create_line_stat();
	print_hello_message();

	move(0, 0);
	refresh();
	//m_local_win = create_newwin(MAX_NLINES, MAX_NCOLS, 0, 0);
}

WindowModel::~WindowModel()
{
	//destroy_win(m_local_win);
	endwin();
}

void WindowModel::set_status(status new_status)
{
	curr_status = new_status;
	notifyUpdate_line_stat();
}
void WindowModel::get_filename(void)
{
	char filename[560] = { 0 };
	int i = 0;
	int c = 0;
	//keypad(stdscr, true);
	nonl();
	int stop = 1;
	while (stop)
	{
		c = getch();
		switch (c)
		{
		case PADENTER:
		{
			filename[i] = '\0';
			stop = 0;
			break;
		}
		case 8:
		{
			//удалить символ (и закрашивать при этом).
			//i--;
		}
		default:
		{
			filename[i] = c;
			i++;
			break;
		}
		}
	}
	//getstr(filename);
	m_filename = filename;
	attroff(COLOR_PAIR(1));
	notifyUpdate_line_stat();
	notifyUpdate_cmd_line();
	attron(COLOR_PAIR(1));
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
}

void WindowModel::create_cmd_line()
{
	attron(COLOR_PAIR(1));
	mvhline(MAX_NLINES - 1, 0, ' ', MAX_NCOLS);
	attroff(COLOR_PAIR(1));
	refresh();
}
void WindowModel::print_hello_message()
{
	char hello_message[] = "Use :h for help\nUse :o filename to open file";
	mvprintw(0, 0, hello_message);
	refresh();
}