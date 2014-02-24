#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <string>
#include <deque>

#pragma comment(lib, "d3dx9.lib")

#include "../receiver.h"

class Chatbox : public Target
{
private:
	ID3DXFont* font;

	Receiver r;

	std::deque<std::string> messages;
	bool active;

public:
	Chatbox()
	{
		font = nullptr;

		messages.push_back("irc overlay");
		messages.push_back("welcoem");

		active = false;
	}

	~Chatbox()
	{
		if (font)
		{
			font->Release();
			delete font;
		}
	}

	void OnCreateDevice(IDirect3DDevice9* pDevice)
	{
		if (!font)
			D3DXCreateFont(pDevice, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("HelveticaLTStd_Blk"), &font);
	}

	void OnLostDevice()
	{
		if (font)
			font->OnLostDevice();
	}

	void OnResetDevice()
	{
		if (font)
			font->OnResetDevice();
	}

	void OnMessage(std::string& username, std::string& message)
	{
		messages.push_back(username + ": " + message);
		if (messages.size() > POLL_QUEUE_SIZE)
			messages.pop_front();
	}

	void Render()
	{
		if (font && r.CheckWindow())
		{
			RECT rect;
			rect.left = 10;
			rect.right = 1920;
			rect.top = -5;
			rect.bottom = 10;

			r.Poll(this);

			for (const std::string& message : messages)
			{
				rect.top += 15;
				rect.bottom += 15;

				Draw(message.c_str(), rect);
			}
		}
	}

	bool IsActive()
	{
		return active;
	}

	void Toggle()
	{
		active = !active;
	}

private:
	/* Draws a message with a black border around it */
	void Draw(const char* message, RECT& rect)
	{
		rect.left--;
		font->DrawText(NULL, message, -1, &rect, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

		rect.left += 2;
		font->DrawText(NULL, message, -1, &rect, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

		rect.left--;
		rect.top--;
		font->DrawText(NULL, message, -1, &rect, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

		rect.top += 2;
		font->DrawText(NULL, message, -1, &rect, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
		rect.top--;

		font->DrawText(NULL, message, -1, &rect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

};
