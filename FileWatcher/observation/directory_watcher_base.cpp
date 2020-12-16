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

	void directory_watcher_base::start(watching_info&& info)
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
			SPDLOG_ERROR("Errno: {}, doserrno: {}", errno, _doserrno);
			throw std::system_error(errno, std::system_category(), "Failed to create observer thread");
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
		}

		LOGEXIT;
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