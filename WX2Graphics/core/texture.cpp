#include "texture.h"
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <boost/algorithm/string.hpp>

namespace wx2::graphics
{
	void Texture::Initialize(Device* devices, const std::filesystem::path& filePath)
	{
		WX2_ASSERT_MSG(devices, "�f�o�C�X��nullptr�ł����B");
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
			WX2_LOG_WARN("�t�@�C������̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B�p�X: {}", filePath.string());

			InitializeColorTexture({1.0f, 0.0f, 1.0f, 1.0f}, 1, 1);
		}
	}

	void Texture::Initialize(Device* devices, const uint8_t* data, size_t size)
	{
		auto* dev = devices_->GetDevice();

		const HRESULT hr = DirectX::CreateWICTextureFromMemory(
			dev,
			data,
			size,
			texture_.GetAddressOf(),
			textureView_.GetAddressOf());

		if (FAILED(hr))
		{
			WX2_LOG_WARN("����������̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B");

			InitializeColorTexture({ 1.0f, 0.0f, 1.0f, 1.0f }, 1, 1);
		}
	}

	void Texture::InitializeColorTexture(const DirectX::XMFLOAT4& colorData, const UINT width, const UINT height)
	{
		const CD3D11_TEXTURE2D_DESC td(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);

		ID3D11Texture2D* tex = nullptr;

		D3D11_SUBRESOURCE_DATA srd{};
		srd.pSysMem = &colorData;
		srd.SysMemPitch = width * sizeof(DirectX::XMFLOAT4);

		const auto dev = devices_->GetDevice();

		HRESULT hr = dev->CreateTexture2D(&td, &srd, &tex);
		WX2_COM_ERROR_IF_FAILED(hr, "�P�F�e�N�X�`���̍쐬�Ɏ��s���܂����B");

		texture_ = static_cast<ID3D11Texture2D*>(tex);

		const CD3D11_SHADER_RESOURCE_VIEW_DESC srvd(D3D11_SRV_DIMENSION_TEXTURE2D, td.Format);
		hr = dev->CreateShaderResourceView(texture_.Get(), &srvd, textureView_.GetAddressOf());
		WX2_COM_ERROR_IF_FAILED(hr, "�e�N�X�`���r���[�̍쐬�Ɏ��s���܂����B");
	}

	void Texture::BindPS(const UINT slot) const noexcept
	{
		auto* devCon = devices_->GetDeviceContext();

		devCon->PSSetShaderResources(slot, 1, textureView_.GetAddressOf());
	}
}