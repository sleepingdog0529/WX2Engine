#include "gamepad.h"
#include <WX2Mathematics.h>
#include <algorithm>

namespace wx2
{
	/**
	 * @brief  コントローラー入力
	 */
	class Gamepad::Impl final
	{
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

		//! @brief ゲームパッド入力の状態
		struct GamepadState
		{
			struct
			{
				std::bitset<NUM_BUTTONS> buttons;
				std::array<float, NUM_AXISES> axises;
			} current, previous;
		};

	public:
		Impl() = default;
		~Impl() = default;

		WX2_DISALLOW_COPY(Impl);
		WX2_MOVEABLE(Impl);

		/**
		 * @brief コントローラーの入力状態を更新
		 */
		void Update() noexcept
		{
			// コントローラーの最大接続数分ループ
			for (auto& [curt, prev] : states_)
			{
				// 状態更新
				prev = curt;
				curt = {};

				// コントローラー状態取得
				if (SUCCEEDED(XInputGetState(0, &buffer_)))
				{
					// ボタン状態を格納
					curt.buttons[DPadUp] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
					curt.buttons[DPadDown] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
					curt.buttons[DPadLeft] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
					curt.buttons[DPadRight] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
					curt.buttons[Start] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_START;
					curt.buttons[Back] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
					curt.buttons[LThumb] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
					curt.buttons[RThumb] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
					curt.buttons[LShoulder] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
					curt.buttons[RShoulder] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
					curt.buttons[A] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_A;
					curt.buttons[B] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_B;
					curt.buttons[X] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_X;
					curt.buttons[Y] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_Y;

					// 軸値をfloat型の0~1に変換
					const float lt = static_cast<float>(buffer_.Gamepad.bLeftTrigger) / static_cast<float>(UCHAR_MAX);
					const float rt = static_cast<float>(buffer_.Gamepad.bRightTrigger) / static_cast<float>(UCHAR_MAX);
					const float ltx = static_cast<float>(buffer_.Gamepad.sThumbLX) / static_cast<float>(SHRT_MAX);
					const float lty = static_cast<float>(buffer_.Gamepad.sThumbLY) / static_cast<float>(SHRT_MAX);
					const float rtx = static_cast<float>(buffer_.Gamepad.sThumbRX) / static_cast<float>(SHRT_MAX);
					const float rty = static_cast<float>(buffer_.Gamepad.sThumbRY) / static_cast<float>(SHRT_MAX);

					// デッドゾーンを適応して軸値を格納
					curt.axises[LTrigger] = std::clamp(Remap(lt, TRIGGER_DEADZONE, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f);
					curt.axises[RTrigger] = std::clamp(Remap(rt, TRIGGER_DEADZONE, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f);
					curt.axises[LThumbX] = std::clamp(Remap(std::fabsf(ltx), LEFT_THUMB_DEADZONE, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f) * Sign(ltx);
					curt.axises[LThumbY] = std::clamp(Remap(std::fabsf(lty), LEFT_THUMB_DEADZONE, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f) * Sign(lty);
					curt.axises[RThumbX] = std::clamp(Remap(std::fabsf(rtx), RIGHT_THUMB_DEADZONE, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f) * Sign(rtx);
					curt.axises[RThumbY] = std::clamp(Remap(std::fabsf(rty), RIGHT_THUMB_DEADZONE, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f) * Sign(rty);
				}
			}
		}

		/**
		 * @brief  ボタンが押されているか
		 * @param  button ボタンの種類
		 * @param  user コントローラーの番号
		 * @return 押されているか
		 */
		[[nodiscard]] bool IsDown(const GamepadButtons button, const size_t user) const noexcept
		{
			return states_[user].current.buttons[button];
		}

		/**
		 * @brief  ボタンが押されたか
		 * @param  button ボタンの種類
		 * @param  user コントローラーの番号
		 * @return ボタンが押されたか
		 */
		[[nodiscard]] bool IsPressed(const GamepadButtons button, const size_t user) const noexcept
		{
			return states_[user].current.buttons[button] && !states_[user].previous.buttons[button];
		}

		/**
		 * @brief  ボタンが離されたか
		 * @param  button ボタンの種類
		 * @param  user コントローラーの番号
		 * @return ボタンが離されたか
		 */
		[[nodiscard]] bool IsReleased(const GamepadButtons button, const size_t user) const noexcept
		{
			return !states_[user].current.buttons[button] && states_[user].previous.buttons[button];
		}

		/**
		 * @brief  軸の入力値を取得する
		 * @param  axis 軸の種類
		 * @param  user コントローラーの番号
		 * @return 軸の入力値
		 */
		[[nodiscard]] float GetAxisValue(const GamepadAxises axis, const size_t user) const noexcept
		{
			return states_[user].current.axises[axis];
		}

		/**
		 * @brief  軸の入力速度を取得する
		 * @param  axis 軸の種類
		 * @param  user コントローラーの番号
		 * @return 軸の入力速度
		 */
		[[nodiscard]] float GetAxisVelocity(const GamepadAxises axis, const size_t user) const noexcept
		{
			return states_[user].current.axises[axis] - states_[user].previous.axises[axis];
		}

	private:
		//! ゲームパッドの状態リスト
		std::array<GamepadState, XUSER_MAX_COUNT> states_{};
		//! ゲームパッドの入力状態を取得する一時バッファ
		XINPUT_STATE buffer_{};
	};

	Gamepad::Gamepad() noexcept
		: pimpl_(std::make_unique<Impl>()) {}

	Gamepad::~Gamepad() = default;

	void Gamepad::Update() const noexcept
	{
		pimpl_->Update();
	}

	bool Gamepad::IsDown(const GamepadButtons button, const size_t user) const noexcept
	{
		return pimpl_->IsDown(button, user);
	}

	bool Gamepad::IsPressed(const GamepadButtons button, const size_t user) const noexcept
	{
		return pimpl_->IsPressed(button, user);
	}

	bool Gamepad::IsReleased(const GamepadButtons button, const size_t user) const noexcept
	{
		return pimpl_->IsReleased(button, user);
	}

	float Gamepad::GetAxisValue(const GamepadAxises axis, const size_t user) const noexcept
	{
		return pimpl_->GetAxisValue(axis, user);
	}

	float Gamepad::GetAxisVelocity(const GamepadAxises axis, const size_t user) const noexcept
	{
		return pimpl_->GetAxisVelocity(axis, user);
	}
}