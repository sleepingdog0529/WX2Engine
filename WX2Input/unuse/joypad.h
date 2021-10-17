/*********************************************************************
 * @file	gamepad.h
 * @author	Tomomi Murakami
 * @date	2021/10/13 21:59
 * @brief	ゲームパッド入力
*********************************************************************/
#pragma once
#pragma warning(push, 0)
#include <dinput.h>
#include <wrl/client.h>
#pragma warning(pop)

namespace wx2
{
	// 左右トリガーを同時に扱えないので没！
	class Joypad
	{
	private:
		using DInputPtr = Microsoft::WRL::ComPtr<IDirectInput8>;
		using DevicePtr = Microsoft::WRL::ComPtr<IDirectInputDevice8>;

		static constexpr size_t NUM_BUTTONS_ = 20;	// ボタン数
		static constexpr size_t NUM_AXISES_ = 6;	// 軸数
		static constexpr size_t MAX_JOYPAD_ = 4;	// ジョイパッドの最大認識数
		static constexpr uint16_t STICK_MAX_MAGNITUDE_ = 10000;	// 軸の最大値
		static constexpr uint16_t STICK_DEADZONE_ = 1000;	// 軸のデッドゾーン(0 ~ 10000)
		static constexpr uint16_t STICK_SATURATION_ = 10000;	// 軸の飽和ゾーン(0 ~ 10000)

		struct JoypadInfo
		{
			DevicePtr device;
			DIDEVICEINSTANCE instance;
			DIDEVCAPS capability;
			DIJOYSTATE2 state;
		};

	public:
		enum ButtonCode : uint8_t
		{
			A = 0,
			B,
			X,
			Y,
			LShoulder,
			RShoulder,
			Start,
			Back,
			LThumb,
			RThumb,
			Button11,
			Button12,
			Button13,
			Button14,
			Button15,
			Button16,
			DPadUp,
			DPadDown,
			DPadLeft,
			DPadRight
		};

		enum AxisCode : uint8_t
		{
			LTrigger = 0,
			RTrigger,
			LStickX,
			LStickY,
			RStickX,
			RStickY
		};

		Joypad();
		virtual ~Joypad();

		void Initialize(DInputPtr directInput, HWND hwnd);
		void Regist();
		void Update();

		static BOOL CALLBACK SetupJoypadCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
		static BOOL CALLBACK SetupAxisCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

	private:
		DInputPtr directInput_;
		HWND hwnd_;

		std::vector<JoypadInfo> joypads_;
	};
}