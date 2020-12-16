module;

#include <memory>
#include "logger_define.h"

module Saigon.ApplicationManager;

import Saigon.ClipboardWatcher;
import Saigon.WatcherRules;
import Saigon.WatchingInfo;

namespace saigon
{
	void ApplicationManager::start()
	{
		try {
			// 1. start clipboard
			//ClipboardWatcher::getInst().start();

			// 2. load rule
			//observation::watcher_rules::get_inst().load_rules();

			// 3. start watcher
			saigon::observation::watching_info info;
			mFileNameWatcher = std::make_unique<observation::filename_watcher>();
			mFileNameWatcher->start(std::move(info));
		}
		catch (std::exception const& ex)
		{
			SPDLOG_ERROR(ex.what());
		}
	}
}