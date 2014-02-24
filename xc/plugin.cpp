#include "../common.h"

#include <Windows.h>
#include <queue>

#include "xchat-plugin.h"

#define PNAME "overlay"
#define PDESC "ingame overlay helper"
#define PVERSION "0.1"

static xchat_plugin *ph; /* plugin handle */

/* Mapping hWnd */
HANDLE hWindowMapping;

/* Mapping polls */
HANDLE hPollMapping;
void* pPollMap;

/* Mapping pushs */
HANDLE hPushMapping;
SMSDataStruct* pPushMap;

/* window classes */
ATOM windowAtom;
HWND windowHandle;

/* all messages not yet polled */
std::queue<SMSDataStruct> pending;

/* readme: change this to be whatever channel name you actually want to be displayed ingame. */
bool message_in_watched_channel()
{
	const char *channelname = xchat_get_info(ph, "channel");
	return _stricmp(channelname, "#banished") == 0;
}

int handle_poll()
{
	/* asking for all messages currently pending */
	int i = 0;
	while (i < POLL_QUEUE_SIZE && !pending.empty())
	{
		SMSDataStruct todo = pending.front();
		pending.pop();

		memcpy((char*)pPollMap + i * sizeof(SMSDataStruct), &todo, sizeof(SMSDataStruct));
		++i;
	}

	return i;
}

int handle_push()
{
	/* send a new message to some channel. */
	xchat_context* oldctx = xchat_get_context(ph);

	/* find the target */
	xchat_context* ctx = xchat_find_context(ph, NULL, pPushMap->user);
	if (ctx == NULL)
	{
		xchat_print(ph, "Missing Context to write to.");
		return 0;
	}

	/* set the context */
	if (xchat_set_context(ph, ctx))
	{
		/* output a single one-liner */
		xchat_commandf(ph, "SAY %s", pPushMap->message);

		/* reset the context */
		xchat_set_context(ph, oldctx);

		return 1;
	}
	/* context switch failed */
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_POLL:
		return handle_poll();

	case WM_PUSH:
		return handle_push();

	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void xchat_plugin_get_info(char **name, char **desc, char **version, void **reserved)
{
  *name = PNAME;
  *desc = PDESC;
  *version = PVERSION;
}

/*
  word[1] -> nick
  word[2] -> message
  word[3] -> mode sign
*/
int message_cb(char* word[], void* userdata)
{
	if (message_in_watched_channel())
	{
		char* stripped = xchat_strip(ph, word[2], -1, 1 | 2);
		if (stripped)
		{
			/* add the message at the end of the queue */
			SMSDataStruct message;
			sprintf_s(message.user, "%s%s", word[3], word[1]);
			strcpy_s(message.message, stripped);

			pending.push(message);

			/* only 10 lines are displayed ingame, no point in storing any more. */
			while (pending.size() > POLL_QUEUE_SIZE)
				pending.pop();

			xchat_free(ph, stripped);
		}
	}
	return XCHAT_EAT_NONE;
}

int xchat_plugin_init(xchat_plugin *plugin_handle,
                      char **plugin_name,
                      char **plugin_desc,
                      char **plugin_version,
                      char *arg)
{
	/* we need to save this for use with any xchat_* functions */
	ph = plugin_handle;

	/* tell xchat our info */
	*plugin_name = PNAME;
	*plugin_desc = PDESC;
	*plugin_version = PVERSION;

	/* register a window class */
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = &WndProc;
	wc.lpszClassName = "xlay_host";

	windowAtom = RegisterClassEx(&wc);
	if (windowAtom == INVALID_ATOM)
	{
		xchat_print(ph, "Couldn't register window class.");
		return 0; /* fail */
	}

	/* create a window to receive messages */
	windowHandle = CreateWindowEx(WS_EX_CLIENTEDGE, MAKEINTATOM(windowAtom), "xlay_host", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 240, 120, NULL, NULL, NULL, NULL);

	/* publish our information about the current window somewhere */
	hWindowMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(HWND), HWND_MAPPING_NAME);
	HWND* pHWND = (HWND*)MapViewOfFile(hWindowMapping, FILE_MAP_WRITE, 0, 0, 0);
	*pHWND = windowHandle;

	/* file mapping isn't of interest now that we published our own windows' information */
	UnmapViewOfFile(pHWND);

	/* poll, i.e. save all messages */
	hPollMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, POLL_QUEUE_SIZE * sizeof(SMSDataStruct), POLL_MAPPING_NAME);
	pPollMap = MapViewOfFile(hPollMapping, FILE_MAP_WRITE, 0, 0, 0);

	memset(pPollMap, 0, POLL_QUEUE_SIZE * sizeof(SMSDataStruct));

	/* push, i.e. texting from ingame */
	hPushMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SMSDataStruct), PUSH_MAPPING_NAME);
	pPushMap = (SMSDataStruct*) MapViewOfFile(hPushMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

	memset(pPushMap, 0, MAX_MESSAGE_LENGTH);

	/* make sure we save messages */
	xchat_hook_print(ph, "Channel Message", XCHAT_PRI_NORM, message_cb, NULL);
	xchat_hook_print(ph, "Your Message", XCHAT_PRI_NORM, message_cb, NULL);

	/* just doing okay */
	xchat_print(ph, PNAME " loaded successfully!");

	return 1; /* success */
}

int xchat_plugin_deinit(xchat_plugin *plugin_handle)
{
	/* remove the poll mapping */
	UnmapViewOfFile(pPollMap);
	CloseHandle(hPollMapping);

	/* remove the push mapping */
	UnmapViewOfFile(pPushMap);
	CloseHandle(hPushMapping);

	/* remove reference to the window */
	CloseHandle(hWindowMapping);

	DestroyWindow(windowHandle);
	UnregisterClass(MAKEINTATOM(windowAtom), NULL);

	/* done */
	xchat_print(ph, PNAME " unloaded.");

	return 1;
}
