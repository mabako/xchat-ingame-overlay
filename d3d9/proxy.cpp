#include "../common.h"
#include "../receiver.h"

#include "../vendor/minhook/include/MinHook.h"
#ifdef _WIN64
	#pragma comment(lib, "../vendor/minhook/lib/libMinHook.x64.lib")
#else
	#pragma comment(lib, "../vendor/minhook/lib/libMinHook.x86.lib")
#endif

#include <d3d9.h>
#include <dinput.h>
#include <Windows.h>
#include <assert.h>

#include "d3.h"
#include "../dinput8/di8.h"

/* dinput8 has no explicit typedef like dx has */
typedef HRESULT(WINAPI *DirectInput8Create_TYPE)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
DirectInput8Create_TYPE DirectInput8Create_fn = nullptr;

/* d3d9.dll */
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

HRESULT WINAPI DirectInput8Create_Hook(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
	// Call the real 'DirectInput8Create'
	HRESULT hr = DirectInput8Create_fn(hinst, dwVersion, riidltf, ppvOut, punkOuter);

	// If the call succeeded create our proxy device
	if (SUCCEEDED(hr))
	{
		IDirectInput8 * pDInput = (IDirectInput8 *)*ppvOut;
		*ppvOut = new DirectInput8Proxy(pDInput);
	}

	return hr;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		MH_Initialize();

		if (GetModuleHandle("dinput8.dll") == NULL)
			LoadLibrary("dinput8.dll");

		MH_CreateHook(GetProcAddress(GetModuleHandle("dinput8.dll"), "DirectInput8Create"), &DirectInput8Create_Hook, reinterpret_cast<void**>(&DirectInput8Create_fn));

		MH_EnableHook(MH_ALL_HOOKS);
	}
		break;

	case DLL_PROCESS_DETACH:
		ExitInstance();

		MH_Uninitialize();

		break;
	}
	return TRUE;
}
