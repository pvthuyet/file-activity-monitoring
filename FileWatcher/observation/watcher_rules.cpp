module;

#include <fstream>
#include <string>
#include "nlohmann/json.hpp"
#include "logger_define.h"
#include <exception>


module Saigon.WatcherRules;

import Saigon.StringUtils;

namespace saigon::observation
{
	watching_setting setting_from_json(nlohmann::json const& js)
	{
		watching_setting was;
		constexpr char const* K_DIRECTORIES = "directories";
		constexpr char const* K_SUBTREE = "subtree";
		constexpr char const* K_ACTION = "action";

		for (auto& [key, value] : js.items()) {
			if (stringutils::equal(K_DIRECTORIES, key)) {
				was.mDirectory = stringutils::convert(std::string_view{ value });
			}
			else if (stringutils::equal(K_SUBTREE, key)) {
				was.mSubtree = value;
			}
			else if (stringutils::equal(K_ACTION, key)) {
				was.mAction = action_from(value);
			}
		}
		return was;
	}

	const std::vector<watching_setting>& watcher_rules::get_settings() const
	{
		return mWatchingSetting;
	}

	bool watcher_rules::load_rules()
	{
		return parse_json();
	}

	bool watcher_rules::verify(std::wstring_view path) const
	{
		//if (not mSysRules.verify(path)) {
		//	return false;
		//}

		//++ TODO
		return true;
	}

	bool watcher_rules::parse_json()
	{
		try {
			// load json config
			using json = nlohmann::json;
			std::ifstream ifs("config\\rules.json");
			auto js = json::parse(ifs);

			// Parse 'watching_settings'
			constexpr char const* K_WATCHING_SETTINGS = "watching_settings";
			auto const& setting = js[K_WATCHING_SETTINGS];
			mWatchingSetting.clear();
			mWatchingSetting.reserve(setting.size());
			for (auto const& el : setting.items()) {
				mWatchingSetting.push_back(setting_from_json(el.value()));
			}

			// Parse 'system_exclude_paths'
			constexpr char const* USER_EX_PATH = "user_exclude_paths";
			auto const& userpath = js[USER_EX_PATH];
			mUserExPaths.clear();
			mUserExPaths.reserve(userpath.size());
			for (auto const& el : userpath.items()) {
				mUserExPaths.push_back(saigon::stringutils::convert(std::string_view{ el.value() }));
			}

			// Parse 'system_exclude_paths'
			constexpr char const* SYS_EX_PATH = "system_exclude_paths";
			auto const& syspath = js[SYS_EX_PATH];
			mSysExPaths.clear();
			mSysExPaths.reserve(syspath.size());
			for (auto const& el : syspath.items()) {
				mSysExPaths.push_back(saigon::stringutils::convert(std::string_view{ el.value() }));
			}

			// Parse 'regex_exclude_paths'
			constexpr char const* REX_EX_PATH = "regex_exclude_paths";
			auto const& rexpath = js[REX_EX_PATH];
			mRexExPaths.clear();
			mRexExPaths.reserve(rexpath.size());
			for (auto const& el : rexpath.items()) {
				mRexExPaths.push_back(saigon::stringutils::convert(std::string_view{ el.value() }));
			}
			return true;
		}
		catch (std::exception const& ex) {
			SPDLOG_ERROR(ex.what());
		}

		return false;
	}
}