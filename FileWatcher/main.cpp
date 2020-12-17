#include <Windows.h>
#include <iostream>
#include "logger_define.h"
#include "vld/vld.h"

import Saigon.Minidump;
import Saigon.Logger; //++ TODO Can't use logger and fmt/format together

int wmain(int arg, const wchar_t* argv[])
{
	saigon::dump::monitoring();
	try {
		saigon::logger::create_logger();
		return wWinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWNORMAL);
	}
	catch (std::exception const& ex) {
		SPDLOG_ERROR(ex.what());
	}
	catch (...) {
		SPDLOG_ERROR("Unhandle exception");
	}
	return -1;
}