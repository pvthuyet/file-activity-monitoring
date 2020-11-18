module;

#include "fibo_define.h"
#include <Windows.h>
#include <memory>
#include <string>

export module Fibo.ClipboardWatcher;

import Fibo.MessageEvent;
import Fibo.ConcurrentCircleMap;
import Fibo.FileInfo;

namespace fibo
{
	// This class should be global because we only have one clipboard in OS
	export class ClipboardWatcher final : public WindowProcedure
	{
		using CircleMap = fibo::Con::CircleMap<fistdpmr::wstring, WFileInfo, 1024>;
	public:
		virtual ~ClipboardWatcher() noexcept;
		static ClipboardWatcher& getInst();
		void start();
		void stop() noexcept;

		// no copyable
		ClipboardWatcher(ClipboardWatcher const&) = delete;
		ClipboardWatcher& operator=(ClipboardWatcher const&) = delete;

	private:
		ClipboardWatcher() noexcept(fistd::is_nothrow_destructible_v<CircleMap>) = default;

	private:
		virtual LRESULT procedure(HWND, UINT, WPARAM, LPARAM) override final;

	private:
		std::unique_ptr<MessageEvent> mMsgEvent{ nullptr };
		CircleMap mClpData{};
	};
}