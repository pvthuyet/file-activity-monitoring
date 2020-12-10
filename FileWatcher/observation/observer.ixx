module;

#include <Windows.h>
#include <atomic>

export module Saigon.Observer;

namespace saigon::observation
{
	export class observer
	{
	public:
		static unsigned int WINAPI start_thread_proc(LPVOID arg);
		unsigned int incRequest();
		unsigned int decRequest();

	private:
		[[nodiscard]] bool terminated() const;
		[[nodiscard]] bool empty_request() const;
		void run();

	private:
		std::atomic_bool mTerminated{};
		std::atomic_uint mOutstandingRequests{};
	};
}