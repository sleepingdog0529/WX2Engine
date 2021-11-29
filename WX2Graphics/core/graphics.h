/*********************************************************************
 * @file   graphics.h
 * @author Tomomi Murakami
 * @date   2021/11/04 13:49
 * @brief  グラフィックス
 ********************************************************************/
#pragma once
#include <DirectXMath.h>
#include <dxgi1_6.h>
#include "device.h"
#include "blend_state.h"
#include "constant_buffer.h"
#include "constant_buffer_type.h"
#include "index_buffer.h"
#include "vertex_buffer.h"
#include "vertex_type.h"
#include "vertex_shader.h"
#include "pixel_shader.h"
#include "model_loader.h"

namespace wx2::graphics
{
	class Graphics final
	{
	private:
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		Graphics() = default;
		~Graphics() = default;

		WX2_DISALLOW_COPY_AND_MOVE(Graphics);

		/**
		 * @brief  グラフィックスの初期化
		 * @param  hwnd ウィンドウハンドル
		 * @param  windowProp ウィンドウ情報
		 * @param  vsync 垂直同期の使用
		 */
		[[nodiscard]] bool Initialize(HWND hwnd, const WindowProperty& windowProp, bool vsync) noexcept;

		void DrawBegin() noexcept;
		void DrawEnd() const noexcept;

		void RenderFrame() noexcept;

		[[nodiscard]] Device& GetDevice() noexcept { return devices_; }
		[[nodiscard]] BlendState& GetBlendState() noexcept { return blendState_; }
		[[nodiscard]] ConstantBuffer<WVPMatrix>& GetConstantBufferWVP() noexcept { return constantBufferWVP_; }
		[[nodiscard]] IndexBuffer& GetIndexBuffer() noexcept { return indexBuffer_; }
		[[nodiscard]] VertexBuffer<DirectX::XMFLOAT3>& GetVertexBuffer() noexcept { return vertexBuffer_; }
		[[nodiscard]] VertexShader& GetVertexShader() noexcept { return vertexShader_; }
		[[nodiscard]] PixelShader& GetPixelShader() noexcept { return pixelShader_; }

		[[nodiscard]] ID3D11RenderTargetView* GetRenderTargetView() const noexcept { return renderTargetView_.Get(); }
		[[nodiscard]] const D3D11_VIEWPORT* GetViewport() const noexcept { return &viewport_; }
		[[nodiscard]] IDXGISwapChain* GetSwapChain() const noexcept { return swapChain_.Get(); }
		[[nodiscard]] ID3D11SamplerState* GetSamplerState() const noexcept { return samplerState_.Get(); }

	private:
		void InitializePipeline();
		void InitializeGraphics();

		WindowProperty windowProperty_;

		Device devices_;
		BlendState blendState_;
		ConstantBuffer<WVPMatrix> constantBufferWVP_;
		IndexBuffer indexBuffer_;
		VertexBuffer<DirectX::XMFLOAT3> vertexBuffer_;
		VertexShader vertexShader_;
		PixelShader pixelShader_;

		ModelLoader modelLoader_;
		Model model_;

		ComPtr<ID3D11Debug> debug_{};

		ComPtr<IDXGISwapChain> swapChain_{};
		D3D_FEATURE_LEVEL featureLevel_{};

		ComPtr<ID3D11Texture2D> backBuffer_{};
		ComPtr<ID3D11RenderTargetView> renderTargetView_{};

		ComPtr<ID3D11Texture2D> depthStencilBuffer_{};
		ComPtr<ID3D11DepthStencilView> depthStencilView_{};

		ComPtr<ID3D11DepthStencilState> depthStencilState_{};
		ComPtr<ID3D11RasterizerState> rasterizerState_{};

		D3D11_VIEWPORT viewport_{};

		ComPtr<ID3D11SamplerState> samplerState_{};
	};
}
