export module Saigon.IObserver;

namespace saigon
{
	export class iobserver
	{
	public:
		virtual ~iobserver() noexcept = default;
		virtual unsigned int inc_request() = 0;
		virtual unsigned int dec_request() = 0;
	};
}