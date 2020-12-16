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
		[[nodiscard]] static watcher_rules& get_inst();
		bool load_rules();
		[[nodiscard]] bool verify(std::wstring_view path) const;
		std::vector<watching_setting> get_settings() const;

	private:
		watcher_rules() = default;
		bool parse_json();

	private:
		std::vector<watching_setting> mWatchingSetting;
		std::vector<std::wstring> mUserExPaths;
		std::vector<std::wstring> mSysExPaths;
		std::vector<std::wstring> mRexExPaths;
	};
}