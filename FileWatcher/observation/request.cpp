module;

#include <shlwapi.h>
#include <Windows.h>
#include <vector>
#include <string>

#pragma comment(lib, "Shlwapi")

module Saigon.Request;

import Saigon.FileNotifyInfo;
namespace saigon::observation
{
	request::request(iobserver* obs,
		std::wstring_view directory,
		DWORD filterFlags,
		BOOL includeChildren,
		DWORD size
	) :
		mObserver{ obs },
		mDirectory{ directory },
		mFilterFlags{ filterFlags },
		mIncludeChildren{ includeChildren },
		mBuffer(size)
	{
		::ZeroMemory(&mOverlapped, sizeof(OVERLAPPED));
		// The hEvent member is not used when there is a completion
		// function, so it's ok to use it to point to the object.
		mOverlapped.hEvent = this;
		_ASSERTE(mObserver);
	}

	request::~request() noexcept
	{
		_ASSERTE(INVALID_HANDLE_VALUE == mHdlDirectory);
	}

	iobserver* request::get_observer() const
	{
		_ASSERT_EXPR(mObserver, "Observer is null");
		return mObserver;
	}

	void request::request_termination()
	{
		if (INVALID_HANDLE_VALUE != mHdlDirectory) {
			::CancelIo(mHdlDirectory);
			::CloseHandle(mHdlDirectory);
			mHdlDirectory = INVALID_HANDLE_VALUE;
		}
	}

	BOOL request::open_directory()
	{
		// Allow this routine to be called redundantly.
		if (INVALID_HANDLE_VALUE != mHdlDirectory) {
			return TRUE;
		}

		mHdlDirectory = ::CreateFileW(
			mDirectory.c_str(),					// pointer to the file name
			FILE_LIST_DIRECTORY,                // access (read/write) mode
			FILE_SHARE_READ						// share mode
			| FILE_SHARE_WRITE
			| FILE_SHARE_DELETE,
			NULL,                               // security descriptor
			OPEN_EXISTING,                      // how to create
			FILE_FLAG_BACKUP_SEMANTICS			// file attributes
			| FILE_FLAG_OVERLAPPED,
			NULL);                              // file with attributes to copy

		return (INVALID_HANDLE_VALUE != mHdlDirectory) ? TRUE : FALSE;
	}

	void request::backup_buffer(DWORD dwSize)
	{
		// We could just swap back and forth between the two
		// buffers, but this code is easier to understand and debug.
		memcpy(&mBackupBuffer[0], &mBuffer[0], dwSize);
	}

	BOOL request::begin_read()
	{
		DWORD dwBytes = 0;

		// This call needs to be reissued after every APC.
		return ::ReadDirectoryChangesW(
			mHdlDirectory,						// handle to directory
			&mBuffer[0],						// read results buffer
			(DWORD)mBuffer.size(),				// length of buffer
			mIncludeChildren,					// monitoring option
			mFilterFlags,						// filter conditions
			&dwBytes,                           // bytes returned
			&mOverlapped,						// overlapped buffer
			&notification_completion);           // completion routine
	}

	void request::process_notification()
	{
		BYTE* pBase = mBackupBuffer.data();

		for (;;) {
			FILE_NOTIFY_INFORMATION& fni = (FILE_NOTIFY_INFORMATION&)*pBase;

			std::wstring wsFileName(fni.FileName, fni.FileNameLength / sizeof(wchar_t));
			wchar_t wcRight = mDirectory.at(mDirectory.length() - 1);
			// Handle a trailing backslash, such as for a root directory.
			if (L'\\' != wcRight) {
				wsFileName = mDirectory + L"\\" + wsFileName;
			}
			else {
				wsFileName = mDirectory + wsFileName;
			}

			// If it could be a short filename, expand it.
			LPCWSTR shortFileName = ::PathFindFileNameW(wsFileName.c_str());
			int len = lstrlenW(shortFileName);
			// The maximum length of an 8.3 filename is twelve, including the dot.
			if (len <= 12 && wcschr(shortFileName, L'~'))
			{
				// Convert to the long filename form. Unfortunately, this
				// does not work for deletions, so it's an imperfect fix.
				wchar_t wbuf[MAX_PATH] = { 0 };
				if (::GetLongPathNameW(wsFileName.c_str(), wbuf, _countof(wbuf)) > 0) {
					wsFileName = wbuf;
				}
			}

			saigon::file_notify_info info(wsFileName, fni.Action);
			//if (!ignoreFile(info)) {
				//get_observer()->getDirectoryMonitor()->push(std::move(info));
			//}

			if (!fni.NextEntryOffset) {
				break;
			}
			pBase += fni.NextEntryOffset;
		}
	}

	VOID CALLBACK request::notification_completion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
	{
		request* pBlock = (request*)lpOverlapped->hEvent;

		if (dwErrorCode == ERROR_OPERATION_ABORTED)
		{
			pBlock->get_observer()->dec_request();
			delete pBlock;
			return;
		}

		// Can't use sizeof(FILE_NOTIFY_INFORMATION) because
		// the structure is padded to 16 bytes.
		//_ASSERTE(dwNumberOfBytesTransfered >= offsetof(FILE_NOTIFY_INFORMATION, FileName) + sizeof(WCHAR));
		// This might mean overflow? Not sure.
		//if (!dwNumberOfBytesTransfered) {
		//	return;
		//}

		pBlock->backup_buffer(dwNumberOfBytesTransfered);

		// Get the new read issued as fast as possible. The documentation
		// says that the original OVERLAPPED structure will not be used
		// again once the completion routine is called.
		pBlock->begin_read();

		pBlock->process_notification();
	}
}