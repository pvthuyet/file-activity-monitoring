module;

#include <memory>
#include "logger_define.h"

module Saigon.ApplicationManager;

import Saigon.Logger; //++ TODO Can't use logger and fmt/format together
import Saigon.Minidump;

import Saigon.ClipboardWatcher;
import Saigon.FilenameWatcher;
import Saigon.WatcherRules;

namespace saigon
{
	std::unique_ptr<observation::filename_watcher> gFileNameWatcher{};

	ApplicationManager::ApplicationManager()
	{
		initialize();
	}

	ApplicationManager::~ApplicationManager() noexcept
	{
		gFileNameWatcher = nullptr;
	}

	ApplicationManager& ApplicationManager::getInst()
	{
		static ApplicationManager appInst{};
		return appInst;
	}

	void ApplicationManager::initialize()
	{
		dump::monitoring();
		logger::create_logger();
		observation::watcher_rules::get_inst().load_rules();
		gFileNameWatcher = std::make_unique<observation::filename_watcher>();
	}

	void ApplicationManager::run()
	{
		try
		{
			ClipboardWatcher::getInst().start();
			gFileNameWatcher->start();
		}
		catch (std::exception const& ex)
		{
			SPDLOG_ERROR(ex.what());
		}
	}

	void ApplicationManager::stop()
	{
		gFileNameWatcher->stop();
	}
}