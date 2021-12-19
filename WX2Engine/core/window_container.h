/*********************************************************************
 * @file	window_container.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:36
 * @brief	�����̃E�B���h�E�ƃ��b�Z�[�W�̊Ǘ�
 ********************************************************************/
#pragma once
#include "window.h"
#include "window_event.h"

namespace wx2
{
	class Window;

	/**
	 * @brief �E�B���h�E���i�[����R���e�i�N���X
	 */
	class WindowContainer final
	{
	private:
		using WindowPtr = std::shared_ptr<Window>;
		using EventListenerType = EventListener<WindowEvent, HWND, WPARAM, LPARAM>;
		using EventCallbackType = std::function<void(HWND, WPARAM, LPARAM)>;

		//! �E�B���h�E�ݒ�̕ۑ���p�X
		static constexpr const char* PROPERTY_FILE_NAME = "window.json";

	public:
		/**
		 * @brief �E�B���h�E�ݒ�̃f�V���A���C�Y���s��
		 */
		WindowContainer() noexcept;

		/**
		 * @brief �E�B���h�E�ݒ�̃V���A���C�Y���s��
		 */
		virtual ~WindowContainer() noexcept;

		WX2_DISALLOW_COPY(WindowContainer);
		WX2_DISALLOW_MOVE(WindowContainer);

		/**
		 * @brief     �E�B���h�E�𐶐�����
		 * @param[in] name �o�^��
		 * @param[in] defaultProp �V���A���C�Y�ς݂̃f�[�^�������ꍇ�Ɏg�p����E�B���h�E�ݒ�
		 * @return    WindowPtr �쐬�����E�B���h�E�̃|�C���^
		 */
		WindowPtr Create(const std::string& name, const WindowProperty& defaultProp);


		EventListenerType AppendCallback(
			WindowEvent event,
			const EventCallbackType& callback) noexcept;

		/**
		 * @brief  �S�ẴE�B���h�E�̃��b�Z�[�W�����������
		 * @return WM_QUIT���������Ă��Ȃ����true
		 */
		static bool ProcessMessage() noexcept;

		/**
		 * @brief �A�v���P�[�V�����̑S�ẴE�B���h�E���ʂ̃E�B���h�E�v���V�[�W��
		 */
		LRESULT CALLBACK WindowProcedure(const HWND hwnd, const UINT msg, const WPARAM wp, const LPARAM lp) noexcept;

	private:
		/**
		 * @brief �E�B���h�E�ݒ���V���A���C�Y�����ăt�@�C���ɕۑ�����
		 */
		void Serialize() noexcept;

		/**
		 * @brief �E�B���h�E�ݒ���V���A���C�Y�����ăt�@�C���ɕۑ�����
		 */
		void Deserialize() noexcept;

		//! �E�B���h�E�|�C���^�̘A�z�z��
		std::unordered_map<std::string, WindowPtr> windows_;

		//! �E�B���h�E�ݒ�̘A�z�z��
		std::unordered_map<std::string, WindowProperty> windowProps_;

		//! �E�B���h�E�C�x���g���s
		EventDispatcher<WindowEvent, HWND, WPARAM, LPARAM> eventDispatcher_;
	};
}