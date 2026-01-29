#include "common.hpp"

#include "backend/imgui_implementation.hpp"
#include "backend/fonts.hpp"

#include "gui/gui.hpp"

int main()
{
	spdlog::set_pattern("[%H:%M:%S] [%t] [%^%l%$] %v");

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, backend::imgui_implementation::wndproc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ngrok-helper"), NULL };
	::RegisterClassEx(&wc);
	HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("ngrok-helper"), WS_OVERLAPPEDWINDOW, 100, 100, 405, 720, NULL, NULL, wc.hInstance, NULL);

	if (!backend::imgui_implementation::create_device_d3d(hwnd))
	{
		backend::imgui_implementation::cleanup_device_d3d();
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		return 1;
	}

	::ShowWindow(hwnd, SW_SHOWDEFAULT); ::UpdateWindow(hwnd);

	ImGui::CreateContext();

	gui::apply_style_and_settings();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(backend::imgui_implementation::g_pd3ddevice);

	ImFontConfig ruda_regular_config, ruda_bold_config{};
	std::strcpy(ruda_regular_config.Name, "Ruda-Regular"); std::strcpy(ruda_bold_config.Name, "Ruda-Bold");
	ImFont* ruda_regular = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(backend::fonts::ruda_regular), sizeof(backend::fonts::ruda_regular), 20.f, &ruda_regular_config); ImFont* ruda_bold = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(backend::fonts::ruda_bold), sizeof(backend::fonts::ruda_bold), 20.f, &ruda_bold_config);
	IM_ASSERT(ruda_regular != NULL); IM_ASSERT(ruda_bold != NULL);

	bool done{ false };
	while (!done)
	{
		MSG msg{};
		while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				done = true;
		}
		if (done)
			break;

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		{
			gui::render();
		}

		ImGui::EndFrame();

		backend::imgui_implementation::g_pd3ddevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		backend::imgui_implementation::g_pd3ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		backend::imgui_implementation::g_pd3ddevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		backend::imgui_implementation::g_pd3ddevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(31, 46, 74, 255), 1.0f, 0);

		if (backend::imgui_implementation::g_pd3ddevice->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			backend::imgui_implementation::g_pd3ddevice->EndScene();
		}

		if (backend::imgui_implementation::g_pd3ddevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST && backend::imgui_implementation::g_pd3ddevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			backend::imgui_implementation::reset_device();
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	backend::imgui_implementation::cleanup_device_d3d();
	::DestroyWindow(hwnd);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);

	return TRUE;
}