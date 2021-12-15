/*********************************************************************
 * @file	gamepad.h
 * @author	Tomomi Murakami
 * @date	2021/10/17 23:43
 * @brief	XInput対応コントローラー入力
*********************************************************************/
#pragma once
#pragma warning(push, 0)
#include <WX2Common.h>
#define STRICT
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h> // Xinputより先にインクルードしないとエラー
#include <Xinput.h>
#include <wrl/client.h>
#include <bitset>
#include <array>
#pragma warning(pop)

namespace wx2
{
	/**
	 * @brief  コントローラー入力
	 */
	class Gamepad final
	{
	public:
		enum Buttons
		{
			DPadUp = 0,
			DPadDown,
			DPadLeft,
			DPadRight,
			Start,
			Back,
			LThumb,
			RThumb,
			LShoulder,
			RShoulder,
			A,
			B,
			X,
			Y
		};

		enum Axises
		{
			LTrigger = 0,
			RTrigger,
			LThumbX,
			LThumbY,
			RThumbX,
			RThumbY
		};

	private:
		//! 左スティックのデッドゾーン(0 ~ 1)
		static constexpr float LEFT_THUMB_DEADZONE =
			XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / static_cast<float>(SHRT_MAX);

		//! 左スティックのデッドゾーン(0 ~ 1)
		static constexpr float RIGHT_THUMB_DEADZONE =
			XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / static_cast<float>(SHRT_MAX);

		//! 両トリガーのデッドゾーン(0 ~ 1)
		static constexpr float TRIGGER_DEADZONE =
			XINPUT_GAMEPAD_TRIGGER_THRESHOLD / static_cast<float>(UCHAR_MAX);

		//! ボタン数
		static constexpr size_t NUM_BUTTONS = 14;
		//! 軸数
		static constexpr size_t NUM_AXISES = 6;

		//! @brief ゲームパッドの状態
		struct GamepadState
		{
			struct
			{
				std::bitset<NUM_BUTTONS> buttons;
				std::array<float, NUM_AXISES> axises;
			} current, previous;
		};

	public:
		Gamepad() = default;
		~Gamepad() = default;

		// コピーとムーブ禁止
		WX2_DISALLOW_COPY_AND_MOVE(Gamepad);

		//! @brief コントローラーの状態を更新
		void Update() noexcept;

		/**
		 * @brief  ボタンが押されているか
		 * @param  button ボタンの種類
		 * @param  user コントローラーの番号
		 * @return 押されているか
		 */
		[[nodiscard]] bool IsDown(const Buttons button, const size_t user = 0) const noexcept
		{
			return states_[user].current.buttons[button];
		}

		/**
		 * @brief  ボタンが押されたか
		 * @param  button ボタンの種類
		 * @param  user コントローラーの番号
		 * @return ボタンが押されたか
		 */
		[[nodiscard]] bool IsPressed(const Buttons button, const size_t user = 0) const noexcept
		{
			return states_[user].current.buttons[button] && !states_[user].previous.buttons[button];
		}

		/**
		 * @brief  ボタンが離されたか
		 * @param  button ボタンの種類
		 * @param  user コントローラーの番号
		 * @return ボタンが離されたか
		 */
		[[nodiscard]] bool IsReleased(const Buttons button, const size_t user = 0) const noexcept
		{
			return !states_[user].current.buttons[button] && states_[user].previous.buttons[button];
		}

		/**
		 * @brief  軸の入力値を取得する
		 * @param  axis 軸の種類
		 * @param  user コントローラーの番号
		 * @return 軸の入力値
		 */
		[[nodiscard]] float GetAxisValue(const Axises axis, const size_t user = 0) const noexcept
		{
			return states_[user].current.axises[axis];
		}

		/**
		 * @brief  軸の入力速度を取得する
		 * @param  axis 軸の種類
		 * @param  user コントローラーの番号
		 * @return 軸の入力速度
		 */
		[[nodiscard]] float GetAxisVelocity(const Axises axis, const size_t user = 0) const noexcept
		{
			return states_[user].current.axises[axis] - states_[user].previous.axises[axis];
		}

	private:
		//! ゲームパッドの状態リスト
		std::array<GamepadState, XUSER_MAX_COUNT> states_{};
		//! ゲームパッドの状態を取得する一時バッファ
		XINPUT_STATE buffer_{};
	};
}