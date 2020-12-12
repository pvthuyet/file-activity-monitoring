module;

#include <Windows.h>
#include <memory>

export module Saigon.DirectoryWatcherBase;

import Saigon.IDirectoryWatcher;
import Saigon.IObserver;
namespace saigon::observation
{
	export class directory_watcher_base : public idirectory_watcher
	{
	public:
		void start();
		void stop();

	private:
		virtual DWORD get_notify_filters() const = 0;

	private:
		HANDLE mObserverThread{nullptr};
		unsigned mThreadId{};
		std::unique_ptr<iobserver> mObserver{};
	};
}