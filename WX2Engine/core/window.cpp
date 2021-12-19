#include "window.h"

namespace wx2
{
	Window::Window(WindowContainer* container, WindowProperty windowProp) 
		: container_(container)
		, windowProp_(std::move(windowProp))
	{
		WX2_ASSERT(windowProp_.width > 0);
		WX2_ASSERT(windowProp_.height > 0);

		// �����_���Ƀ��j�[�NUUID�𐶐����A�E�B���h�E�N���X���ɂ���
		const boost::uuids::uuid uuid = boost::uuids::random_generator()();
		std::stringstream uuidStream;
		uuidStream << uuid;
		className_ = std::format("wx2eg-{}", uuidStream.str());

		// �C���X�^���X�n���h���擾
		const HINSTANCE hinst = GetModuleHandle(nullptr);

		// �E�B���h�E�N���X��o�^
		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_HREDRAW | CS_VREDRAW;	// �T�C�Y�ύX���ɍĕ`�惁�b�Z�[�W�𔭍s
		wcex.lpfnWndProc = HandleMessageSetup;
		wcex.hInstance = hinst;
		wcex.hbrBackground = GetStockBrush(WHITE_BRUSH);
		wcex.lpszClassName = className_.c_str();
		WX2_RUNTIME_ERROR_IF_FAILED(
			RegisterClassEx(&wcex), "�E�B���h�E�N���X�̓o�^�Ɏ��s���܂����B�G���[�R�[�h: {}", GetLastError());

		// �E�B���h�E���쐬
		hwnd_ = CreateWindowEx(
			windowProp_.exStyle,
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
			this);
		WX2_RUNTIME_ERROR_IF_FAILED(
			hwnd_, "�E�B���h�E�̍쐬�Ɏ��s���܂����B�G���[�R�[�h: {}", GetLastError());

		// �E�B���h�E�������A�X�V
		UpdateWindow(hwnd_);

		// �t���X�N���[���ݒ�
		if (windowProp_.fullscreen)
		{
			SetFullscreen(true);
		}
	}

	Window::~Window() noexcept
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
		auto* const window = std::bit_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		WX2_RUNTIME_ERROR_IF_FAILED(window, "�E�B���h�E�N���X���p�����[�^����擾�ł��܂���ł����B");

		switch (msg)
		{
		case WM_MOVING:	// �E�B���h�E�ړ���
			window->OnMoving(wp, lp);
			break;

		case WM_SIZING: // �E�B���h�E�T�C�Y�ύX��
			window->OnSizing(wp, lp);
			break;

		case WM_SYSCOMMAND:	// �E�B���h�E�ő剻�A�ʏ퉻��
			if (wp == SC_MAXIMIZE ||
				wp == SC_RESTORE)
			{
				window->OnDisplayModeChanged(wp, lp);
				return 0;
			}
			break;

		case WM_SETTEXT:	// �E�B���h�E�^�C�g���ύX��
			window->OnTitleChanged(wp, lp);
			break;

		case WM_KEYDOWN:	// �L�[������
			window->OnKeyDown(wp, lp);
			break;

		case WM_DESTROY:	// �E�B���h�E��j�������Ƃ�
			PostQuitMessage(0);
			return 0;

		case WM_CLOSE:	// �E�B���h�E������Ƃ�
			DestroyWindow(hwnd);
			return 0;

		default:
			break;
		}

