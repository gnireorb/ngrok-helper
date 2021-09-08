#pragma once
#include "common.hpp"

namespace util
{
	bool create_file(const std::string& path);
	bool write_file(const std::string& path, const std::string& str);
	std::string read_file(const std::string& path);
	bool file_exists(std::string path);
	void to_clipboard(HWND hwnd, const char* str);
};