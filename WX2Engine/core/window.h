/*********************************************************************
 * @file	window.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:36
 * @brief	�E�B���h�E�쐬
*********************************************************************/
#pragma once
#include "window_container.h"
#include "window_property.h"

namespace wx2 
{
	class WindowContainer;

	class Window
	{
	public:
		/// <summary>
		/// �E�B���h�E����
		/// </summary>
		/// <param name="container">�i�[��R���e�i</param>
		/// <param name="window_prop">�E�B���h�E�ݒ�</param>
		Window(WindowContainer* container, const WindowProperty& windowProp);
		virtual ~Window();

		WX2_DISALLOW_COPY_AND_MOVE(Window);

		HWND GetHandle() const { return hwnd_; }
		int GetWidth() const { return windowProp_.width; }
		int GetHeight() const { return windowProp_.height; }
		bool IsFullscreen() const { return windowProp_.fullscreen; }

	private:
		WindowContainer* container_;	// �E�B���h�E�R���e�i
		HWND hwnd_;						// �E�B���h�n���h��
		std::string className_;		// �N���X��
		WindowProperty windowProp_;	// �E�B���h�E���
	};
}