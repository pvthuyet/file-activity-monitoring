module;

#include <Windows.h>
#include <memory>
#include <concurrent_vector.h>
#include "logger_define.h"

module Fibo.ClipboardWatcher;
import Fibo.ConcurrentBoundedMap;
import Fibo.FileInfo;

namespace fibo
{
	ClipboardWatcher::~ClipboardWatcher() noexcept
	{
		stop();
	}

	ClipboardWatcher::ClipboardWatcher(ClipboardWatcher &&other) noexcept :
		msgEvent_{std::exchange(other.msgEvent_, nullptr)}
	{}

	ClipboardWatcher& ClipboardWatcher::operator=(ClipboardWatcher &&other) noexcept
	{
		if (this != &other) {
			this->~ClipboardWatcher();
			msgEvent_ = std::exchange(other.msgEvent_, nullptr);
		}
		return *this;
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
			{
				std::string key = "hello world";
				auto tmp = fibo::FileInfo("hello world data");
				clpData_.pushBack(key, tmp);
				auto tmp2 = clpData_.find(key);
				int xx = 0;
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