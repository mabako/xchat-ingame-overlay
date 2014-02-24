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

public:
	Chatbox()
	{
		font = nullptr;

		messages.push_back("irc overlay");
		messages.push_back("welcoem");
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
			rect.left = 15;
			rect.right = 800;
			rect.top = -5;
			rect.bottom = 10;

			r.Poll(this);

			for (const std::string message : messages)
			{
				rect.top += 15;
				rect.bottom += 15;

				font->DrawText(NULL, message.c_str(), -1, &rect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
	}
};
