module;

#include <Windows.h>
#include "logger_define.h"

module Saigon.FilenameWatcher;

import Saigon.WatchingInfo;

namespace saigon::observation
{
	void filename_watcher::do_notify(saigon::file_notify_info info)
	{
		SPDLOG_DEBUG("{} - {}", info.get_action(), info.get_path_string());
	}
}