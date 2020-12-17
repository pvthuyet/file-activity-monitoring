module;

#include <Windows.h>
#include <memory>
#include <vector>

export module Saigon.DirectoryWatcherBase;

import Saigon.IDirectoryWatcher;
import Saigon.IObserver;
import Saigon.WatchingSetting;

namespace saigon::observation
{
	export class directory_watcher_base : public idirectory_watcher
	{
	public:
		~directory_watcher_base() noexcept override;
		bool start(const std::vector<watching_setting>&);
		void stop() noexcept;

	private:
		HANDLE mObserverThread{nullptr};
		unsigned mThreadId{};
		std::unique_ptr<iobserver> mObserver{};
	};
}