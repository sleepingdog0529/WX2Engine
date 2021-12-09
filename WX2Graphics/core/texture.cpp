#include "texture.h"
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <boost/algorithm/string.hpp>

namespace wx2::graphics
{
	void Texture::Initialize(Device* devices, const std::filesystem::path& filePath)
	{
		WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");
		devices_ = devices;

		std::string ext = filePath.extension().string();
		boost::algorithm::to_lower(ext);

		const auto dev = devices_->GetDevice();

		HRESULT hr;

		if (ext == ".dds")
		{
			hr = DirectX::CreateDDSTextureFromFile(
				dev,
				filePath.wstring().c_str(),
				texture_.GetAddressOf(),
				textureView_.GetAddressOf());
		}
		else
		{
			hr = DirectX::CreateWICTextureFromFile(
				dev,
				filePath.wstring().c_str(),
				texture_.GetAddressOf(),
				textureView_.GetAddressOf());
		}

		if (FAILED(hr))
		{
			WX2_LOG_WARN("ファイルからのテクスチャの読み込みに失敗しました。パス: {}", filePath.string());

			Initialize(devices, { 1.0f, 0.0f, 1.0f, 1.0f });
		}
	}

	void Texture::Initialize(Device* devices, const uint8_t* data, size_t size)
	{
		WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");
		devices_ = devices;

		auto* dev = devices_->GetDevice();

		const HRESULT hr = DirectX::CreateWICTextureFromMemory(
			dev,
			data,
			size,
			texture_.GetAddressOf(),
			textureView_.GetAddressOf());

		if (FAILED(hr))
		{
			WX2_LOG_WARN("メモリからのテクスチャの読み込みに失敗しました。");

			Initialize(devices, { 1.0f, 0.0f, 1.0f, 1.0f });
		}
	}

	void Texture::Initialize(Device* devices, const Color& color)
	{
		WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");
		devices_ = devices;

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

	void Texture::BindPS(const UINT slot) const noexcept
	{
		auto* devCon = devices_->GetDeviceContext();

		devCon->PSSetShaderResources(slot, 1, textureView_.GetAddressOf());
	}
}