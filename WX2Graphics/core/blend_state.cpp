#include "blend_state.h"

namespace wx2::graphics
{
	void BlendState::Initialize(Device* devices)
	{
		WX2_ASSERT_MSG(devices, "�f�o�C�X��nullptr�ł����B");
		devices_ = devices;

		auto* device = devices_->GetDevice();

		// �u�����h�X�e�[�g�̍쐬
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

		// �ʏ�
		HRESULT hr = device->CreateBlendState(&desc, blendState_[0].GetAddressOf());
		WX2_COM_ERROR_IF_FAILED(hr, "�u�����h�X�e�[�g�̒ʏ탂�[�h�̍쐬�Ɏ��s���܂����B");

		// ���Z
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		hr = device->CreateBlendState(&desc, blendState_[1].GetAddressOf());
		WX2_COM_ERROR_IF_FAILED(hr, "�u�����h�X�e�[�g�̉��Z���[�h�̍쐬�Ɏ��s���܂����B");

		// ���Z
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
		hr = device->CreateBlendState(&desc, blendState_[2].GetAddressOf());
		WX2_COM_ERROR_IF_FAILED(hr, "�u�����h�X�e�[�g�̌��Z���[�h�̍쐬�Ɏ��s���܂����B");

		// ��Z
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		hr = device->CreateBlendState(&desc, blendState_[3].GetAddressOf());
		WX2_COM_ERROR_IF_FAILED(hr, "�u�����h�X�e�[�g�̏�Z���[�h�̍쐬�Ɏ��s���܂����B");
	}

	void BlendState::Bind(const Mode mode) const noexcept
	{
		constexpr FLOAT blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		auto* deviceContext = devices_->GetDeviceContext();

		deviceContext->OMSetBlendState(
			blendState_[static_cast<std::size_t>(mode)].Get(),
			blendFactor,
			0xffffffff);
	}
}
