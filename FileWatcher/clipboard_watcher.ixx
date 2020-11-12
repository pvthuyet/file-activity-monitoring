module;

#include <Windows.h>
#include <memory>

export module Fibo.ClipboardWatcher;

import Fibo.MessageEvent;

namespace fibo
{
	export class ClipboardWatcher final : public WindowProcedure
	{
	public:
		virtual ~ClipboardWatcher() noexcept {}
		virtual LRESULT procedure(HWND, UINT, WPARAM, LPARAM) const override final;
		void start();
		void stop() noexcept;

	private:
		std::unique_ptr<MessageEvent> msgEvent_ = nullptr;
	};
}