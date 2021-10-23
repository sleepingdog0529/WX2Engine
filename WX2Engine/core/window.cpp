#include "window.h"

namespace wx2
{
	Window::Window(WindowContainer* container, WindowPropertyPtr windowProp) :
		container_(container),
		windowProp_(windowProp)
	{
		WX2_ASSERT(windowProp_->width > 0);
		WX2_ASSERT(windowProp_->height > 0);

		// �����_���Ƀ��j�[�NID�𐶐�
		const boost::uuids::uuid uuid = boost::uuids::random_generator()();
		std::stringstream uuid_ss;
		uuid_ss << uuid;
		className_ = std::format("wx2eg-{}", uuid_ss.str());

		// �C���X�^���X�n���h���擾
		const HINSTANCE hinst = GetModuleHandle(nullptr);

		// �E�B���h�E�N���X��o�^
		WNDCLASSEX wcex = {};
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.cbSize = sizeof(wcex);
		wcex.lpfnWndProc = HandleMessageSetup;
		wcex.hInstance = hinst;
		wcex.hbrBackground = GetStockBrush(WHITE_BRUSH);
		wcex.lpszClassName = className_.c_str();
		if (!RegisterClassEx(&wcex))
		{
			WX2_LOG_ERROR("�E�B���h�E�N���X�̓o�^�Ɏ��s���܂����B�G���[�R�[�h:\"{}\"", GetLastError());
			exit(EXIT_FAILURE);
		}

		// �E�B���h�E���쐬
		hwnd_ = CreateWindow(
			className_.c_str(),
			windowProp_->title.c_str(),
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			windowProp_->x,
			windowProp_->y,
			windowProp_->width,
			windowProp_->height,
			nullptr,
			nullptr,
			hinst,
			this
		);
		if (!hwnd_)
		{
			WX2_LOG_ERROR("�E�B���h�E�̍쐬�Ɏ��s���܂����B�G���[�R�[�h:\"{}\"", GetLastError());
			exit(EXIT_FAILURE);
		}

		// �E�B���h�E�������A�X�V
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

	LRESULT CALLBACK Window::HandleMessageRedirect(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		// �p�����[�^����E�B���h�E�R���e�i�ɃL���X�g����
		Window* const window =
			reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		switch (msg)
		{
			case WM_MOVE:
			{
				window->windowProp_->x = static_cast<int>(LOWORD(lp));
				window->windowProp_->y = static_cast<int>(HIWORD(lp));
				return 0;
			}
			case WM_SIZE:
			{
				window->windowProp_->width = static_cast<int>(LOWORD(lp));
				window->windowProp_->height = static_cast<int>(HIWORD(lp));

				if (wp == SIZE_MAXIMIZED)
				{
					window->windowProp_->fullscreen = true;
				}
				else if (wp == SIZE_RESTORED)
				{
					window->windowProp_->fullscreen = false;
				}

				return 0;
			}
			case WM_CLOSE:
			{
				DestroyWindow(hwnd);
				return 0;
			}
		}

		// ���I�ȃE�B���h�E�v���V�[�W���ɏ������Ϗ�����
		return window->container_->WindowProcedure(hwnd, msg, wp, lp);
	}

	LRESULT CALLBACK Window::HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		// �p�����[�^����E�B���h�E�R���e�i�ɃL���X�g����
		const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lp);
		Window* window = reinterpret_cast<Window*>(create->lpCreateParams);

		if(msg == WM_NCCREATE)
		{
			if (!window)
			{
				WX2_LOG_CRITICAL("windows_container��nullptr�ł��B");
				exit(EXIT_FAILURE);
			}

			// �E�B���h�E�v���V�[�W���������ւ�
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessageRedirect));

			// ���I�ȃE�B���h�E�v���V�[�W���ɏ������Ϗ�����
			return window->container_->WindowProcedure(hwnd, msg, wp, lp);
		}

		return DefWindowProc(hwnd, msg, wp, lp);
	}
}