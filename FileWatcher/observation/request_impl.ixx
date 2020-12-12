module;

#include <gsl/pointers>
#include <vector>
#include <string>
#include <Windows.h>

export module Saigon.RequestImpl;

import Saigon.IRequest;

namespace saigon::observation
{
	export class request_impl : public irequest
	{
	public:
		request_impl(iobserver* obs,
			std::wstring_view directory,
			DWORD filterFlags,
			BOOL watchSubtree = TRUE,
			DWORD size = 16384
		);

		request_impl(request_impl const&) = delete;
		request_impl& operator=(request_impl const&) = delete;

		void process_notification();
		void backup_buffer(DWORD dwSize);

	private:
		bool do_open_directory() final;
		bool do_begin_read() final;
		void do_request_termination() final;
		iobserver* do_get_observer() const final;

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

		// Data buffer for the request_impl.
		// Since the memory is allocated by malloc, it will always
		// be aligned as required by ReadDirectoryChangesW().
		std::vector<BYTE> mBuffer;

		// Double buffer strategy so that we can issue a new read
		// request_impl before we process the current buffer.
		std::vector<BYTE> mBackupBuffer;
	};
}