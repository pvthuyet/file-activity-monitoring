module;

#include <string>
#include "nlohmann\json.hpp"

export module Saigon.WatcherRules;

import Saigon.SystemRules;

namespace saigon::observation
{
	export class watcher_rules
	{
	public:
		[[nodiscard]] static watcher_rules& get_inst();
		bool load_rules() const;
		[[nodiscard]] bool verify(std::wstring_view path) const;

	private:
		watcher_rules() = default;

	private:
		system_rules mSysRules{};
	};

	watcher_rules& watcher_rules::get_inst()
	{
		static watcher_rules inst{};
		return inst;
	}

	bool watcher_rules::load_rules() const
	{
		using json = nlohmann::json;
		std::wstring rulePath = L"config\\rules.json";
		auto rules = json::parse(rulePath);
		int xx = 0;
		return true;
	}

	bool watcher_rules::verify(std::wstring_view path) const
	{
		if (not mSysRules.verify(path)) {
			return false;
		}

		//++ TODO
		return true;
	}
}