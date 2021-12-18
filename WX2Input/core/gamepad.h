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
	//! @brief �R���g���[���[�̃{�^���̎��
	enum GamepadButtons
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

	// @brief �R���g���[���[�̎��̎��
	enum GamepadAxises
	{
		LTrigger = 0,
		RTrigger,
		LThumbX,
		LThumbY,
		RThumbX,
		RThumbY
	};

	/**
	 * @brief  �R���g���[���[����
	 */
	class Gamepad final
	{
	public:
		Gamepad() noexcept;
		~Gamepad();
		
		WX2_DISALLOW_COPY(Gamepad);
		WX2_DISALLOW_MOVE(Gamepad);

		//! @brief �R���g���[���[�̓��͏�Ԃ��X�V
		void Update() const noexcept;

		/**
		 * @brief  �{�^����������Ă��邩
		 * @param  button �{�^���̎��
		 * @param  user �R���g���[���[�̔ԍ�
		 * @return ������Ă��邩
		 */
		[[nodiscard]] bool IsDown(const GamepadButtons button, const size_t user = 0) const noexcept;
		/**
		 * @brief  �{�^���������ꂽ��
		 * @param  button �{�^���̎��
		 * @param  user �R���g���[���[�̔ԍ�
		 * @return �{�^���������ꂽ��
		 */
		[[nodiscard]] bool IsPressed(const GamepadButtons button, const size_t user = 0) const noexcept;
		/**
		 * @brief  �{�^���������ꂽ��
		 * @param  button �{�^���̎��
		 * @param  user �R���g���[���[�̔ԍ�
		 * @return �{�^���������ꂽ��
		 */
		[[nodiscard]] bool IsReleased(const GamepadButtons button, const size_t user = 0) const noexcept;
		/**
		 * @brief  ���̓��͒l���擾����
		 * @param  axis ���̎��
		 * @param  user �R���g���[���[�̔ԍ�
		 * @return ���̓��͒l
		 */
		[[nodiscard]] float GetAxisValue(const GamepadAxises axis, const size_t user = 0) const noexcept;
		/**
		 * @brief  ���̓��͑��x���擾����
		 * @param  axis ���̎��
		 * @param  user �R���g���[���[�̔ԍ�
		 * @return ���̓��͑��x
		 */
		[[nodiscard]] float GetAxisVelocity(const GamepadAxises axis, const size_t user = 0) const noexcept;

	private:
		class Impl;
		std::unique_ptr<Impl> pimpl_;
	};
}