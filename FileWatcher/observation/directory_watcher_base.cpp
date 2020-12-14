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

	DWORD directory_watcher_base::get_notify_filters() const
	{
		return do_get_notify_filters();
	}

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
		auto drives = saigon::enumerate_drives();

		// 4. build request
		for (auto const& e : drives) {
			request_param param{};
			param.mObs = mObserver.get();
			param.mNotifyFilters = get_notify_filters();
			param.mDir = e;
			request_impl* req = new request_impl(std::move(param));
			auto succ = ::QueueUserAPC(add_directory_proc,
				mObserverThread,
				reinterpret_cast<ULONG_PTR>(req));

			if (not succ) {
				SPDLOG_ERROR("Failed QueueUserAPC. Error = {}", ::GetLastError());
			}
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
				SPDLOG_ERROR("Failed QueueUserAPC. Error = {}", ::GetLastError());
			}
			
			do {
				DWORD stat = ::WaitForSingleObjectEx(mObserverThread, 3000, true);
				switch (stat) {
				case WAIT_ABANDONED:
					SPDLOG_INFO("wait WAIT_ABANDONED");
					break;

				case WAIT_IO_COMPLETION:
					SPDLOG_INFO("wait WAIT_IO_COMPLETION");
					break;

				case WAIT_OBJECT_0:
					SPDLOG_INFO("wait WAIT_OBJECT_0");
					break;

				case WAIT_TIMEOUT:
					SPDLOG_INFO("wait WAIT_TIMEOUT");
					break;

				case WAIT_FAILED:
					SPDLOG_INFO("wait WAIT_FAILED");
					break;

				default:
					break;
				}

			} while (stat == WAIT_TIMEOUT);

			::CloseHandle(mObserverThread);
			mObserverThread = nullptr;
			mThreadId = 0;
		}

		// 2. destroy observer
		mObserver = nullptr;
		LOGEXIT;
	}
}