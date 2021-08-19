#pragma once
#include <Windows.h>

namespace util
{
	std::string read_file( std::string file_name );

	bool file_exists( std::string string );
	bool create_file( std::string file_name );
	bool write_to_file( std::string file_name, std::string json );
	void to_clipboard( HWND hwnd, const std::string& s );
	void to_lowercase( std::string& string );
	void to_uppercase( std::string& string );
};