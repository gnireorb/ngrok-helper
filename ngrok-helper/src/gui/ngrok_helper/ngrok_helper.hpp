#include "common.hpp"

#include "helpers/ngrok.hpp"

namespace ngrok_helper
{
	static void render()
	{
		static int protocol_type{};
		static int region{};
		static int port{};
		ImGui::InputInt("Port", &port);
		if (ImGui::Button("Create Tunnel"))
		{
			auto t = []()
			{
				ngrok::create_tunnel(protocol_type == 0 ? true : false, port, region);
			};
			std::thread(t).detach();
		}

		ImGui::SameLine();

		if (ImGui::Button("Close Tunnel"))
		{
			auto t = []()
			{
				ngrok::close_tunnel();
			};
			std::thread(t).detach();
		}

		ImGui::Separator();

		static std::string public_url{};
		ImGui::InputText("Public Url", &public_url);
		if (ImGui::Button("Get Public Url"))
		{
			auto t = []()
			{
				public_url = ngrok::get_public_url();
			};
			std::thread(t).detach();
		}

		ImGui::SameLine();

		if (ImGui::Button("Copy Public Url"))
		{
			auto t = []()
			{
				ngrok::copy_public_url();
			};
			std::thread(t).detach();
		}

		ImGui::Separator();

		static std::string authtoken{};
		ImGui::InputText("Authtoken", &authtoken);
		if (ImGui::Button("Set Authtoken"))
		{
			auto t = []()
			{
				std::system(fmt::format("ngrok config add-authtoken {}", authtoken).c_str());
			};
			std::thread(t).detach();
		}

		ImGui::Separator();

		ImGui::PushItemWidth(200.f);
		ImGui::Combo("Region", &region, ngrok::regions, IM_ARRAYSIZE(ngrok::regions));
		ImGui::PopItemWidth();

		ImGui::PushItemWidth(200.f);
		ImGui::Combo("Protocol Type", &protocol_type, ngrok::protocol_type, IM_ARRAYSIZE(ngrok::protocol_type));
		ImGui::PopItemWidth();

		ImGui::Separator();

		if (ImGui::Button("Download Ngrok"))
		{
			auto t = []()
			{
				ngrok::download(fmt::format("https://github.com/gnireorb/ngrok-helper/releases/download/{}/ngrok.exe", ngrok::version).c_str());
			};
			std::thread(t).detach();
		}
	}
}