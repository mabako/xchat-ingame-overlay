#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <string>
#include <deque>
#include <vector>

#pragma comment(lib, "d3dx9.lib")

#include "receiver.h"

class Chatbox : public Target
{
private:
	ID3DXFont* font;

	Receiver r;

	std::deque<std::string> messages;
	
	std::vector<std::string> channels;
	int active;

	std::string input;

public:
	Chatbox()
	{
		font = nullptr;

		channels.push_back("#banished");
		channels.push_back("#mabako");

		messages.push_back("IRC Overlay");

		active = -1;
	}

	~Chatbox()
	{
		OnDestroyDevice();
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

	void OnDestroyDevice()
	{
		if (font)
		{
			font->Release();
			delete font;
		}
	}

	void OnMessage(std::string& username, std::string& message)
	{
		messages.push_back(username + ": " + message);
		if (messages.size() > POLL_QUEUE_SIZE)
			messages.pop_front();
	}

	void Render()
	{
		if (font && GetActiveWindow() != NULL && r.CheckWindow())
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

			if (IsActive())
			{
				rect.top += 25;
				rect.bottom += 25;

				Draw((channels[active] + " > " + input + "_").c_str(), rect);
			}
		}
	}

	bool OnChar(char c)
	{
		if (c == VK_TAB && !IsActive())
		{
			Toggle();
			input.clear();
		}
		else if (IsActive())
		{
			switch (c)
			{
			case VK_TAB:
				++active;
				if (active == channels.size())
					active = -1;
				break;

			case VK_RETURN:
				if (input.size() > 0)
					r.Push(channels[active], input);

				/* fall through */
			case VK_ESCAPE:
				active = -1;
				input.clear();
				break;

			case VK_BACK:
				if (input.size() > 0)
				{
					input.erase(input.size() - 1);
				}
				break;

			default:
				if (input.size() < MAX_MESSAGE_LENGTH - 2 && c >= ' ')
				{
					input += c;
				}
				else
					return false;
			}
		}
		return true;
	}

	inline bool IsActive()
	{
		return active >= 0;
	}

	void Toggle()
	{
		if (active < 0)
			active = 0;
		else
			active = -1;
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
