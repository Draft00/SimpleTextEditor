#ifndef ADAPTER_LIB
#define ADAPTER_LIB

#include "panel.h"
#include "curses.h"

class AdapterPDCurses
{
public:
	AdapterPDCurses() {};
	~AdapterPDCurses() {};

	void A_wrefresh(WINDOW* win) {
		wrefresh(win);
	};
	void A_werase(WINDOW* win) {
		werase(win);
	}
	void A_wprintw(WINDOW* win, const STD::MyString& str) {
		wprintw(win, str.c_str());
	}
	void A_wprintw(WINDOW* win, const char* str) {
		wprintw(win, str);
	}
	void A_printwLines(WINDOW* win, const char* str, size_t curr_l, size_t all_l) {
		wprintw(win, str, curr_l, all_l);
	}
	WINDOW* A_newwin(int nlines, int ncols, int begy, int begx) {
		WINDOW* win;
		win = newwin(nlines, ncols, begy, begx);
		return win;
	}
	void A_wbkgd(WINDOW* win, chtype ch) {
		wbkgd(win, ch);
	}
	int A_start_color(void) {
		return start_color();
	}
	int A_init_pair(short pair, short fg, short bg) {
		return init_pair(pair, fg, bg);
	}
	WINDOW* A_initscr(void) {
		return initscr();	
	}
	int A_resize_term(int nlines, int ncols) {
		return resize_term(nlines, ncols);
	}
	int A_raw(void) {
		return raw();
	}
	int A_keypad(WINDOW* win, bool bf) {
		return keypad(win, bf);
	}
	int A_noecho(void) {
		return noecho();
	}
	int A_wgetch(WINDOW* win) {
		return wgetch(win);
	}
	int A_wmove(WINDOW* win, int y, int x) {
		return wmove(win, y, x);
	}
};
#endif // !ADAPTER_LIB

