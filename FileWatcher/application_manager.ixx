module;

export module Fibo.ApplicationManager;

namespace fibo
{
	export class ApplicationManager
	{
	public:
		~ApplicationManager() noexcept = default;
		static ApplicationManager& getInst();
		void run();

		// no copyable
		ApplicationManager(ApplicationManager const&) = delete;
		ApplicationManager& operator=(ApplicationManager const&) = delete;

	private:
		ApplicationManager();
		void initialize();
	};
}