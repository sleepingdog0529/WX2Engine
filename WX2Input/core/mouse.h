/*********************************************************************
 * @file	mouse.h
 * @author	Tomomi Murakami
 * @date	2021/10/10 15:09
 * @brief	マウス入力
*********************************************************************/
#pragma once
#pragma warning(push, 0)
#include <WX2Common.h>
#include <bitset>
#include <dinput.h>
#include <wrl/client.h>
#pragma warning(pop)

namespace wx2
{
	class Mouse final
	{
	public:
		//! @brief マウスのボタンの種類
		enum Buttons
		{
			Left = 0,
			Right,
			Middle,
			X1,
			X2
		};

		//! @brief マウスの軸の種類
		enum Axises
		{
			CursorX,
			CursorY,
			WheellScroll
		};

	private:
		//! IDirectInput8のComPtr
		using DInputPtr = Microsoft::WRL::ComPtr<IDirectInput8>;
		//! IDirectInputDevice8のComPtr
		using DevicePtr = Microsoft::WRL::ComPtr<IDirectInputDevice8>;

		//! マウスのボタン数
		static constexpr std::size_t NUM_BUTTONS = 5;
		//! マウスの軸数
		static constexpr std::size_t NUM_AXISES = 3;

		//! @brief マウス入力の状態
		struct MouseState
		{
			struct
			{
				std::bitset<NUM_BUTTONS> buttons;
			} current, previous;
			std::array<float, NUM_AXISES> axises{};
		};

		//! @brief マウスのデバイス情報
		struct MouseDevice
		{
			DevicePtr device;
			DIDEVICEINSTANCE instance;
			DIDEVCAPS capability;
		};

	public:
		Mouse() = default;
		~Mouse() noexcept;

		WX2_DISALLOW_COPY_AND_MOVE(Mouse);

		void Initialize(const DInputPtr& directInput, const HWND hwnd) noexcept;
		void Regist() noexcept;
		void Update() noexcept;

		[[nodiscard]] bool IsDown(const Buttons button) const noexcept
		{
			return state_.current.buttons[button];
		}

		[[nodiscard]] bool IsPressed(const Buttons button) const noexcept
		{
			return state_.current.buttons[button] && !state_.previous.buttons[button];
		}

		[[nodiscard]] bool IsReleased(const Buttons button) const noexcept
		{
			return !state_.current.buttons[button] && state_.previous.buttons[button];
		}

		[[nodiscard]] float GetAxisVelocity(const Axises axis) const noexcept
		{
			return state_.axises[axis];
		}

	private:
		//! @brief マウスデバイスを登録する為のコールバック
		static BOOL CALLBACK SetupMouseCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef) noexcept;

		//! DirectInput8のComPtr
		DInputPtr directInput_{};
		//! 入力を受け付けるウィンドウのハンドル
		HWND hwnd_{};

		//! 接続されたマウスのデバイス情報リスト
		std::vector<MouseDevice> mouses_{};
		//! マウスの入力情報
		MouseState state_{};

		//! マウスの入力状態を取得する一時バッファ
		DIMOUSESTATE2 stateBuffer_ = {};
	};
}