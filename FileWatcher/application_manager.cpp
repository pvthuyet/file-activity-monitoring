module;

#include <memory>
#include <vector>
#include "logger_define.h"

module Saigon.ApplicationManager;

import Saigon.ClipboardWatcher;
import Saigon.WatchingSetting;
import Saigon.WatcherRules;

namespace saigon
{
	void ApplicationManager::start()
	{
		try {
			// 1. start clipboard
			//ClipboardWatcher::getInst().start();

			// 2. load rule
			observation::watcher_rules rules;
			rules.load_rules();

			// 3. start watcher
			mFileNameWatcher = std::make_unique<observation::filename_watcher>();
			mFileNameWatcher->start(rules.get_settings());
		}
		catch (std::exception const& ex) {
			SPDLOG_ERROR(ex.what());
		}
	}
}