module;

#include "nlohmann/json.hpp"
#include <Windows.h>
#include <string>

module Saigon.WatchingSetting;

import Saigon.StringUtils;

namespace saigon::observation
{
	unsigned long action_from(std::string_view name)
	{
		if (stringutils::equal(K_ALL, name, true)) {
			return FILE_NOTIFY_CHANGE_FILE_NAME
				bitor FILE_NOTIFY_CHANGE_LAST_WRITE
				bitor FILE_NOTIFY_CHANGE_DIR_NAME
				bitor FILE_NOTIFY_CHANGE_ATTRIBUTES
				bitor FILE_NOTIFY_CHANGE_SECURITY;
		}

		if (stringutils::equal(K_CREATE, name, true)) {
			return FILE_NOTIFY_CHANGE_FILE_NAME
				bitor FILE_NOTIFY_CHANGE_LAST_WRITE
				bitor FILE_NOTIFY_CHANGE_DIR_NAME;
		}

		if (stringutils::equal(K_COPY, name, true)) {
			return FILE_NOTIFY_CHANGE_FILE_NAME
				bitor FILE_NOTIFY_CHANGE_LAST_WRITE;
		}

		if (stringutils::equal(K_MOVE, name, true)) {
			return FILE_NOTIFY_CHANGE_FILE_NAME
				bitor FILE_NOTIFY_CHANGE_LAST_WRITE;
		}

		if (stringutils::equal(K_MODIFY, name, true)) {
			return FILE_NOTIFY_CHANGE_FILE_NAME
				bitor FILE_NOTIFY_CHANGE_LAST_WRITE;
		}

		if (stringutils::equal(K_DELETE, name, true)) {
			return FILE_NOTIFY_CHANGE_FILE_NAME
				bitor FILE_NOTIFY_CHANGE_DIR_NAME;
		}

		if (stringutils::equal(K_RENAME, name, true)) {
			return FILE_NOTIFY_CHANGE_FILE_NAME
				bitor FILE_NOTIFY_CHANGE_DIR_NAME;
		}

		if (stringutils::equal(K_ATTRIBUTE, name, true)) {
			return FILE_NOTIFY_CHANGE_ATTRIBUTES;
		}

		if (stringutils::equal(K_SECURITY, name, true)) {
			return FILE_NOTIFY_CHANGE_SECURITY;
		}

		return 0;
	}
}