#include "mouse.h"
#include "input.h"

namespace wx2
{
	void Mouse::Initialize(HWND hwnd)
	{
		auto* direct_input = Input::GetDirectInput();
		WX2_ASSERT_MSG(direct_input, "Input�N���X������������Ă��܂���B");

		HRESULT hr;

		hr = direct_input->CreateDevice(GUID_SysMouse, device_.GetAddressOf(), nullptr);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�}�E�X�f�o�C�X�̍쐬�Ɏ��s���܂����B");
			exit(EXIT_FAILURE);
		}

		hr = device_->SetDataFormat(&c_dfDIMouse2);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�}�E�X�f�o�C�X�̃t�H�[�}�b�g�ݒ�Ɏ��s���܂����B");
			exit(EXIT_FAILURE);
		}

		hr = device_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�}�E�X�f�o�C�X�̋������[�h�ݒ�Ɏ��s���܂����B");
			exit(EXIT_FAILURE);
		}

		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;
		hr = device_->SetProperty(DIPROP_AXISMODE, &diprop.diph);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�}�E�X�f�o�C�X�̎����[�h�ݒ�Ɏ��s���܂����B");
			exit(EXIT_FAILURE);
		}

		hr = device_->Acquire();
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("�}�E�X�f�o�C�X�̐���J�n�Ɏ��s���܂����B");
			exit(EXIT_FAILURE);
		}

		StateReset();
	}

	void Mouse::Finalize()
	{
		device_->Unacquire();
	}

	void Mouse::Update()
	{
		buttons_[1] = std::move(buttons_[0]);

		if (FAILED(device_->GetDeviceState(sizeof(state_), &state_)))
		{
			// �ؒf����Ă����ꍇ�ēo�^�����݂�
			if (FAILED(device_->Acquire()) ||
				FAILED(device_->GetDeviceState(sizeof(state_), &state_)))
			{
				StateReset();
				return;
			}
		}

		for (int i = 0; i < NUM_BUTTONS; ++i)
		{
			buttons_[0][i] = state_.rgbButtons[i] & 0x80;
		}

		velocity_.x() = static_cast<float>(state_.lX) * CURSOR_SENSITIVITY_;
		velocity_.y() = static_cast<float>(state_.lY) * -CURSOR_SENSITIVITY_;

		scroll_ = static_cast<float>(Sign(state_.lZ)) * SCOLL_SENSITIVITY_;
	}

	void Mouse::StateReset()
	{
		std::memset(&buttons_, 0, sizeof(buttons_));
	}
}