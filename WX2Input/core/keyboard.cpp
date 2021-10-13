#include "keyboard.h"
#include "input.h"

namespace wx2
{
	void Keyboard::Initialize(HWND hwnd)
	{
		auto* direct_input = Input::GetDirectInput();
		WX2_ASSERT_MSG(direct_input, "Inputクラスが初期化されていません。");

		HRESULT hr;

		hr = direct_input->CreateDevice(GUID_SysKeyboard, device_.GetAddressOf(), nullptr);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("キーボードデバイスの作成に失敗しました。");
			exit(EXIT_FAILURE);
		}

		hr = device_->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("キーボードデバイスのフォーマット設定に失敗しました。");
			exit(EXIT_FAILURE);
		}

		hr = device_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("キーボードデバイスの協調モード設定に失敗しました。");
			exit(EXIT_FAILURE);
		}

		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = 100;
		hr = device_->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("キーボードデバイスのバッファサイズ設定に失敗しました。");
			exit(EXIT_FAILURE);
		}

		hr = device_->Acquire();
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("キーボードデバイスの制御開始に失敗しました。");
			exit(EXIT_FAILURE);
		}

		StateReset();
	}

	void Keyboard::Finalize()
	{
		device_->Unacquire();
	}

	void Keyboard::Update()
	{
		if (FAILED(device_->GetDeviceState(sizeof(buffer_), &buffer_)))
		{
			// 切断されていた場合再登録を試みる
			if (FAILED(device_->Acquire()) ||
				FAILED(device_->GetDeviceState(sizeof(buffer_), &buffer_)))
			{
				StateReset();
				return;
			}
		}

		keys_[1] = std::move(keys_[0]);

		for (int i = 0; i < NUM_KEYS; ++i)
		{
			keys_[0][i] = buffer_[i] & 0x80;
		}
	}

	void Keyboard::StateReset()
	{
		std::memset(&keys_, 0, sizeof(keys_));
	}
}