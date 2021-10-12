/*********************************************************************
 * @file	mouse.h
 * @author	Tomomi Murakami
 * @date	2021/10/10 15:09
 * @brief	マウス入力
*********************************************************************/
#pragma once
#pragma warning(push, 0) 
#include <array>
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
		enum Button : uint8_t
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

		static bool IsDown(Button button) { return buttons_[0][button]; }
		static bool IsUp(Button button) { return !buttons_[0][button]; }
		static bool IsPressed(Button button) { return buttons_[0][button] && !buttons_[1][button]; }
		static bool IsReleased(Button button) { return !buttons_[0][button] && buttons_[1][button]; }
		static Eigen::Vector2f GetVelocity() { return cursor_; }
		static float GetScroll() { return scroll_; }

	private:
		static constexpr float CURSOR_SENSITIVITY_ = 0.0018f;
		static constexpr float SCOLL_SENSITIVITY_ = 1.0f;

		static inline HWND hwnd_ = nullptr;
		static inline Microsoft::WRL::ComPtr<IDirectInputDevice8A> device_;
		static inline DIMOUSESTATE2 state_;

		static inline std::array<std::bitset<NUM_BUTTONS>, 2> buttons_;
		static inline Eigen::Vector2f cursor_{ 0.0f, 0.0f };
		static inline float scroll_ = 0.0f;
	};
}