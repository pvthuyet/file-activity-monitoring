module;

#include <memory>
#include "logger_define.h"

module Saigon.ApplicationManager;

import Saigon.Logger; //++ TODO Can't use logger and fmt/format together
import Saigon.Minidump;

import Saigon.ClipboardWatcher;

namespace saigon
{
	ApplicationManager::~ApplicationManager() noexcept
	{
		mFileNameWatcher.stop();
	}

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
		dump::monitoring("");
		logger::create_logger();
	}

	void ApplicationManager::run()
	{
		try
		{
			ClipboardWatcher::getInst().start();
			mFileNameWatcher.start();
		}
		catch (std::exception const& ex)
		{
			SPDLOG_ERROR(ex.what());
		}
	}
}