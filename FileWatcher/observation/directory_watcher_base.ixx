module;

#include <Windows.h>
#include <memory>

export module Saigon.DirectoryWatcherBase;

import Saigon.IDirectoryWatcher;
import Saigon.IObserver;
import Saigon.WatchingInfo;

namespace saigon::observation
{
	export class directory_watcher_base : public idirectory_watcher
	{
	public:
		~directory_watcher_base() noexcept override;
		void start(watching_info&& info);
		void stop() noexcept;

	private:
		HANDLE mObserverThread{nullptr};
		unsigned mThreadId{};
		std::unique_ptr<iobserver> mObserver{};
	};
}