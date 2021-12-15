/*********************************************************************
 * @file	gamepad.h
 * @author	Tomomi Murakami
 * @date	2021/10/17 23:43
 * @brief	XInput�Ή��R���g���[���[����
*********************************************************************/
#pragma once
#pragma warning(push, 0)
#include <WX2Common.h>
#define STRICT
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h> // Xinput����ɃC���N���[�h���Ȃ��ƃG���[
#include <Xinput.h>
#include <wrl/client.h>
#include <bitset>
#include <array>
#pragma warning(pop)

namespace wx2
{
	/**
	 * @brief  �R���g���[���[����
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

		//! @brief �Q�[���p�b�h�̏��
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

		// �R�s�[�ƃ��[�u�֎~
		WX2_DISALLOW_COPY_AND_MOVE(Gamepad);

		//! @brief �R���g���[���[�̏�Ԃ��X�V
		void Update() noexcept;

		/**
		 * @brief  �{�^����������Ă��邩
		 * @param  button �{�^���̎��
		 * @param  user �R���g���[���[�̔ԍ�
		 * @return ������Ă��邩
		 */
		[[nodiscard]] bool IsDown(const Buttons button, const size_t user = 0) const noexcept
		{
			return states_[user].current.buttons[button];
		}

		/**
		 * @brief  �{�^���������ꂽ��
		 * @param  button �{�^���̎��
		 * @param  user �R���g���[���[�̔ԍ�
		 * @return �{�^���������ꂽ��
		 */
		[[nodiscard]] bool IsPressed(const Buttons button, const size_t user = 0) const noexcept
		{
			return states_[user].current.buttons[button] && !states_[user].previous.buttons[button];
		}

		/**
		 * @brief  �{�^���������ꂽ��
		 * @param  button �{�^���̎��
		 * @param  user �R���g���[���[�̔ԍ�
		 * @return �{�^���������ꂽ��
		 */
		[[nodiscard]] bool IsReleased(const Buttons button, const size_t user = 0) const noexcept
		{
			return !states_[user].current.buttons[button] && states_[user].previous.buttons[button];
		}

		/**
		 * @brief  ���̓��͒l���擾����
		 * @param  axis ���̎��
		 * @param  user �R���g���[���[�̔ԍ�
		 * @return ���̓��͒l
		 */
		[[nodiscard]] float GetAxisValue(const Axises axis, const size_t user = 0) const noexcept
		{
			return states_[user].current.axises[axis];
		}

		/**
		 * @brief  ���̓��͑��x���擾����
		 * @param  axis ���̎��
		 * @param  user �R���g���[���[�̔ԍ�
		 * @return ���̓��͑��x
		 */
		[[nodiscard]] float GetAxisVelocity(const Axises axis, const size_t user = 0) const noexcept
		{
			return states_[user].current.axises[axis] - states_[user].previous.axises[axis];
		}

	private:
		//! �Q�[���p�b�h�̏�ԃ��X�g
		std::array<GamepadState, XUSER_MAX_COUNT> states_{};
		//! �Q�[���p�b�h�̏�Ԃ��擾����ꎞ�o�b�t�@
		XINPUT_STATE buffer_{};
	};
}