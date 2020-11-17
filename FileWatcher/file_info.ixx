module;

#include <string>
#include <compare>
#include "define.h"

import Fibo.Concept;

export module Fibo.FileInfo;

namespace fibo
{
	template<Stringable TString>
	class FileInfoBase
	{
		using string_type = tstring_t<TString>;
	public:
		FileInfoBase() noexcept(std::is_nothrow_default_constructible_v<string_type>) = default;

		template<Stringable T>
		FileInfoBase(T&& s) : filePath_{ std::forward<T>(s) }
		{}

		bool operator==(FileInfoBase const& other) const noexcept
		{
			return filePath_ == other.filePath_;
		}
		auto operator<=>(FileInfoBase const& other) const noexcept = default;

	private:
		string_type filePath_{};
		size_t size_{0};
	};

	export using FileInfo = FileInfoBase<fistdpmr::string>;
	export using WFileInfo = FileInfoBase<fistdpmr::wstring>;
}