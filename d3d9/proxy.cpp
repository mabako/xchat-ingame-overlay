// proxydll.cpp
#include "../common.h"
#include "../receiver.h"

#include <d3d9.h>
#include <Windows.h>

#include "d3.h"

HINSTANCE hOriginalDll = NULL;

void LoadOriginalDll(void)
{
	char buffer[MAX_PATH];

	GetSystemDirectory(buffer, MAX_PATH);
	strcat_s(buffer, "\\d3d9.dll");

	// try to load the system's d3d9.dll, if pointer empty
	if (!hOriginalDll)
		hOriginalDll = LoadLibrary(buffer);

	// Debug
	if (!hOriginalDll)
	{
		ExitProcess(0); // exit the hard way
	}
}

// Exported function (faking d3d9.dll's one-and-only export)
IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion)
{
	if (!hOriginalDll)
		LoadOriginalDll();

	// Hooking IDirect3D Object from Original Library
	typedef IDirect3D9 *(WINAPI* D3D9_Type)(UINT SDKVersion);
	D3D9_Type D3DCreate9_fn = (D3D9_Type)GetProcAddress(hOriginalDll, "Direct3DCreate9");

	// Debug
	if (!D3DCreate9_fn)
	{
		ExitProcess(0); // exit the hard way
	}

	IDirect3D9 *pIDirect3D9_orig = D3DCreate9_fn(SDKVersion);

	return new Direct3D9Proxy(pIDirect3D9_orig);
}

void ExitInstance()
{
	// Release the system's d3d9.dll
	if (hOriginalDll)
	{
		FreeLibrary(hOriginalDll);
		hOriginalDll = NULL;
	}
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_DETACH:
		ExitInstance();
		break;
	}
	return TRUE;
}
