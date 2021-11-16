#include "window.h"

namespace wx2
{
	Window::Window(WindowContainer* container, const WindowProperty& windowProp) :
		container_(container),
		windowProp_(windowProp)
	{
		WX2_ASSERT(windowProp_.width > 0);
		WX2_ASSERT(windowProp_.height > 0);

		// �����_���Ƀ��j�[�NID�𐶐�
		const boost::uuids::uuid uuid = boost::uuids::random_generator()();
		std::stringstream uuidStream;
		uuidStream << uuid;
		className_ = std::format("wx2eg-{}", uuidStream.str());

		// �C���X�^���X�n���h���擾
		const HINSTANCE hinst = GetModuleHandle(nullptr);

		// �E�B���h�E�N���X��o�^
		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = HandleMessageSetup;
		wcex.hInstance = hinst;
		wcex.hbrBackground = GetStockBrush(WHITE_BRUSH);
		wcex.lpszClassName = className_.c_str();
		if (!RegisterClassEx(&wcex)) [[unlikely]]
		{
			WX2_LOG_ERROR("�E�B���h�E�N���X�̓o�^�Ɏ��s���܂����B�G���[�R�[�h: {}", GetLastError());
			exit(EXIT_FAILURE);
		}

		// �E�B���h�E���쐬
		hwnd_ = CreateWindowEx(
			windowProp_.ex_style,
			className_.c_str(),
			windowProp_.title.c_str(),
			windowProp_.style,
			windowProp_.x,
			windowProp_.y,
			windowProp_.width,
			windowProp_.height,
			nullptr,
			nullptr,
			hinst,
			this
		);
		if (!hwnd_) [[unlikely]]
		{
			WX2_LOG_ERROR("�E�B���h�E�̍쐬�Ɏ��s���܂����B�G���[�R�[�h: {}", GetLastError());
			exit(EXIT_FAILURE);
		}

		// �E�B���h�E�������A�X�V
		UpdateWindow(hwnd_);

		SetFullscreen(windowProp_.fullscreen);
	}

	Window::~Window()
	{
		// �E�B���h�E��o�^�������A�j������
		if (hwnd_) [[likely]]
		{
			UnregisterClass(className_.c_str(), GetModuleHandle(nullptr));
			DestroyWindow(hwnd_);
		}
	}

	LRESULT CALLBACK Window::HandleMessageRedirect(const HWND hwnd, const UINT msg, const WPARAM wp, const LPARAM lp)
	{
		// �p�����[�^����E�B���h�E�R���e�i�ɃL���X�g����
		Window* const window = std::bit_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (!window) [[unlikely]]
		{
			WX2_LOG_CRITICAL("�E�B���h�E�N���X���p�����[�^����擾�ł��܂���ł����B");
			exit(EXIT_FAILURE);
		}

		const WindowContainer* container = window->container_;

		switch (msg)
		{
			case WM_MOVING: 
				window->OnMoving(wp, lp); 
				return TRUE;

			case WM_SIZING: 
				window->OnSizing(wp, lp); 
				return TRUE;

			case WM_SYSCOMMAND:
				window->OnDisplayModeChanged(wp, lp);
				break;
				
			case WM_KEYDOWN:
				window->OnKeyDown(wp, lp);
				break;

			case WM_DESTROY:
				PostQuitMessage(0);
				break;

			case WM_CLOSE:
				DestroyWindow(hwnd);
				break;

			default:
				container->WindowProcedure(hwnd, msg, wp, lp);
				break;
		}
		return 0;
	}

