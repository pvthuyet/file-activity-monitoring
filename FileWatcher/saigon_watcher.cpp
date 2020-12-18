#include "saigon_watcher.h"
#include "saigon_watcher_impl.h"

namespace saigon
{
	saigon_watcher::saigon_watcher()
	{
		mWatcher = new saigon_watcher_impl;
	}

	saigon_watcher::~saigon_watcher()
	{
		delete mWatcher;
	}

	bool saigon_watcher::start()
	{
		return mWatcher->start();
	}
}