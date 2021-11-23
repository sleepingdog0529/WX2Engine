#include "graphics.h"

namespace wx2
{
	bool Graphics::Initialize(HWND hwnd, const WindowProperty& windowProp, bool vsync)
	{
		HRESULT hr;

		try
		{
			// 機能レベル
			D3D_FEATURE_LEVEL supportedFeatureLevel = D3D_FEATURE_LEVEL_1_0_CORE;
			std::array<D3D_FEATURE_LEVEL, 2> featureLevels = {
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0
			};

			UINT frags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
			if (IS_DEBUGGING)
			{
				frags |= D3D11_CREATE_DEVICE_DEBUG;
			}

			// デバイス作成
			hr = D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				frags,
				featureLevels.data(),
				featureLevels.size(),
				D3D11_SDK_VERSION,
				device_.GetAddressOf(),
				&supportedFeatureLevel,
				deviceContext_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "デバイスの作成に失敗しました。");

			// インターフェース取得
			ComPtr<IDXGIDevice> dxgiDevice;
			hr = device_->QueryInterface(IID_PPV_ARGS(dxgiDevice.GetAddressOf()));
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
				if (SUCCEEDED(device_->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &quality)))
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

			hr = factory->CreateSwapChain(device_.Get(), &scd, swapChain_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "スワップチェインの作成に失敗しました。");