	LRESULT CALLBACK Window::HandleMessageSetup(const HWND hwnd, const  UINT msg, const WPARAM wp, const LPARAM lp)
	{
		// �p�����[�^����E�B���h�E�R���e�i�ɃL���X�g����
		const CREATESTRUCTW* const create = std::bit_cast<CREATESTRUCTW*>(lp);
		Window* const window = std::bit_cast<Window*>(create->lpCreateParams);

		if(msg == WM_NCCREATE)
		{
			if (!window) [[unlikely]]
			{
				WX2_LOG_CRITICAL("windows_container��nullptr�ł��B");
				exit(EXIT_FAILURE);
			}

			// �E�B���h�E�v���V�[�W���������ւ�
			SetWindowLongPtr(hwnd, GWLP_USERDATA, std::bit_cast<LONG_PTR>(window));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessageRedirect));

			// ���I�ȃE�B���h�E�v���V�[�W���ɏ������Ϗ�����
			return window->container_->WindowProcedure(hwnd, msg, wp, lp);
		}

		return DefWindowProc(hwnd, msg, wp, lp);
	}

	void Window::SetFullscreen(const bool fullscreen)
	{
		windowProp_.fullscreen = fullscreen;

		if (windowProp_.fullscreen)
		{
			// �E�B���h�E�X�^�C������t���X�N���[����
			// �s�v�ȃt���O����菜���ēK��
			SetWindowLong(
				hwnd_, 
				GWL_STYLE,
				~(WS_CAPTION | WS_THICKFRAME) & windowProp_.style);
			SetWindowLong(
				hwnd_, 
				GWL_EXSTYLE,
				~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE) & windowProp_.ex_style);

			// �E�B���h�E�T�C�Y�����j�^�[�T�C�Y�Ɠ����ɂ���
			MONITORINFO mi{};
			mi.cbSize = sizeof(mi);
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
			// �E�B���h�E�X�^�C�����Đݒ�
			SetWindowLong(hwnd_, GWL_STYLE, static_cast<LONG>(windowProp_.style));
			SetWindowLong(hwnd_, GWL_EXSTYLE, static_cast<LONG>(windowProp_.ex_style));

			// �E�B���h�E�T�C�Y�𕜌�
			SetWindowPos(
				hwnd_,
				nullptr,
				windowProp_.x,
				windowProp_.y,
				windowProp_.width,
				windowProp_.height,
				SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

			// �t���X�N���[���O�ɍő剻����Ă�����ēx�ő剻
			if (windowProp_.maximized)
			{
				SetMaximize(true);
			}
		}
	}

	void Window::SetMaximize(const bool maximaize)
	{
		windowProp_.maximized = maximaize;
		SendMessage(hwnd_, WM_SYSCOMMAND, windowProp_.maximized ? SC_MAXIMIZE : SC_RESTORE, 0);
	}

	void Window::OnMoving([[maybe_unused]] WPARAM wp, LPARAM lp)
	{
		auto rect = std::bit_cast<RECT*>(lp);
		windowProp_.x = static_cast<int>(rect->left);
		windowProp_.y = static_cast<int>(rect->top);
		windowProp_.maximized = false;
		windowProp_.fullscreen = false;
	}

	void Window::OnSizing([[maybe_unused]] WPARAM wp, LPARAM lp)
	{
		auto rect = std::bit_cast<RECT*>(lp);
		windowProp_.x = static_cast<int>(rect->left);
		windowProp_.y = static_cast<int>(rect->top);
		windowProp_.width = static_cast<int>(rect->right - rect->left);
		windowProp_.height = static_cast<int>(rect->bottom - rect->top);
		windowProp_.maximized = false;
		windowProp_.fullscreen = false;
	}

	void Window::OnDisplayModeChanged(WPARAM wp, [[maybe_unused]] LPARAM lp)
	{
		if      (wp == SC_MAXIMIZE) windowProp_.maximized = true;
		else if (wp == SC_RESTORE)  windowProp_.maximized = false;
	}

	void Window::OnKeyDown(WPARAM wp, [[maybe_unused]] LPARAM lp)
	{
		if (wp == VK_F11) [[unlikely]]
		{
			SetFullscreen(!windowProp_.fullscreen);
		}
	}
}