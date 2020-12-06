module;

#include <Windows.h>
#include <memory>
#include <concurrent_vector.h>
#include "logger_define.h"
#include "defines.h"

module Saigon.ClipboardWatcher;
import Saigon.ConcurrentCircleMap;
import Saigon.FileInfo;
import Saigon.Clipboard;

namespace saigon
{
	ClipboardWatcher::~ClipboardWatcher() noexcept
	{
		LOGENTER;
		stop();
		LOGEXIT;
	}

	ClipboardWatcher& ClipboardWatcher::getInst()
	{
		static ClipboardWatcher gClipboard{};
		return gClipboard;
	}

	void ClipboardWatcher::start()
	{
		if (not mMsgEvent) {
			mMsgEvent = std::make_unique<message_event>(this);
			mMsgEvent->register_window_class(L"clipboard_class");
			auto succ = ::AddClipboardFormatListener(mMsgEvent->get_handle());
			if (not succ) {
				mMsgEvent = nullptr;
				throw std::system_error(
					::GetLastError(),
					std::system_category(),
					std::string{ "Failed to call AddClipboardFormatListener. " } + __FILE__ + std::to_string(__LINE__));
			}
		}
	}

	void ClipboardWatcher::stop() noexcept
	{
		if (mMsgEvent) {
			auto succ = ::RemoveClipboardFormatListener(mMsgEvent->get_handle());
			if (not succ) {
				SPDLOG_ERROR("Failed to call RemoveClipboardFormatListener");
			}
			mMsgEvent->unregister_window_class();
			mMsgEvent = nullptr;
		}
	}

	LRESULT ClipboardWatcher::procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_CLIPBOARDUPDATE:
			{
				auto files = saigon::Clipboard::getCopyingFiles();
				for (auto const& p : files) {
					mClpData[p] = WFileInfo{ p };
					SPDLOG_DEBUG(p);
				}
				SPDLOG_DEBUG("Number of element in map: {}", mClpData.size());
			}
			break;

		case WM_DESTROY:
			SPDLOG_INFO("WM_DESTROY monitor clipboard");
			::PostQuitMessage(0);
			break;

		default:
			return ::DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}
}