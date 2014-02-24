#pragma once

#include "common.h"
#include "chat.h"
#include <Windows.h>
#include <map>

extern Chatbox chat;

class Window
{
private:
	HWND hwnd;
	WNDPROC wndProc;

	static std::map<HWND, Window*> windows;

public:
	Window(HWND hwnd_)
	{
		hwnd = hwnd_;
		windows[hwnd] = this;

		/* store the old window proc */
		wndProc = (WNDPROC) GetWindowLongPtr(hwnd, GWLP_WNDPROC);

		/* set our own window proc to handle things from here on out */
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)Hook);

		chat.OnMessage(std::string("debug"), std::string("hi"));
	}

	~Window()
	{
		/* restore the window proc */
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)wndProc);

		/* and remove the self reference */
		windows.erase(hwnd);

		chat.OnMessage(std::string("debug"), std::string("bye"));
	}


	static LRESULT WINAPI Hook(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if(msg == WM_CHAR)
		{
			wchar_t wc = (wchar_t)wParam;
			if (wc < 128)
			{
				char c = (char)wParam;
				if (chat.OnChar(c))
					return TRUE;
			}
		}
		return CallWindowProcW(windows[hwnd]->wndProc, hwnd, msg, wParam, lParam);
	}
};