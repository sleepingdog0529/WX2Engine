/*********************************************************************
 * @file	window.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:36
 * @brief	�E�B���h�E�쐬
 ********************************************************************/
#pragma once
#include "window_container.h"
#include "window_property.h"

namespace wx2 
{
	class WindowContainer;

	class Window final
	{
	public:
		/**
		 * @brief �E�B���h�E����
		 * @param[in] container �i�[��R���e�i
		 * @param[in] windowProp �E�B���h�E�ݒ�
		 */
		Window(WindowContainer* container, const WindowProperty& windowProp);
		/**
		 * @brief �E�B���h�E�����
		 */
		~Window();

		WX2_DISALLOW_COPY_AND_MOVE(Window);

		//! �E�B���h�E�̃E�B���h�E�v���V�[�W��
		static LRESULT CALLBACK HandleMessageRedirect(const HWND hwnd, const UINT msg, const  WPARAM wp, const  LPARAM lp);
		//! �E�B���h�E�̃E�B���h�E�v���V�[�W��
		static LRESULT CALLBACK HandleMessageSetup(const HWND hwnd, const  UINT msg, const WPARAM wp, const LPARAM lp);

		/**
		 * @return �E�B���h�E�n���h��
		 */
		[[nodiscard]] HWND GetHandle() const
		{
			return hwnd_;
		}

		/**
		 * @brief 
		 * @return 
		 */
		[[nodiscard]] const WindowProperty& GetWindowProperty() const
		{
			return windowProp_;
		}

		/**
		 * @brief �E�B���h�E�̃t���X�N���[����Ԃ�ݒ肷��
		 * @param fullscreen �t���X�N���[���ɂ��邩
		 */
		void SetFullscreen(bool fullscreen);

		/**
		 * @brief �E�B���h�E�̍ő剻��Ԃ�ݒ肷��
		 * @param maximaize �ő剻���邩
		 */
		void SetMaximize(const bool maximaize);

	private:
		//! �E�B���h�E�ړ����̃R�[���o�b�N
		void OnMoving(WPARAM wp, LPARAM lp);

		//! �E�B���h�E�T�C�Y�ύX���̃R�[���o�b�N
		void OnSizing(WPARAM wp, LPARAM lp);

		//! �E�B���h�E�\�����[�h�ύX���̃R�[���o�b�N
		void OnDisplayModeChanged(WPARAM wp, [[maybe_unused]] LPARAM lp);

		//! �L�[������
		void OnKeyDown(WPARAM wp, LPARAM lp);

		WindowContainer* container_;	//! �E�B���h�E�R���e�i
		HWND hwnd_;						//! �E�B���h�E�n���h��
		std::string className_;			//! �E�B���h�E�N���X��
		WindowProperty windowProp_;		//! �E�B���h�E���
	};
}