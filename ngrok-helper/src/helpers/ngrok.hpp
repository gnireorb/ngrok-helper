#pragma once
#include "common.hpp"

namespace ngrok
{
	void init();
	bool create_tunnel(int port, int region);
	std::string get_public_url();
}