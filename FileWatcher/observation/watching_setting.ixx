module;

#include <string>

export module Saigon.WatchingSetting;

namespace saigon::observation
{
	export struct watching_setting
	{
		bool mSubtree{};
		unsigned long mAction{};
		std::wstring mDirectory;
		bool valid() const { return mAction > 0 and not mDirectory.empty(); }
	};

	export unsigned long action_from(std::string_view name);
}