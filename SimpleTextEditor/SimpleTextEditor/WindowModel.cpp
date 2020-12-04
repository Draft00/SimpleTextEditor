#include "Header.h"

WindowModel::WindowModel()
{

}

WindowModel::~WindowModel()
{
}


void WindowModel::SetStartConfig()
{
	/*NotifyUpdateMode(mode_str[curr_status].c_str());
	NotifyUpdateFilename(filename.c_str());
	NotifyUpdateLineStats(num_curr_line, num_lines);
	char hello_message[] = "Use :h for help\nUse :o filename to open file";
	NotifyPrintMsg(hello_message);*/
}

void WindowModel::SetStatus(status new_status)
{
	curr_status = new_status;
	NotifyUpdateMode(mode_str[new_status], new_status);
}

void WindowModel::SetFilename(const char* new_filename)
{
	filename = new_filename;
	NotifyUpdateFilename(new_filename);
}

void WindowModel::m_ProcKeyInWaiting(int key)
{
	switch (key)
	{
	case ':':
	{
		SetStatus(COMMAND);
		NotifyClearCmd();
		break;
	}
	default:
		break;
	}
}

void WindowModel::GetKeyFromController(int key)
{
	switch (curr_status)
	{
	case WAITING:
	{
		m_ProcKeyInWaiting(key);
		break;
	}
	case NAVIGATION:
	{
		GetKeyFromNavigation(key);
		break;
	}
	case COMMAND:
	{
		GetKeyFromCmd(key);
		break;
	}
	case NORMAL:
	{
		GetKeyFromNormal(key);
		break;
	}
	default:
		break;
	}
}
int WindowModel::GetKeyFromNormal(int key)
{
	if (key == KEY_UP) {
		SendNavigation(file_data, idx, key);
	}
	else if (key == KEY_RIGHT) {
		SendNavigation(file_data, idx, key);
	}
	else if (key == KEY_DOWN) {
		SendNavigation(file_data, idx, key);
	}
	else if (key == KEY_LEFT) {
		SendNavigation(file_data, idx, key);
	}
	else if (key == ESC) {
		//TODO выхож из режима и в навигацию ¬–ќƒ≈ “ј  Ќќ Ќјƒќ “≈—“»“№ ≈ў≈
		command_NG.clear();
		curr_status = NAVIGATION;
		NotifyUpdateMode(mode_str[NAVIGATION], NAVIGATION);
		NotifyMoveCursorToIdx(file_data, idx);
	}
	else if (key == BACKSPACE) {
		if (idx != 0) { //по идее последнюю \n ну никак не удалишь так что else и не нужен
			file_data.erase(idx - 1, 1);
			idx--;
			NotifyUpdateVector(file_data);
			NotifyUpdateLineStats();
			NotifyPrintLineByLineXY(file_data, 0, 0, -1);
		}
		else {
			//file_data.erase(idx, 1);
			//NotifyUpdateVector(file_data);
			//NotifyPrintLineByLineX(file_data, 0, 0, -1);
		}
	}
	else if ((key >= 32 && key <= 126) || key == '\r'){
		int n_flag = 1; 
		if (key == '\r') {
			key = '\n';
			n_flag = 0;
		}
		file_data.insert(idx, 1, key);
		idx++;
		NotifyUpdateVector(file_data);
		NotifyUpdateLineStats();
		NotifyPrintLineByLineXY(file_data, 0, 0, n_flag);
	}
	return 1;
}
int WindowModel::GetKeyFromCmd(int key)
{
	switch (key)
	{
	case 8:
	{
		if (!str.empty()) {
			str.erase(str.length() - 1, 1);
		}
		NotifyUpdateCmd(str);
		break;
	}
	case 13:
	{
		NotifyClearCmd();
		int command = -1;
		command = ParseCommand();
		str.clear();
		if (command == STOP) {
			curr_status = EXIT;
			NotifyUpdateMode("...", EXIT);
			//return -1;
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
		NotifyUpdateCmd(str);
		break;
	}
	return 1;
}

int WindowModel::ParseCommand()
{
	if (!str.empty())
	{
		if (str == "q!") {
			return STOP;
		}
		else if (str == "q") {
			if (flag_changes == 1) {
				beep();
				return 1;
			}
			return STOP;
		}
		else if (str == "x") {
			SaveFile();
			return STOP;
		}
		else if (str == "w") {
			SaveFile();
		}
		else if (str[0] == 'w') {
			STD::MyString filename_save;
			filename_save = str.substr(2);
			SaveFile(filename_save);
		}
		else if (str[0] == 'o') {
			STD::MyString filename;
			filename = str.substr(2);
			OpenFile(filename);
			NotifyPrintMsg(file_data.c_str());
			NotifyClearCmd(); //to return cursor in CMD
		}
		else if (str == "wq!") {
			SaveFile();
			return STOP;
		}
		else if (str == "h") {

		}
		else { //TODO TEST IT!!!
			for (int i = 0; i < str.length(); i++) {
				if (str[i] <= '0' || str[i] >= '9') {
					return 1;
				}
			}
			NotifyJumpTo(file_data, idx, str);
		}
	}
	return 1;
}
void WindowModel::SaveFile()
{
	std::ofstream fout;
	fout.open(filename.c_str(), std::ios_base::out);
	if (!fout.is_open()) {
		beep();
		return;
	}
	fout << file_data;
	fout.close();
}
void WindowModel::SaveFile(STD::MyString s_filename)
{
	std::ofstream fout;
	fout.open(s_filename.c_str(), std::ios_base::out);
	if (!fout.is_open()) {
		beep(); 
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
	/*do {
		fin.get(c);
		file_data.append(1, c);
	} while (!fin.eof());*/
	while (!fin.eof()) {
		fin.get(c);
		file_data.append(1, c);
	}
	fin.close();
	filename = s_filename;
	NotifyUpdateFilename(s_filename);
	//CountLines();
	//NotifyUpdateLineStats(num_curr_line, num_lines);
}

int WindowModel::GetKeyFromNavigation(int key)
{
	if (key >= '0' && key <= '9') {
		command_NG.append(1, key);
		return 1;
	}

	switch (key)
	{
	case '$': {
		SendNavigation(file_data, idx, key);
		break;
	}
	case '0': {
		SendNavigation(file_data, idx, key);
		break;
	}
	case KEY_DOWN:
	{
		SendNavigation(file_data, idx, key);
		break;
	}
	case KEY_UP:
	{
		SendNavigation(file_data, idx, key);
		break;
	}

	case KEY_LEFT:
	{
		SendNavigation(file_data, idx, key);
		break;
	}
	case KEY_RIGHT:
	{
		SendNavigation(file_data, idx, key);
		break;
	}
	case KEY_A3: { //left shift + PGUP
		SendNavigation(file_data, idx, key);
		break;
	}
	case KEY_C3: { //left shift + PGDN
		SendNavigation(file_data, idx, key);
		break;
	}
	case 'b':
	{
		NotifyPressedb();
		break;
	}
	case 'w': {
		ProcPressedw();
		break;
	}
	case 'g':
	{
		SendNavigation(file_data, idx, key);
		break;
	}
	case 'G':
	{
		if (command_NG.empty()) {
			SendNavigation(file_data, idx, key);
		}
		else NotifyJumpTo(file_data, idx, command_NG);
		break;
	}
	case 'i':
	{
		m_ProcPressedi();
		break;
	}
	case 'I': { //TODO TEST
		SendNavigation(file_data, idx, '0'); //перейти в начало строки
		m_ProcPressedi();
		break;
	}
	case 'A': {
		SendNavigation(file_data, idx, '$'); //перейти в конец строки
		m_ProcPressedi();
		break;
	}
	case 'x': {
		m_ProcPressedx();
		break;
	}
	case 'S': {
		NotifyGetLastFirstIdx();
		m_ProcPreseedS();
		m_ProcPressedi(); //мен€ем режим на режим ввода
		break;
	}
	case 'd': { //вырезать строку
		NotifyGetLastFirstIdx();
		m_ProcPreseedd();
		break;
	}
	case 'y': {
		NotifyGetLastFirstIdx();
		m_ProcPreseedy();
		break;
	}
	case 'p': {
		m_ProcPressedp();
		break;
	}
	case 'c': { //”ƒјЋ»“№ —Ћќ¬ј »— Ћё„јя ѕ–ќЅ≈Ћ —ѕ–ј¬ј (\N Ќ≈ “–ќ√ј≈“—я)
		m_ProcPressedc();
		break;
	}
	default:
		break;
	}

	command_NG.clear();
	return 1;
}
void WindowModel::m_FindOneWord(size_t* left, size_t* right, size_t temp_idx)
{
	if (file_data[temp_idx] == ' ') {
		while (file_data[temp_idx - 1] == ' ' && temp_idx - 1 != 0)
			--temp_idx;
		*left = temp_idx;
		temp_idx = idx;
		while (file_data[temp_idx + 1] == ' ' && temp_idx + 1 != file_data.size() - 1) { //вроде бы у нас не получитс€ удалить \n
			++temp_idx;
		}
		*right = temp_idx;
	}
	else {
		while (file_data[temp_idx - 1] > ' ' && file_data[temp_idx - 1] < 127 && temp_idx - 1 != 0) {
			--temp_idx;
		}
		*left = temp_idx;
		temp_idx = idx;
		while (file_data[temp_idx + 1] > ' ' && file_data[temp_idx + 1] < 127 && temp_idx + 1 != file_data.size() - 1) { //вроде бы у нас не получитс€ удалить \n
			++temp_idx;
		}
		*right = temp_idx;
	}
}
void WindowModel::m_ProcPressedc()
{
	if (file_data[idx] == '\n') return;

	size_t left = 0, right = 0;
	size_t temp_idx = idx;
	if (file_data[temp_idx] == ' ') {
		while (file_data[temp_idx - 1] == ' ' && temp_idx - 1 != 0)
			--temp_idx;
		left = temp_idx;
		temp_idx = idx;
		while (file_data[temp_idx + 1] == ' ' && temp_idx  + 1 != file_data.size() - 1) { //вроде бы у нас не получитс€ удалить \n
			++temp_idx;
		}
		right = temp_idx;
		size_t len = right - left + 1;
		file_data.erase(left, len);
		idx = left; 
	}
	else {
		while (file_data[temp_idx - 1] > ' ' && file_data[temp_idx - 1] < 127 && temp_idx - 1!= 0) {
			--temp_idx;
		}
		left = temp_idx;
		temp_idx = idx;
		while (file_data[temp_idx + 1] > ' ' && file_data[temp_idx + 1] < 127 && temp_idx + 1 != file_data.size() - 1) { //вроде бы у нас не получитс€ удалить \n
			++temp_idx;
		}
		right = temp_idx;
		size_t len = right - left + 1;
		file_data.erase(left, len);
		idx = left;
	}

	size_t len = right - left + 1;
	file_data.erase(left, len);
	idx = left;
	NotifyUpdateVector(file_data);
	//NotifyUpdateLineStats(); //сомневаюсь, что лини€ будет мен€тьс€, ведь € не могу удалить \n
	NotifyPrintLineByLine(file_data, 0, 0);
	NotifyMoveCursorToIdx(file_data, idx);
}
void WindowModel::m_ProcPreseedy()
{
	copy_str.clear();
	size_t len = LastIdxCopyDel - FirstIdxCopyDel + 1;
	copy_str = file_data.substr(FirstIdxCopyDel, len);
}
void WindowModel::m_ProcPressedp()
{
	if (copy_str.empty()) return;
	if (idx == file_data.length() - 1) return; //€ запрещаю вставл€ть после последней \n
	file_data.insert(idx + 1, copy_str.c_str()); //ѕ–ќ“≈—“»“№ ћќ∆Ќќ Ћ» ¬—“ј¬»“№ ѕќ—Ћ≈ ѕќ—Ћ≈ƒЌ≈… \n
	idx += copy_str.length();
	NotifyUpdateVector(file_data);
	NotifyUpdateLineStats();
	NotifyPrintLineByLine(file_data, 0, 0);
	NotifyMoveCursorToIdx(file_data, idx);
	FirstIdxCopyDel = 0; LastIdxCopyDel = 0;
}

void WindowModel::m_ProcPreseedd()
{
	copy_str.clear();
	if (file_data.size() == 1 && file_data[0] == '\n') return; //последнюю \n нельз€ удал€ть.
	size_t len = LastIdxCopyDel - FirstIdxCopyDel + 1;
	copy_str = file_data.substr(FirstIdxCopyDel, len);
	m_DeleteLine(0, 0); //0 0 потому что пока что € нигде не использую аргументы этой команды. ћЅ ”ƒјЋ»“№ »« ј–√”ћ≈Ќ“ќ¬ »Ћ» ѕ≈–≈ƒј¬ј“№ ѕќЋя Ё“»
}
void WindowModel::m_DeleteLine(size_t first_idx, size_t last_idx)
{
	size_t len = LastIdxCopyDel - FirstIdxCopyDel + 1;
	file_data.erase(FirstIdxCopyDel, len);
	idx = FirstIdxCopyDel;
	NotifyUpdateVector(file_data);
	NotifyUpdateLineStats();
	NotifyPrintLineByLine(file_data, 0, 0);
	NotifyMoveCursorToIdx(file_data, FirstIdxCopyDel);
	FirstIdxCopyDel = 0; LastIdxCopyDel = 0;
}

void WindowModel::m_ProcPreseedS()
{
	if (file_data.size() == 1 && file_data[0] == '\n') return; //последнюю \n нельз€ удал€ть.

	//ѕќ—ћќ“–» ѕќ«ћќ∆Ќќ «јћ≈Ќ»“№ Ќј m_DeleteLine
	size_t len = LastIdxCopyDel - FirstIdxCopyDel + 1;
	file_data.erase(FirstIdxCopyDel, len);
	idx = FirstIdxCopyDel;
	NotifyUpdateVector(file_data);
	NotifyUpdateLineStats();
	NotifyPrintLineByLine(file_data, 0, 0);
	NotifyMoveCursorToIdx(file_data, FirstIdxCopyDel);
	FirstIdxCopyDel = 0; LastIdxCopyDel = 0;
}

void WindowModel::m_ProcPressedx()
{
	if (file_data.size() == 1 && file_data[0] == '\n') return;
	if (idx + 1 != file_data.size()) {
		file_data.erase(idx + 1, 1);
		NotifyUpdateVector(file_data);
		NotifyUpdateLineStats();
		NotifyPrintLineByLine(file_data, 0, 0);
		NotifyMoveCursorToIdx(file_data, idx); //можно заменить такое трудоемкое дейсвтие MOVEXY и сказать остатьс€ на месте
	}
}
void WindowModel::m_ProcPressedi()
{
	flag_changes = true;
	curr_status = NORMAL;
	NotifyUpdateMode(mode_str[NORMAL], NORMAL);
	NotifyMoveCursorToIdx(file_data, idx);
}


//ƒјЋ№Ў≈ ¬≈–ќя“Ќќ ”ƒјЋ»“№ Ќќ Ќ≈ ¬—≈!!!
bool WindowModel::m_CheckScrollDown() const
{
	if (y > IDX_LAST_LINE /*&& TODO еще что-то */) 
		return true;
	return false;

}

bool WindowModel::m_CheckScrollUp() const
{
	if (y < 0 && idx - x != 0) {
		return true;
	}
	return false; 
}
bool WindowModel::m_ScrollDown(int curr_pos, int n)
{
	if (!m_CheckScrollDown())
		return false;
	idx_first_line = m_CountIdxFirstLineDown(1);
	const char* ptr = file_data.c_str() + idx_first_line;
	NotifymvPrintMsg(ptr, 0, 0);
	y = IDX_LAST_LINE;
	return true;
}
bool WindowModel::m_ScrollUp(int curr_pos, int n)
{
	if (!m_CheckScrollUp())
		return false;
	idx_first_line = m_CountIdxFirstLineUp(1);
	const char* ptr = file_data.c_str() + idx_first_line;
	NotifymvPrintMsg(ptr, 0, 0);
	y = 0;
	return true;
}



void WindowModel::m_ProcPressedKeyLeft()
{
	if (file_data[idx] == '\n') return;
	size_t curr_idx = idx;
	if (curr_idx > 0 && file_data[idx - 1] != '\n') {
		if (x == 0) {
			x = MAX_NCOLS;
			y--;
			m_ScrollUp(0, 1);
		}
		x--; idx--; x_nav = x;
	}
	NotifyMoveCursor(y, x_nav);
}

void WindowModel::m_ProcPressedKeyRight()
{
	if (file_data[idx] == '\n') return;
	if (idx + 1 < file_data.length() - 1 && file_data[idx + 1] != '\n') {
		if (x == IDX_LAST_COL) {
			x = -1;
			y++;
			m_ScrollDown(0, 1);
		}
		x++; idx++; x_nav = x;
	}
	NotifyMoveCursor(y, x_nav);
}

void WindowModel::m_ProcPressedDollar()
{
	if (file_data[idx] == '\n') return;
	while (idx + 1 < file_data.length() - 1 && file_data[idx + 1] != '\n') {
		m_ProcPressedKeyRight();
	}
}

void WindowModel::m_ProcPressedZero()
{
	if (file_data[idx] == '\n') return;
	while (idx > 0 && file_data[idx - 1] != '\n'){
		m_ProcPressedKeyLeft();
	}
}

size_t WindowModel::m_CountIdxFirstLineDown(int n) const
{
	if (idx - x == 0) return 0; //строка единственна€
	size_t curr_idx_f = idx_first_line;
	int x_curr = 0;
	for (int i = 0; i < n; i++)
	{
		while (file_data[curr_idx_f] != '\n') {
			++curr_idx_f; ++x_curr;
			if (x_curr == IDX_LAST_COL) {
				x_curr = 0;
				break;
			}
		}
		++curr_idx_f;
	}
	return curr_idx_f;
}

size_t WindowModel::m_CountIdxFirstLineUp(int n) const
{
	//todo тут вообще все
	if (idx - x == 0) 
		return 0; //строка единственна€, но там и так тыща проверок, вр€д ли такое случитс€.
	size_t curr_idx_f = idx_first_line - 1;
	if (curr_idx_f == 0 || file_data[curr_idx_f - 1] == '\n')
		return curr_idx_f;
	--curr_idx_f;
	int x_curr = IDX_LAST_COL;
	for (int i = 0; i < n; i++) {
		while (curr_idx_f != 0 && file_data[curr_idx_f] != '\n') {
			--curr_idx_f; --x_curr;
			if (x_curr == 0) {
				break;
			}
		}
		if (curr_idx_f > 0) ++curr_idx_f;
	}
	return 0;
}

void WindowModel::m_GotoXNav()
{
	int curr_pos = 0;
	while (curr_pos < (x_nav + 1) && file_data[idx] != '\n' && file_data[idx] != '\0')
	{
		//(x_nav + 1) чтобы --curr_pos после цикла всегда давало верный шаг
		++curr_pos; ++idx; ++x;
	}
	--idx; --curr_pos; --x;
	if (curr_pos == -1) {
		idx++;
		curr_pos++; ++x;
	}
}
//void WindowModel::m_ProcPressedKeyDown()
//{
//	int x_nav_curr = x_nav;
//	m_ProcPressedDollar();
//	x_nav = x_nav_curr;
//	if (idx + 1 > (file_data.length() - 1)) return;
//	if (file_data[idx] == '\n') {
//		++idx;
//	}
//	else idx += 2;
//
//	x = 0; ++y; ++num_curr_line;
//	m_ScrollDown(2, 1);
//	if (y > IDX_LAST_LINE) y = IDX_LAST_LINE; //вообще не нужно, но на вс€кий случай
//	NotifyUpdateLineStats(num_curr_line, num_lines);
//	m_GotoXNav();
//	NotifyMoveCursor(y, x);
//}
//
//size_t WindowModel::m_FindStartIdxLine()
//{
//	return 0;
//}
//
//void WindowModel::m_ProcPressedKeyUp()
//{
//	//«ƒ≈—№ проблемы
//	if (idx - x == 0) return;
//
//	char c = file_data[idx];
//	int x_nav_curr = x_nav; 
//	m_ProcPressedZero();
//	x_nav = x_nav_curr;
//	--idx; --y; 
//	m_ScrollUp(2, 1);
//
//	if (file_data[idx - 1] != '\n') idx--;
//	
//	while (idx != 0 && file_data[idx] != '\n') {
//		--idx;
//	}
//
//	//после этого текущий х - Ќ≈ ноль
//
//	x = 0; --y;  --num_curr_line;
//	m_ScrollUp(2, 1);
//	if (y < 0) y = 0; //костыль?
//	NotifyUpdateLineStats(num_curr_line, num_lines);
//	m_GotoXNav();
//	NotifyMoveCursor(y, x);
//	/*
//	int curr_pos = 0;
//	size_t temp_idx = 0, next_idx = 0, curr_idx = idx;
//
//	if (file_data[idx] == '\n') {
//		idx--;
//	}
//	else idx = m_ReversFind("\n", idx);
//
//	if (idx != 0) {
//		next_idx = m_ReversFind("\n", idx - 1);
//		if (next_idx == STD::MyString::npos) {
//			if (file_data[idx - 1] == '\n') {}
//			else idx = 0;
//		}
//		else idx = next_idx + 1;
//	}
//
//	while (curr_pos < (x_nav + 1) && file_data[idx] != '\n' && file_data[idx] != '\0')
//	{
//		//(x_nav + 1) чтобы --curr_pos после цикла всегда давало верный шаг
//		++curr_pos; ++idx;
//	}
//	--idx; --curr_pos;
//	if (curr_pos == -1) {
//		idx++;
//		curr_pos++;
//	}
//
//	if (y - 1 < 0)
//	{
//		if (m_CheckScrollDown())
//			//if (m_ScrollUp(curr_pos, 1))
//		{
//			m_ScrollDown(curr_pos, 1);
//			//idx_last_line = idx - curr_pos;
//			num_curr_line--;
//			NotifyUpdateLineStats(num_curr_line, num_lines);
//			NotifyMoveCursor(y, curr_pos);
//		}
//		else idx = curr_idx;
//	}
//	else {
//		num_curr_line--;
//		NotifyUpdateLineStats(num_curr_line, num_lines);
//		NotifyMoveCursor(y - 1, curr_pos);
//	}
//	*/
//}

bool WindowModel::m_find_compare(const char* str, size_t len, size_t pos) const
{
	for (size_t i = 1; i < len; ++i)
	{
		if (file_data[pos + i] != *(str + i))
			return false;
	}
	return true;
}
size_t WindowModel::m_reverse_find(const char* str, size_t start_idx, size_t len_s) const
{
	size_t ResultPos = STD::MyString::npos;
	if (len_s == 0)
		return 0;
	//return ResultPos;
	//if (start_idx == STD::MyString::npos) //возможно, полна€ ерунда
		//start_idx = file_data.length()- 1;
	size_t len_ = file_data.length();
	for (; start_idx > 0; --start_idx)
	{
		if (file_data[start_idx] == *str && m_find_compare(str, len_s, start_idx))
		{
			ResultPos = start_idx;
			break;
		}
	}
	if (start_idx == 0) {
		if (file_data[start_idx] == *str && m_find_compare(str, len_s, start_idx)) {
			ResultPos = start_idx;
		}
	}
	return ResultPos;
}

size_t WindowModel::m_ReversFind(const char* str, size_t start_idx) const
{
	return m_reverse_find(str, start_idx, strlen(str));
}


size_t WindowModel::m_FindSymbol(size_t idx) const
{
	return 0;
}

size_t WindowModel::m_DropSpace(size_t idx_) const
{
	while (idx_ - file_data.find(" ", idx_ + 1) == 1) idx_++; //если idx_ = npos - 1, то катастрофа. 
	if (file_data[idx_ + 1] == '\0') {
		beep();
		return STD::MyString::npos;
	}
	else idx_++;
	return idx;
}

void WindowModel::ProcPressedw()
{
	//m_GetYX(&y, &x);
	int new_x = x, new_y = y;
	size_t temp_idx = idx, idx_s = idx;

	if (file_data[idx + 1] != '\0') {
		size_t idx_ = file_data.find(" ", idx);
		size_t idx_n = file_data.find("\n", idx);

		if (idx_ < idx_n) { // раньше _
			idx_s = m_DropSpace(idx_);
			if (idx_s == STD::MyString::npos) {
				beep();
				return;
			}

			if (file_data[idx_ + 1] == '\0') {
				beep();
				return;
			}
			else if (file_data[idx_ + 1] == '\n') {
				while (1) {
					size_t idx_n_2 = file_data.find("\n", idx);
					size_t idx_symbol = idx_;
					while (file_data[idx_] < 33 && file_data[idx_] > 127) idx_symbol++;
					//if ()
				}


				if (file_data[idx_ + 1] != '\0') {
					new_y++; idx_++;
					size_t idx_n = file_data.find("\n", idx);
				}
				else {//TODO
				}
				//while ()

			}
			else {
				idx = idx_ + 1;
				x_nav += idx - temp_idx; x = x_nav; //прокатит чисто разницей idx потом учто не было перехода на строку.
				NotifyMoveCursor(y, x_nav);
			}

		}
		else if (idx_ != STD::MyString::npos) {
			new_y++;
			
			if (file_data[idx_n + 1] == ' ') {

			}
			else if (file_data[idx_n + 1] == '\0') {
				beep();
				return;
			}
			else if (file_data[idx_n + 1] == '\n') {

			}
			else {
				idx = idx_n + 1;
				x_nav = 0; x = x_nav;
				NotifyMoveCursor(new_y, x_nav);
			}
		}
		else beep();
	}
	else beep();
}