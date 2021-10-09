/*********************************************************************
 * @file	application.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:27
 * @brief	�A�v���P�[�V������b
*********************************************************************/
#pragma once
#include "macro.h"
#include "log.h"
#include "window_container.h"

int main(int argc, char** argv);

namespace wx2
{
	/// <summary>
	/// <para>�A�v���P�[�V������b�N���X</para>
	/// <para>�N���C�A���g�͂�����p�����ăA�v���P�[�V�����̓�����`����</para>
	/// </summary>
	class application
	{
	public:
		application();
		virtual ~application();

		WX2_DISALLOW_COPY_AND_MOVE(application);

	private:
		/// <summary>
		/// �A�v���P�[�V���������s����
		/// </summary>
		void run();

		// main�֐��ł̂ݎ��s������
		friend int ::main(int argc, char** argv);

		logger log_;
		window_container window_container_;

		static bool instantiated_;
	};

	// �N���C�A���g����`����
	std::unique_ptr<application> create_app();
}