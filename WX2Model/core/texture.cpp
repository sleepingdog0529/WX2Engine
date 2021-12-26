#include "texture.h"
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <boost/algorithm/string.hpp>

namespace wx2
{
	Texture::Texture(Device* devices, std::string_view filePath)
		: devices_(devices)
	{
		WX2_ASSERT_MSG(devices_, "デバイスがnullptrでした。");

		HRESULT hr;

		if (boost::algorithm::iends_with(filePath, ".dds"))
		{
			hr = DirectX::CreateDDSTextureFromFile(
				devices->GetDevice(),
				Utf8ToUtf16(filePath).c_str(),
				texture_.GetAddressOf(),
				textureView_.GetAddressOf());
		}
		else
		{
			hr = DirectX::CreateWICTextureFromFile(
				devices->GetDevice(),
				Utf8ToUtf16(filePath).c_str(),
				texture_.GetAddressOf(),
				textureView_.GetAddressOf());
		}

		if (FAILED(hr))
		{
			WX2_LOG_WARN("ファイルからのテクスチャの読み込みに失敗しました。パス: {}", filePath);

			Texture(devices, { 1.0f, 0.0f, 1.0f });
		}
	}

	Texture::Texture(Device* devices, const std::span<uint8_t> data)
		: devices_(devices)
	{
		WX2_ASSERT_MSG(devices_, "デバイスがnullptrでした。");

		const HRESULT hr = DirectX::CreateWICTextureFromMemory(
			devices_->GetDevice(),
			data.data(),
			data.size(),
			texture_.GetAddressOf(),
			textureView_.GetAddressOf());

		if (FAILED(hr))
		{
			WX2_LOG_WARN("メモリからのテクスチャの読み込みに失敗しました。");

			Texture(devices, { 1.0f, 0.0f, 1.0f });
		}
	}

	Texture::Texture(Device* devices, const Color& color)
		: devices_(devices)
	{
		WX2_ASSERT_MSG(devices_, "デバイスがnullptrでした。");

		const CD3D11_TEXTURE2D_DESC td(DXGI_FORMAT_R8G8B8A8_UNORM, 1, 1);

		ID3D11Texture2D* tex = nullptr;

		std::array<BYTE, 4> colorData;
		colorData[0] = static_cast<BYTE>(color[0] * 255.0f);
		colorData[1] = static_cast<BYTE>(color[1] * 255.0f);
		colorData[2] = static_cast<BYTE>(color[2] * 255.0f);
		colorData[3] = static_cast<BYTE>(color[3] * 255.0f);

		D3D11_SUBRESOURCE_DATA srd{};
		srd.pSysMem = colorData.data();
		srd.SysMemPitch = sizeof(colorData);

		const auto dev = devices_->GetDevice();

		HRESULT hr = dev->CreateTexture2D(&td, &srd, &tex);
		WX2_COM_ERROR_IF_FAILED(hr, "単色テクスチャの作成に失敗しました。");

		texture_ = static_cast<ID3D11Texture2D*>(tex);

		const CD3D11_SHADER_RESOURCE_VIEW_DESC srvd(D3D11_SRV_DIMENSION_TEXTURE2D, td.Format);
		hr = dev->CreateShaderResourceView(texture_.Get(), &srvd, textureView_.GetAddressOf());
		WX2_COM_ERROR_IF_FAILED(hr, "テクスチャビューの作成に失敗しました。");
	}

	void Texture::Bind(const ShaderType type, const UINT slot) const noexcept
	{
		switch (type)
		{
		case ShaderType::Pixel:
			devices_->GetDeviceContext()->PSSetShaderResources(slot, 1, textureView_.GetAddressOf());
			break;
		case ShaderType::Vertex:
			devices_->GetDeviceContext()->VSSetShaderResources(slot, 1, textureView_.GetAddressOf());
			break;
		case ShaderType::Geometry:
			devices_->GetDeviceContext()->GSSetShaderResources(slot, 1, textureView_.GetAddressOf());
			break;
		case ShaderType::Hull:
			devices_->GetDeviceContext()->HSSetShaderResources(slot, 1, textureView_.GetAddressOf());
			break;
		case ShaderType::Domain:
			devices_->GetDeviceContext()->DSSetShaderResources(slot, 1, textureView_.GetAddressOf());
			break;
		case ShaderType::Compute:
			devices_->GetDeviceContext()->CSSetShaderResources(slot, 1, textureView_.GetAddressOf());
			break;
		default:
			WX2_LOG_ERROR("不正なシェーダータイプです。");
			break;
		}
	}
}
