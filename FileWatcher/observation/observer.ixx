module;

#include <Windows.h>
#include <atomic>
#include <gsl/pointers>
#include <vector>

export module Saigon.Observer;

namespace saigon::observation
{
	class request;
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
		void addDirectory(request* pBlock);
		void requestTermination();

	private:
		std::atomic_bool mTerminated{};
		std::atomic_uint mOutstandingRequests{};
		std::vector<gsl::not_null<request*>> mBlocks;
	};
}