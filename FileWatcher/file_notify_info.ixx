module;

#include <string>
#include <compare>
#include <filesystem>
#include "defines.h"

export module Saigon.FileNotifyInfo;

import Saigon.Concepts;

namespace saigon
{
	export class file_notify_info
	{
	public:
		file_notify_info() = default;
		template<Stringable T> file_notify_info(T&&, unsigned long);

		bool operator==(file_notify_info const& other) const noexcept;
		auto operator<=>(file_notify_info const& other) const noexcept = default;

	private:
		std::filesystem::path mPath{};
		unsigned long mAction{};
	};

	bool file_notify_info::operator==(file_notify_info const& other) const noexcept
	{
		return mPath == other.mPath;
	}

	template<Stringable T>
	file_notify_info::file_notify_info(T&& s, unsigned long action = 0ul) : 
		mPath{ std::forward<T>(s) },
		mAction{action}
	{}
}