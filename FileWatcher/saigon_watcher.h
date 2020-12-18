#ifndef SAIGON_WATCHER_H
#define SAIGON_WATCHER_H

namespace saigon
{
	class saigon_watcher_impl;
	class saigon_watcher
	{
	public:
		saigon_watcher();
		~saigon_watcher();
		bool start();

	private:
		saigon_watcher_impl* mWatcher;
	};
}

#endif // SAIGON_WATCHER_H