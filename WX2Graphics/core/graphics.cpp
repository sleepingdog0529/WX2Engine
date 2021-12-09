#include "graphics.h"

namespace wx2::graphics
{
	bool Graphics::Initialize(HWND hwnd, const WindowProperty& windowProp, bool vsync) noexcept
	{
		try
		{
			windowProperty_ = windowProp;

			devices_.Initialize();

			auto* dev = devices_.GetDevice();
			auto* devCon = devices_.GetDeviceContext();

			HRESULT hr;

#if !defined(NDEBUG)
			hr = dev->QueryInterface(IID_PPV_ARGS(debug_.GetAddressOf()));
			WX2_COM_ERROR_IF_FAILED(hr, "ID3Dデバッグの登録に失敗しました。");
#endif

			// インターフェース取得
			ComPtr<IDXGIDevice> dxgiDevice;
			hr = dev->QueryInterface(IID_PPV_ARGS(dxgiDevice.GetAddressOf()));
			WX2_COM_ERROR_IF_FAILED(hr, "DXGIデバイスの問い合わせに失敗しました。");

			// アダプター取得
			ComPtr<IDXGIAdapter> adapter;
			hr = dxgiDevice->GetAdapter(adapter.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "DXGIアダプタの取得に失敗しました。");

			// ファクトリー取得
			ComPtr<IDXGIFactory> factory;
			hr = adapter->GetParent(IID_PPV_ARGS(factory.GetAddressOf()));
			WX2_COM_ERROR_IF_FAILED(hr, "DXGIファクトリーの取得に失敗しました。");

			// 利用可能なMSAAを取得
			DXGI_SAMPLE_DESC msaa = {};
			for (int i = 0; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i++)
			{
				UINT quality;
				if (SUCCEEDED(dev->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &quality)))
				{
					if (0 < quality)
					{
						msaa.Count = i;
						msaa.Quality = quality - 1;
					}
				}
			}

			// スワップチェイン設定
			DXGI_SWAP_CHAIN_DESC scd{};
			scd.OutputWindow = hwnd;
			scd.BufferDesc.Width = windowProperty_.width;
			scd.BufferDesc.Height = windowProperty_.height;
			scd.Windowed = !windowProperty_.fullscreen;
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

			hr = factory->CreateSwapChain(dev, &scd, swapChain_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "スワップチェインの作成に失敗しました。");

			// スワップチェインのバッファ取得
			hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(backBuffer_.GetAddressOf()));
			WX2_COM_ERROR_IF_FAILED(hr, "バックバッファの作成に失敗しました。");

			// レンダーターゲットビュー作成
			hr = dev->CreateRenderTargetView(backBuffer_.Get(), nullptr, renderTargetView_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "レンダーターゲットビューの作成に失敗しました。");

			// デプスステンシルバッファ作成
			CD3D11_TEXTURE2D_DESC dsbd(DXGI_FORMAT_D24_UNORM_S8_UINT, windowProperty_.width, windowProperty_.height);
			dsbd.MipLevels = 1;
			dsbd.SampleDesc = msaa;
			dsbd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			hr = dev->CreateTexture2D(&dsbd, nullptr, depthStencilBuffer_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "デプスステンシルバッファの作成に失敗しました。");

			// デプスステンシルビュー作成
			hr = dev->CreateDepthStencilView(depthStencilBuffer_.Get(), nullptr, depthStencilView_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "デプスステンシルビューの作成に失敗しました。");

