export module Saigon.IObserver;

namespace saigon
{
	export class iobserver
	{
	public:
		virtual ~iobserver() noexcept = default;

		unsigned int inc_request()
		{
			return do_inc_request();
		}

		unsigned int dec_request()
		{
			return do_dec_request();
		}

	private:
		virtual unsigned int do_inc_request() = 0;
		virtual unsigned int do_dec_request() = 0;
	};
}