module;

#include <string>
#include <vector>

export module Saigon.ConfigReader;

import Saigon.StringUtils;

namespace saigon::observation
{
	export class config_reader
	{
	public:
		void read();

	private:
		std::vector<std::wstring> mUserExPaths;
		std::vector<std::wstring> mSysExPaths;
		std::vector<std::wstring> mRexExPaths;
	};
}