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
		WNDCLASSEX wcex = { sizeof(wcex) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
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
		hwnd_ = CreateWindowEx(
			windowProp_->ex_style,
			className_.c_str(),
			windowProp_->title.c_str(),
			windowProp_->style,
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

		if (windowProp_->maximized)
		{
			ShowWindow(hwnd_, SW_MAXIMIZE);
		}
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
			case WM_MOVE: // �E�B���h�E�̈ړ�
			{
				window->windowProp_->x = static_cast<int>(LOWORD(lp));
				window->windowProp_->y = static_cast<int>(HIWORD(lp));
				return 0;
			}
			case WM_SIZE: // �E�B���h�E�̃T�C�Y�ύX
			{
				window->windowProp_->width = static_cast<int>(LOWORD(lp));
				window->windowProp_->height = static_cast<int>(HIWORD(lp));

				if (wp == SIZE_RESTORED)
				{
					window->windowProp_->maximized = false;
					window->windowProp_->fullscreen = false;
				}
				else if (wp == SIZE_MAXIMIZED)
				{
					window->windowProp_->maximized = true;
				}

				return 0;
			}
			case WM_KEYDOWN:
			{
				if (wp == VK_F11)
				{
					window->SetFullscreen(!window->windowProp_->fullscreen);
				}
				return 0;
			}
			case WM_CLOSE: // �E�B���h�E�����
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

	void Window::SetFullscreen(bool fullscreen)
	{
		if (windowProp_->fullscreen == fullscreen)
		{
			return;
		}
		windowProp_->fullscreen = fullscreen;

		if (windowProp_->fullscreen)
		{
			SetWindowLong(hwnd_, GWL_STYLE,
				windowProp_->style & ~(WS_CAPTION | WS_THICKFRAME));
			SetWindowLong(hwnd_, GWL_EXSTYLE, 
				windowProp_->ex_style & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));

			MONITORINFO mi = { sizeof(mi) };
			GetMonitorInfo(MonitorFromWindow(hwnd_, MONITOR_DEFAULTTONEAREST), &mi);
			SetWindowPos(
				hwnd_, 
				nullptr, 
				mi.rcMonitor.left, 
				mi.rcMonitor.top,
				mi.rcMonitor.right - mi.rcMonitor.left,
				mi.rcMonitor.bottom - mi.rcMonitor.top,
				SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
		}
		else
		{
			// �E�B���h�E���Đݒ�
			SetWindowLong(hwnd_, GWL_STYLE, windowProp_->style);
			SetWindowLong(hwnd_, GWL_EXSTYLE, windowProp_->ex_style);

			SetWindowPos(
				hwnd_, 
				nullptr, 
				windowProp_->x,
				windowProp_->y,
				windowProp_->width,
				windowProp_->height,
				SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
			if (windowProp_->maximized)
				::SendMessage(hwnd_, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
	}
}