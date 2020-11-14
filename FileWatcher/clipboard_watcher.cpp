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
				using namespace std::string_literals;
				static int n = 0;
				std::string key = "key " + std::to_string(n);
				auto key2 = key;
				auto tmp = fibo::FileInfo("hello to "s + std::to_string(n++));
				clpData_[key] = tmp;
				SPDLOG_INFO("operator[{}]: {}", key, clpData_[key].toString());

				for (int i = 0; i < 2; ++i) {
					auto found = clpData_.find(key);
					if (found) {
						SPDLOG_INFO("find[{}]: {}", key, (*found).toString());
					}
					else {
						SPDLOG_INFO("find[{}]: not found", key);
					}
					key = "abc";
				}

				auto found2 = clpData_.find_if([&key2](auto const& keyItem, auto const& item) {
					return keyItem == key2;
					});

				if (found2) {
					SPDLOG_INFO("find_if[{}]: {}", key2, (*found2).toString());
				}
				else {
					SPDLOG_INFO("find_if[{}]: not found", key);
				}
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