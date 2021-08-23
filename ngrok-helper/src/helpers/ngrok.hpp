#pragma once
#include "common.hpp"

class ngrok
{
public:
	explicit ngrok();
public:
	static bool create_tunnel(int port, int region);
	static std::string get_public_url();
};

inline ngrok* g_ngrok{};