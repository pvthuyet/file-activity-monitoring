module;

#include <Windows.h>
#include <memory>
#include <concurrent_vector.h>
#include "logger_define.h"

module Fibo.ClipboardWatcher;
import Fibo.ConcurrentCircleMap;
import Fibo.FileInfo;

namespace fibo
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
		if (not msgEvent_) {
			msgEvent_ = std::make_unique<MessageEvent>(this);
			msgEvent_->registerWndClass(L"clipboard_class");
			auto succ = ::AddClipboardFormatListener(msgEvent_->getWndHandle());
			if (not succ) {
				msgEvent_ = nullptr;
				throw std::system_error(
					::GetLastError(),
					std::system_category(),
					std::string{ "Failed to call AddClipboardFormatListener. " } + __FILE__ + std::to_string(__LINE__));
			}
		}
	}

	void ClipboardWatcher::stop() noexcept
	{
		if (msgEvent_) {
			auto succ = ::RemoveClipboardFormatListener(msgEvent_->getWndHandle());
			if (not succ) {
				SPDLOG_ERROR("Failed to call RemoveClipboardFormatListener");
			}
			msgEvent_->unregisterWndClass();
			msgEvent_ = nullptr;
		}
	}

	LRESULT ClipboardWatcher::procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_CLIPBOARDUPDATE:
			SPDLOG_INFO("Clipboard is updated!");
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