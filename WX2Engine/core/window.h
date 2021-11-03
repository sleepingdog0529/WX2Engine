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

	class Window
	{
	public:
		/**
		 * @brief	  �E�B���h�E����
		 * @param[in] container �i�[��R���e�i
		 * @param[in] window_prop �E�B���h�E�ݒ�
		 */
		Window(WindowContainer* container, const WindowProperty& windowProp);
		/**
		 * @brief 
		 */
		virtual ~Window();

		//! �E�B���h�E�̃E�B���h�E�v���V�[�W��
		static LRESULT CALLBACK HandleMessageRedirect(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
		//! �E�B���h�E�̃E�B���h�E�v���V�[�W��
		static LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

		WX2_DISALLOW_COPY_AND_MOVE(Window);

		HWND GetHandle() const { return hwnd_; }
		const WindowProperty& GetWindowProperty() const { return windowProp_; }

		void SetFullscreen(bool fullscreen);
		void SetMaximize(bool maximaize);

	private:
		WindowContainer* container_;	// �E�B���h�E�R���e�i
		HWND hwnd_;						// �E�B���h�n���h��
		std::string className_;		// �N���X��
		WindowProperty windowProp_;	// �E�B���h�E���
	};
}