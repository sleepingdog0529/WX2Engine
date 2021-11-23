/*********************************************************************
 * @file   graphics.h
 * @author Tomomi Murakami
 * @date   2021/11/04 13:49
 * @brief  グラフィック管理クラス
 ********************************************************************/
#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <dxgi1_6.h>
#include "../core/window_property.h"

namespace wx2
{
	class Graphics
	{
	private:
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		static constexpr int NUM_BLEND_STATE = 4;

	public:
		enum class BlendState
		{
			Default = 0,
			Additive,
			Subtractive,
			Multiplicative
		};

	public:

		/**
		 * @brief  グラフィックスの初期化
		 * @param  hwnd ウィンドウハンドル
		 * @param  windowProp ウィンドウ情報
		 * @param  vsync 垂直同期の使用
		 */
		[[nodiscard]] bool Initialize(HWND hwnd, const WindowProperty& windowProp, bool vsync);


		/**
		 * @brief  
		 * @param  blendState ブレンドモード
		 */
		void SetBlendState(const BlendState blendState) const;

	private:
		/**
		 * @brief  GPUメモリの最も大きいアダプタを取得
		 * @return アダプタ
		 */
		ComPtr<IDXGIAdapter> GetAdapterByGpuMemory(IDXGIFactory* factory) const;

		ComPtr<IDXGISwapChain> swapChain_;
		ComPtr<ID3D11Device> device_;
		ComPtr<ID3D11DeviceContext> deviceContext_;
		D3D_FEATURE_LEVEL featureLevel_{};

		ComPtr<ID3D11Texture2D> backBuffer_;
		ComPtr<ID3D11RenderTargetView> renderTargetView_;

		ComPtr<ID3D11Texture2D> depthStencilBuffer_;
		ComPtr<ID3D11DepthStencilView> depthStencilView_;

		ComPtr<ID3D11DepthStencilState> depthStencilState_;
		ComPtr<ID3D11RasterizerState>	rasterizerState_;

		D3D11_VIEWPORT viewport_{};

		ComPtr<ID3D11BlendState> blendState_[NUM_BLEND_STATE];
		ComPtr<ID3D11SamplerState> samplerState_;
	};
}