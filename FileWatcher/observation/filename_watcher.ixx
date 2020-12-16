module;

#include <Windows.h>

export module Saigon.FilenameWatcher;

import Saigon.DirectoryWatcherBase;

namespace saigon::observation
{
	export class filename_watcher : public directory_watcher_base
	{
	private:
		void do_notify(saigon::file_notify_info) final;
	};
}