#include "Header.h"

WindowModel::WindowModel() {

}

WindowModel::~WindowModel() {

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
	case SEARCH:
	{
		GetKeyFromSearch(key);
		break;
	}
	case HELP:
	{
		GetKeyFromHelp(key);
		break;
	}
	default:
		break;
	}
}

int WindowModel::GetKeyFromSearch(int key)
{
	if (key == '\r') {
		if (search_str.empty() || search_str.length() == 1) return 1;

		memory_search_str = search_str;
		STD::MyString str = memory_search_str.substr(1);
		if (search_str[0] == '/') {
			m_ProcPressedSearchEnd(idx, str);
		}
		else if (search_str[0] == '?') {
			m_ProcPressedSearchBegin(idx, str);
		}
		search_str.clear();
		NotifyUpdateSearch(search_str);
		SetStatus(NAVIGATION);
		return 1;
	}
	else if (key == BACKSPACE) {
		if (search_str.length() > 1) {
			search_str.erase(search_str.length() - 1, 1);
		}
		NotifyUpdateSearch(search_str);
		return 1;
	}
	else if (key == ESC) {
		search_str.clear();
		NotifyUpdateSearch(search_str);
		SetStatus(NAVIGATION);
		return 1;
	}

	search_str.append(1, key);
	NotifyUpdateSearch(search_str);
	return 1;
}
int WindowModel::GetKeyFromHelp(int key)
{
	switch (key)
	{
	case ESC:
	{
		if (!file_data.empty()) {
			SetStatus(NAVIGATION);
			NotifyUpdateVector(file_data);
			NotifyPrintLineByLine(file_data, 0, 0);
		}
		else {
			SetStatus(WAITING);
			NotifyPrintMsg("Use :h for help\nUse :o filename to open file");
		}
		//NotifyMoveCursorToIdx(file_data, idx);
	}
	default:
		break;
	}
	return 1;
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
		command_NG.clear();
		SetStatus(NAVIGATION);
	}
	else if (key == BACKSPACE) {
		if (idx != 0 && idx != file_data.length() - 1) { //по идее последнюю \n ну никак не удалишь так что else и не нужен/
			//еще заблокировала возможность удалить предпоследнюю \n.
			file_data.erase(idx - 1, 1);
			idx--;
			NotifyUpdateVector(file_data);
			NotifyUpdateLineStats();
			NotifyPrintLineByLine(file_data, 0, 0);
			NotifyMoveCursorToIdx(file_data, idx);
			//NotifyPrintLineByLineXY(file_data, 0, 0, -1);
		}
		else {
			//file_data.erase(idx, 1);
			//NotifyUpdateVector(file_data);
			//NotifyPrintLineByLineX(file_data, 0, 0, -1);
		}
	}
	else if (m_IsLetterASCII(key) || key == ' ' || key == '\r'){
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
	if (key == BACKSPACE) {
		if (!str.empty()) {
			str.erase(str.length() - 1, 1);
			NotifyUpdateCmd(str);
		}
	}
	else if (key == '\r') {
		NotifyClearCmd();
		int command = -1;
		command = ParseCommand();
		str.clear();
		if (command == STOP) {
			SetStatus(EXIT);
		}
	}
	else if (key == ESC) {
		str.clear();
		NotifyEndCmd();
		SetStatus(NAVIGATION);
	}
	else if (m_IsLetterASCII(key) || key == ' ') {
		str.append(1, key);
		NotifyUpdateCmd(str);
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
			SaveFile(filename);
			return STOP;
		}
		else if (str == "w") {
			SaveFile(filename);
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
			NotifyEndCmd();
			NotifyNewText();
			NotifyUpdateVector(file_data);
			NotifyUpdateLineStats();
			NotifyPrintLineByLine(file_data, 0, 0);
			//NotifyClearCmd(); //to return cursor in CMD
			SetStatus(NAVIGATION);
		}
		else if (str == "wq!") {
			SaveFile(filename);
			return STOP;
		}
		else if (str == "h") {
			OpenHelp();
			SetStatus(HELP);
			NotifyEndCmd();
			NotifyUpdateVector(help_str);
			NotifyPrintLineByLine(help_str, 0, 0);
		}
		else {
			for (size_t i = 0; i < str.length(); i++) {
				if (str[i] < '0' || str[i] > '9') {
					return 1;
				}
			}
			NotifyEndCmd();
			SetStatus(NAVIGATION);
			NotifyJumpTo(file_data, idx, str);
		}
	}
	return 1;
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
	file_data.clear();idx = 0; command_NG.clear(); copy_str.clear(); flag_changes = false;
	LastIdxCopyDel = 0; FirstIdxCopyDel = 0;
	char c;
	std::ifstream fin;
	fin.open(s_filename.c_str(), std::ios_base::in);
	if (!fin.is_open()) {
		beep();
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
}

