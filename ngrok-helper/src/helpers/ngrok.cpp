#include "HTTPRequest.hpp"

#include "ngrok.hpp"
#include "util/util.hpp"

using namespace rapidjson;

void ngrok::init()
{
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

		if (!util::write_file("settings.json", "{\"last_port\":0,\"ngrok_region\":0}"))
		{
			MessageBox(NULL, "Ocorreu uma falha ao escrever em 'settings.json'.", "Error", MB_ICONERROR);
			return;
		}
	}
}

void ngrok::download_ngrok(const char* url)
{
	if (!util::file_exists("ngrok.exe"))
	{
		auto result = URLDownloadToFile(nullptr, url, "ngrok.exe", 0, nullptr);
		switch (result)
		{
		case E_OUTOFMEMORY:
		{
			MessageBox(NULL, "There is insufficient memory to complete the operation.", "E_OUTOFMEMORY", MB_ICONERROR);
			break;
		}
		case INET_E_DOWNLOAD_FAILURE:
		{
			MessageBox(NULL, "The specified resource or callback interface was invalid.", "INET_E_DOWNLOAD_FAILURE", MB_ICONERROR);
			break;
		}
		case S_OK:
		{
			MessageBox(NULL, "O arquivo foi baixado com sucesso.", "S_OK", MB_ICONINFORMATION);
			break;
		}
		}
	}
}

bool ngrok::create_tunnel(int port, int region)
{
	// Impressive.
	auto ngrok_tcp = [](int port, const char* region) {
		auto _system = [](int port, const char* region) {
			system(fmt::format("ngrok tcp {} --region {}", port, region).c_str());
		};
		std::thread(_system, port, region).detach();
	};

	if (region >= 4 || region < 0)
		return false;

	switch (region)
	{
	case 0:
		ngrok_tcp(port, "sa");
		break;
	case 1:
		ngrok_tcp(port, "us");
		break;
	case 2:
		ngrok_tcp(port, "eu");
		break;
	case 3:
		ngrok_tcp(port, "ap");
		break;
	case 4:
		ngrok_tcp(port, "au");
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