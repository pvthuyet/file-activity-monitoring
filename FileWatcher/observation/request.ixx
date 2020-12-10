module;

#include <gsl/pointers>
#include <vector>
#include <Windows.h>
#include <shlwapi.h>

export module Saigon.Request;

import Saigon.Observer;
import Saigon.FileNotifyInfo;

#pragma comment(lib, "Shlwapi")

namespace saigon::observation
{
	class request
	{
	public:
		[[nodiscard]] observer* getObserver() const;

		// The dwSize is the actual number of bytes sent to the APC.
		void backupBuffer(DWORD dwSize);
		BOOL beginRead();
		void processNotification();

	private:
		static VOID CALLBACK notificationCompletion(
			DWORD dwErrorCode,							// completion code
			DWORD dwNumberOfBytesTransfered,			// number of bytes transferred
			LPOVERLAPPED lpOverlapped);					// I/O information buffer

	private:
		gsl::not_null<observer*> mObserver;

		// Parameters from the caller for ReadDirectoryChangesW().
		DWORD			mFilterFlags;
		BOOL			mIncludeChildren;
		std::wstring	mDirectory;

		// Result of calling CreateFile().
		HANDLE		mHdlDirectory;

		// Required parameter for ReadDirectoryChangesW().
		OVERLAPPED	mOverlapped;

		// Data buffer for the request.
		// Since the memory is allocated by malloc, it will always
		// be aligned as required by ReadDirectoryChangesW().
		std::vector<BYTE> mBuffer;

		// Double buffer strategy so that we can issue a new read
		// request before we process the current buffer.
		std::vector<BYTE> mBackupBuffer;
	};

	observer* request::getObserver() const
	{
		_ASSERT_EXPR(mObserver, "Observer is null");
		return mObserver;
	}

	void request::backupBuffer(DWORD dwSize)
	{
		// We could just swap back and forth between the two
		// buffers, but this code is easier to understand and debug.
		memcpy(&mBackupBuffer[0], &mBuffer[0], dwSize);
	}

	BOOL request::beginRead()
	{
		DWORD dwBytes = 0;

		// This call needs to be reissued after every APC.
		return ::ReadDirectoryChangesW(
			mHdlDirectory,						// handle to directory
			&mBuffer[0],						// read results buffer
			(DWORD) mBuffer.size(),						// length of buffer
			mIncludeChildren,					// monitoring option
			mFilterFlags,						// filter conditions
			&dwBytes,                           // bytes returned
			&mOverlapped,						// overlapped buffer
			&notificationCompletion);           // completion routine
	}

	void request::processNotification()
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
				//mMonitorServer->getDirectoryMonitor()->push(std::move(info));
			//}

			if (!fni.NextEntryOffset) {
				break;
			}
			pBase += fni.NextEntryOffset;
		}
	}

	VOID CALLBACK request::notificationCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
	{
		request* pBlock = (request*)lpOverlapped->hEvent;

		if (dwErrorCode == ERROR_OPERATION_ABORTED)
		{
			pBlock->getObserver()->decRequest();
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

		pBlock->backupBuffer(dwNumberOfBytesTransfered);

		// Get the new read issued as fast as possible. The documentation
		// says that the original OVERLAPPED structure will not be used
		// again once the completion routine is called.
		pBlock->beginRead();

		pBlock->processNotification();
	}
}