#include "Header.h"

WindowModel::WindowModel()
{

	/*initscr();
	resize_term(MAX_NLINES, MAX_NCOLS); //int nlines, int ncols
	
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_CYAN);
	create_cmd_line();
	create_line_stat();
	print_hello_message();

	move(0, 0);
	refresh();*/
	//m_local_win = create_newwin(MAX_NLINES, MAX_NCOLS, 0, 0);
}

WindowModel::~WindowModel()
{
}

void WindowModel::SetStartConfig()
{
	NotifySetStartConfig();
}
void WindowModel::SetStatus(status new_status)
{
	curr_status = new_status;
	NotifyUpdateMode();
}

void WindowModel::SetFilename(const char* new_filename)
{
	filename = new_filename;
	NotifyUpdateFilename();
}

/*
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
*/
