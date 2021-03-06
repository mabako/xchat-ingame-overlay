#pragma once

#define WIN32_LEAN_AND_MEAN

#define WM_POLL (WM_USER + 10) /* someone in irc sends a message, gets polled by ingame overlay */
#define WM_PUSH (WM_USER + 11) /* ingame overlay sends chat message to this */

#define HWND_MAPPING_NAME "{xlay_hwnd}" /* name of the mapped file */

#define POLL_MAPPING_NAME "{xlay_poll}"
#define POLL_QUEUE_SIZE 10

#define PUSH_MAPPING_NAME "{xlay_push}"

#define DIRECTINPUT_VERSION 0x0800

#define MAX_CHANNEL_LENGTH 66
#define MAX_USER_LENGTH 66
#define MAX_MESSAGE_LENGTH 512

enum CHAT_ACTION
{
	ACTION_SAY = 0,
	ACTION_ME = 1
};

struct SMSDataStruct
{
	char channel[MAX_CHANNEL_LENGTH];
	char user[MAX_USER_LENGTH];
	char message[MAX_MESSAGE_LENGTH];
	char action;
};


