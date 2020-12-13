module;

#include <fstream>
#include <sstream>
#include "nlohmann/json.hpp"
#include <typeinfo>

export module Saigon.ConfigReader;

import Saigon.StringUtils;

namespace saigon::observation
{
	export class config_reader
	{
	public:
		void read();
	};

	void config_reader::read()
	{
		constexpr char const* SYS_EX_PATH = "system_exclude_paths";
		using json = nlohmann::json;
		std::vector<std::wstring> vecsys;
		std::ifstream ifs("config\\rules.json");
		auto js = json::parse(ifs);
		auto& syspath = js[SYS_EX_PATH];
		for (auto const& el : syspath.items()) {
			vecsys.push_back(saigon::stringutils::convert(std::string{el.value()}));
		}
	}
}