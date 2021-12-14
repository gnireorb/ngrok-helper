#pragma once
#include "common.hpp"

namespace ngrok
{
	void init();
	void download_ngrok(const char* url);
	bool create_tunnel(int port, int region);
	std::string get_public_url();
}