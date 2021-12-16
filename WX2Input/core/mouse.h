/*********************************************************************
 * @file	mouse.h
 * @author	Tomomi Murakami
 * @date	2021/10/10 15:09
 * @brief	�}�E�X����
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
		//! @brief �}�E�X�̃{�^���̎��
		enum Buttons
		{
			Left = 0,
			Right,
			Middle,
			X1,
			X2
		};

		//! @brief �}�E�X�̎��̎��
		enum Axises
		{
			CursorX,
			CursorY,
			WheellScroll
		};

	private:
		//! IDirectInput8��ComPtr
		using DInputPtr = Microsoft::WRL::ComPtr<IDirectInput8>;
		//! IDirectInputDevice8��ComPtr
		using DevicePtr = Microsoft::WRL::ComPtr<IDirectInputDevice8>;

		//! �}�E�X�̃{�^����
		static constexpr std::size_t NUM_BUTTONS = 5;
		//! �}�E�X�̎���
		static constexpr std::size_t NUM_AXISES = 3;

		//! @brief �}�E�X���͂̏��
		struct MouseState
		{
			struct
			{
				std::bitset<NUM_BUTTONS> buttons;
			} current, previous;
			std::array<float, NUM_AXISES> axises{};
		};

		//! @brief �}�E�X�̃f�o�C�X���
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
		//! @brief �}�E�X�f�o�C�X��o�^����ׂ̃R�[���o�b�N
		static BOOL CALLBACK SetupMouseCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef) noexcept;

		//! DirectInput8��ComPtr
		DInputPtr directInput_{};
		//! ���͂��󂯕t����E�B���h�E�̃n���h��
		HWND hwnd_{};

		//! �ڑ����ꂽ�}�E�X�̃f�o�C�X��񃊃X�g
		std::vector<MouseDevice> mouses_{};
		//! �}�E�X�̓��͏��
		MouseState state_{};

		//! �}�E�X�̓��͏�Ԃ��擾����ꎞ�o�b�t�@
		DIMOUSESTATE2 stateBuffer_ = {};
	};
}