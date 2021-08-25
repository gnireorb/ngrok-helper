#include "util.hpp"

bool util::create_file(const std::string& path)
{
	std::ofstream file(path, std::ofstream::out);
	file.close();
	return file.good();
}

bool util::write_file(const std::string& path, const std::string& str)
{
	std::ofstream file(path, std::ofstream::out);
	if (!file)
		return false;

	file << str;
	file.close();
	return file.good();
}

std::string util::read_file(const std::string& path)
{
	std::string output;
	std::ifstream file(path, std::ifstream::in);
	if (!file)
		return "";

	if (!file.is_open())
		return "";

	std::getline(file, output);
	file.close();
	return output;
}

bool util::file_exists(std::string path)
{
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}

void util::to_clipboard(HWND hwnd, const std::string& str)
{
	OpenClipboard(hwnd);
	EmptyClipboard();

	HGLOBAL hglobal = GlobalAlloc(GMEM_MOVEABLE, str.size() + 1);
	if (!hglobal) 
		CloseClipboard(); return;

	std::memcpy(GlobalLock(hglobal), str.c_str(), str.size() + 1);
	GlobalUnlock(hglobal);
	SetClipboardData(CF_TEXT, hglobal);
	CloseClipboard();
	GlobalFree(hglobal);
}