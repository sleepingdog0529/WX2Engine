#include "joypad.h"
#include <ranges>

namespace wx2
{
	Joypad::Joypad() :
		directInput_(),
		hwnd_()
	{
	}

	Joypad::~Joypad()
	{
		for (auto& jp : joypads_)
		{
			if (jp.device)
			{
				jp.device->Unacquire();
			}
		}
	}

	void Joypad::Initialize(DInputPtr directInput, HWND hwnd)
	{
		WX2_ASSERT_MSG(directInput, "IDirectInputDevice8��nullptr�ł��B");
		WX2_ASSERT_MSG(IsWindowEnabled(hwnd) != 0, "�E�B���h�E�n���h���������ł��B");
		directInput_ = directInput;
		hwnd_ = hwnd;
	}

	void Joypad::Regist()
	{
		HRESULT hr;

		hr = directInput_->EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			SetupJoypadCallback,
			this,
			DIEDFL_ATTACHEDONLY);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�W���C�X�e�B�b�N�f�o�C�X�̍쐬�Ɏ��s���܂����B");
			exit(EXIT_FAILURE);
		}
	}

	void Joypad::Update()
	{
		HRESULT hr;

		for (auto& jd : joypads_)
		{
			hr = jd.device->Poll();
			if (FAILED(hr))
			{
				// �ؒf����Ă����ꍇ�ēo�^�����݂�
				hr = jd.device->Acquire();
				while (hr == DIERR_INPUTLOST)
				{
					hr = jd.device->Acquire();
				}

				std::memset(&jd.state, 0, sizeof(jd.state));
				continue;
			}

			hr = jd.device->GetDeviceState(sizeof(jd.state), &jd.state);
			if (FAILED(hr))
			{
				WX2_LOG_WARN("�W���C�X�e�B�b�N��Ԏ擾�Ɏ��s���܂����B");
				continue;
			}
		}
	}

	BOOL Joypad::SetupJoypadCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
	{
		WX2_ASSERT_MSG(
			GET_DIDEVICE_TYPE(lpddi->dwDevType) == DI8DEVTYPE_GAMEPAD ||
			GET_DIDEVICE_TYPE(lpddi->dwDevType) == DI8DEVTYPE_JOYSTICK,
			"�W���C�p�b�h�ƈقȂ�^�C�v�̃f�o�C�X�͗񋓂ł��܂���");

		auto* joypad = reinterpret_cast<Joypad*>(pvRef);
		WX2_ASSERT_MSG(joypad, "Gamepad�N���X��nullptr�ł��B");

		// �f�o�C�X�쐬�ς݂̏ꍇ�X�L�b�v����
		if (std::ranges::any_of(joypad->joypads_,
			[&lpddi](const JoypadInfo& di) { return di.instance.guidInstance == lpddi->guidInstance; }))
		{
			return DIENUM_CONTINUE;
		}

		HRESULT hr;
		JoypadInfo joypadInfo = {};
		joypadInfo.instance = std::move(static_cast<DIDEVICEINSTANCE>(*lpddi));

		// �f�o�C�X�쐬
		hr = joypad->directInput_->CreateDevice(
			joypadInfo.instance.guidInstance,
			joypadInfo.device.GetAddressOf(),
			nullptr);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�W���C�p�b�h�f�o�C�X�̍쐬�Ɏ��s���܂����B���i��\"{}\" ���\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		joypadInfo.capability.dwSize = sizeof(joypadInfo.capability);
		hr = joypadInfo.device->GetCapabilities(&joypadInfo.capability);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�W���C�p�b�h�f�o�C�X�̋@�\���擾�Ɏ��s���܂����B���i��\"{}\" ���\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		hr = joypadInfo.device->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�W���C�X�e�B�b�N�f�o�C�X�̃t�H�[�}�b�g�̐ݒ�Ɏ��s���܂����B���i��\"{}\" ���\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		hr = joypadInfo.device->SetCooperativeLevel(joypad->hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�W���C�p�b�h�f�o�C�X�̋������[�h�ݒ�Ɏ��s���܂����B���i��\"{}\" ���\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// ��Ύ��ɐݒ�
		DIPROPDWORD diprop = {};
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.diph.dwObj = 0;
		diprop.dwData = 1;
		hr = joypadInfo.device->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�W���C�p�b�h�f�o�C�X�̃o�b�t�@�T�C�Y�ݒ�Ɏ��s���܂����B���i��\"{}\" ���\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		hr = joypadInfo.device->EnumObjects(SetupAxisCallback, joypadInfo.device.Get(), DIDFT_AXIS);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�W���C�p�b�h�f�o�C�X�̎��ݒ�Ɏ��s���܂����B���i��\"{}\" ���\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		hr = joypadInfo.device->Acquire();
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�W���C�p�b�h�f�o�C�X�̐���J�n�Ɏ��s���܂����B���i��\"{}\" ���\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		hr = joypadInfo.device->Poll();
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�W���C�p�b�h�f�o�C�X�̃|�[�����O�J�n�Ɏ��s���܂����B���i��\"{}\" ���\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		joypad->joypads_.push_back(std::move(joypadInfo));

		// �W���C�p�b�h�̍ő吔�ɓ��B������񋓂��I������
		if (joypad->joypads_.size() >= MAX_JOYPAD_)
		{
			return DIENUM_STOP;
		}

		return DIENUM_CONTINUE;
	}

	BOOL CALLBACK Joypad::SetupAxisCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
	{
		auto* device = reinterpret_cast<IDirectInputDevice8*>(pvRef);
		WX2_ASSERT_MSG(device, "�W���C�p�b�h�f�o�C�X��nullptr�ł��B");

		HRESULT hr;

		// ���͈͂�ݒ�
		DIPROPRANGE diprg = {};
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = lpddoi->dwType;
		diprg.diph.dwHow = DIPH_BYID;
		diprg.lMin = -STICK_MAX_MAGNITUDE_;
		diprg.lMax = STICK_MAX_MAGNITUDE_;
		hr = device->SetProperty(DIPROP_RANGE, &diprg.diph);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�W���C�p�b�h�f�o�C�X�̎��͈̔͐ݒ�Ɏ��s���܂����B");
			return DIENUM_CONTINUE;
		}

		// ��Ύ��ɐݒ�
		DIPROPDWORD dropdw = {};
		dropdw.diph.dwSize = sizeof(dropdw);
		dropdw.diph.dwHeaderSize = sizeof(dropdw.diph);
		dropdw.diph.dwHow = DIPH_DEVICE;
		dropdw.dwData = DIPROPAXISMODE_ABS;
		hr = device->SetProperty(DIPROP_AXISMODE, &dropdw.diph);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�W���C�p�b�h�f�o�C�X�̎��̃��[�h�ݒ�Ɏ��s���܂����B");
			return DIENUM_CONTINUE;
		}

		// �f�b�h�]�[����ݒ�
		dropdw.dwData = STICK_DEADZONE_;
		hr = device->SetProperty(DIPROP_DEADZONE, &dropdw.diph);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�W���C�p�b�h�f�o�C�X�̎��̃f�b�h�]�[���ݒ�Ɏ��s���܂����B");
			return DIENUM_CONTINUE;
		}

		// �O�a�]�[����ݒ�
		dropdw.dwData = STICK_SATURATION_;
		hr = device->SetProperty(DIPROP_SATURATION, &dropdw.diph);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�W���C�p�b�h�f�o�C�X�̎��̖O�a�]�[���ݒ�Ɏ��s���܂����B");
			return DIENUM_CONTINUE;
		}

		return DIENUM_CONTINUE;
	}
}