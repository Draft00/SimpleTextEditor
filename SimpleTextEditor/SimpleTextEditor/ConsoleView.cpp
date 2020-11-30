#include "Header.h"

ConsoleView::ConsoleView()
{
    m_myAdapter->A_initscr();
    m_myAdapter->A_resize_term(MAX_NLINES, MAX_NCOLS);

    m_init_coloros_pair();

    text_win = m_create_text_win();
    help_win = m_create_text_win();

    //панели без адаптера, но мб я их и нафиг уберу.
    help_pannel = new_panel(help_win);
    text_pannel = new_panel(text_win);
    set_panel_userptr(text_pannel, help_pannel);
    set_panel_userptr(help_pannel, text_pannel);
    update_panels();

    cmd_win = m_create_cmd_win();
    m_create_status_wins();

}
void ConsoleView::AddController(Controller* Controller)
{
    m_myController = Controller;
}
void ConsoleView::AddAdapter(AdapterPDCurses* Adapter)
{
    m_myAdapter = Adapter;
}


ConsoleView::~ConsoleView()
{
    del_panel(text_pannel);
    del_panel(help_pannel);
    delwin(help_win);
    delwin(text_win);
    delwin(cmd_win);
    delwin(mode_win);
    delwin(filename_win);
    delwin(line_stats_win);
    endwin();
}

void ConsoleView::SetStartConfig()
{
    UpdateMode("...", WAITING);
    UpdateFilename("none");
    UpdateLineStats(0, 0);
    PrintMessage("Use :h for help\nUse :o filename to open file");
}
void ConsoleView::START()
{
    SetStartConfig();

    m_myAdapter->A_raw();
    m_myAdapter->A_keypad(stdscr, true);
    int global_num = 1;


}
//void ConsoleView::scroll_down(int curr_pos)
//{
//    wscrl(text_win, 1);
//    wrefresh(text_win);
//    //char c = m_mymodel->file_data[m_mymodel->idx];
//    size_t idx_print = m_mymodel->idx - curr_pos;
//    size_t pos = m_mymodel->file_data.find("\n", idx_print);
//    if (pos == STD::MyString::npos) {
//        beep();
//        return;
//    }
//    STD::MyString str_print = m_mymodel->file_data.substr(idx_print, pos - idx_print);
//    mvwprintw(text_win, y, 0, str_print.c_str());
//    m_mymodel->num_curr_line++;
//    //ЭТО ВЕРНО НО ПЕРЕДЕЛЫВАТЬ
//    //UpdateLineStats();
//    wmove(text_win, y, curr_pos);
//}

//void ConsoleView::PressedKeyUp()
//{
//    int temp_x = 0;
//    getyx(text_win, y, x);
//    if (m_mymodel->idx - x > 0) {
//        int curr_pos = 0;
//        //getyx(text_win, y, x);
//
//        if (m_mymodel->file_data[m_mymodel->idx] == '\n') {
//            m_mymodel->idx--;
//            if (m_mymodel->file_data[m_mymodel->idx - 1] != '\n')
//                m_mymodel->idx--;
//        }
//        else
//        {
//            while (m_mymodel->file_data[m_mymodel->idx] != '\n') {
//                m_mymodel->idx--;
//            }
//            if (m_mymodel->idx > 0 && m_mymodel->file_data[m_mymodel->idx - 1] != '\n') {
//                m_mymodel->idx--;
//            }
//            else {
//
//            }
//                
//        }
//        if (m_mymodel->file_data[m_mymodel->idx] != '\n')
//        {
//            while (m_mymodel->file_data[m_mymodel->idx] != '\n' && m_mymodel->idx > 0) {
//                m_mymodel->idx--;
//            }
//            //if (m_mymodel->idx == STD::MyString::npos) m_mymodel->idx = 0;
//            if (m_mymodel->idx != 0)
//                m_mymodel->idx++;
//        }
//        while (curr_pos < (x_nav + 1) && m_mymodel->file_data[m_mymodel->idx] != '\n') { //&& m_mymodel->file_data[m_mymodel->idx] != '\0') {
//            ++curr_pos; m_mymodel->idx++;
//        }
//        m_mymodel->idx--;  --curr_pos;
//        if (curr_pos == -1) {
//            m_mymodel->idx++;
//            curr_pos++;
//        }
//        if (y - 1 < 0)
//        {
//            wscrl(text_win, -1);
//            wrefresh(text_win);
//            //char c = m_mymodel->file_data[m_mymodel->idx];
//            size_t idx_print = m_mymodel->idx - curr_pos;
//            size_t pos = m_mymodel->file_data.find("\n", idx_print);
//            if (pos == STD::MyString::npos) {
//                beep();
//                return;
//            }
//            STD::MyString str_print = m_mymodel->file_data.substr(idx_print, pos - idx_print);
//            mvwprintw(text_win, y, 0, str_print.c_str());
//            m_mymodel->num_curr_line--;
//            //UpdateLineStats(); верно
//            wmove(text_win, y, curr_pos);
//        }
//        else {
//            m_mymodel->num_curr_line--;
//            //UpdateLineStats(); верно
//            wmove(text_win, y - 1, curr_pos);
//        }
//        wrefresh(text_win);
//    }
//    else beep();
//}

