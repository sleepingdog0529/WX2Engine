#include "graphics.h"
#include "vertex_buffer.h"
#include "vertex.h"
#include "index_buffer.h"
#include "model.h"
#include "math/angle.h"

namespace wx2 {

	//std::unique_ptr<Model> g_car_model;
	//std::unique_ptr<Model> g_cart_model;

	Graphics::Graphics(RenderWindow* render_window) :
		render_window_(render_window)
	{
		InitializeDirectX();
		InitializeScene();
	}

	Graphics::~Graphics()
	{
		sampler_state_.Reset();

		for (int i = 0; i < _BS_LAST; i++)
		{
			blend_state_[i].Reset();
		}

		rasterizer_state_.Reset();
		depth_stencil_state_apply_mask_.Reset();
		depth_stencil_state_draw_mask_.Reset();
		depth_stencil_state_.Reset();
		depth_stencil_.Reset();
		depth_stencil_view_.Reset();

		render_target_view_.Reset();
		swap_chain_.Reset();

		device_context_.Reset();
		device_.Reset();

#ifdef WX2_DEBUG_ENABLED
		ReportLiveObjects();
#endif
	}

	void Graphics::RenderFrame()
	{
		using namespace AngleUnitLiterals;

		HRESULT hr;

		device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		FLOAT clear_color[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		device_context_->ClearRenderTargetView(render_target_view_.Get(), clear_color);

		device_context_->ClearDepthStencilView(
			depth_stencil_view_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		device_context_->OMSetRenderTargets(
			1, render_target_view_.GetAddressOf(), depth_stencil_view_.Get());
		device_context_->RSSetViewports(1, &view_port_);

		// シェーダー設定
		device_context_->VSSetShader(vertex_shader_->GetShader(), nullptr, 0);
		device_context_->IASetInputLayout(vertex_shader_->GetInputLayout());
		device_context_->PSSetShader(pixel_shader_->GetShader(), nullptr, 0);
		device_context_->PSSetSamplers(0, 1, sampler_state_.GetAddressOf());

		static Radian rad;
		XMVECTORF32 dir{ cosf(rad), -1.0f, sinf(rad), 1 };
		XMStoreFloat3(&cb_ps_scene_->data.DirectionLightDirection, dir);
		cb_ps_scene_->data.DirectionLightPower = 1.0f;
		rad += 5.0_deg;
		cb_ps_scene_->Bind();

		XMVECTORF32 eye{ 0, 5, -5, 1 };
		XMVECTORF32 focus{ 0, 3, 0, 1 };
		XMVECTORF32 up{ 0, 1, 0, 1 };
		view_matrix_ = XMMatrixLookAtLH(eye, focus, up);
		projection_matrix_ = XMMatrixPerspectiveFovLH(
			XM_PIDIV2,
			static_cast<float>(render_window_->GetWidth()) / static_cast<float>(render_window_->GetHeight()),
			0.1f,
			1000.0f);

		cb_vs_scene_->data.view_proj = view_matrix_ * projection_matrix_;
		cb_vs_scene_->Bind();

		//g_car_model->Draw(XMMatrixTranslation(5, 0, 1), view_matrix_, projection_matrix_);
		//g_cart_model->Draw(XMMatrixTranslation(0, 0, 1) * XMMatrixScaling(3, 3, 3), view_matrix_, projection_matrix_);

		hr = swap_chain_->Present(0, NULL);
		WX2_ASSERT(SUCCEEDED(hr), "Present failed.");
	}

	void Graphics::SetBlendState(BlendState state)
	{
		if (state >= 0 && state < _BS_LAST)
			return;
		FLOAT blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		device_context_->OMSetBlendState(blend_state_[static_cast<uint8_t>(state)].Get(), blend_factor, 0xFFFFFFFF);
	}

	void Graphics::InitializeDirectX()
	{
		HRESULT hr;

		{
			// 機能レベル
			D3D_FEATURE_LEVEL supported_feature_level = D3D_FEATURE_LEVEL_1_0_CORE;
			D3D_FEATURE_LEVEL feature_levels[] = {
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0
			};

			UINT frags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef WX2_DEBUG_ENABLED
			frags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			// デバイス作成
			hr = D3D11CreateDevice(
				NULL,
				D3D_DRIVER_TYPE_HARDWARE,
				NULL,
				frags,
				feature_levels,
				ARRAYSIZE(feature_levels),
				D3D11_SDK_VERSION,
				device_.GetAddressOf(),
				&supported_feature_level,
				device_context_.GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "D3D11CreateDevice failed.");
		}

#ifdef WX2_DEBUG_ENABLED
		{
			// デバッグレイヤーを登録
			hr = device_->QueryInterface(IID_PPV_ARGS(debug_controller_.GetAddressOf()));
			WX2_ASSERT(SUCCEEDED(hr), "QueryInterface failed.");
		}
#endif

		ComPtr<IDXGIFactory> factory;
		{
			// インターフェース取得
			ComPtr<IDXGIDevice> dxgi_device;
			hr = device_->QueryInterface(IID_PPV_ARGS(dxgi_device.GetAddressOf()));
			WX2_ASSERT(SUCCEEDED(hr), "QueryInterface failed.");

			// アダプター取得
			ComPtr<IDXGIAdapter> adapter;
			hr = dxgi_device->GetAdapter(adapter.GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "GetAdapter failed.");

			// ファクトリー取得
			hr = adapter->GetParent(IID_PPV_ARGS(factory.GetAddressOf()));
			WX2_ASSERT(SUCCEEDED(hr), "GetParent failed.");
		}

		// 利用可能なMSAAを取得
		DXGI_SAMPLE_DESC msaa = {};
		for (int i = 0; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i++) {
			UINT quality;
			if (SUCCEEDED(device_->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &quality))) {
				if (0 < quality) {
					msaa.Count = i;
					msaa.Quality = quality - 1;
				}
			}
		}

		{
			// スワップチェイン設定
			DXGI_SWAP_CHAIN_DESC desc = {};
			desc.OutputWindow = render_window_->GetWindowHandle();
			desc.BufferDesc.Width = render_window_->GetWidth();
			desc.BufferDesc.Height = render_window_->GetHeight();
			desc.Windowed = !render_window_->IsFullScreen();
			desc.BufferDesc.RefreshRate.Numerator = 60;
			desc.BufferDesc.RefreshRate.Denominator = 1;
			desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.BufferCount = 1;
			desc.SampleDesc = msaa;
			desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			// スワップチェイン作成
			hr = factory->CreateSwapChain(device_.Get(), &desc, swap_chain_.GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateSwapChain failed.")
		}

		// DXGIファクトリー解放
		factory.Reset();

		// スワップチェインのバッファ取得
		ComPtr<ID3D11Texture2D> back_buffer;
		{
			hr = swap_chain_->GetBuffer(0, IID_PPV_ARGS(back_buffer.GetAddressOf()));
			WX2_ASSERT(SUCCEEDED(hr), "GetBuffer failed.");
		}

		// レンダーターゲットビュー作成
		{
			hr = device_->CreateRenderTargetView(back_buffer.Get(), nullptr, render_target_view_.GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateRenderTargetView failed.");
		}

		back_buffer.Reset();

		// フレームバッファ設定
		{
			CD3D11_TEXTURE2D_DESC desc(DXGI_FORMAT_D24_UNORM_S8_UINT, render_window_->GetWidth(), render_window_->GetHeight());
			desc.MipLevels = 1;
			desc.SampleDesc = msaa;
			desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

			// フレームバッファ作成
			hr = device_->CreateTexture2D(&desc, nullptr, depth_stencil_.GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateTexture2D failed.");
		}

		// デプスステンシルビュー作成
		{
			hr = device_->CreateDepthStencilView(depth_stencil_.Get(), nullptr, depth_stencil_view_.GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateDepthStencilView failed.");
		}

		// レンダーターゲットビュー設定
		device_context_->OMSetRenderTargets(1, render_target_view_.GetAddressOf(), depth_stencil_view_.Get());

		// デプスステンシルステート作成
		{
			CD3D11_DEPTH_STENCIL_DESC desc(D3D11_DEFAULT);
			hr = device_->CreateDepthStencilState(&desc, depth_stencil_state_.GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateDepthStencilState failed.");
		}

		// マスク用デプスステンシルステート設定
		{
			CD3D11_DEPTH_STENCIL_DESC desc(D3D11_DEFAULT);
			desc.DepthEnable = FALSE;
			desc.StencilEnable = TRUE;

			desc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
			desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;

			// マスク用デプスステンシルステート作成
			hr = device_->CreateDepthStencilState(&desc, depth_stencil_state_draw_mask_.GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateDepthStencilState failed.");
		}

		// 被マスク用デプスステンシルステートの設定
		{
			CD3D11_DEPTH_STENCIL_DESC desc(D3D11_DEFAULT);
			desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			desc.StencilEnable = TRUE;

			desc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
			desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

			desc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;
			desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

			// 被マスク用デプスステンシルステートの作成
			hr = device_->CreateDepthStencilState(&desc, depth_stencil_state_apply_mask_.GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateDepthStencilState failed.");
		}

		// ビューポート設定
		view_port_ = {
			0.0f,
			0.0f, 
			static_cast<float>(render_window_->GetWidth()),
			static_cast<float>(render_window_->GetHeight()),
			0, 
			1 };

		// ラスタライザステートの作成
		{
			CD3D11_RASTERIZER_DESC desc(D3D11_DEFAULT);
			hr = device_->CreateRasterizerState(&desc, rasterizer_state_.GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateRasterizerState failed.");
		}

		// ブレンドステート作成
		{
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
			hr = device_->CreateBlendState(&desc, blend_state_[0].GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateBlendState failed.");

			// 加算
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			hr = device_->CreateBlendState(&desc, blend_state_[1].GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateBlendState failed.");

			// 減算
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
			hr = device_->CreateBlendState(&desc, blend_state_[2].GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateBlendState failed.");

			// 乗算
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			hr = device_->CreateBlendState(&desc, blend_state_[3].GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateBlendState failed.");

			// ブレンドステート設定
			SetBlendState(BS_NORM);
		}

		// サンプラーステート作成
		{
			CD3D11_SAMPLER_DESC desc(D3D11_DEFAULT);
			desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

			hr = device_->CreateSamplerState(&desc, sampler_state_.GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateSamplerState failed.");
		}
	}

	void Graphics::InitializeScene()
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout {
			{ "POSITION"  , 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD"  , 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"    , 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT"   , 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL"  , 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		vertex_shader_ = std::make_unique<VertexShader>(
			device_.Get(), "basic_vs.hlsl", layout, "BasicVS");

		pixel_shader_ = std::make_unique<PixelShader>(
			device_.Get(), "basic_ps.hlsl", "BasicPS");

		cb_vs_mesh_ = std::make_unique<ConstantBuffer<CBVSMesh>>(this);
		cb_vs_scene_ = std::make_unique<ConstantBuffer<CBVSScene>>(this);
		cb_ps_scene_ = std::make_unique<ConstantBuffer<CBPSScene>>(this);
		cb_ps_material_ = std::make_unique<ConstantBuffer<CBPSMaterial>>(this);

		//g_car_model = std::make_unique<Model>(
		//	device_.Get(), device_context_.Get(), cb_vs_mesh_.get(), cb_ps_material_.get(), "spec_test.fbx");
		//g_cart_model = std::make_unique<Model>(
		//	device_.Get(), device_context_.Get(), cb_vs_mesh_.get(), cb_ps_material_.get(), "cart.fbx");
	}

	void Graphics::ReportLiveObjects()
	{
		auto handle = GetModuleHandle("dxgidebug.dll");
		WX2_ASSERT(handle != INVALID_HANDLE_VALUE, "");

		auto func = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(handle, "DXGIGetDebugInterface"));
		WX2_ASSERT(func != nullptr, "");

		ComPtr<IDXGIDebug> dxgi_debug;
		func(IID_PPV_ARGS(dxgi_debug.GetAddressOf()));
		WX2_ASSERT(dxgi_debug, "");
		
		dxgi_debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
	}

}
