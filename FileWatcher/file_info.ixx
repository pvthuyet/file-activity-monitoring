module;

#include <string>
#include <compare>
#include "defines.h"

export module Saigon.FileInfo;

import Saigon.Concepts;

namespace saigon
{
	template<Stringable TString>
	class FileInfoBase
	{
		using string_type = tstring_t<TString>;
	public:
		FileInfoBase() noexcept(std::is_nothrow_default_constructible_v<string_type>) = default;

		template<Stringable T>
		FileInfoBase(T&& s) : mFilePath{ std::forward<T>(s) }
		{}

		bool operator==(FileInfoBase const& other) const noexcept
		{
			return mFilePath == other.mFilePath;
		}
		auto operator<=>(FileInfoBase const& other) const noexcept = default;

	private:
		string_type mFilePath{};
		size_t mSize{0};
	};

	export using FileInfo = FileInfoBase<fipmr::string>;
	export using WFileInfo = FileInfoBase<fipmr::wstring>;
}