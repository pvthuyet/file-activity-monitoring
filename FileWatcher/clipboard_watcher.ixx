module;

#include <Windows.h>
#include <memory>
#include <string>

export module Fibo.ClipboardWatcher;

import Fibo.MessageEvent;
import Fibo.ConcurrentCircleMap;
import Fibo.FileInfo;

namespace fibo
{
	export class ClipboardWatcher final : public WindowProcedure
	{
		using CircleMap = fibo::Con::CircleMap<std::string, FileInfo, 1024>;
	public:
		ClipboardWatcher() noexcept = default;
		virtual ~ClipboardWatcher() noexcept;

		// no copyable
		ClipboardWatcher(ClipboardWatcher const&) = delete;
		ClipboardWatcher& operator=(ClipboardWatcher const&) = delete;

		// movable
		ClipboardWatcher(ClipboardWatcher&&) noexcept;
		ClipboardWatcher& operator=(ClipboardWatcher&&) noexcept;

		void start();
		void stop() noexcept;

	private:
		virtual LRESULT procedure(HWND, UINT, WPARAM, LPARAM) override final;

	private:
		std::unique_ptr<MessageEvent> msgEvent_ = nullptr;
		CircleMap clpData_;
	};
}