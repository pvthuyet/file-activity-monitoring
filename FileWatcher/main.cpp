#include <Windows.h>
#include "vld/vld.h"

int wmain(int arg, const wchar_t* argv[])
{
	return wWinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWNORMAL);
}