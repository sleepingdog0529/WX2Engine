#include "window.h"

namespace wx2
{
	LRESULT CALLBACK HandleMessageRedirect(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	Window::Window(WindowContainer* container, const WindowProperty& windowProp) :
		container_(container),
		windowProp_(windowProp)
	{
		WX2_ASSERT(windowProp_.width > 0);
		WX2_ASSERT(windowProp_.height > 0);

		// �����_���Ƀ��j�[�NID�𐶐�
		const boost::uuids::uuid uuid = boost::uuids::random_generator()();
		std::stringstream uuid_ss;
		uuid_ss << uuid;
		className_ = std::format("wx2eg-{}", uuid_ss.str());

		// �C���X�^���X�n���h���擾
		const HINSTANCE hinst = GetModuleHandle(nullptr);

		// �E�B���h�E�N���X��o�^
		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(wcex);
		wcex.lpfnWndProc = HandleMessageSetup;
		wcex.hInstance = hinst;
		wcex.lpszClassName = className_.c_str();
		if (!RegisterClassEx(&wcex))
		{
			WX2_LOG_ERROR("�E�B���h�E�N���X�̓o�^�Ɏ��s���܂����B�G���[�R�[�h:\"{}\"", GetLastError());
			exit(EXIT_FAILURE);
		}

		// �E�B���h�E���쐬
		hwnd_ = CreateWindow(
			className_.c_str(),
			windowProp_.title.c_str(),
			WS_OVERLAPPED | WS_CAPTION,
			windowProp_.x,
			windowProp_.y,
			windowProp_.width,
			windowProp_.height,
			nullptr,
			nullptr,
			hinst,
			container_
		);
		if (!hwnd_)
		{
			WX2_LOG_ERROR("�E�B���h�E�̍쐬�Ɏ��s���܂����B�G���[�R�[�h:\"{}\"", GetLastError());
			exit(EXIT_FAILURE);
		}

		// �E�B���h�E�������A�X�V
		ShowWindow(hwnd_, SW_SHOW);
		UpdateWindow(hwnd_);
	}

	Window::~Window()
	{
		// �E�B���h�E��o�^�������A�j������
		if (hwnd_)
		{
			UnregisterClass(className_.c_str(), GetModuleHandle(nullptr));
			DestroyWindow(hwnd_);
		}
	}

	LRESULT CALLBACK HandleMessageRedirect(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;
		default:
		{
			// �p�����[�^����E�B���h�E�R���e�i�ɃL���X�g����
			WindowContainer* const wndContainer = 
				reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			// ���I�ȃE�B���h�E�v���V�[�W���ɏ������Ϗ�����
			return wndContainer->WindowProcedure(hwnd, msg, wp, lp);
		}
		}
	}

	LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_NCCREATE:
		{
			// �p�����[�^����E�B���h�E�R���e�i�ɃL���X�g����
			const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lp);
			WindowContainer* wndContainer = 
				reinterpret_cast<WindowContainer*>(create->lpCreateParams);

			if (!wndContainer)
			{
				WX2_LOG_CRITICAL("windows_container��nullptr�ł��B");
				exit(EXIT_FAILURE);
			}

			// �E�B���h�E�v���V�[�W���������ւ�
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wndContainer));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessageRedirect));

			// ���I�ȃE�B���h�E�v���V�[�W���ɏ������Ϗ�����
			return wndContainer->WindowProcedure(hwnd, msg, wp, lp);
		}
		default:
			return DefWindowProc(hwnd, msg, wp, lp);
		}
	}
}