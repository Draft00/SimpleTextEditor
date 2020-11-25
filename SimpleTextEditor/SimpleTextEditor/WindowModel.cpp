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

int WindowModel::GetKeyFromCmd(int key)
{
	switch (key)
	{
	case 8:
	{
		str.erase(str.length() - 1);
		NotifyUpdateCmd();
		break;
	}
	case 13:
	{
		NotifyClearCmd();
		//парс команды
		str.clear();
		//buffer = {0};
		idx = 0;
		break;
	}
	case 27: //ESC
	{
		str.clear();
		NotifyEndCmd();
		SetStatus(NAVIGATION);
		return 0;
	}
	default:
		str.push_back(key);
		//str[idx] = key;
		//buffer[idx] = key;
		//idx++;
		NotifyUpdateCmd();
		break;
	}
	return 1;
}
