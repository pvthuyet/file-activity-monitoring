#include "saigon_watcher_impl.h"

import Saigon.ClipboardWatcher;
import Saigon.WatchingRules;

namespace saigon
{
	bool saigon_watcher_impl::start()
	{
		// 1. start clipboard
		ClipboardWatcher::getInst().start();

		// 2. load rule
		observation::watching_rules rules;
		rules.load_rules();

		// 3. start watcher
		auto settings = rules.get_settings();
		for (auto const& el : settings) {
			auto watcher = std::make_unique<observation::filename_watcher>();
			watcher->start(std::vector<observation::watching_setting>{el});
			mWatchers.push_back(std::move(watcher));
		}
		return true;
	}
}
