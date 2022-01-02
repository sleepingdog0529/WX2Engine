#include "mouse.h"
#include "input.h"

namespace wx2
{
	Mouse::~Mouse() noexcept
	{
		// �o�^���ꂽ�S�Ẵ}�E�X�f�o�C�X����� 
		for (auto& [device, instance, capability] : mouses_)
		{
			if (device)
			{
				device->Unacquire();
			}
		}
	}

	void Mouse::Initialize(const DInputPtr& directInput, const HWND hwnd) noexcept
	{
		WX2_ASSERT_MSG(directInput, "IDirectInputDevice8��nullptr�ł��B");
		WX2_ASSERT_MSG(IsWindowEnabled(hwnd) != 0, "�E�B���h�E�n���h���������ł��B");

		directInput_ = directInput;
		hwnd_ = hwnd;
	}

	void Mouse::Regist()
	{
		// ���ݐڑ����̃f�o�C�X��񋓂��ēo�^
		const HRESULT hr = directInput_->EnumDevices(
			DI8DEVTYPE_MOUSE,
			SetupMouseCallback,
			this,
			DIEDFL_ATTACHEDONLY);
		WX2_COM_ERROR_IF_FAILED(hr, "�}�E�X�f�o�C�X�̍쐬�Ɏ��s���܂����B");
	}

	void Mouse::Update() noexcept
	{
		// �O�t���[���̏����X�V
		state_.previous = state_.current;
		state_.current = {};
		state_.axises = {};

		// �S�Ẵ}�E�X�̏�Ԃ��擾
		for (const auto& [device, instance, capability] : mouses_)
		{
			HRESULT hr = device->Poll();
			if (FAILED(hr))
			{
				// �ؒf����Ă����ꍇ�ēo�^�����݂�
				device->Acquire();
				continue;
			}

			// �}�E�X��Ԏ擾
			hr = device->GetDeviceState(static_cast<DWORD>(sizeof(stateBuffer_)), &stateBuffer_);
			if (FAILED(hr))
			{
				WX2_LOG_WARN("�}�E�X��Ԏ擾�Ɏ��s���܂����B");
				continue;
			}

			// �{�^�������i�[
			for (std::size_t i = 0; i < NUM_BUTTONS; ++i)
			{
				state_.current.buttons[i] = state_.current.buttons[i] || (stateBuffer_.rgbButtons[i] & 0x80);
			}

			// �������i�[
			state_.axises[CursorX] += static_cast<float>(stateBuffer_.lX);
			state_.axises[CursorY] += static_cast<float>(stateBuffer_.lY);
			state_.axises[WheellScroll] += static_cast<float>(stateBuffer_.lZ) / static_cast<float>(WHEEL_DELTA);
		}
	}

	BOOL Mouse::SetupMouseCallback(LPCDIDEVICEINSTANCE lpddi, const LPVOID pvRef) noexcept
	{
		WX2_ASSERT_MSG(
			GET_DIDEVICE_TYPE(lpddi->dwDevType) == DI8DEVTYPE_MOUSE,
			"�}�E�X�ȊO�̃f�o�C�X�͗񋓂ł��܂���");

		auto* mouse = static_cast<Mouse*>(pvRef);
		WX2_ASSERT_MSG(mouse, "Mouse�N���X��nullptr�ł��B");

		// �f�o�C�X�쐬�ς݂̏ꍇ�X�L�b�v����
		if (std::ranges::any_of(mouse->mouses_,
			[&lpddi](const MouseDevice& md) { return md.instance.guidInstance == lpddi->guidInstance; }))
		{
			return DIENUM_CONTINUE;
		}

		// �}�E�X�̃f�o�C�X���
		MouseDevice mouseDev = {};
		mouseDev.instance = static_cast<DIDEVICEINSTANCE>(*lpddi);

		// �}�E�X�̃f�o�C�X�쐬
		HRESULT hr = mouse->directInput_->CreateDevice(
			mouseDev.instance.guidInstance,
			mouseDev.device.GetAddressOf(),
			nullptr);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�}�E�X�f�o�C�X�̍쐬�Ɏ��s���܂����B���i��: {} ���: {:X}",
				mouseDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(mouseDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// �@�\�����擾
		mouseDev.capability.dwSize = static_cast<DWORD>(sizeof(mouseDev.capability));
		hr = mouseDev.device->GetCapabilities(&mouseDev.capability);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�}�E�X�f�o�C�X�̋@�\���擾�Ɏ��s���܂����B���i��: {} ���: {:X}",
				mouseDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(mouseDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// �f�[�^�t�H�[�}�b�g�ݒ�
		hr = mouseDev.device->SetDataFormat(&c_dfDIMouse2);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�}�E�X�f�o�C�X�̃t�H�[�}�b�g�̐ݒ�Ɏ��s���܂����B���i��: {} ���: {:X}",
				mouseDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(mouseDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// �������[�h�ݒ�
		hr = mouseDev.device->SetCooperativeLevel(mouse->hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�}�E�X�f�o�C�X�̋������[�h�ݒ�Ɏ��s���܂����B���i��: {} ���: {:X}",
				mouseDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(mouseDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// ���Βl���[�h�ɐݒ�
		DIPROPDWORD diprop;
		diprop.diph.dwSize = static_cast<DWORD>(sizeof(diprop));
		diprop.diph.dwHeaderSize = static_cast<DWORD>(sizeof(diprop.diph));
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;
		hr = mouseDev.device->SetProperty(DIPROP_AXISMODE, &diprop.diph);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�}�E�X�f�o�C�X�̎����[�h�ݒ�Ɏ��s���܂����B");
			return DIENUM_CONTINUE;
		}

		// �o�^�ς݃}�E�X�f�o�C�X���X�g�ɒǉ�
		mouse->mouses_.push_back(std::move(mouseDev));

		return DIENUM_CONTINUE;
	}
}