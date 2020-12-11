module;

#include <Windows.h>
#include <atomic>
#include "logger_define.h"

module Saigon.ObserverImpl;

namespace saigon::observation
{
	unsigned int WINAPI observer_impl::start_thread_proc(LPVOID arg)
	{
		LOGENTER;
		observer_impl* obs = static_cast<observer_impl*>(arg);
		obs->run();
		LOGEXIT;
		return 0;
	}

	unsigned int observer_impl::inc_request()
	{
		return ++mOutstandingRequests;
	}

	unsigned int observer_impl::dec_request()
	{
		return --mOutstandingRequests;
	}

	bool observer_impl::terminated() const
	{
		return mTerminated.load(std::memory_order::memory_order_relaxed);
	}

	bool observer_impl::empty_request() const
	{
		return !mOutstandingRequests.load(std::memory_order::memory_order_relaxed);
	}

	void observer_impl::run()
	{
		LOGENTER;
		while (not empty_request() or not terminated()) {
			::SleepEx(INFINITE, TRUE);
		}
		LOGEXIT;
	}

	void observer_impl::add_directory(request* pBlock)
	{
		_ASSERTE(pBlock);
		if (pBlock->open_directory())
		{
			pBlock->get_observer()->inc_request();
			mBlocks.push_back(pBlock);
			pBlock->begin_read();
		}
		else {
			delete pBlock;
			pBlock = nullptr;
		}
	}

	void observer_impl::request_termination()
	{
		mTerminated.store(true, std::memory_order::memory_order_relaxed);
		for (DWORD i = 0; i < mBlocks.size(); ++i)
		{
			// Each Request object will delete itself.
			_ASSERTE(mBlocks[i]);
			mBlocks[i]->request_termination();
		}
		mBlocks.clear();
	}
}