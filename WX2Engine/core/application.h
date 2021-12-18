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

// �G���g���|�C���g��O���錾
#if !defined(NDEBUG)
int main(int argc, char** argv);
#endif

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
		//! @brief  �A�v���P�[�V����������
		Application() noexcept;

		//! @brief  �A�v���P�[�V�����I��
		virtual ~Application() noexcept;

		
		WX2_DISALLOW_COPY(Application);
		WX2_DISALLOW_MOVE(Application);

	protected:
		//! @brief �A�v���P�[�V�����J�n���̏���
		virtual void Start() noexcept = 0;

		/**
		 * @brief  �A�v���P�[�V�����X�V���̏���
		 * @param  deltaTime �O��̍X�V����̌o�ߎ���
		 * @return false�ŃA�v���P�[�V�����I��
		 */
		virtual bool Update(const float deltaTime) noexcept = 0;

		/**
		 * @brief  �A�v���P�[�V�����̕`�揈��
		 * @param  deltaTime �O��̍X�V����̌o�ߎ���
		 */
		virtual void Draw(const float deltaTime) noexcept = 0;

		//! @brief  ImGui�̕`�揈��
		virtual void DrawImGui() noexcept = 0;
		
		Logger log_;	//! ���O�o��
		WindowContainer windowContainer_;	//! �E�B���h�E�̊Ǘ��R���e�i
		Input input_;						//! ���͊Ǘ�
		WindowPtr mainWindow_;	//! ���C���E�B���h�E
		Graphics graphics_;		//! �`��Ǘ�

	private:
		//! @brief �A�v���P�[�V���������s���� 
		int Run() noexcept;

		//! ���ɃC���X�^���X������Ă��邩
		static inline bool instantiated_ = false;
	};

	//! �N���C�A���g����`����
	std::unique_ptr<Application> CreateApp() noexcept;
}