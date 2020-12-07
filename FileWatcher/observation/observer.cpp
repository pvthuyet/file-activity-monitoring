module;

#include <Windows.h>
#include <atomic>
#include "logger_define.h"

module Saigon.Observer;

namespace saigon::observation
{
	unsigned int WINAPI observer::start_thread_proc(LPVOID arg)
	{
		LOGENTER;
		observer* obs = static_cast<observer*>(arg);
		obs->run();
		LOGEXIT;
		return 0;
	}

	unsigned int observer::incRequest()
	{
		return ++mOutstandingRequests;
	}

	unsigned int observer::decRequest()
	{
		return --mOutstandingRequests;
	}

	bool observer::terminated() const
	{
		return mTerminated.load(std::memory_order::memory_order_relaxed);
	}

	bool observer::empty_request() const
	{
		return !mOutstandingRequests.load(std::memory_order::memory_order_relaxed);
	}

	void observer::run()
	{
		LOGENTER;
		while (not empty_request() or not terminated()) {
			::SleepEx(INFINITE, TRUE);
		}
		LOGEXIT;
	}
}