module;

#include <string>

export module Saigon.WatcherRules;

import Saigon.SystemRules;
import Saigon.ConfigReader;

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
		config_reader cnf;
		cnf.read();
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