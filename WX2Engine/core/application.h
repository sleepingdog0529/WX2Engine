/*********************************************************************
 * @file	application.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:27
 * @brief	�A�v���P�[�V������b
 ********************************************************************/
#pragma once
#include <WX2Input.h>
#include <WX2Graphics.h>
#include "window_container.h"

int main(int argc, char** argv);

namespace wx2
{
	/**
	 * @brief �A�v���P�[�V������b�N���X �N���C�A���g�͂�����p�����ăA�v���P�[�V�����̓�����`����
	 */
	class Application
	{
	private:
		// �G���g���|�C���g�ł̂ݎ��s������
#if !defined(NDEBUG)
		friend int ::main(int, char**);
#else
		friend int ::WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
#endif
		using WindowPtr = std::shared_ptr<Window>;

	public:
		/**
		 * @brief  �A�v���P�[�V����������
		 */
		Application() noexcept;

		/**
		 * @brief  �A�v���P�[�V�����I��
		 */
		virtual ~Application() noexcept;

		WX2_DISALLOW_COPY_AND_MOVE(Application);

	protected:
		virtual void Start() noexcept = 0;
		virtual bool Update() noexcept = 0;
		void Draw() noexcept;

		Timer<> frameTimer_;
		Logger log_;
		WindowContainer windowContainer_;
		Input input_;
		WindowPtr mainWindow_;
		graphics::Graphics graphics_;

	private:
		/**
		 * @brief �A�v���P�[�V���������s����
		 */
		int Run() noexcept;

		//! ���ɃC���X�^���X������Ă��邩
		static inline bool instantiated_ = false;
	};

	//! �N���C�A���g����`����
	std::unique_ptr<Application> CreateApp() noexcept;
}