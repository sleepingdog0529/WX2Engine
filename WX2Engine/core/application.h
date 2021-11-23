/*********************************************************************
 * @file	application.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:27
 * @brief	�A�v���P�[�V������b
 ********************************************************************/
#pragma once
#include <WX2Input.h>
#include "window_container.h"
#include "../graphics/graphics.h"

int main(int argc, char** argv);

namespace wx2
{
	/**
	 * @brief �A�v���P�[�V������b�N���X �N���C�A���g�͂�����p�����ăA�v���P�[�V�����̓�����`����
	 */
	class Application
	{
	private:
		using WindowPtr = std::shared_ptr<Window>;

		// �G���g���|�C���g�ł̂ݎ��s������
#if defined(NDEBUG)
		friend INT WINAPI ::WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
#else
		friend int ::main(int, char**);
#endif

	public:
		/**
		 * @brief  �A�v���P�[�V����������
		 */
		Application();

		/**
		 * @brief  �A�v���P�[�V�����I��
		 */
		virtual ~Application();

		WX2_DISALLOW_COPY_AND_MOVE(Application);

	private:
		/**
		 * @brief �A�v���P�[�V���������s����
		 */
		void Run();

		Logger log_;
		WindowContainer windowContainer_;
		Input input_;
		WindowPtr mainWindow_;
		Graphics graphics_;

		//! ���ɃC���X�^���X������Ă��邩
		static inline bool instantiated_ = false;
	};

	//! �N���C�A���g����`����
	std::unique_ptr<Application> CreateApp();
}