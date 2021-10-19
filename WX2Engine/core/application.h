/*********************************************************************
 * @file	application.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:27
 * @brief	�A�v���P�[�V������b
*********************************************************************/
#pragma once
#include <wx2common.h>
#include <WX2Input.h>
#include "window_container.h"

int main(int argc, char** argv);

namespace wx2
{
	/// <summary>
	/// <para>�A�v���P�[�V������b�N���X</para>
	/// <para>�N���C�A���g�͂�����p�����ăA�v���P�[�V�����̓�����`����</para>
	/// </summary>
	class Application
	{
	private:
		// main�֐��ł̂ݎ��s������
		friend int ::main(int argc, char** argv);

	public:
		Application();
		virtual ~Application();

		WX2_DISALLOW_COPY_AND_MOVE(Application);

	private:
		/// <summary>
		/// �A�v���P�[�V���������s����
		/// </summary>
		void Run();

		Logger log_;
		WindowContainer windowContainer_;
		Input input_;

		static inline bool instantiated_ = false;
	};

	// �N���C�A���g����`����
	std::unique_ptr<Application> CreateApp();
}