		// ���I�ȃE�B���h�E�v���V�[�W���ɏ������Ϗ�����
		return window->container_->WindowProcedure(hwnd, msg, wp, lp);
	}

	LRESULT CALLBACK Window::HandleMessageSetup(const HWND hwnd, const  UINT msg, const WPARAM wp, const LPARAM lp)
	{
		// �p�����[�^����E�B���h�E�R���e�i�ɃL���X�g����
		const CREATESTRUCTW* const create = std::bit_cast<CREATESTRUCTW*>(lp);
		auto* const window = std::bit_cast<Window*>(create->lpCreateParams);

		if (msg == WM_NCCREATE)
		{
			WX2_RUNTIME_ERROR_IF_FAILED(window, "�E�B���h�E�N���X���p�����[�^����擾�ł��܂���ł����B");

			// �E�B���h�E�v���V�[�W���������ւ�
			SetWindowLongPtr(hwnd, GWLP_USERDATA, std::bit_cast<LONG_PTR>(window));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessageRedirect));

			// ���I�ȃE�B���h�E�v���V�[�W���ɏ������Ϗ�����
			return window->container_->WindowProcedure(hwnd, msg, wp, lp);
		}

		// �f�t�H���g�E�B���h�E�v���V�[�W��
		return DefWindowProc(hwnd, msg, wp, lp);
	}

	void Window::SetFullscreen(const bool fullscreen) noexcept
	{
		// �t���X�N���[���t���O�ύX
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
				~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE) & windowProp_.exStyle);

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

			// �ʏ펞�̃E�B���h�E�����L�^
			recordWindowX_ = windowProp_.x;
			recordWindowY_ = windowProp_.y;
			recordWindowWidth_ = windowProp_.width;
			recordWindowHeight_ = windowProp_.height;

			// �E�B���h�E���̈ʒu�ƃT�C�Y���f�B�X�v���C�Ɠ����ɂ���
			windowProp_.x = mi.rcMonitor.left;
			windowProp_.y = mi.rcMonitor.top;
			windowProp_.width = mi.rcMonitor.right - mi.rcMonitor.left;
			windowProp_.height = mi.rcMonitor.bottom - mi.rcMonitor.top;
		}
		else
		{
			// �E�B���h�E�X�^�C�����Đݒ�
			SetWindowLong(hwnd_, GWL_STYLE, static_cast<LONG>(windowProp_.style));
			SetWindowLong(hwnd_, GWL_EXSTYLE, static_cast<LONG>(windowProp_.exStyle));

			// �E�B���h�E�T�C�Y�𕜌�
			SetWindowPos(
				hwnd_,
				nullptr,
				recordWindowX_,
				recordWindowY_,
				recordWindowWidth_,
				recordWindowHeight_,
				SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

			// �E�B���h�E�����L�^�������ɖ߂�
			windowProp_.x = recordWindowX_;
			windowProp_.y = recordWindowY_;
			windowProp_.width = recordWindowWidth_;
			windowProp_.height = recordWindowHeight_;

			// �t���X�N���[���O�ɍő剻����Ă����Ȃ�ēx�ő剻
			if (windowProp_.maximized)
			{
				SetMaximize(true);
			}
		}
	}

	void Window::SetMaximize(const bool maximaize) noexcept
	{
		windowProp_.maximized = maximaize;

		// �ő剻or�ʏ퉻���b�Z�[�W�𔭍s
		SendMessage(hwnd_, WM_SYSCOMMAND, windowProp_.maximized ? SC_MAXIMIZE : SC_RESTORE, 0);
	}

	void Window::SetTitle(const std::string& title) noexcept
	{
		windowProp_.title = title;

		// �E�B���h�E�^�C�g����ݒ�
		SetWindowText(hwnd_, title.c_str());
	}

	void Window::OnMoving([[maybe_unused]] const  WPARAM wp, const  LPARAM lp) noexcept
	{
		// �p�����[�^����E�B���h�E�ʒu���擾
		const auto rect = std::bit_cast<RECT*>(lp);
		windowProp_.x = static_cast<int>(rect->left);
		windowProp_.y = static_cast<int>(rect->top);
		windowProp_.maximized = false;
		windowProp_.fullscreen = false;
	}

	void Window::OnSizing([[maybe_unused]] const  WPARAM wp, const  LPARAM lp) noexcept
	{
		// �p�����[�^����E�B���h�E�ʒu�ƃT�C�Y���擾
		const auto rect = std::bit_cast<RECT*>(lp);
		windowProp_.x = static_cast<int>(rect->left);
		windowProp_.y = static_cast<int>(rect->top);
		windowProp_.width = static_cast<int>(rect->right - rect->left);
		windowProp_.height = static_cast<int>(rect->bottom - rect->top);
		windowProp_.maximized = false;
		windowProp_.fullscreen = false;
	}

	void Window::OnDisplayModeChanged(const WPARAM wp, [[maybe_unused]] const  LPARAM lp) noexcept
	{
		// �p�����[�^����ő剻��Ԃ��擾
		if (wp == SC_MAXIMIZE) windowProp_.maximized = true;
		else if (wp == SC_RESTORE)  windowProp_.maximized = false;
	}

	void Window::OnTitleChanged(const WPARAM wp, const LPARAM lp) noexcept
	{
		// �p�����[�^����^�C�g�������擾
		windowProp_.title = std::string(reinterpret_cast<char*>(lp));
	}

	void Window::OnKeyDown(const WPARAM wp, [[maybe_unused]] const  LPARAM lp) noexcept
	{
		// F11�L�[�ŃE�B���h�E���ő剻
		if (wp == VK_F11) [[unlikely]]
		{
			SetFullscreen(!windowProp_.fullscreen);
		}
	}
}