module;

#include <Windows.h>

export module Saigon.DirectoryWatcherBase;

import Saigon.IDirectoryWatcher;

namespace saigon::observation
{
	export class directory_watcher_base : public idirectory_watcher
	{
	public:
		void start();
		void stop();

		// interface
	private:
		void do_notify(saigon::file_notify_info info) override
		{

		}

	private:
		bool init();

	private:
		HANDLE mObserverThread;
		unsigned mThreadId;
	};
}