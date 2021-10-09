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
	class window_container;

	class window
	{
	public:
		/// <summary>
		/// �E�B���h�E����
		/// </summary>
		/// <param name="container">�i�[��R���e�i</param>
		/// <param name="window_prop">�E�B���h�E�ݒ�</param>
		window(window_container* container, const window_property& window_prop);
		virtual ~window();

		WX2_DISALLOW_COPY_AND_MOVE(window);

		/// <returns>�E�B���h�E�n���h��</returns>
		HWND get_handle() const { return hwnd_; }
		/// <returns>�E�B���h�E����</returns>
		int get_width() const { return window_prop_.width; }
		/// <returns>�E�B���h�E�c��</returns>
		int get_height() const { return window_prop_.height; }
		/// <returns>�t���X�N���[����</returns>
		bool is_fullscreen() const { return window_prop_.fullscreen; }

	private:
		window_container* container_;	// �E�B���h�E�R���e�i
		HWND hwnd_;						// �E�B���h�n���h��
		std::string class_name_;		// �N���X��
		window_property window_prop_;	// �E�B���h�E���
	};
}