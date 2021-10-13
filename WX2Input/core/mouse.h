/*********************************************************************
 * @file	mouse.h
 * @author	Tomomi Murakami
 * @date	2021/10/10 15:09
 * @brief	マウス入力
*********************************************************************/
#pragma once
#pragma warning(push, 0) 
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
	class Mouse
	{
	public:
		enum ButtonCode : uint8_t
		{
			Left = 0,
			Right,
			Middle,
			X1,
			X2,
			NUM_BUTTONS
		};

		static void Initialize(HWND hwnd);
		static void Finalize();
		static void Update();

		static void StateReset();

		static bool IsDown(ButtonCode button) { return buttons_[0][button]; }
		static bool IsUp(ButtonCode button) { return !buttons_[0][button]; }
		static bool IsPressed(ButtonCode button) { return buttons_[0][button] && !buttons_[1][button]; }
		static bool IsReleased(ButtonCode button) { return !buttons_[0][button] && buttons_[1][button]; }
		static Eigen::Vector2f GetVelocity() { return velocity_; }
		static float GetScroll() { return scroll_; }

	private:
		static constexpr float CURSOR_SENSITIVITY_ = 0.0018f;
		static constexpr float SCOLL_SENSITIVITY_ = 1.0f;

		static inline HWND hwnd_;
		static inline Microsoft::WRL::ComPtr<IDirectInputDevice8> device_;
		static inline DIMOUSESTATE2 state_;

		static inline std::bitset<NUM_BUTTONS> buttons_[2];
		static inline Eigen::Vector2f velocity_;
		static inline float scroll_;
	};
}