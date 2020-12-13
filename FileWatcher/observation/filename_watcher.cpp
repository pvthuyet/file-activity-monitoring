module;

#include <Windows.h>
#include "logger_define.h"

module Saigon.FilenameWatcher;

import Saigon.WatchingAction;

namespace saigon::observation
{
	void filename_watcher::do_notify(saigon::file_notify_info info)
	{
		SPDLOG_DEBUG("{} - {}", info.get_action(), info.get_path_string());
	}

	DWORD filename_watcher::do_get_notify_filters() const
	{
		auto filter = waching_action::file_create 
			| waching_action::file_copy 
			| waching_action::file_move 
			| waching_action::file_modify 
			| waching_action::file_delete 
			| waching_action::file_rename;
		return filter;
	}
}