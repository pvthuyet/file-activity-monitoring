module;

#include <windows.h>
#include <process.h>
#include <system_error>
#include "logger_define.h"

module Saigon.DirectoryWatcherBase;

import Saigon.ObserverImpl;
import Saigon.RequestImpl;
import Saigon.CommonUtils;

namespace saigon::observation
{
	directory_watcher_base::~directory_watcher_base() noexcept
	{
		stop();
	}

	bool directory_watcher_base::start(watching_setting&& info)
	{
		LOGENTER;
		if (not info.valid()) {
			SPDLOG_ERROR("Invalid watching setting");
			return false;
		}

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
			SPDLOG_ERROR("Create thread. errno: {}, doserrno: {}", errno, _doserrno);
			return false;
		}
		mObserverThread = reinterpret_cast<HANDLE>(ret);

		// 2. build request
		request_impl::request_param param{};
		param.mObs = mObserver.get();
		param.mInfo = std::move(info);
		request_impl* req = new request_impl(std::move(param));
		auto succ = ::QueueUserAPC(add_directory_proc,
			mObserverThread,
			reinterpret_cast<ULONG_PTR>(req));

		if (not succ) {
			SPDLOG_ERROR("Last error code: {}", ::GetLastError());
			return false;
		}

		LOGEXIT;
		return true;
	}

	void directory_watcher_base::stop() noexcept
	{
		LOGENTER;
		using namespace observer::callback;
		// 1. terminate queue then close thread
		if (mObserverThread) {
			auto succ = ::QueueUserAPC(terminate_proc, mObserverThread, reinterpret_cast<ULONG_PTR>(mObserver.get()));
			if (not succ) {
				SPDLOG_ERROR("Last error code: {}", ::GetLastError());
			}
			
			DWORD stat = 0;
			do {
				stat = ::WaitForSingleObjectEx(mObserverThread, 3000, true);
				SPDLOG_INFO("WaitForSingleObjectEx state: {}", stat);
			} while (WAIT_TIMEOUT == stat);

			::CloseHandle(mObserverThread);
			mObserverThread = nullptr;
			mThreadId = 0;
		}

		// 2. destroy observer
		mObserver = nullptr;
		LOGEXIT;
	}
}