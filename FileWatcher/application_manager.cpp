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
		// 1. start clipboard
		ClipboardWatcher::getInst().start();

		// 2. load rule
		observation::watcher_rules rules;
		rules.load_rules();

		// 3. start watcher
		auto settings = rules.get_settings();
		for (auto const& el : settings) {
			auto watcher = std::make_unique<observation::filename_watcher>();
			watcher->start(std::vector<observation::watching_setting>{el});
			mWatchers.push_back(std::move(watcher));
		}
	}
}