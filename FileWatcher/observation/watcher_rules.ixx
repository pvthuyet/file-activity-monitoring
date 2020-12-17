module;

#include <string>
#include <vector>

export module Saigon.WatcherRules;

import Saigon.WatchingSetting;

namespace saigon::observation
{
	export class watcher_rules
	{
	public:
		bool load_rules();
		[[nodiscard]] bool verify(std::wstring_view path) const;
		const std::vector<watching_setting>& get_settings() const;

	private:
		bool parse_json();

	private:
		std::vector<watching_setting> mWatchingSetting;
		std::vector<std::wstring> mUserExPaths;
		std::vector<std::wstring> mSysExPaths;
		std::vector<std::wstring> mRexExPaths;
	};
}