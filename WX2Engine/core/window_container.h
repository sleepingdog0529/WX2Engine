/*********************************************************************
 * @file	window_container.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:36
 * @brief	�����̃E�B���h�E�ƃ��b�Z�[�W�̊Ǘ�
*********************************************************************/
#pragma once
#include "window.h"
#include "window_property.h"

namespace wx2
{
	class Window;

	/// <summary>
	/// �����̃E�B���h�E�ƃ��b�Z�[�W�̊Ǘ�
	/// </summary>
	class WindowContainer
	{
	public:
		WindowContainer() = default;
		virtual ~WindowContainer() = default;

		WX2_DISALLOW_COPY_AND_MOVE(WindowContainer);

		/// <summary>
		/// �E�B���h�E�𐶐�����
		/// </summary>
		/// <param name="name">�o�^��</param>
		/// <param name="windowProp">�E�B���h�E�ݒ�</param>
		/// <returns>�E�B���h�E</returns>
		std::shared_ptr<Window> Create(const std::string& name, const WindowProperty& windowProp);

		/// <summary>
		/// ���b�Z�[�W�����������ԂɍX�V�������Ăяo��
		/// </summary>
		/// <param name="process">�X�V�֐�</param>
		void ProcessMessages(std::function<bool()> process);

		/// <summary>
		/// �E�B���h�E�v���V�[�W��
		/// </summary>
		LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	private:
		std::unordered_map<std::string, std::shared_ptr<Window>> windows_;	// �E�B���h�E���X�g
	};
}