/*********************************************************************
 * @file   texture.h
 * @author Tomomi Murakami
 * @date   2021/11/29 17:20
 * @brief  テクスチャ
 ********************************************************************/
#pragma once
#include <filesystem>
#include <WX2Mathematics.h>
#include "device.h"

namespace wx2
{
	class Texture final
	{
	private:
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		Texture() = default;
		~Texture() = default;

		Texture(Device* devices, const std::filesystem::path& filePath);
		Texture(Device* devices, const uint8_t* data, std::size_t size);
		Texture(Device* devices, const Color& color);

		void BindPS(const UINT slot) const noexcept;

	private:
		Device* devices_{};
		ComPtr<ID3D11Resource> texture_;
		ComPtr<ID3D11ShaderResourceView> textureView_;
	};
}