void WindowModel::OpenHelp()
{
	help_str.clear();
	char c;
	std::ifstream fin;
	fin.open("help.txt", std::ios_base::in);
	if (!fin.is_open()) {
		beep();
		return;
	}
	while (!fin.eof()) {
		fin.get(c);
		help_str.append(1, c);
	}
	fin.close();
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
		m_ProcPressedb();
		NotifyMoveCursorToIdx(file_data, idx);
		break;
	}
	case 'w': {
		m_ProcPressedw();
		NotifyMoveCursorToIdx(file_data, idx);
		break;
	}
	case 'n': {
		if (memory_search_str.empty()) break;

		STD::MyString str = memory_search_str.substr(1);

		if (memory_search_str[0] == '/' && idx != file_data.length() - 1) {
			if (m_ProcPressedSearchEnd(idx + 1, str)) {
				NotifyMoveCursorToIdx(file_data, idx);
			}
		}

		else if (memory_search_str[0] == '?' && idx != 0) {
			if (m_ProcPressedSearchBegin(idx - 1, str)) {
				NotifyMoveCursorToIdx(file_data, idx);
			}
		}
		
		break;
	}
	case 'N': {
		if (memory_search_str.empty()) return 1;
		STD::MyString str = memory_search_str.substr(1);

		if (memory_search_str[0] == '/' && idx != 0) {
			if (m_ProcPressedSearchBegin(idx - 1, str)) {
				NotifyMoveCursorToIdx(file_data, idx);
			}
		}

		else if (memory_search_str[0] == '?' && idx != file_data.length() - 1) {
			if (m_ProcPressedSearchEnd(idx + 1, str)) {
				NotifyMoveCursorToIdx(file_data, idx);
			}
		}

		break;
	}
	case 'g':
	{
		SendNavigation(file_data, idx, key);
		break;
	}
	case 'r':
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
		m_ProcPressedi(); //меняем режим на режим ввода
		break;
	}
	case 'd': { //вырезать строку
		NotifyGetLastFirstIdx();
		m_ProcPreseedd();
		break;
	}
	case 'y': { //копировать строку
		NotifyGetLastFirstIdx();
		m_ProcPreseedy();
		break;
	}
	case 'p': {
		m_ProcPressedp();
		break;
	}
	case 'c': { //УДАЛИТЬ СЛОВА ИСКЛЮЧАЯ ПРОБЕЛ СПРАВА (\N НЕ ТРОГАЕТСЯ)
		m_ProcPressedc();
		break;
	}
	case 'v': { //копировать слово под курсором. видимо те же пробелы и все такое, \n не трогать
		m_ProcPressedv();
		break;
	}
	case '/':
	{
		SetStatus(SEARCH);
		search_str.append(1, key);
		NotifyUpdateSearch(search_str);
		break;
	}
	case '?':
	{
		SetStatus(SEARCH);
		search_str.append(1, key);
		NotifyUpdateSearch(search_str);
		break;
	}
	case ':':
	{
		SetStatus(COMMAND);
		NotifyClearCmd();
		break;
	}
	default:
		break;
	}

	command_NG.clear();
	return 1;
}
void WindowModel::GetSymbolReplace(int c)
{
	if (!m_IsLetterASCII(file_data[idx]) && file_data[idx] != ' ') return;
	file_data[idx] = c;
	NotifyPrintLineByLineXY(file_data, 0, 0, 2);
}
bool WindowModel::m_ProcPressedSearchBegin(size_t new_idx, STD::MyString search_str)
{
	size_t find_idx = m_ReversFind(search_str.c_str(), new_idx - search_str.length() + 1);
	if (find_idx == STD::MyString::npos) return false;
	idx = find_idx;
	return true;
}
bool WindowModel::m_ProcPressedSearchEnd(size_t new_idx, STD::MyString search_str)
{
	size_t find_idx = file_data.find(search_str.c_str(), new_idx);

	if (find_idx == STD::MyString::npos) return false;
	idx = find_idx;
	return true;
}
void WindowModel::m_FindOneWord(size_t* left, size_t* right, size_t temp_idx)
{
	if (file_data[temp_idx] == ' ') {
		while (temp_idx - 1 != STD::MyString::npos && file_data[temp_idx - 1] == ' ')
			--temp_idx;
		*left = temp_idx;
		temp_idx = idx;
		while (temp_idx + 1 != file_data.size() - 1 && file_data[temp_idx + 1] == ' ') { //вроде бы у нас не получится удалить \n
			++temp_idx;
		}
		*right = temp_idx;
	}
	else {
		while (temp_idx - 1 != STD::MyString::npos && m_IsLetterASCII(file_data[temp_idx - 1])) {
			--temp_idx;
		}
		*left = temp_idx;
		temp_idx = idx;
		while (temp_idx + 1 != file_data.size() - 1 && m_IsLetterASCII(file_data[temp_idx - 1])) {
			++temp_idx;
		}
		*right = temp_idx;
	}
}
void WindowModel::m_ProcPressedc()
{
	if (file_data[idx] == '\n') return;

	size_t left = 0, right = 0;
	m_FindOneWord(&left, &right, idx);

	size_t len = right - left + 1;
	file_data.erase(left, len);
	idx = left;
	NotifyUpdateVector(file_data);
	//NotifyUpdateLineStats(); //сомневаюсь, что линия будет меняться, ведь я не могу удалить \n
	NotifyPrintLineByLine(file_data, 0, 0);
	NotifyMoveCursorToIdx(file_data, idx);
}
void WindowModel::m_ProcPressedv()
{
	if (file_data[idx] == '\n') return;
	size_t left = 0, right = 0;
	m_FindOneWord(&left, &right, idx);
	size_t len = right - left + 1;
	copy_str = file_data.substr(left, len);
}
bool WindowModel::m_IsLetterASCII(int ch) //not quite a "letter"
{
	if (ch <= ' ' || ch >= 127) return false;

	return true;
}

