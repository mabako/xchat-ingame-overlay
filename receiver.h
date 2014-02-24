#pragma once

#include "common.h"
#include <string>
#include <Windows.h>

class Target
{
public:
	virtual void OnMessage(std::string& username, std::string& text) = 0;
};

class Receiver
{
public:
	Receiver()
	{
		hWnd = NULL;
	}

	~Receiver()
	{

	}

	/* Verifies hWnd points to a valid window handle. returns false otherwise. */
	bool CheckWindow()
	{
		if (IsWindow(hWnd))
			return true;

		/* open the existing mapping file */
		HANDLE hMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, HWND_MAPPING_NAME);
		if (hMapping == nullptr)
		{
			hWnd = NULL;
			return false;
		}

		/* get the window handle */
		HWND* pFileMap = (HWND*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, sizeof(HWND));
		hWnd = *pFileMap;

		/* close it all */
		UnmapViewOfFile(pFileMap);
		CloseHandle(hMapping);

		return hWnd != NULL;
	}

	bool Poll(Target* target)
	{
		if (!CheckWindow())
			return false;

		/* poll from xchat */
		HANDLE hPollMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, POLL_MAPPING_NAME);
		if (hPollMapping == nullptr)
			return false;

		void* pPollMap = MapViewOfFile(hPollMapping, FILE_MAP_READ, 0, 0, 0);

		LRESULT polled = SendMessage(hWnd, WM_POLL, 0, 0);
		for (LRESULT i = 0; i < polled; ++i)
		{
			SMSDataStruct message;
			memcpy(&message, (char*)pPollMap + i * sizeof(SMSDataStruct), sizeof(SMSDataStruct));

			target->OnMessage(std::string(message.user), std::string(message.message));
		}

		UnmapViewOfFile(pPollMap);
		CloseHandle(hPollMapping);

		return true;
	}

	bool Push(std::string& channel, std::string& text)
	{
		if (!CheckWindow())
			return false;

		/* push toxchat */
		HANDLE hPushMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, PUSH_MAPPING_NAME);
		if (hPushMapping == nullptr)
			return false;

		SMSDataStruct* pPushMap = (SMSDataStruct*)MapViewOfFile(hPushMapping, FILE_MAP_WRITE, 0, 0, 0);
		strcpy_s(pPushMap->user, MAX_USER_LENGTH, channel.c_str());
		strcpy_s(pPushMap->message, MAX_MESSAGE_LENGTH, text.c_str());

		LRESULT value = SendMessage(hWnd, WM_PUSH, 0, 0);

		UnmapViewOfFile(pPushMap);
		CloseHandle(hPushMapping);
		return value != 0;
	}

private:
	HWND hWnd;
};
