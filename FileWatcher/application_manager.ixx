module;

#include <memory>
#include <vector>

export module Saigon.ApplicationManager;

import Saigon.FilenameWatcher;

namespace saigon
{
	export class ApplicationManager
	{
	public:
		void start();

	private:
		std::vector<std::unique_ptr<observation::filename_watcher>> mWatchers;
	};
}