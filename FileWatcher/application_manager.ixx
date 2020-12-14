module;

#include <memory>

export module Saigon.ApplicationManager;

import Saigon.FilenameWatcher;

namespace saigon
{
	export class ApplicationManager
	{
	public:
		void start();

	private:
		std::unique_ptr<observation::filename_watcher> mFileNameWatcher;
	};
}