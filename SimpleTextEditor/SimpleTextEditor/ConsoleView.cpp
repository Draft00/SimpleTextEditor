#define _WIN32_WINNT 0x0500
#include <windows.h>
#include "Header.h"

ConsoleView::ConsoleView(AdapterPDCurses* Adapter)
{
    //m_myAdapter->A_initscr();
   //m_myAdapter->A_resize_term(MAX_NLINES, MAX_NCOLS);

    m_myAdapter = Adapter;
    m_myAdapter->A_initscr();
    m_myAdapter->A_resize_term(MAX_NLINES, MAX_NCOLS);
    //refresh();
    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

    m_init_coloros_pair();

    text_win = m_create_text_win();
    curr_win = text_win;
    help_win = m_create_text_win();

    //панели без адаптера, но мб € их и нафиг уберу.
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
void ConsoleView::NewText()
{
    new_idx = 0;  y = 0; x = 0;
    m_TableYFirstLine = 0;
    x_nav = 0;
    table.clear();
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
void ConsoleView::PrintLineByLine(const STD::MyString& str, int y_start, int x_start)
{
    m_myAdapter->A_werase(text_win);
    m_myAdapter->A_wmove(text_win, y_start, x_start);
    STD::MyString sub_str;
    int len = 0;
    for (int i = 0; i <= LAST_Y && m_TableYFirstLine + i <= table.size() - 1; i++) {
        len = table[m_TableYFirstLine + i][LAST_IDX_LINE] - table[m_TableYFirstLine + i][FIRST_IDX_LINE] + 1;
        sub_str = str.substr(table[m_TableYFirstLine + i][FIRST_IDX_LINE], len);
        m_myAdapter->A_wprintw(text_win, sub_str.c_str());
        m_myAdapter->A_wrefresh(text_win);
    }
}
void ConsoleView::PrintLineByLineXY(const STD::MyString& str, int y_start, int x_start, int offset)
{
    int curr_y = 0, curr_x = 0, temp_x = 0;
    getyx(text_win, curr_y, curr_x);
    if (offset == 2) { //остатьс€ на месте
    }
    else if (curr_x == LAST_X || offset == 0) {
        ++curr_y; curr_x = 0;
    }
    else curr_x = curr_x + offset;

    if (curr_y > LAST_Y) {
        ++m_TableYFirstLine;
    }
    else if (curr_y < 0) {
        if (m_TableYFirstLine > 0) --m_TableYFirstLine;
        curr_y = 0;
        curr_x = table[curr_y + m_TableYFirstLine][LAST_IDX_LINE] - table[curr_y + m_TableYFirstLine][FIRST_IDX_LINE];
        //if (temp_x < curr_x + offset) curr_x = temp_x;
        //else if (curr_x != 0) curr_x += offset;
    }
    getyx(text_win, y, x);
    UpdateLineStats();
    PrintLineByLine(str, y_start, x_start);
    m_myAdapter->A_wmove(text_win, curr_y, curr_x);
    m_myAdapter->A_wrefresh(text_win);
    //new_idx = table[m_TableYFirstLine + y][FIRST_IDX_LINE] + x;
    //PutModelNewIdx(new_idx);
}

void ConsoleView::SetStartConfig()
{
    UpdateMode("...", WAITING);
    UpdateFilename("none");
    //UpdateLineStats();
    PrintMessage("Use :h for help\nUse :o filename to open file");
}
void ConsoleView::TEST_NAVI()
{
    curr_status = NAVIGATION;
    m_myController->TEST();
}
void ConsoleView::GetIdxForMove()
{
    m_myController->GetIdxForMove();
}
void ConsoleView::SetIdxForMove(const STD::MyString& str, size_t new_idx_move)
{
    MoveCursorToIdx(str, new_idx_move);
}

void ConsoleView::START()
{
    SetStartConfig();

    m_myAdapter->A_raw();
    m_myAdapter->A_keypad(stdscr, true);
    m_myAdapter->A_noecho();
    int global_num = 1, choice = 0;

    //TEST_NAVI();

    while (curr_status != EXIT)
    {
        if (curr_status == WAITING) {
            m_myAdapter->A_keypad(text_win, true);
            while (curr_status == WAITING) {
                choice = m_myAdapter->A_wgetch(text_win);
                m_myController->GetKeyFromView(choice);
            }
        }
        if (curr_status == COMMAND) {
            m_myAdapter->A_keypad(cmd_win, true);
            m_myAdapter->A_wmove(cmd_win, 0, 1);
            m_myAdapter->A_wrefresh(cmd_win);
            while (curr_status == COMMAND) {
                choice = m_myAdapter->A_wgetch(cmd_win);
                m_myController->GetKeyFromView(choice);
            }
        }
        if (curr_status == NAVIGATION) {
            m_myAdapter->A_keypad(text_win, true);
            GetIdxForMove();
            //m_myAdapter->A_wmove(text_win, 0, 0);
            m_myAdapter->A_wrefresh(text_win);
            while (curr_status == NAVIGATION) {
                choice = m_myAdapter->A_wgetch(text_win);
                m_myController->GetKeyFromView(choice);
            }
        }
        if (curr_status == NORMAL) {
            m_myAdapter->A_keypad(text_win, true);
            GetIdxForMove();
            while (curr_status == NORMAL) {
                choice = m_myAdapter->A_wgetch(text_win);
                m_myController->GetKeyFromView(choice);
            }
        }
        if (curr_status == SEARCH) {
            m_myAdapter->A_keypad(cmd_win, true);
            m_myAdapter->A_wmove(cmd_win, 0, 1);
            m_myAdapter->A_wrefresh(cmd_win);
            while (curr_status == SEARCH) {
                choice = m_myAdapter->A_wgetch(cmd_win);
                m_myController->GetKeyFromView(choice);
            }
        }
        if (curr_status == HELP) {
            m_myAdapter->A_keypad(text_win, true);
            m_myAdapter->A_wmove(text_win, 0, 0);
            m_myAdapter->A_wrefresh(text_win);
            while (curr_status == HELP) {
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
        m_ProcPressedKeyLeft(idx);
        PutModelNewIdx(new_idx);
        break;
    }
    case KEY_RIGHT: {
        m_ProcPressedKeyRight(idx);
        PutModelNewIdx(new_idx);
        break;
    }
    case KEY_A3: { //left shift + PGUP
        m_ProcPressedKeyPGUP(str, idx);
        PutModelNewIdx(new_idx);
        break;
    }
    case KEY_C3: { //left shift + PGDN
        m_ProcPressedKeyPGDN(str, idx);
        PutModelNewIdx(new_idx);
        break;
    }
    case '$': {
        m_ProcPressedDollar();
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
        break;
    }
    case 'g':
    {
        int c = m_myAdapter->A_wgetch(text_win);
        if (c == 'g') {
            m_ProcPressedgg(str, idx);
            PutModelNewIdx(new_idx);
        }
        break;
    }
    case 'r':
    {
        int c = m_myAdapter->A_wgetch(text_win);
        PutModelReplace(c);
        break;
    }
    case 'G':
    {
        m_ProcPressedG(str, idx);
        break;
    }
    default:
        break;
    }
}
void ConsoleView::PutModelReplace(int c)
{
    m_myController->PutModelReplace(c);
}
void ConsoleView::m_ProcPressedKeyPGUP(const STD::MyString& str, size_t idx)
{
    getyx(text_win, y, x);
    if (m_TableYFirstLine - TEXT_W_LINES < 0) {
        y = 0; m_TableYFirstLine = 0;
    }
    else m_TableYFirstLine -= TEXT_W_LINES;
    PrintLineByLine(str, 0, 0);
    m_myAdapter->A_wrefresh(text_win);
    x_nav = 0; x = 0;
    m_myAdapter->A_wmove(text_win, y, x);
    m_myAdapter->A_wrefresh(text_win);
    getyx(text_win, y, x);
    new_idx = table[m_TableYFirstLine + y][FIRST_IDX_LINE] + x;
}
void ConsoleView::m_ProcPressedKeyPGDN(const STD::MyString& str, size_t idx)
{
    getyx(text_win, y, x);
    if (m_TableYFirstLine + TEXT_W_LINES > table.size() - 1) {
        y = LAST_Y; m_TableYFirstLine = table.size() - TEXT_W_LINES;
    }
    else m_TableYFirstLine += TEXT_W_LINES;
    PrintLineByLine(str, 0, 0);
    m_myAdapter->A_wrefresh(text_win);
    x_nav = 0; x = 0;
    //UpdateLineStats();
    m_myAdapter->A_wmove(text_win, y, x);
    m_myAdapter->A_wrefresh(text_win);
    getyx(text_win, y, x);
    new_idx = table[m_TableYFirstLine + y][FIRST_IDX_LINE] + x;
}

void ConsoleView::m_ProcPressedKeyLeft(size_t idx) //гул€ем только по линии консоли, не по строке. скролл не нужен.
{
    getyx(text_win, y, x);
    if (x == 0) return;
    m_myAdapter->A_wmove(text_win, y, x - 1);
    m_myAdapter->A_wrefresh(text_win);
    new_idx = idx - 1; --x_nav;
}
void ConsoleView::m_ProcPressedKeyRight(size_t idx)
{
    getyx(text_win, y, x);
    if (x == table[m_TableYFirstLine + y][LAST_IDX_LINE] - table[m_TableYFirstLine + y][FIRST_IDX_LINE]) return;
    m_myAdapter->A_wmove(text_win, y, x + 1);
    m_myAdapter->A_wrefresh(text_win);
    new_idx = idx + 1; ++x_nav;
}
void ConsoleView::m_ProcPressedDollar()
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
    if (y + m_TableYFirstLine + 1 == table.size()) return; //это последн€€ строка
    getyx(text_win, y, x);
    if (y + 1 > LAST_Y) {
        ++m_TableYFirstLine;
        PrintLineByLine(str, 0, 0);
        m_myAdapter->A_wrefresh(text_win);
    }
    else y = y + 1;

    UpdateLineStats();

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
    if (y + m_TableYFirstLine - 1 < 0) return; //это перва€ строка
    getyx(text_win, y, x);
    if (y - 1 < 0) {
        --m_TableYFirstLine;
        PrintLineByLine(str, 0, 0);
        m_myAdapter->A_wrefresh(text_win);
    }
    else y = y - 1;

    UpdateLineStats();

    int new_x = table[m_TableYFirstLine + y][LAST_IDX_LINE] - table[m_TableYFirstLine + y][FIRST_IDX_LINE];
    if (new_x > x_nav) {
        m_myAdapter->A_wmove(text_win, y, x_nav);
    }
    else m_myAdapter->A_wmove(text_win, y, new_x);
    m_myAdapter->A_wrefresh(text_win);
    getyx(text_win, y, x);
    new_idx = table[m_TableYFirstLine + y][FIRST_IDX_LINE] + x;
}
void ConsoleView::m_ProcPressedgg(const STD::MyString& str, size_t idx)
{
    x = 0; y = 0; x_nav = 0;
    m_TableYFirstLine = 0;
    PrintLineByLine(str, 0, 0);

    UpdateLineStats();

    m_myAdapter->A_wrefresh(text_win);
    m_myAdapter->A_wmove(text_win, 0, 0);
    m_myAdapter->A_wrefresh(text_win);
    new_idx = 0;
}
void ConsoleView::m_ProcPressedG(const STD::MyString& str, size_t idx)
{
    getyx(text_win, y, x);
    int new_y_table = (table.size() - 1);
    if (new_y_table <= m_TableYFirstLine + LAST_Y) { //помещаетс€ в текущий экран
        int n_y = new_y_table - (m_TableYFirstLine + y);
        y += n_y;
    }
    else {
        //while (new_y_table > m_TableYFirstLine + LAST_Y) { //тут определенно можо без цикла, но врем€ позднее и € не соображаю уже
        //    ++m_TableYFirstLine;
        //} //без цикла: см. MOveCursorToidx
        m_TableYFirstLine = new_y_table - LAST_Y;
        y = LAST_Y;
        PrintLineByLine(str, 0, 0);
        m_myAdapter->A_wrefresh(text_win);
    }

    UpdateLineStats();

    m_myAdapter->A_wmove(text_win, y, 0);
    m_myAdapter->A_wrefresh(text_win);
    x_nav = 0;
    getyx(text_win, y, x);
    new_idx = table[m_TableYFirstLine + y][FIRST_IDX_LINE] + x;
}

void ConsoleView::m_ScrollForNewLine(const STD::MyString& str, size_t idx, size_t line)
{
    if (line > m_TableYFirstLine + LAST_Y) { //если новый индекс ниже экрана, скроллим вниз
        m_TableYFirstLine = line - LAST_Y;
        y = LAST_Y;
        PrintLineByLine(str, 0, 0);
        m_myAdapter->A_wrefresh(text_win);
    }
    else if (line < m_TableYFirstLine) {//новый индекс выше, скролл вверх
        m_TableYFirstLine = line;
        y = 0;
        PrintLineByLine(str, 0, 0);
        m_myAdapter->A_wrefresh(text_win);
    }
    else y = line - m_TableYFirstLine;

    UpdateLineStats();
}

void ConsoleView::MoveCursorToIdx(const STD::MyString& str, size_t idx)
{
    //size_t line = 0;
    int line = 0;
    for (; line < table.size(); line++) {
        if (table[line][FIRST_IDX_LINE] > idx) 
            break;
    }
    --line;
    int new_x = idx - table[line][FIRST_IDX_LINE];

    m_ScrollForNewLine(str, idx, line);

    m_myAdapter->A_wmove(text_win, y, new_x);
    m_myAdapter->A_wrefresh(text_win);
    x_nav = new_x;
}

void ConsoleView::JumpTo(const STD::MyString& str, size_t idx, const STD::MyString& line)
{
    //size_t new_line = atoi(line.c_str());
    int new_line = atoi(line.c_str());
    if (new_line > table.size() - 1) return;
    m_ScrollForNewLine(str, idx, new_line);
    m_myAdapter->A_wmove(text_win, y, 0);
    m_myAdapter->A_wrefresh(text_win);
    getyx(text_win, y, x);
    new_idx = new_idx = table[m_TableYFirstLine + y][FIRST_IDX_LINE] + x;
    PutModelNewIdx(new_idx);
    x_nav = 0;
}
void ConsoleView::PutLastFirstIdx()
{
    getyx(text_win, y, x);
    size_t first_idx = table[m_TableYFirstLine + y][FIRST_IDX_LINE];
    size_t last_idx = table[m_TableYFirstLine + y][LAST_IDX_LINE];
    m_myController->PutModelFirstLastIdx(first_idx, last_idx);
}

void ConsoleView::ClearCmd()
{
    m_myAdapter->A_werase(cmd_win);
    m_myAdapter->A_wprintw(cmd_win, ":");
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

void ConsoleView::UpdateLineStats()
{
    //getyx(text_win, y, x);
    m_myAdapter->A_werase(line_stats_win);
    m_myAdapter->A_printwLines(line_stats_win, "Line: %d/%d", m_TableYFirstLine + y, table.size() - 1);
    m_myAdapter->A_wrefresh(line_stats_win);
}

void ConsoleView::UpdateCmd(const STD::MyString& str)
{
    m_myAdapter->A_werase(cmd_win);
    m_myAdapter->A_wprintw(cmd_win, ":");
    m_myAdapter->A_wprintw(cmd_win, str);
    m_myAdapter->A_wrefresh(cmd_win);
}
void ConsoleView::UpdateSearch(const STD::MyString& str)
{
    m_myAdapter->A_werase(cmd_win);
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