			// スワップチェインのバッファ取得
			hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(backBuffer_.GetAddressOf()));
			WX2_COM_ERROR_IF_FAILED(hr, "バックバッファの作成に失敗しました。");

			// レンダーターゲットビュー作成
			hr = device_->CreateRenderTargetView(backBuffer_.Get(), nullptr, renderTargetView_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "レンダーターゲットビューの作成に失敗しました。");

			// デプスステンシルバッファ作成
			CD3D11_TEXTURE2D_DESC dsbd(DXGI_FORMAT_D24_UNORM_S8_UINT, windowProp.width, windowProp.height);
			dsbd.MipLevels = 1;
			dsbd.SampleDesc = msaa;
			dsbd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			hr = device_->CreateTexture2D(&dsbd, nullptr, depthStencilBuffer_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "デプスステンシルバッファの作成に失敗しました。");

			// デプスステンシルビュー作成
			hr = device_->CreateDepthStencilView(depthStencilBuffer_.Get(), nullptr, depthStencilView_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "デプスステンシルビューの作成に失敗しました。");

			// レンダーターゲットビュー設定
			deviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());

			// デプスステンシルステート作成
			CD3D11_DEPTH_STENCIL_DESC dsd(D3D11_DEFAULT);
			hr = device_->CreateDepthStencilState(&dsd, depthStencilState_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "デプスステンシルステートの作成に失敗しました。");

			// ビューポート設定
			viewport_.TopLeftX = 0.0f;
			viewport_.TopLeftY = 0.0f;
			viewport_.Width = static_cast<float>(windowProp.width);
			viewport_.Height = static_cast<float>(windowProp.height);
			viewport_.MinDepth = 0.0f;
			viewport_.MaxDepth = 1.0f;

			// ラスタライザステートの作成
			CD3D11_RASTERIZER_DESC rd(D3D11_DEFAULT);
			hr = device_->CreateRasterizerState(&rd, rasterizerState_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "ラスタライザステートの作成に失敗しました。");

			// ブレンドステートの作成
			CD3D11_BLEND_DESC desc(D3D11_DEFAULT);
			desc.AlphaToCoverageEnable = TRUE;
			desc.IndependentBlendEnable = FALSE;
			desc.RenderTarget[0].BlendEnable = TRUE;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			// 半透明
			hr = device_->CreateBlendState(&desc, blendState_[0].GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "通常ブレンドステートの作成に失敗しました。");

			// 加算
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			hr = device_->CreateBlendState(&desc, blendState_[1].GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "加算ブレンドステートの作成に失敗しました。");

			// 減算
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
			hr = device_->CreateBlendState(&desc, blendState_[2].GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "減算ブレンドステートの作成に失敗しました。");

			// 乗算
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			hr = device_->CreateBlendState(&desc, blendState_[3].GetAddressOf());
			hr = OLE_E_ADVISENOTSUPPORTED;
			WX2_COM_ERROR_IF_FAILED(hr, "乗算ブレンドステートの作成に失敗しました。");

			// ブレンドステート設定
			SetBlendState(BlendState::Default);
		}
		catch (const ComException& exception)
		{
			WX2_LOG_ERROR(exception.what());
			return false;
		}

		return true;
	}

	void Graphics::SetBlendState(const BlendState blendState) const
	{
		if (static_cast<int>(blendState) < 0 && static_cast<int>(blendState) >= NUM_BLEND_STATE)
		{
			WX2_LOG_ERROR("ブレンドステートの値が範囲外です。");
			exit(EXIT_FAILURE);
		}
		constexpr FLOAT blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		deviceContext_->OMSetBlendState(
			blendState_[static_cast<std::size_t>(blendState)].Get(),
			blendFactor, 
			0xFFFFFFFF);
	}

	Graphics::ComPtr<IDXGIAdapter> Graphics::GetAdapterByGpuMemory(IDXGIFactory* factory) const
	{
		HRESULT hr = 0;

		// メイングラフィックカードを列挙
		ComPtr<IDXGIAdapter> adapter;
		int maxMemory = 0;
		int adapterIdx = 0;
		for (int i = 0; factory->EnumAdapters(i, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			DXGI_ADAPTER_DESC ad = {};
			adapter->GetDesc(&ad);
			std::string description = ToString(ad.Description);

			const int videoCardMem = static_cast<int>(ad.DedicatedVideoMemory) / 1024 / 1024;
			WX2_LOG_INFO("==================================================");
			WX2_LOG_INFO("adapter[{}]: {}", i, description);
			WX2_LOG_INFO(" - revision: {}", ad.Revision);
			WX2_LOG_INFO(" - video memory: {}", ad.DedicatedVideoMemory / 1024 / 1024);
			WX2_LOG_INFO(" - system memory: {}", ad.DedicatedSystemMemory / 1024 / 1024);
			WX2_LOG_INFO(" - shared system memory: {}", ad.SharedSystemMemory / 1024 / 1024);

			// メインモニターを列挙
			ComPtr<IDXGIOutput> output;
			for (int j = 0; adapter->EnumOutputs(j, output.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++j)
			{
				UINT numModes = 0;
				constexpr DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

				// ディスプレイの表示モード数を取得
				hr = output->GetDisplayModeList(format, 0, &numModes, nullptr);
				if (FAILED(hr))
				{
					WX2_LOG_ERROR("ディスプレイの表示モード数の取得に失敗しました。");
					continue;
				}

				std::vector<DXGI_MODE_DESC> modes;
				modes.resize(numModes);

				// ディスプレイの表示モード情報リストを取得
				hr = output->GetDisplayModeList(format, 0, &numModes, modes.data());
				if (FAILED(hr))
				{
					WX2_LOG_ERROR("ディスプレイの表示モード情報リストの取得に失敗しました。");
					continue;
				}

				WX2_LOG_INFO(" - output[{}] num display mode: {}\n", j, numModes);

				for (int k = 0; k < numModes; ++k)
				{
					WX2_LOG_INFO(" - - mode[{}]: {} * {}, refresh rate: {}",
						k, modes[k].Width, modes[k].Height, modes[k].RefreshRate.Denominator);
				}

				output.Reset();
			}

			if (videoCardMem > maxMemory)
			{
				maxMemory = videoCardMem;
				adapterIdx = i;
			}

			adapter.Reset();
		}
		
		WX2_LOG_INFO("==================================================");

		factory->EnumAdapters(adapterIdx, adapter.GetAddressOf());
		WX2_COM_ERROR_IF_FAILED(hr, "アダプタの取得に失敗しました。")

		return adapter;
	}
}