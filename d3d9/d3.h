#pragma once

#include <d3d9.h>

class Direct3D9Proxy : public IDirect3D9
{
private:
	IDirect3D9* m_pD3D;

public:
	Direct3D9Proxy(IDirect3D9 * pD3D);

	/*** IUnknown methods ***/
	HRESULT STDMETHODCALLTYPE  QueryInterface(REFIID riid, void ** ppvObj);
	ULONG STDMETHODCALLTYPE    AddRef();
	ULONG STDMETHODCALLTYPE    Release();

	/*** IDirect3D9 methods ***/
	HRESULT STDMETHODCALLTYPE  RegisterSoftwareDevice(void * pInitializeFunction);
	UINT STDMETHODCALLTYPE     GetAdapterCount();
	HRESULT STDMETHODCALLTYPE  GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9 * pIdentifier);
	UINT STDMETHODCALLTYPE     GetAdapterModeCount(UINT Adapter, D3DFORMAT Format);
	HRESULT STDMETHODCALLTYPE  EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode);
	HRESULT STDMETHODCALLTYPE  GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode);
	HRESULT STDMETHODCALLTYPE  CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed);
	HRESULT STDMETHODCALLTYPE  CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
	HRESULT STDMETHODCALLTYPE  CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD * pQualityLevels);
	HRESULT STDMETHODCALLTYPE  CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
	HRESULT STDMETHODCALLTYPE  CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat);
	HRESULT STDMETHODCALLTYPE  GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps);
	HMONITOR STDMETHODCALLTYPE GetAdapterMonitor(UINT Adapter);
	HRESULT STDMETHODCALLTYPE  CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS * pPresentationParameters, IDirect3DDevice9 ** ppReturnedDeviceInterface);
};
