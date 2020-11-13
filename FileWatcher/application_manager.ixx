module;

#include <memory>

export module Fibo.ApplicationManager;

import Fibo.ClipboardWatcher;
import Fibo.FileInfo; // Why ??? I don't use here

namespace fibo
{
	export class ApplicationManager
	{
	public:
		static ApplicationManager& getInst();
		~ApplicationManager() noexcept;
		void run();

		// no copyable
		ApplicationManager(ApplicationManager const&) = delete;
		ApplicationManager& operator=(ApplicationManager const&) = delete;

	private:
		ApplicationManager();
		void initialize();

	private:
		std::unique_ptr<ClipboardWatcher> clipboard_{ nullptr };
	};
}