#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <string>

#include "../common.h"
#include "../receiver.h"

int main() {
	Receiver r;
	r.Push(std::string("#playground"), std::string("hmm."));
#if 0
	HWND hWnd;
	{
		/* open the existing mapping file */
		HANDLE hMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, HWND_MAPPING_NAME);
		if (hMapping == nullptr)
		{
			/* server prolly not running */
			printf("Couldn't open file map.");
			getchar();
			return 1;
		}

		/* get the window handle */
		HWND* pFileMap = (HWND*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, sizeof(HWND));
		hWnd = *pFileMap;

		/* close it all */
		UnmapViewOfFile(pFileMap);
		CloseHandle(hMapping);

		if (hWnd == nullptr)
		{
			printf("no window.");
			getchar();
			return 3;
		}
	}

	{
		/* poll from xchat */
		HANDLE hPollMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, POLL_MAPPING_NAME);
		if (hPollMapping == nullptr)
		{
			/* server prolly not running? wat. */
			printf("Couldn't open poll map.");
			getchar();
			return 2;
		}

		void* pPollMap = MapViewOfFile(hPollMapping, FILE_MAP_READ, 0, 0, 0);

		LRESULT polled = SendMessage(hWnd, WM_POLL, 0, 0);
		printf("polled %d messages", polled);

		for (int i = 0; i < polled; ++i)
		{
			SMSDataStruct message;
			memcpy(&message, (char*)pPollMap + i * sizeof(SMSDataStruct), sizeof(SMSDataStruct));

			printf("%d = %s: %s\n", i, message.user, message.message);
		}

		UnmapViewOfFile(pPollMap);
		CloseHandle(hPollMapping);
	}

	{
		/* push toxchat */
		HANDLE hPushMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, PUSH_MAPPING_NAME);
		if (hPushMapping == nullptr)
		{
			/* server prolly not running? wat. */
			printf("Couldn't open push map.");
			getchar();
			return 4;
		}

		char* pPushMap = (char*)MapViewOfFile(hPushMapping, FILE_MAP_WRITE, 0, 0, 0);
		strcpy_s(pPushMap, MAX_MESSAGE_LENGTH, "sup?");

		SendMessage(hWnd, WM_PUSH, 0, 0);

		UnmapViewOfFile(pPushMap);
		CloseHandle(hPushMapping);
	}
	getchar();
	return 0;
#endif
}
