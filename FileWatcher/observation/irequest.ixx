module;

#include <string>

export module Saigon.IRequest;

import Saigon.IObserver;

namespace saigon::observation
{
	export class irequest
	{
	public:
		virtual ~irequest() noexcept = default;
		[[nodiscard]] bool open_directory()
		{
			return do_open_directory();
		}

		[[nodiscard]] bool begin_read()
		{
			return do_begin_read();
		}

		void request_termination()
		{
			do_request_termination();
		}

		[[nodiscard]] iobserver* get_observer() const
		{
			return do_get_observer();
		}

		[[nodiscard]] std::wstring get_request_id() const
		{
			return do_get_request_id();
		}

	private:
		virtual bool do_open_directory() = 0;
		virtual bool do_begin_read() = 0;
		virtual void do_request_termination() = 0;
		virtual iobserver* do_get_observer() const = 0;
		virtual std::wstring do_get_request_id() const = 0;
	};
}