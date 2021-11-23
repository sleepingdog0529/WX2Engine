/*********************************************************************
 * @file   graphics.h
 * @author Tomomi Murakami
 * @date   2021/11/04 13:49
 * @brief  �O���t�B�b�N�Ǘ��N���X
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
		 * @brief  �O���t�B�b�N�X�̏�����
		 * @param  hwnd �E�B���h�E�n���h��
		 * @param  windowProp �E�B���h�E���
		 * @param  vsync ���������̎g�p
		 */
		[[nodiscard]] bool Initialize(HWND hwnd, const WindowProperty& windowProp, bool vsync);


		/**
		 * @brief  
		 * @param  blendState �u�����h���[�h
		 */
		void SetBlendState(const BlendState blendState) const;

	private:
		/**
		 * @brief  GPU�������̍ł��傫���A�_�v�^���擾
		 * @return �A�_�v�^
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