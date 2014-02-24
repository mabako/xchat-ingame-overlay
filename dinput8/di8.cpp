#include "di8.h"
#include "di8device.h"

DirectInput8Proxy::DirectInput8Proxy(IDirectInput8* pDInput)
{
	m_pDInput = pDInput;
}

HRESULT STDMETHODCALLTYPE DirectInput8Proxy::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	return m_pDInput->QueryInterface(riid, ppvObj);
}

ULONG STDMETHODCALLTYPE DirectInput8Proxy::AddRef()
{
	return m_pDInput->AddRef();
}

ULONG STDMETHODCALLTYPE DirectInput8Proxy::Release()
{
	ULONG uRet = m_pDInput->Release();

	// If the reference count is 0 delete ourselves
	if (uRet == 0)
		delete this;

	return uRet;
}

HRESULT STDMETHODCALLTYPE DirectInput8Proxy::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8A * lplpDirectInputDevice, LPUNKNOWN pUknOuter)
{
	// Create the dinput device
	HRESULT hr = m_pDInput->CreateDevice(rguid, lplpDirectInputDevice, pUknOuter);

	if (SUCCEEDED(hr))
	{
		*lplpDirectInputDevice = new DirectInputDevice8Proxy(this, *lplpDirectInputDevice, rguid);
	}

	return hr;
}

HRESULT STDMETHODCALLTYPE DirectInput8Proxy::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return m_pDInput->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
}

HRESULT STDMETHODCALLTYPE DirectInput8Proxy::GetDeviceStatus(REFGUID rguidInstance)
{
	return m_pDInput->GetDeviceStatus(rguidInstance);
}

HRESULT STDMETHODCALLTYPE DirectInput8Proxy::RunControlPanel(HWND hwndOwner, DWORD dwFlags)
{
	return m_pDInput->RunControlPanel(hwndOwner, dwFlags);
}

HRESULT STDMETHODCALLTYPE DirectInput8Proxy::Initialize(HINSTANCE hinst, DWORD dwVersion)
{
	return m_pDInput->Initialize(hinst, dwVersion);
}

HRESULT STDMETHODCALLTYPE DirectInput8Proxy::FindDevice(REFGUID rguidClass, LPCSTR ptszName, LPGUID pguidInstance)
{
	return m_pDInput->FindDevice(rguidClass, ptszName, pguidInstance);
}

HRESULT STDMETHODCALLTYPE DirectInput8Proxy::EnumDevicesBySemantics(LPCSTR ptszUserName, LPDIACTIONFORMATA lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBA lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return m_pDInput->EnumDevicesBySemantics(ptszUserName, lpdiActionFormat, lpCallback, pvRef, dwFlags);
}

HRESULT STDMETHODCALLTYPE DirectInput8Proxy::ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSA lpdiCDParams, DWORD dwFlags, LPVOID pvRefData)
{
	return m_pDInput->ConfigureDevices(lpdiCallback, lpdiCDParams, dwFlags, pvRefData);
}
