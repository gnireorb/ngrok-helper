#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <tchar.h>
#include <future>

#include <d3d9.h>
#include <dinput.h>
#include <tchar.h>
#pragma comment (lib, "d3d9.lib")

#include <urlmon.h>
#pragma comment (lib, "urlmon.lib")

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "misc/cpp/imgui_stdlib.h"

#include "document.h"
#include "stringbuffer.h"
#include "writer.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/fmt.h"