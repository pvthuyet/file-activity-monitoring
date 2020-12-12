module;

#include <windows.h>
#include <process.h>
#include "logger_define.h"

module Saigon.DirectoryWatcherBase;

import Saigon.ObserverImpl;

namespace saigon::observation
{
	void directory_watcher_base::start()
	{

	}

	void directory_watcher_base::stop()
	{

	}

	bool directory_watcher_base::init()
	{
		if (not mObserverThread) {
			auto ret = _beginthreadex(
				NULL,
				0,
				&start_observer_thread_proc,
				nullptr,
				0,
				&mThreadId
			);
			if (not ret) {
				SPDLOG_ERROR("Failed to create observer thread");
				return false;
			}
			mObserverThread = reinterpret_cast<HANDLE>(ret);
		}
		return true;
	}
}