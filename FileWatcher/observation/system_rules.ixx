module;

#include <array>
#include <string>

export module Saigon.SystemRules;

import Saigon.StringUtils;

namespace saigon::observation
{
	export class system_rules
	{
	public:
		system_rules();
		[[nodiscard]] bool verify(std::wstring_view path) const;

	private:
		bool is_system(std::wstring_view path) const;
		bool is_appdata(std::wstring_view path) const;
		bool is_tmp(std::wstring_view path) const;

	private:
		std::array<std::wstring, 10> mSpamPath;
	};

	bool system_rules::verify(std::wstring_view path) const
	{
		if (is_system(path)) {
			return false;
		}

		if (is_appdata(path)) {
			return false;
		}

		if (is_tmp(path)) {
			return false;
		}

		return true;
	}

	system_rules::system_rules()
	{
		mSpamPath[0] = L":\\$Recycle.Bin\\";
		mSpamPath[1] = L":\\System Volume Information\\";
		mSpamPath[2] = L"C:\\hiberfil.sys";
		mSpamPath[3] = L"C:\\Config.Msi";
	}

	bool system_rules::is_system(std::wstring_view path) const
	{
		for (auto const& p : mSpamPath) {
			if (saigon::stringutils::find(path, p)) {
				return true;
			}
		}
		return false;
	}

	bool system_rules::is_appdata(std::wstring_view path) const
	{
		return false;
	}

	bool system_rules::is_tmp(std::wstring_view path) const
	{
		return false;
	}
}