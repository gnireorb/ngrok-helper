#include "HTTPRequest.hpp"

#include "ngrok.hpp"
#include "util/util.hpp"

using namespace rapidjson;

ngrok::ngrok()
{
#ifndef DEBUG
	if (!util::file_exists("ngrok.exe"))
	{
		MessageBox(NULL, "O arquivo 'ngrok.exe' eh inexistente, baixe-o em 'ngrok.com'.", "Error", MB_ICONERROR);
		exit(-1);
	}

	if (!util::file_exists("settings.json"))
	{
		if (!util::create_file("settings.json"))
		{
			MessageBox(NULL, "Ocorreu uma falha ao criar o arquivo 'settings.json'.", "Error", MB_ICONERROR);
			return;
		}

		if (!util::file_exists("settings.json"))
		{
			MessageBox(NULL, "O arquivo 'settings.json' eh inexistente.", "Error", MB_ICONERROR);
			return;
		}

		if (!util::write_to_file("settings.json", "{\"last_port\":0,\"ngrok_region\":0}"))
		{
			MessageBox(NULL, "Ocorreu uma falha ao escrever em 'settings.json'.", "Error", MB_ICONERROR);
			return;
		}
	}
#endif
}

bool ngrok::create_tunnel(int port, int region)
{
	WinExec("", 1);

	// Impressive.
	auto run_command = [](std::string command) {
		auto _system = [](std::string _command) {
			system(_command.c_str());
		};
		std::thread(_system, command).detach();
	};

	std::string command;

	if (region >= 4 || region < 0)
		return false;

	switch (region)
	{
	case 0:
		command = std::string("ngrok tcp " + std::to_string(port) + " --region sa");
		run_command(command);
		break;
	case 1:
		command = std::string("ngrok tcp " + std::to_string(port) + " --region us");
		run_command(command);
		break;
	case 2:
		command = std::string("ngrok tcp " + std::to_string(port) + " --region eu");
		run_command(command);
		break;
	case 3:
		command = std::string("ngrok tcp " + std::to_string(port) + " --region ap");
		run_command(command);
		break;
	case 4:
		command = std::string("ngrok tcp " + std::to_string(port) + " --region au");
		run_command(command);
		break;
	}

	return true;
}

std::string ngrok::get_public_url()
{
	try
	{
		http::Request request("http://127.0.0.1:4040/api/tunnels");
		const http::Response response = request.send("GET");

		Document document;
		document.Parse(std::string(response.body.begin(), response.body.end()).c_str());
		for (auto& tunnels : document["tunnels"].GetArray())
		{
			std::string public_url = tunnels["public_url"].GetString();
			return public_url.erase(0, 6);
		}
	}
	catch (const std::exception& e)
	{
		MessageBox(NULL, e.what(), "Error", MB_ICONERROR);
	}

	return ("Failed to get IP");
}