void ConsoleView::ClearCmd()
{
    m_myAdapter->A_werase(cmd_win);
    m_myAdapter->A_wprintw(mode_win, ":");
    m_myAdapter->A_wrefresh(cmd_win);
}

void ConsoleView::EndCmd()
{
    m_myAdapter->A_werase(cmd_win);
    m_myAdapter->A_wrefresh(cmd_win);
}

void ConsoleView::UpdateMode(const STD::MyString& mode, status new_status)
{
    m_myAdapter->A_werase(mode_win);
    m_myAdapter->A_wprintw(mode_win, mode);
    m_myAdapter->A_wrefresh(mode_win);
    curr_status = new_status;
}

void ConsoleView::UpdateFilename(const STD::MyString& filename)
{
    m_myAdapter->A_werase(filename_win);
    m_myAdapter->A_wprintw(filename_win, filename);
    m_myAdapter->A_wrefresh(filename_win);
}

void ConsoleView::PrintMessage(const STD::MyString& str)
{
    m_myAdapter->A_werase(text_win);
    m_myAdapter->A_wprintw(text_win, str.c_str());
    m_myAdapter->A_wrefresh(text_win);
}

//возможно, удалить
void ConsoleView::mvPrintMessage(const char* str, int y, int x)
{
    mvwprintw(text_win, y, x, str); //работает совсем не как нужно! //а мб и работает
    wrefresh(text_win);
}

void ConsoleView::UpdateLineStats(size_t curr_line, size_t lines)
{
    m_myAdapter->A_werase(line_stats_win);
    m_myAdapter->A_printwLines(line_stats_win, "Line: %ld/%ld", curr_line, lines);
    m_myAdapter->A_wrefresh(line_stats_win);
}

void ConsoleView::UpdateCmd(const STD::MyString& str)
{
    m_myAdapter->A_werase(cmd_win);
    m_myAdapter->A_wprintw(cmd_win, ":");
    m_myAdapter->A_wprintw(cmd_win, str);
    m_myAdapter->A_wrefresh(cmd_win);
}

/*
    private methods
*/
WINDOW* ConsoleView::m_create_cmd_win()
{
    WINDOW* cmd_win;
    cmd_win = m_myAdapter->A_newwin(1, MAX_NCOLS, MAX_NLINES - 1, 0);
    m_myAdapter->A_wbkgd(cmd_win, COLOR_PAIR(1));
    m_myAdapter->A_wrefresh(cmd_win);
    return cmd_win;
}

WINDOW* ConsoleView::m_create_text_win()
{
    WINDOW* text_win;
    text_win = m_myAdapter->A_newwin(TEXT_W_LINES, TEXT_W_COLS, 0, 0);
    m_myAdapter->A_wbkgd(text_win, COLOR_PAIR(2));
    m_myAdapter->A_wrefresh(text_win);
    return text_win;
}

void ConsoleView::m_init_coloros_pair()
{
    m_myAdapter->A_start_color();
    m_myAdapter->A_init_pair(1, COLOR_BLACK, COLOR_CYAN);
    m_myAdapter->A_init_pair(2, COLOR_WHITE, COLOR_BLACK);
    m_myAdapter->A_init_pair(3, COLOR_BLACK, COLOR_MAGENTA);
    m_myAdapter->A_init_pair(4, COLOR_BLACK, COLOR_WHITE);
}

void ConsoleView::m_create_status_wins()
{
    mode_win = m_myAdapter->A_newwin(1, 4, MAX_NLINES - 2, 0);
    m_myAdapter->A_wbkgd(mode_win, COLOR_PAIR(3));
    m_myAdapter->A_wrefresh(mode_win);
    filename_win = m_myAdapter->A_newwin(1, 30, MAX_NLINES - 2, 4);
    m_myAdapter->A_wbkgd(filename_win, COLOR_PAIR(2));
    m_myAdapter->A_wrefresh(filename_win);
    line_stats_win = m_myAdapter->A_newwin(1, MAX_NCOLS - 34, MAX_NLINES - 2, 34);
    m_myAdapter->A_wbkgd(line_stats_win, COLOR_PAIR(3));
    m_myAdapter->A_wrefresh(line_stats_win);
}