module;

#include <Windows.h>
#include <atomic>
#include <gsl/assert>
#include "logger_define.h"

module Saigon.ObserverImpl;

namespace saigon::observation
{
	observer_impl::observer_impl(idirectory_watcher* dirWatcher) :
		mDirWatcher{dirWatcher}
	{
		Ensures(mDirWatcher);
	}

	unsigned int observer_impl::do_inc_request()
	{
		return ++mOutstandingRequests;
	}

	unsigned int observer_impl::do_dec_request()
	{
		return --mOutstandingRequests;
	}

	idirectory_watcher* observer_impl::do_get_watcher() const
	{
		return mDirWatcher;
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

	void observer_impl::add_directory(irequest* pBlock)
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

	namespace observer::callback
	{
		unsigned WINAPI start_thread_proc(LPVOID arg)
		{
			LOGENTER;
			observer_impl* obs = static_cast<observer_impl*>(arg);
			obs->run();
			LOGEXIT;
			_endthreadex(0);
			return 0;
		}

		VOID CALLBACK terminate_proc(__in ULONG_PTR arg)
		{
			auto obs = reinterpret_cast<observer_impl*>(arg);
			obs->request_termination();
		}

		VOID CALLBACK add_directory_proc(__in ULONG_PTR arg)
		{
			auto req = reinterpret_cast<irequest*>(arg);
			auto obs = req->get_observer();
			auto obsImpl = dynamic_cast<observer_impl*>(obs);
			if (obsImpl) {
				obsImpl->add_directory(req);
			}
		}
	}
}