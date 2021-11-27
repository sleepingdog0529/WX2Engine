#include "graphics.h"

namespace wx2::graphics
{
	bool Graphics::Initialize(HWND hwnd, const WindowProperty& windowProp, bool vsync) noexcept
	{
		try
		{
			devices_.Initialize();

			auto* device = devices_.GetDevice();
			auto* deviceContext = devices_.GetDeviceContext();

			// �C���^�[�t�F�[�X�擾
			ComPtr<IDXGIDevice> dxgiDevice;
			HRESULT hr = device->QueryInterface(IID_PPV_ARGS(dxgiDevice.GetAddressOf()));
			WX2_COM_ERROR_IF_FAILED(hr, "DXGI�f�o�C�X�̖₢���킹�Ɏ��s���܂����B");

			// �A�_�v�^�[�擾
			ComPtr<IDXGIAdapter> adapter;
			hr = dxgiDevice->GetAdapter(adapter.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "DXGI�A�_�v�^�̎擾�Ɏ��s���܂����B");

			// �t�@�N�g���[�擾
			ComPtr<IDXGIFactory> factory;
			hr = adapter->GetParent(IID_PPV_ARGS(factory.GetAddressOf()));
			WX2_COM_ERROR_IF_FAILED(hr, "DXGI�t�@�N�g���[�̎擾�Ɏ��s���܂����B");

			// ���p�\��MSAA���擾
			DXGI_SAMPLE_DESC msaa = {};
			for (int i = 0; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i++)
			{
				UINT quality;
				if (SUCCEEDED(device->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &quality)))
				{
					if (0 < quality)
					{
						msaa.Count = i;
						msaa.Quality = quality - 1;
					}
				}
			}

			// �X���b�v�`�F�C���ݒ�
			DXGI_SWAP_CHAIN_DESC scd{};
			scd.OutputWindow = hwnd;
			scd.BufferDesc.Width = windowProp.width;
			scd.BufferDesc.Height = windowProp.height;
			scd.Windowed = !windowProp.fullscreen;
			scd.BufferDesc.RefreshRate.Numerator = 60;
			scd.BufferDesc.RefreshRate.Denominator = 1;
			scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			scd.BufferCount = 1;
			scd.SampleDesc = msaa;
			scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			hr = factory->CreateSwapChain(device, &scd, swapChain_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "�X���b�v�`�F�C���̍쐬�Ɏ��s���܂����B");

			// �X���b�v�`�F�C���̃o�b�t�@�擾
			hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(backBuffer_.GetAddressOf()));
			WX2_COM_ERROR_IF_FAILED(hr, "�o�b�N�o�b�t�@�̍쐬�Ɏ��s���܂����B");

			// �����_�[�^�[�Q�b�g�r���[�쐬
			hr = device->CreateRenderTargetView(backBuffer_.Get(), nullptr, renderTargetView_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "�����_�[�^�[�Q�b�g�r���[�̍쐬�Ɏ��s���܂����B");

			// �f�v�X�X�e���V���o�b�t�@�쐬
			CD3D11_TEXTURE2D_DESC dsbd(DXGI_FORMAT_D24_UNORM_S8_UINT, windowProp.width, windowProp.height);
			dsbd.MipLevels = 1;
			dsbd.SampleDesc = msaa;
			dsbd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			hr = device->CreateTexture2D(&dsbd, nullptr, depthStencilBuffer_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "�f�v�X�X�e���V���o�b�t�@�̍쐬�Ɏ��s���܂����B");

			// �f�v�X�X�e���V���r���[�쐬
			hr = device->CreateDepthStencilView(depthStencilBuffer_.Get(), nullptr, depthStencilView_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "�f�v�X�X�e���V���r���[�̍쐬�Ɏ��s���܂����B");

			// �����_�[�^�[�Q�b�g�r���[�ݒ�
			deviceContext->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());

			// �f�v�X�X�e���V���X�e�[�g�쐬
			CD3D11_DEPTH_STENCIL_DESC dsd(D3D11_DEFAULT);
			hr = device->CreateDepthStencilState(&dsd, depthStencilState_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "�f�v�X�X�e���V���X�e�[�g�̍쐬�Ɏ��s���܂����B");

			// �r���[�|�[�g�ݒ�
			viewport_.TopLeftX = 0.0f;
			viewport_.TopLeftY = 0.0f;
			viewport_.Width = static_cast<float>(windowProp.width);
			viewport_.Height = static_cast<float>(windowProp.height);
			viewport_.MinDepth = 0.0f;
			viewport_.MaxDepth = 1.0f;

			// ���X�^���C�U�X�e�[�g�̍쐬
			CD3D11_RASTERIZER_DESC rd(D3D11_DEFAULT);
			device->CreateRasterizerState(&rd, rasterizerState_.GetAddressOf());

			blendState_.Initialize(&devices_);

			constantBufferWVP_.Initialize(&devices_);

			DWORD indices[] = { 0, 1, 2 };
			indexBuffer_.Initialize(&devices_, indices);

			DirectX::XMFLOAT3 vertices[] = {
				{ 0,  0,  0},
				{ 1, -1,  0},
				{-1, -1,  0} };
			vertexBuffer_.Initialize(&devices_, vertices);

			D3D11_INPUT_ELEMENT_DESC layoutDescs[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			vertexShader_.Initialize(&devices_, ".\\shader\\simple.hlsl", layoutDescs);

			pixelShader_.Initialize(&devices_, ".\\shader\\simple.hlsl");
		}
		catch (const COMException& exception)
		{
			WX2_LOG_ERROR(exception.what());
			return false;
		}
		return true;
	}
}