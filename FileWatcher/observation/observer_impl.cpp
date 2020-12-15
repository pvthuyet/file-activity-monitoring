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
		if (empty_request()) {
			SPDLOG_INFO("number of requests are 0");
			return 0u;
		}
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
		return 0u == mOutstandingRequests.load(std::memory_order::memory_order_relaxed);
	}

	void observer_impl::run()
	{
		LOGENTER;
		while (not empty_request() or not terminated()) {
			auto retVal = ::SleepEx(INFINITE, TRUE);
			SPDLOG_DEBUG("SleepEx return value: {}, number request: {}, terminated: {}", retVal, empty_request(), terminated());
		}
		LOGEXIT;
	}

	bool observer_impl::add_directory(irequest* pBlock)
	{
		_ASSERTE(pBlock);
		if (pBlock->open_directory() and pBlock->begin_read()) {
			pBlock->get_observer()->inc_request();
			mBlocks.push_back(pBlock);
			return true;
		}

		// failed
		SPDLOG_ERROR("Failed to call open_directory() or begin_read()");
		delete pBlock;
		pBlock = nullptr;
		return false;
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
			LOGENTER;
			auto obs = reinterpret_cast<observer_impl*>(arg);
			obs->request_termination();
			LOGEXIT;
		}

		VOID CALLBACK add_directory_proc(__in ULONG_PTR arg)
		{
			LOGENTER;
			auto req = reinterpret_cast<irequest*>(arg);
			auto obs = req->get_observer();
			auto obsImpl = dynamic_cast<observer_impl*>(obs);
			if (obsImpl) {
				obsImpl->add_directory(req);
			}
			LOGEXIT;
		}
	}
}