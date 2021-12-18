/*********************************************************************
 * @file	window.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:36
 * @brief	�E�B���h�E�쐬
 ********************************************************************/
#pragma once
#include "window_container.h"

namespace wx2
{
	// �O���錾
	class WindowContainer;

	/**
	 * @brief  �E�B���h�E
	 */
	class Window final
	{
	public:
		/**
		 * @brief �E�B���h�E����
		 * @param[in] container �i�[��R���e�i
		 * @param[in] windowProp �E�B���h�E�ݒ�
		 */
		Window(WindowContainer* container, WindowProperty windowProp);

		//! @brief �E�B���h�E�����
		~Window() noexcept;

		WX2_DISALLOW_COPY(Window);
		WX2_DISALLOW_MOVE(Window);

		//! �E�B���h�E�̃E�B���h�E�v���V�[�W��
		static LRESULT CALLBACK HandleMessageRedirect(const HWND hwnd, const UINT msg, const  WPARAM wp, const  LPARAM lp);
		//! �E�B���h�E�̃E�B���h�E�v���V�[�W��
		static LRESULT CALLBACK HandleMessageSetup(const HWND hwnd, const  UINT msg, const WPARAM wp, const LPARAM lp);

		/**
		 * @brief  �E�B���h�E�n���h�����擾����
		 * @return �E�B���h�E�n���h��
		 */
		[[nodiscard]] HWND GetHandle() const noexcept
		{
			return hwnd_;
		}

		/**
		 * @brief  �E�B���h�E�����擾����
		 * @return �E�B���h�E���
		 */
		[[nodiscard]] const WindowProperty& GetWindowProperty() const noexcept
		{
			return windowProp_;
		}

		/**
		 * @brief  �E�B���h�E���|�C���^���擾����
		 * @return �E�B���h�E���|�C���^
		 */
		[[nodiscard]] WindowProperty* GetWindowPropertyPtr() noexcept
		{
			return &windowProp_;
		}

		/**
		 * @brief �E�B���h�E�̃t���X�N���[����Ԃ�ݒ肷��
		 * @param fullscreen �t���X�N���[���ɂ��邩
		 */
		void SetFullscreen(bool fullscreen) noexcept;

		/**
		 * @brief �E�B���h�E�̍ő剻��Ԃ�ݒ肷��
		 * @param maximaize �ő剻���邩
		 */
		void SetMaximize(const bool maximaize) noexcept;

		/**
		 * @brief  �E�B���h�E�̃^�C�g����ݒ肷��
		 * @param  title �ݒ肷��^�C�g����
		 */
		void SetTitle(const std::string& title) noexcept;

	private:
		//! �E�B���h�E�ړ����̃R�[���o�b�N
		void OnMoving(const WPARAM wp, const  LPARAM lp) noexcept;

		//! �E�B���h�E�T�C�Y�ύX���̃R�[���o�b�N
		void OnSizing(const WPARAM wp, const LPARAM lp) noexcept;

		//! �E�B���h�E�\�����[�h�ύX���̃R�[���o�b�N
		void OnDisplayModeChanged(const WPARAM wp, const LPARAM lp) noexcept;

		//! �E�B���h�E���ύX���̃R�[���o�b�N
		void OnTitleChanged(const WPARAM wp, const LPARAM lp) noexcept;

		//! �L�[�������̃R�[���o�b�N
		void OnKeyDown(const WPARAM wp, const LPARAM lp) noexcept;

		WindowContainer* container_{};	//! �E�B���h�E�R���e�i
		HWND hwnd_{};					//! �E�B���h�E�n���h��
		std::string className_{};		//! �E�B���h�E�N���X��
		WindowProperty windowProp_{};	//! �E�B���h�E���

		// �t���X�N���[������������Ƃ��ɕK�v�ȏ����L�����Ă������߂̕ϐ�
		int recordWindowX_{};			//! �ʏ펞�E�B���h�E��X���W�̋L�^
		int recordWindowY_{};			//! �ʏ펞�E�B���h�E��Y���W�̋L�^
		int recordWindowWidth_{};		//! �ʏ펞�E�B���h�E�̕��̋L�^
		int recordWindowHeight_{};		//! �ʏ펞�E�B���h�E�̍����̋L�^
	};
}