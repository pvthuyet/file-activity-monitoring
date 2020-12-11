module;

#include <gsl/pointers>
#include <vector>
#include <string>
#include <Windows.h>

export module Saigon.Request;

namespace saigon::observation
{
	class observer;
	export class request
	{
	public:
		request(observer* obs,
			std::wstring_view directory,
			DWORD filterFlags,
			BOOL watchSubtree = TRUE,
			DWORD size = 16384
		);
		~request() noexcept;

		request(request const&) = delete;
		request& operator=(request const&) = delete;

		[[nodiscard]] observer* getObserver() const;

		// The dwSize is the actual number of bytes sent to the APC.
		void requestTermination();
		[[nodiscard]] BOOL openDirectory();
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