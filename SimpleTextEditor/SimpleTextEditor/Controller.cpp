#include "Header.h"

void Controller::start()
{
    int global_num = 1;
    while (global_num)
    {
        if (m_mymodel->curr_status == WindowModel::WAITING)
        {
            noecho();
            keypad(stdscr, true);
            do {
                m_choice = getch(stdscr);
                switch (m_choice)
                {
                case ':':
                {
                    m_mymodel->set_status(WindowModel::COMMAND);
                    break;
                }
                default:
                    break;
                }
            } while (m_choice != ':');
            keypad(stdscr, false);
            echo();
        }
        if (m_mymodel->curr_status == WindowModel::COMMAND)
        {
            move(m_mymodel->MAX_NLINES - 1, 0);
            attron(COLOR_PAIR(1));
            addch(':');
            refresh();
            getyx(stdscr, m_y, m_x);
            echo();
            keypad(stdscr, true);
            do {
                m_choice = getch(stdscr);
                switch (m_choice)
                {
                case 'o':
                {
                    m_choice = getch(stdscr); // ' '
                    m_mymodel->get_filename();
                }
                default:
                    break;
                }
            } while (m_choice != 27);
            attroff(COLOR_PAIR(1));
        }
        if (m_mymodel->curr_status == WindowModel::NAVIGATION)
        {
            noecho();
            keypad(stdscr, true);
            //cbreak();
            getyx(stdscr, m_y, m_x);
            do {
                m_choice = getch(stdscr);
                switch (m_choice)
                {
                case KEY_UP:
                {
                    presssed_up();
                    break;
                }
                case KEY_DOWN:
                {
                    pressed_down();
                    break;
                }
                case KEY_LEFT:
                {
                    pressed_left();
                    break;
                }
                case KEY_RIGHT:
                {
                    pressed_right();
                    break;
                }
                case '0':
                {
                    pressed_beg_line();
                    break;
                }
                case '$':
                {
                    pressed_end_line();
                    break;
                }
                default:
                    break;
                }
            } while (m_choice != 'Q');
            keypad(stdscr, false);
            echo();
        }
    }
}
void Controller::presssed_up()
{
	if (m_y > 0)
	{
		m_y--;
		move(m_y, m_x);
		refresh();
	}
}
void Controller::pressed_down() //TODO SKROLLING
{
    if (m_y < (m_mymodel->MAX_NLINES - 3))
    {
        m_y++;
        move(m_y, m_x);
        refresh();
    }
}
void Controller::pressed_right()
{
    if (m_x < m_mymodel->MAX_NCOLS)
    {
        m_x++;
        move(m_y, m_x);
        refresh();
    }
}

void Controller::pressed_left()
{
    if (m_x > 0)
    {
        m_x--;
        move(m_y, m_x);
        refresh();
    }
}
void Controller::pressed_beg_line()
{
    m_x = 0;
    move(m_y, m_x);
    refresh();
}

void Controller::pressed_end_line()
{
    m_x = (m_mymodel->MAX_NCOLS - 1);
    move(m_y, m_x);
    refresh();
}