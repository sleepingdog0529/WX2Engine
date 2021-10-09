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
	class window;

	/// <summary>
	/// �����̃E�B���h�E�ƃ��b�Z�[�W�̊Ǘ�
	/// </summary>
	class window_container
	{
	public:
		window_container() = default;
		~window_container() = default;

		WX2_DISALLOW_COPY_AND_MOVE(window_container);

		/// <summary>
		/// �E�B���h�E�𐶐�����
		/// </summary>
		/// <param name="name">�o�^��</param>
		/// <param name="window_prop">�E�B���h�E�ݒ�</param>
		/// <returns>�E�B���h�E</returns>
		std::shared_ptr<window> create(const std::string& name, const window_property& window_prop);

		/// <summary>
		/// ���b�Z�[�W�����������ԂɍX�V�������Ăяo��
		/// </summary>
		/// <param name="process">�X�V�֐�</param>
		void process_msgs(std::function<bool()> process);

		/// <summary>
		/// �E�B���h�E�v���V�[�W��
		/// </summary>
		LRESULT wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	private:
		std::unordered_map<std::string, std::shared_ptr<window>> render_windows_;	// �E�B���h�E���X�g
	};
}