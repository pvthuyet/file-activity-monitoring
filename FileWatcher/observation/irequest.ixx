module;

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

		bool begin_read()
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

	private:
		virtual bool do_open_directory() = 0;
		virtual bool do_begin_read() = 0;
		virtual void do_request_termination() = 0;
		virtual iobserver* do_get_observer() const = 0;
	};
}