module;

#include <Windows.h>

export module Saigon.WatchingAction;

namespace saigon::observation
{
	export enum waching_action : DWORD
	{
		none			= 0,
		// file name
		file_create		= FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
		file_copy		= FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
		file_move		= FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
		file_modify		= FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
		file_delete		= FILE_NOTIFY_CHANGE_FILE_NAME,
		file_rename		= FILE_NOTIFY_CHANGE_FILE_NAME,
		// file attribute
		file_attribute	= FILE_NOTIFY_CHANGE_ATTRIBUTES,
		// file security
		file_security	= FILE_NOTIFY_CHANGE_SECURITY,
		// folder name
		folder_create	= FILE_NOTIFY_CHANGE_DIR_NAME,
		folder_delete	= FILE_NOTIFY_CHANGE_DIR_NAME,
		folder_rename	= FILE_NOTIFY_CHANGE_DIR_NAME
	};
}