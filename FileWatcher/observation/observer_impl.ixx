module;

#include <gsl/pointers>
#include <Windows.h>
#include <atomic>
#include <vector>

export module Saigon.ObserverImpl;

import Saigon.IObserver;
import Saigon.Request;

namespace saigon::observation
{
	export class observer_impl final : public iobserver
	{
	public:
		~observer_impl() noexcept override = default;
		static unsigned int WINAPI start_thread_proc(LPVOID arg);
		unsigned int inc_request() final;
		unsigned int dec_request() final;

	private:
		[[nodiscard]] bool terminated() const;
		[[nodiscard]] bool empty_request() const;
		void run();
		void add_directory(request* pBlock);
		void request_termination();

	private:
		std::atomic_bool mTerminated{};
		std::atomic_uint mOutstandingRequests{};
		std::vector<gsl::not_null<request*>> mBlocks;
	};
}