module;

#include <string>
#include <vector>

export module Saigon.WatchingRules;

import Saigon.WatchingSetting;

namespace saigon::observation
{
	export class watching_rules
	{
	public:
		void load_rules();
		[[nodiscard]] bool verify(std::wstring_view path) const;
		const std::vector<watching_setting>& get_settings() const;

	private:
		void parse_json();

	private:
		std::vector<watching_setting> mWatchingSetting;
		std::vector<std::wstring> mUserExPaths;
		std::vector<std::wstring> mSysExPaths;
		std::vector<std::wstring> mRexExPaths;
	};
}