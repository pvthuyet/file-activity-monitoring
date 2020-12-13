module;

#include <windows.h>
#include <process.h>
#include <system_error>
#include "logger_define.h"

module Saigon.DirectoryWatcherBase;

import Saigon.ObserverImpl;
import Saigon.RequestImpl;

namespace saigon::observation
{
	void directory_watcher_base::start()
	{
		LOGENTER;
		using namespace observer::callback;
		// 1. Create observer object
		mObserver = std::make_unique<observer_impl>(this);

		// 2. Create observer thread
		auto ret = _beginthreadex(
			NULL,
			0,
			&start_thread_proc,
			mObserver.get(),
			0,
			&mThreadId);

		if (not ret) {
			SPDLOG_ERROR("Failed to create observer thread. errno = {}, doserrno = {}", errno, _doserrno);
			throw std::system_error(errno, std::system_category(), "Failed to create observer thread");
		}
		mObserverThread = reinterpret_cast<HANDLE>(ret);

		// 3. enumerate all disks
		// ...

		// 4. build request
		request_param param{};
		param.mObs = mObserver.get();
		param.mNotifyFilters = get_notify_filters();
		param.mDir = L"C:\\"; //++ TODO;
		request_impl* req = new request_impl(std::move(param));
		auto succ = ::QueueUserAPC(add_directory_proc,
			mObserverThread, 
			reinterpret_cast<ULONG_PTR>(req));

		if (not succ) {
			SPDLOG_ERROR("Failed QueueUserAPC. Error = {}", ::GetLastError());
		}

		LOGEXIT;
	}

	void directory_watcher_base::stop()
	{
		LOGENTER;
		using namespace observer::callback;
		if (mObserverThread) {
			::QueueUserAPC(terminate_proc, mObserverThread, reinterpret_cast<ULONG_PTR>(mObserver.get()));
			::WaitForSingleObjectEx(mObserverThread, 10000, true);
			::CloseHandle(mObserverThread);

			mObserverThread = nullptr;
			mThreadId = 0;
		}
		mObserver = nullptr;
		LOGEXIT;
	}
}