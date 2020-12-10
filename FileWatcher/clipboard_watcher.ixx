module;

#include "defines.h"
#include <Windows.h>
#include <memory>
#include <string>

export module Saigon.ClipboardWatcher;

import Saigon.MessageEvent;
import Saigon.ConcurrentCircleMap;
import Saigon.FileNotifyInfo;

namespace saigon
{
	// This class should be global because we only have one clipboard in OS
	export class ClipboardWatcher final : public window_procedure
	{
		using CircleMap = saigon::con::circle_map<fipmr::wstring, file_notify_info, 1024>;
	public:
		virtual ~ClipboardWatcher() noexcept;
		static ClipboardWatcher& getInst();
		void start();
		void stop() noexcept;

		// no copyable
		ClipboardWatcher(ClipboardWatcher const&) = delete;
		ClipboardWatcher& operator=(ClipboardWatcher const&) = delete;

	private:
		ClipboardWatcher() noexcept(std::is_nothrow_destructible_v<CircleMap>) = default;

	private:
		LRESULT do_procedure(HWND, UINT, WPARAM, LPARAM) final;

	private:
		std::unique_ptr<message_event> mMsgEvent{ nullptr };
		CircleMap mClpData{};
	};
}