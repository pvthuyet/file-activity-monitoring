module;

#include <fstream>
#include "nlohmann/json.hpp"
#include "logger_define.h"
#include <exception>

module Saigon.ConfigReader;

namespace saigon::observation
{
	void config_reader::read()
	{
		try {
			// load json config
			using json = nlohmann::json;
			std::ifstream ifs("config\\rules.json");
			auto js = json::parse(ifs);

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
		}
		catch (std::exception const& ex) {
			SPDLOG_ERROR(ex.what());
		}
	}
}