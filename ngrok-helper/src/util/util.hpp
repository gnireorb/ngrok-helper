#pragma once
#include "common.hpp"

namespace util
{
	bool create_process(STARTUPINFO startup_info, PROCESS_INFORMATION process_info, std::string cmd_args);

	std::string read_file( std::string file_name );

	bool file_exists( std::string string );
	bool create_file( std::string file_name );
	bool write_to_file( std::string file_name, std::string json );
	void to_clipboard( HWND hwnd, const std::string& s );
};