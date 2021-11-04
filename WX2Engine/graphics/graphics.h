/*********************************************************************
 * @file	graphics.h
 * @author	SleepingDog0529
 * @date	2021/07/10 0:13
*********************************************************************/
#pragma once
#include "shader.h"
#include "constant_buffer.h"
#include "constant_buffer_types.h"
#include "core/render_window.h"

namespace wx2 {

	template <typename ConstantType>
	class ConstantBuffer;

	class Graphics
	{
	public:
		enum BlendState
		{
			BS_NORM,
			BS_ADD,
			BS_SUB,
			BS_MUL,
			_BS_LAST
		};

		Graphics(RenderWindow* render_window);
		~Graphics();

		void RenderFrame();

		void SetEyePosition(const XMVECTOR& eye_position) { XMStoreFloat3(&cb_ps_scene_->data.EyePosition, eye_position); }
		void SetViewMatrix(const XMMATRIX& view_matrix) { view_matrix_ = view_matrix; };
		void SetProjectionMatrix(const XMMATRIX& projection_matrix) { projection_matrix_ = projection_matrix; };
		void SetBlendState(BlendState blend_state);

		ID3D11Device* GetDevice() const { return device_.Get(); }
		ID3D11DeviceContext* GetDeviceContext() const { return device_context_.Get(); }

		const ConstantBuffer<CBVSMesh>& GetCBVSMesh() const { return *cb_vs_mesh_; }
		const ConstantBuffer<CBVSScene>& GetCBVSScene() const { return *cb_vs_scene_; }
		const ConstantBuffer<CBPSScene>& GetCBPSScene() const { return *cb_ps_scene_; }
		const ConstantBuffer<CBPSMaterial>& GetCBPSMaterial() const { return *cb_ps_material_; }

	private:
		void InitializeDirectX();
		void InitializeScene();
		void ReportLiveObjects();

		std::unique_ptr<VertexShader> vertex_shader_;
		std::unique_ptr<PixelShader> pixel_shader_;

		std::unique_ptr<ConstantBuffer<CBVSMesh>> cb_vs_mesh_;
		std::unique_ptr<ConstantBuffer<CBVSScene>> cb_vs_scene_;
		std::unique_ptr<ConstantBuffer<CBPSScene>> cb_ps_scene_;
		std::unique_ptr<ConstantBuffer<CBPSMaterial>> cb_ps_material_;

		ComPtr<ID3D11Device> device_;
		ComPtr<ID3D11DeviceContext> device_context_;

		ComPtr<IDXGISwapChain> swap_chain_;
		ComPtr<ID3D11RenderTargetView> render_target_view_;

		ComPtr<ID3D11DepthStencilView>  depth_stencil_view_;
		ComPtr<ID3D11Texture2D>		    depth_stencil_;
		ComPtr<ID3D11DepthStencilState> depth_stencil_state_;
		ComPtr<ID3D11DepthStencilState> depth_stencil_state_draw_mask_;
		ComPtr<ID3D11DepthStencilState> depth_stencil_state_apply_mask_;
		ComPtr<ID3D11RasterizerState>	rasterizer_state_;

		D3D11_VIEWPORT view_port_;

		ComPtr<ID3D11BlendState> blend_state_[_BS_LAST];
		ComPtr<ID3D11SamplerState> sampler_state_;

#ifdef WX2_DEBUG_ENABLED
		ComPtr<ID3D11Debug> debug_controller_;
#endif

		XMMATRIX view_matrix_;
		XMMATRIX projection_matrix_;

		RenderWindow* render_window_;
	};

}