#include "keyboard.h"
#include "input.h"

namespace wx2
{
	Keyboard::Keyboard() :
		directInput_(),
		hwnd_()
	{
	}

	Keyboard::~Keyboard()
	{
		for (auto& kd : keyboards_)
		{
			if (kd.device)
			{
				kd.device->Unacquire();
			}
		}
	}

	void Keyboard::Initialize(DInputPtr directInput, HWND hwnd)
	{
		WX2_ASSERT_MSG(directInput, "IDirectInputDevice8��nullptr�ł��B");
		WX2_ASSERT_MSG(IsWindowEnabled(hwnd) != 0, "�E�B���h�E�n���h���������ł��B");
		directInput_ = directInput;
		hwnd_ = hwnd;
	}

	void Keyboard::Regist()
	{
		HRESULT hr;

		hr = directInput_->EnumDevices(
			DI8DEVTYPE_KEYBOARD,
			SetupKeyboardCallback,
			this,
			DIEDFL_ATTACHEDONLY);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�W���C�X�e�B�b�N�f�o�C�X�̍쐬�Ɏ��s���܂����B");
			exit(EXIT_FAILURE);
		}
	}

	void Keyboard::Update()
	{
		HRESULT hr;

		state_.previous = std::move(state_.current);
		std::memset(&state_.current, 0, sizeof(state_.current));

		BYTE stateBuffer[NUM_KEYS_] = {};

		// �S�ẴL�[�{�[�h�̏�Ԃ��擾
		for (auto& kd : keyboards_)
		{
			hr = kd.device->Poll();
			if (FAILED(hr))
			{
				// �ؒf����Ă����ꍇ�ēo�^�����݂�
				kd.device->Acquire();
				continue;
			}

			std::memset(&stateBuffer, 0, sizeof(stateBuffer));

			// �L�[�{�[�h��Ԏ擾
			hr = kd.device->GetDeviceState(static_cast<DWORD>(sizeof(stateBuffer)), &stateBuffer);
			if (FAILED(hr))
			{
				WX2_LOG_WARN("�L�[�{�[�h��Ԏ擾�Ɏ��s���܂����B");
				continue;
			}

			// �L�[����i�[
			for (std::size_t i = 0; i < NUM_KEYS_; ++i)
			{
				state_.current.keys[i] = state_.current.keys[i] | (stateBuffer[i] & 0x80);
			}
		}
	}

	bool Keyboard::IsDown(Keys key) const
	{
		return state_.current.keys[key];
	}

	bool Keyboard::IsPressed(Keys key) const
	{
		return state_.current.keys[key] & !state_.previous.keys[key];
	}

	bool Keyboard::IsReleased(Keys key) const
	{
		return !state_.current.keys[key] & state_.previous.keys[key];
	}

	BOOL Keyboard::SetupKeyboardCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
	{
		WX2_ASSERT_MSG(
			GET_DIDEVICE_TYPE(lpddi->dwDevType) == DI8DEVTYPE_KEYBOARD,
			"�}�E�X�ȊO�̃f�o�C�X�͗񋓂ł��܂���");

		auto* keyboard = reinterpret_cast<Keyboard*>(pvRef);
		WX2_ASSERT_MSG(keyboard, "Keyboard�N���X��nullptr�ł��B");

		// �f�o�C�X�쐬�ς݂̏ꍇ�X�L�b�v����
		if (std::ranges::any_of(keyboard->keyboards_,
			[&lpddi](const KeyboardDevice& md) { return md.instance.guidInstance == lpddi->guidInstance; }))
		{
			return DIENUM_CONTINUE;
		}

		HRESULT hr;
		KeyboardDevice keyboardDev = {};
		keyboardDev.instance = std::move(static_cast<DIDEVICEINSTANCE>(*lpddi));

		// �f�o�C�X�쐬
		hr = keyboard->directInput_->CreateDevice(
			keyboardDev.instance.guidInstance,
			keyboardDev.device.GetAddressOf(),
			nullptr);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�L�[�{�[�h�f�o�C�X�̍쐬�Ɏ��s���܂����B���i��\"{}\" ���\"{:X}\"",
				keyboardDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(keyboardDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// �@�\�����擾
		keyboardDev.capability.dwSize = static_cast<DWORD>(sizeof(keyboardDev.capability));
		hr = keyboardDev.device->GetCapabilities(&keyboardDev.capability);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�L�[�{�[�h�f�o�C�X�̋@�\���擾�Ɏ��s���܂����B���i��\"{}\" ���\"{:X}\"",
				keyboardDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(keyboardDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// �f�[�^�t�H�[�}�b�g�ݒ�
		hr = keyboardDev.device->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�L�[�{�[�h�f�o�C�X�̃t�H�[�}�b�g�̐ݒ�Ɏ��s���܂����B���i��\"{}\" ���\"{:X}\"",
				keyboardDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(keyboardDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// �������[�h�ݒ�
		hr = keyboardDev.device->SetCooperativeLevel(keyboard->hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�L�[�{�[�h�f�o�C�X�̋������[�h�ݒ�Ɏ��s���܂����B���i��\"{}\" ���\"{:X}\"",
				keyboardDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(keyboardDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		keyboard->keyboards_.push_back(std::move(keyboardDev));

		return DIENUM_CONTINUE;
	}
}