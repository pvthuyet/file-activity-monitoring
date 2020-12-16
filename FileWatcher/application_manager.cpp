module;

#include <memory>
#include "logger_define.h"

module Saigon.ApplicationManager;

import Saigon.ClipboardWatcher;
import Saigon.WatcherRules;
import Saigon.WatchingSetting;

namespace saigon
{
	void ApplicationManager::start()
	{
		try {
			// 1. start clipboard
			//ClipboardWatcher::getInst().start();

			// 2. load rule
			observation::watcher_rules::get_inst().load_rules();

			// 3. start watcher
			auto settings = observation::watcher_rules::get_inst().get_settings();
			mFileNameWatcher = std::make_unique<observation::filename_watcher>();
			for (auto& el : settings) {
				mFileNameWatcher->start(std::move(el));
			}
		}
		catch (std::exception const& ex)
		{
			SPDLOG_ERROR(ex.what());
		}
	}
}