void WindowModel::m_ProcPressedw()
{
	if (idx == file_data.length() - 1) return;

	size_t temp_idx = idx;

	if (file_data[idx] == '\n') ++temp_idx;

	if (file_data[temp_idx] == ' ') {
		while (temp_idx + 1 < file_data.length() && !m_IsLetterASCII(file_data[temp_idx + 1])) {
			++temp_idx; //нашла НАЧАЛО след. слова
		}
		while (temp_idx + 1 < file_data.length() && m_IsLetterASCII(file_data[temp_idx + 1]))
			++temp_idx; //конец след. слова
	}
	else {
		if (temp_idx + 1 < file_data.length() && m_IsLetterASCII(file_data[temp_idx + 1])) {
			while (temp_idx + 1 < file_data.length() && m_IsLetterASCII(file_data[temp_idx + 1]))
				++temp_idx;
		}
		else if (temp_idx + 1 != file_data.length()) {
			++temp_idx;
			while (temp_idx + 1 < file_data.length() && !m_IsLetterASCII(file_data[temp_idx + 1])) {
				++temp_idx; //нашла НАЧАЛО след. слова
			}
			while (temp_idx + 1 < file_data.length() && m_IsLetterASCII(file_data[temp_idx + 1]))
				++temp_idx; //конец след. слова
		}
	}
	idx = temp_idx;
}

