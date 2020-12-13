module;

#include <gsl/pointers>
#include <Windows.h>
#include <atomic>
#include <vector>

export module Saigon.ObserverImpl;

import Saigon.IObserver;
import Saigon.IRequest;

namespace saigon::observation
{
	namespace observer::callback
	{
		export unsigned WINAPI start_thread_proc(LPVOID);
		export VOID CALLBACK terminate_proc(__in ULONG_PTR);
		export VOID CALLBACK add_directory_proc(__in ULONG_PTR);
	}

	export class observer_impl final : public iobserver
	{
	public:
		observer_impl(idirectory_watcher*);

		// diable copy
		observer_impl(observer_impl const&) = delete;
		observer_impl& operator=(observer_impl const&) = delete;

		// interface
	private:
		unsigned int do_inc_request() final;
		unsigned int do_dec_request() final;
		idirectory_watcher* do_get_watcher() const final;

	private:
		void run();
		bool terminated() const;
		bool empty_request() const;
		void add_directory(irequest* pBlock);
		void request_termination();

		friend unsigned WINAPI observer::callback::start_thread_proc(LPVOID);
		friend VOID CALLBACK observer::callback::terminate_proc(__in ULONG_PTR);
		friend VOID CALLBACK observer::callback::add_directory_proc(__in ULONG_PTR);

	private:
		gsl::not_null<idirectory_watcher*> mDirWatcher;
		std::atomic_bool mTerminated{};
		std::atomic_uint mOutstandingRequests{};
		std::vector<gsl::not_null<irequest*>> mBlocks;
	};
}