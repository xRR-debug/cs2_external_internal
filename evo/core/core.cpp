#include "../../sdk/offsets/offsets.h"
#include "../../hacks/ctx/hacks_ctx.h"
#include "../../utils/xorstr/xorstr.hpp"
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <filesystem>
#include <cstdlib>
#include <KnownFolders.h>
#include <ShlObj.h>
#include "../utils/logging.hpp"
#include "../utils/lazy.hpp"
#include "../../utils/discord/discord_rpc_init.h"
#include "../hacks/features/aimbot/mouse.hpp"
#include "initsdl.hpp"

namespace fs = std::filesystem;

static bool main_loop(LPVOID lpParameter)
{

	try
	{

#ifdef DEBUG_CONSOLE
		if (!Logging::Attach(X("[angeldancy] developer-mode\n")))
			throw std::runtime_error(X("[angeldancy] Failed to attach console"));

		Logging::Print(X("[angeldancy] Console opened!\n"));
#endif
		//initsdl::init_sdl(); //exe build only
		auto process_status = _proc_manager.attach(X("cs2.exe"));

		char documentsPath[MAX_PATH];

		if (LI_FN(SHGetFolderPathA).get()(NULL, CSIDL_APPDATA, NULL, 0, documentsPath) != S_OK)
		{
#ifdef DEBUG_CONSOLE
			Logging::Print(X("[Info] Error: Failed to get the Documents folder path."));
#endif
			goto END;
		}

		path::path = documentsPath;
		path::path += X("\\angeldancy\\");

		switch (process_status)
		{
		case 1:
			goto END;
		case 2:
			goto END;
		case 3:
			goto END;
		default:
			break;
		}

		if (!Offset::update_offsets())
		{
			goto END;
		}

		if (angel::_settings->discordrpc)
		{
			_discord.initialize();
			_discord.update();
		}

			_driver->Init();

		if (!g_game.init_address())
		{
			goto END;
		}

		if (fs::exists(path::path))
		{
#ifdef DEBUG_CONSOLE
			Logging::Print(X("[Info] Config folder connected: "));
#endif
		}
		else
		{
			if (fs::create_directory(path::path))
			{
#ifdef DEBUG_CONSOLE
				Logging::Print(X("[Info] Config folder created: "));
#endif
			}
			else
			{
#ifdef DEBUG_CONSOLE
				Logging::Print(X("[Info] Error: Failed to create the config directory."));
#endif
				goto END;
			}
		}
	}
	catch (const std::exception& ex)
	{
		LI_FN(FreeLibraryAndExitThread)(static_cast<HMODULE>(lpParameter), EXIT_FAILURE);
	}
	try
	{
		//Gui.AttachAnotherWindow(X("amd dvr overlay"), X("AMDDVROVERLAYWINDOWCLASS"), cheats::run);
		//Gui.AttachAnotherWindow(X("angeldancy"), X("SDL_app"), cheats::run);
		Gui.AttachAnotherWindow(X("Counter-Strike 2"), X("SDL_app"), cheats::run);
	}
	catch (OSImGui::OSException& e)
	{
		try
		{
			// Chinese version
			Gui.AttachAnotherWindow(X("·´¿Ö¾«Ó¢£ºÈ«Çò¹¥ÊÆ"), X("SDL_app"), cheats::run);
		}
		catch (OSImGui::OSException& e)
		{
#ifdef DEBUG_CONSOLE
			Logging::Print(e.what());
#endif
		}
	}
END:
#ifdef DEBUG_CONSOLE
	Logging::Print(X("Cheat running successfully!"));
#endif
	LI_FN(system)("pause");
	return 0;
}

#ifdef DLL
DWORD WINAPI OnDllAttach(LPVOID lpParameter)
{
	if (!main_loop(lpParameter))
		return 0UL;

	return 1UL;
}

DWORD WINAPI OnDllDetach(LPVOID lpParameter)
{
	LI_FN(FreeLibraryAndExitThread)((HMODULE)lpParameter, EXIT_SUCCESS);

#ifdef DEBUG_CONSOLE
	Logging::Detach();
#endif
	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	static std::thread th;
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		LI_FN(DisableThreadLibraryCalls)(hModule);

		if (auto th = LI_FN(CreateThread)(nullptr, 0U, OnDllAttach, hModule, 0UL, nullptr); th != nullptr)
			LI_FN(CloseHandle)(th);
		LI_FN(Beep).cached()(254.26, 150);
		LI_FN(Beep).cached()(254.26, 150);

		return TRUE;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#else
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPreviousInstance, LPSTR pArgs, int iCmdShow)
{
	if (!main_loop(hInstance))
		return 0UL;

	return 1UL;
}
#endif
