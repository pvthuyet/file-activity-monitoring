#pragma once

#include <memory>
#include <vector>

import Saigon.FilenameWatcher;

namespace saigon
{
	class saigon_watcher_impl
	{
	public:
		bool start();

	private:
		std::vector<std::unique_ptr<observation::filename_watcher>> mWatchers;
	};
}

