module;

export module Saigon.ApplicationManager;

namespace saigon
{
	export class ApplicationManager
	{
	public:
		~ApplicationManager() noexcept = default;
		static ApplicationManager& getInst();
		void run();
		void stop();

		// no copyable
		ApplicationManager(ApplicationManager const&) = delete;
		ApplicationManager& operator=(ApplicationManager const&) = delete;

	private:
		ApplicationManager();
		void initialize();
	};
}