			// レンダーターゲットビュー設定
			devCon->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());

			// デプスステンシルステート作成
			CD3D11_DEPTH_STENCIL_DESC dsd(D3D11_DEFAULT);
			hr = dev->CreateDepthStencilState(&dsd, depthStencilState_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "デプスステンシルステートの作成に失敗しました。");

			// ビューポート設定
			viewport_.TopLeftX = 0.0f;
			viewport_.TopLeftY = 0.0f;
			viewport_.Width = static_cast<float>(windowProperty_.width);
			viewport_.Height = static_cast<float>(windowProperty_.height);
			viewport_.MinDepth = 0.0f;
			viewport_.MaxDepth = 1.0f;

			// ラスタライザステートの作成
			CD3D11_RASTERIZER_DESC rd(D3D11_DEFAULT);
			dev->CreateRasterizerState(&rd, rasterizerState_.GetAddressOf());

			blendState_.Initialize(&devices_);

			constantBufferWVP_.Initialize(&devices_);

			DWORD indices[] = { 0,1,2,0,2,3 };
			indexBuffer_.Initialize(&devices_, indices);

			InitializePipeline();
			InitializeGraphics();
		}
		catch (const COMException& exception)
		{
			WX2_LOG_ERROR(exception.what());
			return false;
		}
		return true;
	}

	void Graphics::DrawBegin() noexcept
	{
		auto* devCon = devices_.GetDeviceContext();

		devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		blendState_.Bind(BlendState::Mode::Default);

		constexpr float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
		devCon->ClearRenderTargetView(renderTargetView_.Get(), clearColor);
		devCon->ClearDepthStencilView(
			depthStencilView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		devCon->RSSetViewports(1, &viewport_);
		devCon->RSSetState(rasterizerState_.Get());
		devCon->OMSetDepthStencilState(
			depthStencilState_.Get(), 0);
		devCon->OMSetRenderTargets(
			1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());

		devCon->PSSetSamplers(0, 1, samplerState_.GetAddressOf());
	}

	void Graphics::DrawEnd() const noexcept
	{
		swapChain_->Present(0, 0);
	}

	void Graphics::RenderFrame() noexcept
	{
		DrawBegin();

		vertexShader_.Bind();
		pixelShader_.Bind();

		static float rad = 0.0f;
		rad += 0.01f;

		constantBufferWVP_.data.world = Matrix::World(
			Vector3::Zero(),
			Vector3::Forward(),
			Vector3::Up());
		constantBufferWVP_.data.projection = Matrix::PerspectiveFieldOfView(
			PIDIV4,
			static_cast<float>(windowProperty_.width) / static_cast<float>(windowProperty_.height),
			0.01f,
			1000.0f);
		constantBufferWVP_.data.view = Matrix::LookAt(
			Vector3::Transform(Vector3(0.0f, 10.0f, 50.0f), Matrix::RotationY(rad)),
			Vector3(0.0f, 10.0f, 0.0f),
			Vector3::Up());
		constantBufferWVP_.ApplyChange();
		constantBufferWVP_.VSBind(0);

		model_.Draw(Matrix::Scale(0.01f));

		DrawEnd();
	}

	void Graphics::InitializePipeline()
	{
		DirectX::XMFLOAT3 vertices[] =
		{
			{ DirectX::XMFLOAT3(-0.5f,-0.5f,0) },
			{ DirectX::XMFLOAT3(0.5f,-0.5f,0)  },
			{ DirectX::XMFLOAT3(0.5f, 0.5f,0)  },
			{ DirectX::XMFLOAT3(-0.5f, 0.5f,0) }
		};
		vertexBuffer_.Initialize(&devices_, vertices);

		D3D11_INPUT_ELEMENT_DESC layoutDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		vertexShader_.Initialize(&devices_, ".\\asset\\shader\\simple.hlsl", layoutDescs);

		pixelShader_.Initialize(&devices_, ".\\asset\\shader\\simple.hlsl");
	}

	void Graphics::InitializeGraphics()
	{
		auto* dev = devices_.GetDevice();
		auto* devCon = devices_.GetDeviceContext();

		blendState_.Bind(BlendState::Mode::Default);

		CD3D11_SAMPLER_DESC sd(D3D11_DEFAULT);
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		HRESULT hr = dev->CreateSamplerState(&sd, samplerState_.GetAddressOf());
		WX2_COM_ERROR_IF_FAILED(hr, "サンプラーステートの作成に失敗しました。");

		modelLoader_.Initialize(&devices_, &constantBufferWVP_);
		model_ = modelLoader_.Load(".\\asset\\model\\WG.fbx");
	}
}