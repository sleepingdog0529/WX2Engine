#include "vertex_shader.h"
#include <d3dcompiler.h>
#include <stierr.h>

namespace wx2
{
	void VertexShader::Initialize(
		Device* devices,
		const std::filesystem::path& filePath,
		const std::span<D3D11_INPUT_ELEMENT_DESC> layoutDescs)
	{
		WX2_ASSERT_MSG(devices, "�f�o�C�X��nullptr�ł����B");
		devices_ = devices;

		buffer_.Reset();

		auto* device = devices_->GetDevice();

		UINT compileFlags = 0;
		if(IS_DEBUGGING)
		{
			compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		}

		ID3DBlob* errorBlob;

		HRESULT hr = D3DCompileFromFile(
			filePath.c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"VSMain",
			"vs_5_0",
			compileFlags,
			0,
			buffer_.GetAddressOf(),
			&errorBlob);
		if (hr == STIERR_OBJECTNOTFOUND)
		{
			throw COMException(hr, std::format("���_�V�F�[�_�[�t�@�C���̃p�X���Ԉ���Ă��܂��B�p�X: {}", filePath.string()));
		}
		else if (FAILED(hr))
		{
			const std::string_view errorMsg(
				static_cast<const char*>(errorBlob->GetBufferPointer()),
				errorBlob->GetBufferSize());

			throw COMException(hr, std::format("���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂����B{}", errorMsg));
		}

		hr = device->CreateVertexShader(
			buffer_->GetBufferPointer(),
			buffer_->GetBufferSize(),
			nullptr,
			shader_.GetAddressOf());
		WX2_COM_ERROR_IF_FAILED(hr, "���_�V�F�[�_�[�̍쐬�Ɏ��s���܂����B");

		hr = device->CreateInputLayout(
			layoutDescs.data(),
			static_cast<UINT>(layoutDescs.size()),
			buffer_->GetBufferPointer(),
			buffer_->GetBufferSize(),
			inputLayout_.GetAddressOf());
		WX2_COM_ERROR_IF_FAILED(hr, "�C���v�b�g���C�A�E�g�̍쐬�Ɏ��s���܂����B");
	}

	void VertexShader::Bind() const noexcept
	{
		auto* deviceContext = devices_->GetDeviceContext();

		deviceContext->VSSetShader(shader_.Get(), nullptr, 0);
		deviceContext->IASetInputLayout(inputLayout_.Get());
	}
}
