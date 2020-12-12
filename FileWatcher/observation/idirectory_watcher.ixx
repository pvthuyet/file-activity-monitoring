module;

#include <utility>

export module Saigon.IDirectoryWatcher;

import Saigon.FileNotifyInfo;

namespace saigon::observation
{
	export class idirectory_watcher
	{
	public:
		virtual ~idirectory_watcher() noexcept = default;
		void notify(saigon::file_notify_info info)
		{
			do_notify(std::move(info));
		}

	private:
		virtual void do_notify(saigon::file_notify_info info) = 0;
	};
}