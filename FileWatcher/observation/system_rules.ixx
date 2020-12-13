module;

#include <array>
#include <string>

export module Saigon.SystemRules;

import Saigon.FileNotifyInfo;
import Saigon.StringUtils;

namespace saigon::observation
{
	export class system_rules
	{
	public:
		static system_rules& get_inst();
		[[nodiscard]] bool verify(saigon::file_notify_info const& info) const;

	private:
		system_rules();
		bool is_system(std::wstring const& path) const;
		bool is_appdata(std::wstring const& path) const;
		bool is_tmp(std::wstring const& path) const;

	private:
		std::array<std::wstring, 10> mSpamPath;
	};

	system_rules& system_rules::get_inst()
	{
		static system_rules inst{};
		return inst;
	}

	bool system_rules::verify(saigon::file_notify_info const& info) const
	{
		return false;
	}

	system_rules::system_rules()
	{
		mSpamPath[0] = L":\\$Recycle.Bin\\";
		mSpamPath[1] = L":\\System Volume Information\\";
		mSpamPath[2] = L"C:\\hiberfil.sys";
		mSpamPath[3] = L"C:\\Config.Msi";
	}

	bool system_rules::is_system(std::wstring const& path) const
	{
		return true;
	}

	bool system_rules::is_appdata(std::wstring const& path) const
	{
		return true;
	}

	bool system_rules::is_tmp(std::wstring const& path) const
	{
		return true;
	}
}