/*********************************************************************
 * @file	texture.h
 * @author	SleepingDog0529
 * @date	2021/08/11 11:37
 * @brief	テクスチャ
*********************************************************************/
#pragma once
#include <assimp/material.h>

namespace wx2 
{

	class Texture
	{
	public:
		Texture(ID3D11Device* device, const XMVECTOR& color, aiTextureType type = aiTextureType::aiTextureType_NONE);

		Texture(
			ID3D11Device* device,
			const std::filesystem::path& file_name,
			aiTextureType type = aiTextureType::aiTextureType_NONE);

		Texture(
			ID3D11Device* device,
			const uint8_t* data,
			size_t size,
			aiTextureType type = aiTextureType::aiTextureType_NONE);

		[[nodiscard]] aiTextureType GetType() const { return type_; }

		[[nodiscard]] ID3D11ShaderResourceView* GetResourceView() const { return texture_view_.Get(); }
		[[nodiscard]] ID3D11ShaderResourceView* const * GetResourceViewAddress() const { return texture_view_.GetAddressOf(); }

	private:
		void InitializeColorTexture(
			ID3D11Device* device,
			const XMVECTOR& color_data,
			UINT width,
			UINT height,
			aiTextureType type
		);

		Microsoft::WRL::ComPtr<ID3D11Resource> texture_;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture_view_;

		aiTextureType type_ = aiTextureType::aiTextureType_UNKNOWN;

		std::filesystem::path file_path_;
	};

}