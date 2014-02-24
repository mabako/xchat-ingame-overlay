#include "di8device.h"
#include <dinput.h>

#include "../d3d9/chat.h"

extern Chatbox chat;

DirectInputDevice8Proxy::DirectInputDevice8Proxy(IDirectInput8* pDInput, IDirectInputDevice8 * pDIDevice, REFGUID rguid)
{
	m_pDInput = pDInput;
	m_pDIDevice = pDIDevice;

	if (rguid == GUID_SysMouse)
		m_deviceType = DEVICE_TYPE_MOUSE;
	else if (rguid == GUID_SysKeyboard)
		m_deviceType = DEVICE_TYPE_KEYBOARD;
	else
		m_deviceType = DEVICE_TYPE_JOYSTICK;
}

/*** IUnknown methods ***/
HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	return m_pDIDevice->QueryInterface(riid, ppvObj);
}

ULONG STDMETHODCALLTYPE DirectInputDevice8Proxy::AddRef()
{
	return m_pDIDevice->AddRef();
}

ULONG STDMETHODCALLTYPE DirectInputDevice8Proxy::Release()
{
	ULONG uRet = m_pDIDevice->Release();

	// If the reference count is 0 delete ourselves
	if (uRet == 0)
		delete this;

	return uRet;
}

/*** IDirectInputDevice8A methods ***/
HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::GetCapabilities(LPDIDEVCAPS lpDIDevCaps)
{
	return m_pDIDevice->GetCapabilities(lpDIDevCaps);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return m_pDIDevice->EnumObjects(lpCallback, pvRef, dwFlags);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph)
{
	return m_pDIDevice->GetProperty(rguidProp, pdiph);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph)
{
	return m_pDIDevice->SetProperty(rguidProp, pdiph);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::Acquire()
{
	return m_pDIDevice->Acquire();
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::Unacquire()
{
	return m_pDIDevice->Unacquire();
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::GetDeviceState(DWORD cbData, LPVOID lpvData)
{
	HRESULT hResult = m_pDIDevice->GetDeviceState(cbData, lpvData);

	// Clear the buffer if input is disabled or we have no focus
	if (chat.IsActive())
		memset(lpvData, 0, cbData);

	return hResult;
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
	HRESULT hResult = m_pDIDevice->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);

	// Clear the buffer if input is disabled or we have no focus
	if (chat.IsActive())
		memset(rgdod, 0, (cbObjectData * (*pdwInOut)));

	return hResult;
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::SetDataFormat(LPCDIDATAFORMAT p0)
{
	return m_pDIDevice->SetDataFormat(p0);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::SetEventNotification(HANDLE p0)
{
	return m_pDIDevice->SetEventNotification(p0);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::SetCooperativeLevel(HWND p0, DWORD p1)
{
	return m_pDIDevice->SetCooperativeLevel(p0, p1);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEA p0, DWORD p1, DWORD p2)
{
	return m_pDIDevice->GetObjectInfo(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::GetDeviceInfo(LPDIDEVICEINSTANCEA p0)
{
	return m_pDIDevice->GetDeviceInfo(p0);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::RunControlPanel(HWND p0, DWORD p1)
{
	return m_pDIDevice->RunControlPanel(p0, p1);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::Initialize(HINSTANCE p0, DWORD p1, REFGUID rguid)
{
	return m_pDIDevice->Initialize(p0, p1, rguid);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::CreateEffect(REFGUID rguid, LPCDIEFFECT p1, LPDIRECTINPUTEFFECT * p2, LPUNKNOWN p3)
{
	return m_pDIDevice->CreateEffect(rguid, p1, p2, p3);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::EnumEffects(LPDIENUMEFFECTSCALLBACKA p0, LPVOID p1, DWORD p2)
{
	return m_pDIDevice->EnumEffects(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::GetEffectInfo(LPDIEFFECTINFOA p0, REFGUID rguid)
{
	return m_pDIDevice->GetEffectInfo(p0, rguid);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::GetForceFeedbackState(LPDWORD pdwOut)
{
	return m_pDIDevice->GetForceFeedbackState(pdwOut);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::SendForceFeedbackCommand(DWORD dwFlags)
{
	return m_pDIDevice->SendForceFeedbackCommand(dwFlags);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl)
{
	return m_pDIDevice->EnumCreatedEffectObjects(lpCallback, pvRef, fl);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::Escape(LPDIEFFESCAPE pesc)
{
	return m_pDIDevice->Escape(pesc);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::Poll()
{
	return m_pDIDevice->Poll();
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl)
{
	return m_pDIDevice->SendDeviceData(cbObjectData, rgdod, pdwInOut, fl);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::EnumEffectsInFile(LPCSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags)
{
	return m_pDIDevice->EnumEffectsInFile(lpszFileName, pec, pvRef, dwFlags);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::WriteEffectToFile(LPCSTR lpszFileName, DWORD dwEntries, LPDIFILEEFFECT rgDiFileEft, DWORD dwFlags)
{
	return m_pDIDevice->WriteEffectToFile(lpszFileName, dwEntries, rgDiFileEft, dwFlags);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::BuildActionMap(LPDIACTIONFORMATA lpdiaf, LPCSTR lpszUserName, DWORD dwFlags)
{
	return m_pDIDevice->BuildActionMap(lpdiaf, lpszUserName, dwFlags);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::SetActionMap(LPDIACTIONFORMATA lpdiActionFormat, LPCSTR lptszUserName, DWORD dwFlags)
{
	return m_pDIDevice->SetActionMap(lpdiActionFormat, lptszUserName, dwFlags);
}

HRESULT STDMETHODCALLTYPE DirectInputDevice8Proxy::GetImageInfo(LPDIDEVICEIMAGEINFOHEADERA lpdiDevImageInfoHeader)
{
	return m_pDIDevice->GetImageInfo(lpdiDevImageInfoHeader);
}
