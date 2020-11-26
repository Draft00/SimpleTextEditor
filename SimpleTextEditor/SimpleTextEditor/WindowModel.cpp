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
		if (!str.empty())
		{
			str.erase(str.length() - 1, 1);
		}
		NotifyUpdateCmd();
		break;
	}
	case 13:
	{
		NotifyClearCmd();
		int command = -1;
		command = ParseCommand();
		str.clear();
		if (command == STOP) {
			return -1;
		}
		break;
	}
	case 27: //ESC
	{
		str.clear();
		NotifyEndCmd();
		SetStatus(NAVIGATION);
		return 0;
		break;
	}
	default:
		str.append(1, key);
		NotifyUpdateCmd();
		break;
	}
	return 1;
}

int WindowModel::ParseCommand()
{
	if (!str.empty())
	{
		if (str == "q!")
		{
			return STOP;
		}
		else if (str == "q") {
			if (flag_changes == 1)
			{
				beep();
				return 1;
			}
			return STOP;

		}
		else if (str == "x") {
			SaveInFileAndExit();
		}
		else if (str == "w") {
			
		}
		else if (str[0] == 'w') {
			//TODO parse filename
		}
		else if (str[0] == 'o') {
			STD::MyString filename;
			filename = str.substr(2);
			OpenFile(filename);
			NotifyPrintNewText();
			NotifyClearCmd(); //чтобы вернуться в cmd.
		}
		else if (str == "wq!") {

		}
		else if (str == "h") {

		}
		else
		{

		}
	}
	return 1;
}
void WindowModel::SaveInFileAndExit()
{
	std::ofstream fout;
	fout.open(filename.c_str(), std::ios_base::out);
	if (!fout.is_open()) {
		beep(); //TODO return message
		return;
	}
	fout << file_data;
	fout.close();
}

void WindowModel::OpenFile(STD::MyString s_filename)
{
	char c;
	std::ifstream fin;
	fin.open(s_filename.c_str(), std::ios_base::in);
	if (!fin.is_open()) {
		beep(); //TODO RETURN MESSAGE
		return;
	}
	while (!fin.eof()) {
		fin.get(c);
		file_data.append(1, c);
	}
	fin.close();
	filename = s_filename;
	NotifyUpdateFilename();
}

int WindowModel::GetKeyFromNavigation(int key)
{
	return 0;
}
