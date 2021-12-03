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
#include <Eigen/Core>
#pragma warning(pop)

namespace wx2
{
	/// <summary>
	/// マウス入力を扱うクラス
	/// </summary>
	class Mouse final
	{
	public:
		enum Buttons
		{
			Left = 0,
			Right,
			Middle,
			X1,
			X2
		};

		enum Axises
		{
			CursorX,
			CursorY,
			WheellScroll
		};

	private:
		using DInputPtr = Microsoft::WRL::ComPtr<IDirectInput8>;
		using DevicePtr = Microsoft::WRL::ComPtr<IDirectInputDevice8>;

		static constexpr std::size_t NUM_BUTTONS = 5;	// ボタン数
		static constexpr std::size_t NUM_AXISES = 3;	// 軸数

		struct MouseState
		{
			struct 
			{
				std::bitset<NUM_BUTTONS> buttons;
			} current, previous;
			std::array<float, NUM_AXISES> axises{};
		};

		struct MouseDevice
		{
			DevicePtr device;
			DIDEVICEINSTANCE instance;
			DIDEVCAPS capability;
		};

	public:
		Mouse() noexcept;
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
		static BOOL CALLBACK SetupMouseCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef) noexcept;

		DInputPtr directInput_;
		HWND hwnd_;

		std::vector<MouseDevice> mouses_;
		MouseState state_;
	};
}