module;

#include <Windows.h>
#include <memory>
#include <concurrent_vector.h>
#include "logger_define.h"
#include "define.h"

module Fibo.ClipboardWatcher;
import Fibo.ConcurrentCircleMap;
import Fibo.FileInfo;
import Fibo.Clipboard;

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
			msgEvent_ = fistd::make_unique<MessageEvent>(this);
			msgEvent_->registerWndClass(L"clipboard_class");
			auto succ = ::AddClipboardFormatListener(msgEvent_->getWndHandle());
			if (not succ) {
				msgEvent_ = nullptr;
				throw fistd::system_error(
					::GetLastError(),
					fistd::system_category(),
					fistd::string{ "Failed to call AddClipboardFormatListener. " } + __FILE__ + std::to_string(__LINE__));
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
			{
				auto files = fibo::Clipboard::getCopyingFiles();
				for (auto const& f : files) {
					clpData_[f] = WFileInfo{ f };
					SPDLOG_DEBUG(f);
				}
				SPDLOG_DEBUG("Number of element in map: {}", clpData_.size());
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