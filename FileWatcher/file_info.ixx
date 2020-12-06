module;

#include <string>
#include <compare>
#include <filesystem>
#include "defines.h"

export module Saigon.FileInfo;

import Saigon.Concepts;

namespace saigon
{
	export class FileInfo
	{
	public:
		FileInfo() = default;
		template<Stringable T> FileInfo(T&& s);

		bool operator==(FileInfo const& other) const noexcept;
		auto operator<=>(FileInfo const& other) const noexcept = default;

	private:
		std::filesystem::path mPath;
	};

	template<Stringable T>
	FileInfo::FileInfo(T&& s) : mPath{ std::forward<T>(s) }
	{}

	bool FileInfo::operator==(FileInfo const& other) const noexcept
	{
		return mPath == other.mPath;
	}
}