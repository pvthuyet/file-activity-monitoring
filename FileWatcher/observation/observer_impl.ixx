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
	export class observer_impl final : public iobserver
	{
	public:
		// diable copy
		observer_impl(observer_impl const&) = delete;
		observer_impl& operator=(observer_impl const&) = delete;

	private:
		unsigned int do_inc_request() final;
		unsigned int do_dec_request() final;

	private:
		void run();
		[[nodiscard]] bool terminated() const;
		[[nodiscard]] bool empty_request() const;
		void add_directory(irequest* pBlock);
		void request_termination();
		friend unsigned int WINAPI start_observer_thread_proc(LPVOID);

	private:
		std::atomic_bool mTerminated{};
		std::atomic_uint mOutstandingRequests{};
		std::vector<gsl::not_null<irequest*>> mBlocks;
	};

	static unsigned int WINAPI start_observer_thread_proc(LPVOID arg);
}