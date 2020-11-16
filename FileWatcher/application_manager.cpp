module;

#include <memory>
#include "logger_define.h"

module Fibo.ApplicationManager;

import Fibo.Logger; //++ TODO Can't use logger and fmt/format together
import Fibo.Minidump;

import Fibo.ClipboardWatcher;

namespace fibo
{
	ApplicationManager::ApplicationManager()
	{
		initialize();
	}

	ApplicationManager& ApplicationManager::getInst()
	{
		static ApplicationManager appInst{};
		return appInst;
	}

	void ApplicationManager::initialize()
	{
		MiniDump::monitoring("");
		Logger::createLogger();
	}

	void ApplicationManager::run()
	{
		try
		{
			ClipboardWatcher::getInst().start();
		}
		catch (std::exception const& ex)
		{
			SPDLOG_ERROR(ex.what());
		}
	}
}