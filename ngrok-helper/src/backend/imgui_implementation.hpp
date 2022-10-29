#include "common.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace backend::imgui_implementation
{
	static LPDIRECT3D9 g_pd3d = { NULL };
	static LPDIRECT3DDEVICE9 g_pd3ddevice = { NULL };
	static D3DPRESENT_PARAMETERS g_d3dpp = {};

	static void reset_device()
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		HRESULT result = g_pd3ddevice->Reset(&g_d3dpp);
		if (result == D3DERR_INVALIDCALL)
			IM_ASSERT(0);
		ImGui_ImplDX9_CreateDeviceObjects();
	}

	LRESULT WINAPI wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
			return true;

		switch (msg)
		{
		case WM_SIZE:
		if (g_pd3ddevice != NULL && wparam != SIZE_MINIMIZED)
		{
			g_d3dpp.BackBufferWidth = LOWORD(lparam);
			g_d3dpp.BackBufferHeight = HIWORD(lparam);
			reset_device();
		}
		return 0;
		case WM_SYSCOMMAND:
		if ((wparam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
		case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
		}
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	static HWND create_window()
	{
		WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, wndproc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("window"), NULL };
		::RegisterClassEx(&wc);
		HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("window"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 720, NULL, NULL, wc.hInstance, NULL);

		return hwnd;
	}

	static bool create_device_d3d(HWND hwnd)
	{
		if ((g_pd3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
			return false;

		ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
		g_d3dpp.Windowed = TRUE;
		g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		g_d3dpp.EnableAutoDepthStencil = TRUE;
		g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

		if (g_pd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3ddevice) < 0)
			return false;

		return true;
	}

	static bool cleanup_device_d3d(HWND hwnd)
	{
		if ((g_pd3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
			return false;

		ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
		g_d3dpp.Windowed = TRUE;
		g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		g_d3dpp.EnableAutoDepthStencil = TRUE;
		g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

		if (g_pd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3ddevice) < 0)
			return false;

		return true;
	}

	static void cleanup_device_d3d()
	{
		if (g_pd3ddevice) { g_pd3ddevice->Release(); g_pd3ddevice = NULL; }
		if (g_pd3d) { g_pd3d->Release(); g_pd3d = NULL; }
	}
}