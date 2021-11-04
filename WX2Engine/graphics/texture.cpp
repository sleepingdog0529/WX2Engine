#include "graphics/texture.h"
#include "util/file_explorer.h"
#include <DirectxTex/DirectXTex.h>
#include "math/constant.h"

namespace wx2 {

	static const auto kTextureDirectory = ".\\texture";

	Texture::Texture(ID3D11Device* device, const XMVECTOR& color, aiTextureType type)
	{
		InitializeColorTexture(device, color, 1, 1, type);
	}

	Texture::Texture(
		ID3D11Device* device,
		const std::filesystem::path& file_name,
		aiTextureType type)
	{
		namespace fs = std::filesystem;

		type_ = type;

		if(file_name.has_parent_path())
			file_path_ = file_name;
		else
			file_path_ = FindFileInDirectory(kTextureDirectory, file_name);
		HRESULT hr;

		fs::path ext = file_name.extension();
		TexMetadata meta;
		auto image = std::make_unique<ScratchImage>();

		if (ext.string() == ".dds" || ext.string() == ".DDS")
		{
			hr = LoadFromDDSFile(file_path_.c_str(), DDS_FLAGS_NONE, &meta, *image);
			WX2_ASSERT(SUCCEEDED(hr), "LoadFromDDSFile failed.\" file\"{}", file_path_.string());
		}
		else if(ext.string() == ".tga" || ext.string() == ".TGA")
		{
			hr = LoadFromTGAFile(file_path_.c_str(), &meta, *image);
			WX2_ASSERT(SUCCEEDED(hr), "LoadFromTGAFile failed.\" file\"{}", file_path_.string());
		}
		else
		{
			hr = LoadFromWICFile(file_path_.c_str(), WIC_FLAGS_NONE, &meta, *image);
			WX2_ASSERT(SUCCEEDED(hr), "LoadFromWICFile failed.\" file\"{}", file_path_.string());
		}

		hr = CreateShaderResourceView(device, image->GetImages(), image->GetImageCount(), meta, texture_view_.GetAddressOf());

		if (FAILED(hr))
		{
			InitializeColorTexture(device, Color::Magenta, 1, 1, type);
			WX2_LOG_WARN("Failed to load texture from file.\" file \"{}", file_path_.string());
		}
	}

	Texture::Texture(ID3D11Device* device, const uint8_t* data, size_t size, aiTextureType type)
	{
		type_ = type;

		TexMetadata meta;
		auto image = std::make_unique<ScratchImage>();

		HRESULT hr = LoadFromWICMemory(data, size, WIC_FLAGS_NONE, &meta, *image);
		WX2_ASSERT(SUCCEEDED(hr), "LoadFromWICMemory failed.");

		if(SUCCEEDED(hr))
			hr = CreateShaderResourceView(device, image->GetImages(), image->GetImageCount(), meta, texture_view_.GetAddressOf());

		if (FAILED(hr))
		{
			InitializeColorTexture(device, Color::Magenta, 1, 1, type);
			WX2_LOG_WARN("Failed to load texture from momory.");
		}
	}

	void Texture::InitializeColorTexture(ID3D11Device* device, const XMVECTOR& color, UINT width, UINT height, aiTextureType type)
	{
		type_ = type;
		
		HRESULT hr;
		ID3D11Texture2D* texture = nullptr;

		{
			CD3D11_TEXTURE2D_DESC desc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
			D3D11_SUBRESOURCE_DATA data{};
			data.pSysMem = &color;
			data.SysMemPitch = width * sizeof(XMVECTOR);
			hr = device->CreateTexture2D(&desc, &data, &texture);
			WX2_ASSERT(SUCCEEDED(hr), "CreateTexture2D failed.");
		}

		{
			texture_ = static_cast<ID3D11Texture2D*>(texture);
			CD3D11_SHADER_RESOURCE_VIEW_DESC desc(D3D11_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R8G8B8A8_UNORM);
			hr = device->CreateShaderResourceView(texture_.Get(), &desc, texture_view_.GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateShaderResourceView failed.");
		}
	}

}