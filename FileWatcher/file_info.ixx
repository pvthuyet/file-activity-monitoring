module;

#include <string>

export module Fibo.FileInfo;

namespace fibo
{
	export class FileInfo
	{
	public:
		FileInfo() = default;
		FileInfo(std::string_view);

		std::string toString() const;

	private:
		std::string filePath_{};
		size_t size_{0};
	};

	FileInfo::FileInfo(std::string_view path) :
		FileInfo()
	{
		filePath_ = path;
		size_ = 1;
	}

	std::string FileInfo::toString() const
	{
		return "file path: " + filePath_ + ", size: " + std::to_string(size_);
	}
}