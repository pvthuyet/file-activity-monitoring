module;

#include <string>
#include <vector>

export module Saigon.ConfigReader;

import Saigon.StringUtils;
import Saigon.WatchingSetting;

namespace saigon::observation
{
	export class config_reader
	{
	public:
		bool read();

	private:
		std::vector<watching_setting> mWatchingSetting;
		std::vector<std::wstring> mUserExPaths;
		std::vector<std::wstring> mSysExPaths;
		std::vector<std::wstring> mRexExPaths;
	};
}