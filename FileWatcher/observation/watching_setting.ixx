module;

#include <string>

export module Saigon.WatchingSetting;

namespace saigon::observation
{
	constexpr const char* K_NONE		= "none";
	constexpr const char* K_ALL			= "all";
	constexpr const char* K_CREATE		= "create";
	constexpr const char* K_COPY		= "copy";
	constexpr const char* K_MOVE		= "move";
	constexpr const char* K_MODIFY		= "modify";
	constexpr const char* K_DELETE		= "delete";
	constexpr const char* K_RENAME		= "rename";
	constexpr const char* K_ATTRIBUTE	= "attribute";
	constexpr const char* K_SECURITY	= "security";

	export struct watching_setting
	{
		bool mSubtree{};
		unsigned long mAction{};
		std::wstring mDirectory;
		bool valid() const { return mAction > 0 and not mDirectory.empty(); }
	};

	export unsigned long action_from(std::string_view name);
}