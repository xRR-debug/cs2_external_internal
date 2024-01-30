#pragma once
#pragma warning ( disable : 4005 )
#pragma warning ( disable : 26495 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4267 )
#pragma warning ( disable : 4715 )
#pragma warning ( disable : 4018 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4305 )

#include <ShlObj_core.h>

#include <KnownFolders.h>
#include <windows.h>
#include <time.h>
#include <windowsx.h>
#include <memory>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <functional>
#include <WinUser.h>
#include <stdio.h>
#include <string>
#include <string_view>
#include <map>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <deque>
#include <mutex>
#include <optional>
#include <filesystem>
#include <random>
#include <array>


/* directx includes */
#include <d3d11.h>
#include <D3D11.h>

/* libs for directx */
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "freetype_debug.lib")

/* imgui includes */
#include "utils/os-imgui/imgui/imgui.h"
#include "utils/os-imgui/imgui/imgui_freetype.h"
#include "utils/os-imgui/imgui/imgui_impl_win32.h"
#include "utils/os-imgui/imgui/imgui_impl_dx9.h"
#include "utils/os-imgui/imgui/imgui_impl_dx11.h"

#include "../../utils/xorstr/xorstr.hpp"
#include "../../utils/logging.hpp"
#include "../../utils/lazy.hpp"

#define ALPHA    ( ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )
#define NO_ALPHA ( ImGuiColorEditFlags_NoTooltip    | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )




