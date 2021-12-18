#include "gamepad.h"
#include <WX2Mathematics.h>
#include <algorithm>

namespace wx2
{
	/**
	 * @brief  �R���g���[���[����
	 */
	class Gamepad::Impl final
	{
	private:
		//! ���X�e�B�b�N�̃f�b�h�]�[��(0 ~ 1)
		static constexpr float LEFT_THUMB_DEADZONE =
			XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / static_cast<float>(SHRT_MAX);

		//! ���X�e�B�b�N�̃f�b�h�]�[��(0 ~ 1)
		static constexpr float RIGHT_THUMB_DEADZONE =
			XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / static_cast<float>(SHRT_MAX);

		//! ���g���K�[�̃f�b�h�]�[��(0 ~ 1)
		static constexpr float TRIGGER_DEADZONE =
			XINPUT_GAMEPAD_TRIGGER_THRESHOLD / static_cast<float>(UCHAR_MAX);

		//! �{�^����
		static constexpr size_t NUM_BUTTONS = 14;
		//! ����
		static constexpr size_t NUM_AXISES = 6;

		//! @brief �Q�[���p�b�h���͂̏��
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
		 * @brief �R���g���[���[�̓��͏�Ԃ��X�V
		 */
		void Update() noexcept
		{
			// �R���g���[���[�̍ő�ڑ��������[�v
			for (auto& [curt, prev] : states_)
			{
				// ��ԍX�V
				prev = curt;
				curt = {};

				// �R���g���[���[��Ԏ擾
				if (SUCCEEDED(XInputGetState(0, &buffer_)))
				{
					// �{�^����Ԃ��i�[
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

					// ���l��float�^��0~1�ɕϊ�
					const float lt = static_cast<float>(buffer_.Gamepad.bLeftTrigger) / static_cast<float>(UCHAR_MAX);
					const float rt = static_cast<float>(buffer_.Gamepad.bRightTrigger) / static_cast<float>(UCHAR_MAX);
					const float ltx = static_cast<float>(buffer_.Gamepad.sThumbLX) / static_cast<float>(SHRT_MAX);
					const float lty = static_cast<float>(buffer_.Gamepad.sThumbLY) / static_cast<float>(SHRT_MAX);
					const float rtx = static_cast<float>(buffer_.Gamepad.sThumbRX) / static_cast<float>(SHRT_MAX);
					const float rty = static_cast<float>(buffer_.Gamepad.sThumbRY) / static_cast<float>(SHRT_MAX);

					// �f�b�h�]�[����K�����Ď��l���i�[
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
		 * @brief  �{�^����������Ă��邩
		 * @param  button �{�^���̎��
		 * @param  user �R���g���[���[�̔ԍ�
		 * @return ������Ă��邩
		 */
		[[nodiscard]] bool IsDown(const GamepadButtons button, const size_t user) const noexcept
		{
			return states_[user].current.buttons[button];
		}

		/**
		 * @brief  �{�^���������ꂽ��
		 * @param  button �{�^���̎��
		 * @param  user �R���g���[���[�̔ԍ�
		 * @return �{�^���������ꂽ��
		 */
		[[nodiscard]] bool IsPressed(const GamepadButtons button, const size_t user) const noexcept
		{
			return states_[user].current.buttons[button] && !states_[user].previous.buttons[button];
		}

		/**
		 * @brief  �{�^���������ꂽ��
		 * @param  button �{�^���̎��
		 * @param  user �R���g���[���[�̔ԍ�
		 * @return �{�^���������ꂽ��
		 */
		[[nodiscard]] bool IsReleased(const GamepadButtons button, const size_t user) const noexcept
		{
			return !states_[user].current.buttons[button] && states_[user].previous.buttons[button];
		}

		/**
		 * @brief  ���̓��͒l���擾����
		 * @param  axis ���̎��
		 * @param  user �R���g���[���[�̔ԍ�
		 * @return ���̓��͒l
		 */
		[[nodiscard]] float GetAxisValue(const GamepadAxises axis, const size_t user) const noexcept
		{
			return states_[user].current.axises[axis];
		}

		/**
		 * @brief  ���̓��͑��x���擾����
		 * @param  axis ���̎��
		 * @param  user �R���g���[���[�̔ԍ�
		 * @return ���̓��͑��x
		 */
		[[nodiscard]] float GetAxisVelocity(const GamepadAxises axis, const size_t user) const noexcept
		{
			return states_[user].current.axises[axis] - states_[user].previous.axises[axis];
		}

	private:
		//! �Q�[���p�b�h�̏�ԃ��X�g
		std::array<GamepadState, XUSER_MAX_COUNT> states_{};
		//! �Q�[���p�b�h�̓��͏�Ԃ��擾����ꎞ�o�b�t�@
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