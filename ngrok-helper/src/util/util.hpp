#include "common.hpp"

namespace util
{
	static bool file_exists(const std::string& path)
	{
		struct stat buffer;
		return (stat(path.c_str(), &buffer) == 0);
	}

	static void to_clipboard(const char* str)
	{
		HGLOBAL hglobal = GlobalAlloc(GMEM_MOVEABLE, (strlen(str) + 1));
		memcpy(GlobalLock(hglobal), str, (strlen(str) + 1));
		GlobalUnlock(hglobal);
		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hglobal);
		CloseClipboard();
		GlobalFree(hglobal);
	}
}