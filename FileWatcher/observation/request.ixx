module;

#include <gsl/pointers>
#include <vector>
#include <string>
#include <Windows.h>

export module Saigon.Request;

import Saigon.IObserver;

namespace saigon::observation
{
	export class request
	{
	public:
		request(iobserver* obs,
			std::wstring_view directory,
			DWORD filterFlags,
			BOOL watchSubtree = TRUE,
			DWORD size = 16384
		);
		~request() noexcept;

		request(request const&) = delete;
		request& operator=(request const&) = delete;

		[[nodiscard]] iobserver* get_observer() const;

		// The dwSize is the actual number of bytes sent to the APC.
		void request_termination();
		[[nodiscard]] BOOL open_directory();
		void backup_buffer(DWORD dwSize);
		BOOL begin_read();
		void process_notification();

	private:
		static VOID CALLBACK notification_completion(
			DWORD dwErrorCode,							// completion code
			DWORD dwNumberOfBytesTransfered,			// number of bytes transferred
			LPOVERLAPPED lpOverlapped);					// I/O information buffer

	private:
		gsl::not_null<iobserver*> mObserver;

		// Parameters from the caller for ReadDirectoryChangesW().
		DWORD			mFilterFlags;
		BOOL			mIncludeChildren;
		std::wstring	mDirectory;

		// Result of calling CreateFile().
		HANDLE		mHdlDirectory{ INVALID_HANDLE_VALUE };

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
}