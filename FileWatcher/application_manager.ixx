module;

export module Saigon.ApplicationManager;

import Saigon.FilenameWatcher;

namespace saigon
{
	export class ApplicationManager
	{
	public:
		~ApplicationManager() noexcept;
		static ApplicationManager& getInst();
		void run();

		// no copyable
		ApplicationManager(ApplicationManager const&) = delete;
		ApplicationManager& operator=(ApplicationManager const&) = delete;

	private:
		ApplicationManager();
		void initialize();

	private:
		observation::filename_watcher mFileNameWatcher;
	};
}