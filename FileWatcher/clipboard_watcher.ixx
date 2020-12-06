module;

#include "defines.h"
#include <Windows.h>
#include <memory>
#include <string>

export module Saigon.ClipboardWatcher;

import Saigon.MessageEvent;
import Saigon.ConcurrentCircleMap;
import Saigon.FileInfo;

namespace saigon
{
	// This class should be global because we only have one clipboard in OS
	export class ClipboardWatcher final : public window_procedure
	{
		using CircleMap = saigon::Con::circle_map<fipmr::wstring, FileInfo, 1024>;
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
		virtual LRESULT procedure(HWND, UINT, WPARAM, LPARAM) override final;

	private:
		std::unique_ptr<message_event> mMsgEvent{ nullptr };
		CircleMap mClpData{};
	};
}