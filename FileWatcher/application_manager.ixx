module;

#include <memory>

export module Fibo.ApplicationManager;

import Fibo.ClipboardWatcher;

namespace fibo
{
	export class ApplicationManager
	{
	public:
		static ApplicationManager& getInst();
		~ApplicationManager() noexcept;
		void run();

	private:
		ApplicationManager();
		void initialize();

	private:
		std::unique_ptr<ClipboardWatcher> clipboard_{ nullptr };
	};
}