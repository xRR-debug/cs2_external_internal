#include "logging.hpp"

bool Logging::Attach(const char* szConsoleTitle)
{
	if (!AllocConsole())
		return false;

	AttachConsole(ATTACH_PARENT_PROCESS);

	if (freopen_s(&pStream, "CONOUT$", "w", stdout) != 0)
		return false;

	if (!SetConsoleTitleA(szConsoleTitle))
		return false;

	return true;
}

void Logging::Detach()
{
	fclose(pStream);

	FreeConsole();

	if (const auto hConsoleWnd = GetConsoleWindow(); hConsoleWnd != nullptr)
		PostMessageW(hConsoleWnd, WM_CLOSE, 0U, 0L);
}