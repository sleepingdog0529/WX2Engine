/*********************************************************************
 * @file	window_container.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:36
 * @brief	�����̃E�B���h�E�ƃ��b�Z�[�W�̊Ǘ�
 ********************************************************************/
#pragma once
#include "window.h"
#include "window_property.h"

namespace wx2
{
	class Window;

	/**
	 * @brief �E�B���h�E���i�[����R���e�i�N���X
	 */
	class WindowContainer
	{
	private:
		using WindowPtr = std::shared_ptr<Window>;

		//! �E�B���h�E�ݒ�̕ۑ���p�X
		static constexpr const char* WINDOW_PROPERTY_PATH_ = ".\\window_property.json";

	public:
		/**
		 * @brief �E�B���h�E�ݒ�̃f�V���A���C�Y���s��
		 */
		WindowContainer();

		/**
		 * @brief �E�B���h�E�ݒ�̃V���A���C�Y���s��
		 */
		virtual ~WindowContainer();

		WX2_DISALLOW_COPY_AND_MOVE(WindowContainer);

		/**
		 * @brief     �E�B���h�E�𐶐�����
		 * @param[in] name �o�^��
		 * @param[in] defaultProp �V���A���C�Y�ς݂̃f�[�^�������ꍇ�Ɏg�p����E�B���h�E�ݒ�
		 * @return    WindowPtr �쐬�����E�B���h�E�̃|�C���^
		 */
		WindowPtr Create(const std::string& name, const WindowProperty& defaultProp);

		/**
		 * @brief     ���b�Z�[�W�����������ԂɍX�V�������Ăяo��
		 * @param[in] process �A�v���P�[�V�����̍X�V�֐�
		 */
		void ProcessMessages(std::function<bool()> process);

		/**
		 * @brief �A�v���P�[�V�����̑S�ẴE�B���h�E���ʂ̃E�B���h�E�v���V�[�W��
		 */
		LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	private:
		/**
		 * @brief �E�B���h�E�ݒ���V���A���C�Y�����ăt�@�C���ɕۑ�����
		 */
		void Serialize();

		/**
		 * @brief �E�B���h�E�ݒ���V���A���C�Y�����ăt�@�C���ɕۑ�����
		 */
		void Deserialize();

		std::unordered_map<std::string, WindowPtr> windows_; //! �E�B���h�E�|�C���^�̘A�z�z��
		std::unordered_map<std::string, WindowProperty> windowProps_; //! �E�B���h�E�ݒ�̘A�z�z��
	};
}