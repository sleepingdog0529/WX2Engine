#include "input.h"
#include <algorithm>
#include <iostream>

namespace wx2
{
	BOOL EnumDeviceCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);

	Input::Input(HWND hwnd) :
		hwnd_(hwnd)
	{
		HRESULT hr;

		hr = (DirectInput8Create(
			GetModuleHandle(nullptr),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			&directInput_,
			nullptr));
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("DirectInput8�̍쐬�Ɏ��s���܂����B");
			exit(EXIT_FAILURE);
		}
	}

	void Input::AcquireDevices()
	{
		HRESULT hr;

		hr = directInput_->EnumDevices(
			DI8DEVCLASS_ALL,
			EnumDeviceCallBack,
			&devices_,
			DIEDFL_ATTACHEDONLY);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�f�o�C�X�̗񋓂Ɏ��s���܂����B");
			exit(EXIT_FAILURE);
		}
	}

	BOOL CALLBACK EnumDeviceCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
	{
		auto* const devices = reinterpret_cast<std::vector<DIDEVICEINSTANCE>*>(pvRef);

		const bool contains = std::any_of(
			devices->begin(), devices->end(),
			[&ipddi](const auto& dev) { return ipddi->guidInstance == dev.guidInstance; });

		if (GET_DIDEVICE_TYPE(ipddi->dwDevType) == DI8DEVTYPE_JOYSTICK)
		{
			WX2_LOG_INFO("���̓f�o�C�X���o ���i��\"{}\"", ipddi->tszProductName);
		}

		if (!contains)
		{
			devices->push_back(std::move(*ipddi));
		}

		//if (res)
		//{
		//	auto device = devices_.emplace_back();
		//	HRESULT hr;

		//	hr = directInput_->CreateDevice(
		//		ipddi->guidInstance,
		//		device.GetAddressOf(),
		//		nullptr);
		//	if (FAILED(hr))
		//	{
		//		WX2_LOG_WARN("���̓f�o�C�X�̍쐬�Ɏ��s���܂����B���i��:\"{}\"", ipddi->tszProductName);
		//		return DIENUM_CONTINUE;
		//	}

		//	hr = device->SetCooperativeLevel(hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		//	if (FAILED(hr))
		//	{
		//		WX2_LOG_WARN("���̓f�o�C�X�̗D�挠�ݒ�Ɏ��s���܂����B���i��:\"{}\"", ipddi->tszProductName);
		//		return DIENUM_CONTINUE;
		//	}
		//}

		return DIENUM_CONTINUE;
	}
}