#include "common.hpp"

#include "document.h"
#include "stringbuffer.h"
#include "writer.h"

#include "helpers/ngrok_helper.hpp"
#include "util/util.hpp"

using namespace rapidjson;

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

static bool CreateDeviceD3D(HWND hWnd);
static void CleanupDeviceD3D();
static void ResetDevice();
static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main(int, char**)
{
    ngrok::init();
    Document doc;
    doc.Parse(util::read_file("settings.json").c_str());
    static int region = doc["ngrok_region"].GetInt();
    std::cout << "[region]: " << doc["ngrok_region"].GetInt() << std::endl;
    static int port = doc["last_port"].GetInt();
    std::cout << "[port]: " << doc["last_port"].GetInt() << std::endl << std::endl;
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ngrok-helper"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("ngrok-helper"), WS_OVERLAPPEDWINDOW, 100, 100, 405, 720, NULL, NULL, wc.hInstance, NULL);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::GetIO().WantSaveIniSettings = false;

    ImGui::GetStyle().FrameRounding = 4.0f;
    ImGui::GetStyle().FramePadding = ImVec2(5, 5);
    ImGui::GetStyle().GrabRounding = 4.0f;
    ImGui::GetStyle().ChildRounding = 4.0f;
    ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f);

    ImGui::StyleColorsDark();

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(0.93f, 0.94f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.93f, 0.94f, 0.95f, 0.58f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.09f, 0.15f, 0.95f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.18f, 0.29f, 0.58f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.14f, 0.21f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.06f, 0.09f, 0.15f, 0.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.06f, 0.09f, 0.15f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.18f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.18f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.12f, 0.18f, 0.29f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.18f, 0.29f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.18f, 0.29f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.12f, 0.18f, 0.29f, 0.75f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.18f, 0.29f, 0.47f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.12f, 0.18f, 0.29f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 0.80f);
    colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.12f, 0.18f, 0.29f, 0.50f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.12f, 0.18f, 0.29f, 0.86f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.12f, 0.18f, 0.29f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.12f, 0.18f, 0.29f, 0.76f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.12f, 0.18f, 0.29f, 0.86f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.12f, 0.18f, 0.29f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.12f, 0.18f, 0.29f, 0.15f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.12f, 0.18f, 0.29f, 0.78f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.12f, 0.18f, 0.29f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.93f, 0.94f, 0.95f, 0.63f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.12f, 0.18f, 0.29f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.93f, 0.94f, 0.95f, 0.63f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.12f, 0.18f, 0.29f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.12f, 0.18f, 0.29f, 0.43f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.12f, 0.18f, 0.29f, 0.73f);

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ImVec4 clear_color = ImVec4(0.039f, 0.066f, 0.105f, 1.00f);

    bool done = false;
    while (!done)
    {
        MSG msg;
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
            ImGui::SetNextWindowSize(ImVec2(360, 656), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowPos(ImVec2(15, 12), ImGuiCond_FirstUseEver);
            if (ImGui::Begin("ngrok-helper", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
            {
                ImGui::Text("ngrok-helper");
                ImGui::Separator();
                ImGui::Text("tunnel");
                ImGui::PushItemWidth(200);
                if (ImGui::InputInt("port", &port))
                {
                    Document doc;
                    doc.Parse(util::read_file("settings.json").c_str());
                    Value& var = doc["last_port"];
                    var.SetInt(port);
                    StringBuffer buffer;
                    Writer<StringBuffer> writer(buffer);
                    doc.Accept(writer);
                    util::write_to_file("settings.json", buffer.GetString());
                }
                ImGui::PopItemWidth();
                if (ImGui::Button("create a tunnel"))
                {
                    ngrok::create_tunnel(port, region);
                }
                ImGui::SameLine();
                if (ImGui::Button("close tunnel"))
                {
                    std::system("taskkill /f /im ngrok.exe");
                }
                static std::string ip{};
                ImGui::InputText("IP", &ip);
                if (ImGui::Button("get IP"))
                {
                    ip = ngrok::get_public_url();
                }
                ImGui::SameLine();
                if (ImGui::Button("copy IP"))
                {
                    HWND hwnd = GetDesktopWindow();
                    util::to_clipboard(hwnd, ngrok::get_public_url());
                }
                ImGui::Separator();
                ImGui::Text("authtoken");
                static std::string authtoken;
                ImGui::InputText("authtoken", &authtoken);
                if (ImGui::Button("set ngrok authtoken"))
                {
                    std::string commandline = "ngrok authtoken " + authtoken;
                    std::system(commandline.c_str());
                }
                ImGui::Separator();
                ImGui::Text("region");
                static int region{};
                const char* const regions[] { "south america", "north america", "europe", "asia", "australia" };
                if (ImGui::Combo("tunnel region", &region, regions, IM_ARRAYSIZE(regions)))
                {
                    Document doc;
                    doc.Parse(util::read_file("settings.json").c_str());
                    Value& var = doc["ngrok_region"];
                    var.SetInt(region);
                    StringBuffer buffer;
                    Writer<StringBuffer> writer(buffer);
                    doc.Accept(writer);
                    util::write_to_file("settings.json", buffer.GetString());
                }
                ImGui::Separator();
                if (ImGui::CollapsingHeader("debug stuff"))
                {
                    static bool debug_mode{};
                    if (ImGui::Checkbox("debug mode", &debug_mode))
                    {
                        ::ShowWindow(::GetConsoleWindow(), debug_mode ? SW_SHOW : SW_HIDE);
                    }
                }

                ImGui::Separator();

                ImGui::Text("github.com/gnireorb/ngrok-helper");
            }
            ImGui::End();
        }

        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}