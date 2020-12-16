module;

#include <Windows.h>
#include <string>

export module Saigon.WatchingInfo;

namespace saigon::observation
{
	export enum waching_action : DWORD
	{
		none			= 0,
		// file name
		file_create		= FILE_NOTIFY_CHANGE_FILE_NAME bitor FILE_NOTIFY_CHANGE_LAST_WRITE,
		file_copy		= FILE_NOTIFY_CHANGE_FILE_NAME bitor FILE_NOTIFY_CHANGE_LAST_WRITE,
		file_move		= FILE_NOTIFY_CHANGE_FILE_NAME bitor FILE_NOTIFY_CHANGE_LAST_WRITE,
		file_modify		= FILE_NOTIFY_CHANGE_FILE_NAME bitor FILE_NOTIFY_CHANGE_LAST_WRITE,
		file_delete		= FILE_NOTIFY_CHANGE_FILE_NAME,
		file_rename		= FILE_NOTIFY_CHANGE_FILE_NAME,
		// file attribute
		file_attribute	= FILE_NOTIFY_CHANGE_ATTRIBUTES,
		// file security
		file_security	= FILE_NOTIFY_CHANGE_SECURITY,
		// folder name
		folder_create	= FILE_NOTIFY_CHANGE_DIR_NAME,
		folder_delete	= FILE_NOTIFY_CHANGE_DIR_NAME,
		folder_rename	= FILE_NOTIFY_CHANGE_DIR_NAME,
		all				= file_create 
							bitor file_copy 
							bitor file_move 
							bitor file_modify 
							bitor file_delete 
							bitor file_rename 
							bitor file_attribute 
							bitor file_security 
							bitor folder_create 
							bitor folder_delete 
							bitor folder_rename
	};

	export struct watching_info
	{
		BOOL mSubtree{TRUE};
		waching_action mAction{ waching_action::all };
		std::wstring mDirectory;
	};
}