#include "pixel_shader.h"
#include <d3dcompiler.h>
#include <stierr.h>

namespace wx2::graphics
{
	void PixelShader::Initialize(
		Device* devices,
		const std::filesystem::path& filePath)
	{
		WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");
		devices_ = devices;

		buffer_.Reset();

		auto* device = devices_->GetDevice();

		UINT compileFlags = 0;
		if (IS_DEBUGGING)
		{
			compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		}

		ID3DBlob* errorBlob;

		HRESULT hr = D3DCompileFromFile(
			filePath.c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"PSMain",
			"ps_5_0",
			compileFlags,
			0,
			buffer_.GetAddressOf(),
			&errorBlob);
		if (hr == STIERR_OBJECTNOTFOUND)
		{
			throw COMException(hr, std::format("ピクセルシェーダーファイルのパスが間違っています。パス: {}", filePath.string()));
		}
		else if (FAILED(hr))
		{
			const std::string_view errorMsg(
				static_cast<const char*>(errorBlob->GetBufferPointer()),
				errorBlob->GetBufferSize());

			throw COMException(hr, std::format("ピクセルシェーダーのコンパイルに失敗しました。{}", errorMsg));
		}

		hr = device->CreatePixelShader(
			buffer_->GetBufferPointer(),
			buffer_->GetBufferSize(),
			nullptr,
			shader_.GetAddressOf());
		WX2_COM_ERROR_IF_FAILED(hr, "ピクセルシェーダーの作成に失敗しました。");
	}

	void PixelShader::Bind() const noexcept
	{
		auto* deviceContext = devices_->GetDeviceContext();

		deviceContext->PSSetShader(shader_.Get(), nullptr, 0);
	}
}