void WindowModel::m_ProcPressedb()
{
	if (idx == 0) return;

	size_t temp_idx = idx;

	if (file_data[idx] == '\n') --temp_idx;

	if (file_data[temp_idx] == ' ') {
		while (temp_idx - 1 != STD::MyString::npos && !m_IsLetterASCII(file_data[temp_idx - 1])) {
			--temp_idx; //нашла КОНЕЦ след. слова
		}
		while (temp_idx - 1 != STD::MyString::npos && m_IsLetterASCII(file_data[temp_idx - 1]))
			--temp_idx; //начало след. слова
	}
	else {
		if (temp_idx - 1 != STD::MyString::npos && m_IsLetterASCII(file_data[temp_idx - 1])) {
			while (temp_idx - 1 != STD::MyString::npos && m_IsLetterASCII(file_data[temp_idx - 1]))
				--temp_idx;
		}
		else if (temp_idx - 1 != STD::MyString::npos) {
			--temp_idx;
			while (temp_idx - 1 != STD::MyString::npos && !m_IsLetterASCII(file_data[temp_idx - 1])) {
				--temp_idx; //нашла конец след. слова
			}
			while (temp_idx - 1 != STD::MyString::npos && m_IsLetterASCII(file_data[temp_idx - 1]))
				--temp_idx; //начало след. слова
		}
	}
	idx = temp_idx;
}

void WindowModel::m_ProcPreseedy()
{
	copy_str.clear();
	size_t len = LastIdxCopyDel - FirstIdxCopyDel + 1;
	copy_str = file_data.substr(FirstIdxCopyDel, len);
	if (copy_str[len - 1] != '\n') copy_str.append(1, '\n');
	//if (copy_str[len - 1] != '\n') copy_str.append(1, '\n');
	FirstIdxCopyDel = 0; LastIdxCopyDel = 0;
}
void WindowModel::m_ProcPressedp()
{
	if (idx == file_data.length() - 1) return;//я запрещаю вставлять после \n последнего
	if (copy_str.empty()) return;

	file_data.insert(idx + 1, copy_str.c_str());
	idx += copy_str.length();
	NotifyUpdateVector(file_data);
	NotifyUpdateLineStats();
	NotifyPrintLineByLine(file_data, 0, 0);
	NotifyMoveCursorToIdx(file_data, idx);
}

void WindowModel::m_ProcPreseedd()
{
	copy_str.clear();
	size_t len = LastIdxCopyDel - FirstIdxCopyDel + 1;

	copy_str = file_data.substr(FirstIdxCopyDel, len);
	m_DeleteLine(0, 0); //0 0 потому что пока что я нигде не использую аргументы этой команды. МБ УДАЛИТЬ ИЗ АРГУМЕНТОВ ИЛИ ПЕРЕДАВАТЬ ПОЛЯ ЭТИ
}

void WindowModel::m_DeleteLine(size_t first_idx, size_t last_idx)
{
	size_t len = LastIdxCopyDel - FirstIdxCopyDel + 1;

	if (len == 1 && file_data[FirstIdxCopyDel] == '\n') return; //нельзя удалить одну \n.

	file_data.erase(FirstIdxCopyDel, len);
	file_data.insert(FirstIdxCopyDel, "\n");
	idx = FirstIdxCopyDel;
	NotifyUpdateVector(file_data);
	NotifyUpdateLineStats();
	NotifyPrintLineByLine(file_data, 0, 0);
	NotifyMoveCursorToIdx(file_data, FirstIdxCopyDel);
	FirstIdxCopyDel = 0; LastIdxCopyDel = 0;
}

void WindowModel::m_ProcPreseedS()
{
	m_DeleteLine(0, 0);
}

void WindowModel::m_ProcPressedx()
{
	if (idx == file_data.size() - 1 || idx + 1 == file_data.size() - 1) return;
	if (idx + 2 == file_data.size() - 1) return;
	//if (idx + 1 == '\n') return;

	file_data.erase(idx + 1, 1);
	NotifyUpdateVector(file_data);
	NotifyUpdateLineStats();
	NotifyPrintLineByLineXY(file_data, 0, 0, 2);
	//NotifyPrintLineByLine(file_data, 0, 0);
	//NotifyMoveCursorToIdx(file_data, idx); //можно заменить такое трудоемкое дейсвтие MOVEXY и сказать остаться на месте
}
void WindowModel::m_ProcPressedi()
{
	flag_changes = true;
	SetStatus(NORMAL);
	NotifyMoveCursorToIdx(file_data, idx);
}

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
	//if (start_idx == STD::MyString::npos) //возможно, полная ерунда
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

