#include "Header.h"

ConsoleView::ConsoleView()
{
    //m_myAdapter->A_initscr();
   //m_myAdapter->A_resize_term(MAX_NLINES, MAX_NCOLS);

    m_myAdapter->A_initscr();
    m_myAdapter->A_resize_term(MAX_NLINES, MAX_NCOLS);
    //refresh();

    m_init_coloros_pair();

    text_win = m_create_text_win();
    curr_win = text_win;
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

void ConsoleView::UpdateVector(const STD::MyString& str)
{
    table.clear();
    for (size_t i = 0; i < str.length(); ) {
        for (size_t k = 0; i + k < str.length(); k++) {
            if (str[i + k] == '\n' || (k % MAX_NCOLS) == MAX_NCOLS - 1) {
                table.push_back(std::vector<int>());
                table[table.size() - 1].push_back(i);
                table[table.size() - 1].push_back(i + k);
                i += k + 1;
                break;
            }
        }
     }
}

void ConsoleView::SetStartConfig()
{
    UpdateMode("...", WAITING);
    UpdateFilename("none");
    UpdateLineStats(0, 0);
    PrintMessage("Use :h for help\nUse :o filename to open file");
}
void ConsoleView::TEST_NAVI()
{
    curr_status = NAVIGATION;
    m_myController->TEST();
}

void ConsoleView::START()
{
    SetStartConfig();

    m_myAdapter->A_raw();
    m_myAdapter->A_keypad(stdscr, true);
    m_myAdapter->A_noecho();
    int global_num = 1, choice = 0;

    TEST_NAVI();

    while (curr_status != EXIT)
    {
        //choice = wgetch(curr_win);

        if (curr_status == WAITING) {
            m_myAdapter->A_keypad(text_win, true);
            //m_myAdapter->A_noecho();
            while (curr_status == WAITING) {
                choice = m_myAdapter->A_wgetch(text_win);
                m_myController->GetKeyFromView(choice);
            }
        }
        if (curr_status == COMMAND) {
            m_myAdapter->A_wmove(cmd_win, 0, 1);
            m_myAdapter->A_wrefresh(cmd_win);
            m_myAdapter->A_keypad(cmd_win, true);
            while (curr_status == COMMAND) {
                choice = m_myAdapter->A_wgetch(text_win);
                m_myController->GetKeyFromView(choice);
            }
        }
        if (curr_status == NAVIGATION) {
            m_myAdapter->A_wrefresh(text_win);
            m_myAdapter->A_wmove(text_win, 0, 0);
            m_myAdapter->A_keypad(text_win, true);
            while (curr_status == NAVIGATION) {
                choice = m_myAdapter->A_wgetch(text_win);
                m_myController->GetKeyFromView(choice);
            }
        }
    }
}
void ConsoleView::PutModelNewIdx(size_t new_idx)
{
    m_myController->PutModelNewIdx(new_idx);
}
void ConsoleView::KeyNavigation(const STD::MyString& str, size_t idx, int command)
{
    switch (command)
    {
    case KEY_LEFT: {
        m_ProcPressedKeyLeft();
        PutModelNewIdx(new_idx);
        break;
    }
    case KEY_RIGHT: {
        m_ProcPressedKeyRight();
        PutModelNewIdx(new_idx);
        break;
    }
    case '$': {
        m_ProcPressedDollar(str, idx);
        PutModelNewIdx(new_idx);
        break;
    }
    case '0': {
        m_ProcPressedZero();
        PutModelNewIdx(new_idx);
        break;
    }
    case KEY_DOWN:
    {
        m_ProcPressedKeyDown(str, idx);
        PutModelNewIdx(new_idx);
        break;
    }
    case KEY_UP:
    {
        m_ProcPressedKeyUp(str, idx);
        PutModelNewIdx(new_idx);
        //m_ProcPressedKeyUp();
        break;
    }
    case 'g':
    {
        int c = m_myAdapter->A_wgetch(text_win);
        if (c == 'g') {
            m_ProcPressedKeygg(str, idx);
            PutModelNewIdx(new_idx);
        }
        break;
    }
    default:
        break;
    }
}
void ConsoleView::m_ProcPressedKeyLeft() //гуляем только по линии консоли, не по строке. скролл не нужен.
{
    getyx(text_win, y, x);
    if (x == 0) return;
    m_myAdapter->A_wmove(text_win, y, x - 1);
    m_myAdapter->A_wrefresh(text_win);
    --new_idx; --x_nav;
}
void ConsoleView::m_ProcPressedKeyRight()
{
    getyx(text_win, y, x);
    if (table[m_TableYFirstLine + y][LAST_IDX_LINE] == x) return;
    m_myAdapter->A_wmove(text_win, y, x + 1);
    m_myAdapter->A_wrefresh(text_win);
    ++new_idx; ++x_nav;
}
void ConsoleView::m_ProcPressedDollar(const STD::MyString& str, size_t idx)
{
    getyx(text_win, y, x);
    int new_x = table[m_TableYFirstLine + y][LAST_IDX_LINE] - table[m_TableYFirstLine + y][FIRST_IDX_LINE];
    if (new_x == x) return;
    m_myAdapter->A_wmove(text_win, y, new_x);
    m_myAdapter->A_wrefresh(text_win);
    x_nav = new_x;
    new_idx = table[m_TableYFirstLine + y][LAST_IDX_LINE];
}
void ConsoleView::m_ProcPressedZero()
{
    getyx(text_win, y, x);
    int new_x = 0;
    if (new_x == x) return;
    m_myAdapter->A_wmove(text_win, y, new_x);
    m_myAdapter->A_wrefresh(text_win);
    x_nav = new_x;
    new_idx = table[m_TableYFirstLine + y][FIRST_IDX_LINE];
}

void ConsoleView::m_ProcPressedKeyDown(const STD::MyString& str, size_t idx)
{
    if (y + m_TableYFirstLine + 1 == table.size()) return; //это последняя строка
    getyx(text_win, y, x);
    //int curr_x = x, curr_y = y;
    if (y + 1 > LAST_Y) {
        ++m_TableYFirstLine;
        m_myAdapter->A_wmove(text_win, 0, 0);
        m_myAdapter->A_wprintw(text_win, str.c_str() + table[m_TableYFirstLine][FIRST_IDX_LINE]);
        m_myAdapter->A_wrefresh(text_win);
    }
    else y = y + 1;
    int new_x = table[m_TableYFirstLine + y][LAST_IDX_LINE] - table[m_TableYFirstLine + y][FIRST_IDX_LINE];
    if (new_x > x_nav) {
        m_myAdapter->A_wmove(text_win, y, x_nav);
    }
    else m_myAdapter->A_wmove(text_win, y, new_x);
    m_myAdapter->A_wrefresh(text_win);
    getyx(text_win, y, x);
    new_idx = table[m_TableYFirstLine + y][FIRST_IDX_LINE] + x;
}
void ConsoleView::m_ProcPressedKeyUp(const STD::MyString& str, size_t idx)
{
    if (y + m_TableYFirstLine - 1 < 0) return; //это первая строка
    getyx(text_win, y, x);
    if (y - 1 < 0) {
        --m_TableYFirstLine;
        m_myAdapter->A_wmove(text_win, 0, 0);
        m_myAdapter->A_wprintw(text_win, str.c_str() + table[m_TableYFirstLine][FIRST_IDX_LINE]);
        m_myAdapter->A_wrefresh(text_win);
    }
    else y = y - 1;
    int new_x = table[m_TableYFirstLine + y][LAST_IDX_LINE] - table[m_TableYFirstLine + y][FIRST_IDX_LINE];
    if (new_x > x_nav) {
        m_myAdapter->A_wmove(text_win, y, x_nav);
    }
    else m_myAdapter->A_wmove(text_win, y, new_x);
    m_myAdapter->A_wrefresh(text_win);
    getyx(text_win, y, x);
    new_idx = table[m_TableYFirstLine + y][FIRST_IDX_LINE] + x;
}
void ConsoleView::m_ProcPressedKeygg(const STD::MyString& str, size_t idx)
{
    x = 0; y = 0; x_nav = 0;
    m_TableYFirstLine = 0;
    int new_idx = 0;
    m_myAdapter->A_wmove(text_win, 0, 0);
    m_myAdapter->A_wprintw(text_win, str.c_str() + table[m_TableYFirstLine][FIRST_IDX_LINE]);
    m_myAdapter->A_wrefresh(text_win);
    m_myAdapter->A_wmove(text_win, 0, 0);
    m_myAdapter->A_wrefresh(text_win);
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

void ConsoleView::PressedKeyUp()
{
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
}

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