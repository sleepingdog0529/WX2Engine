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

namespace wx2::graphics
{
	class Texture final
	{
	private:
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		Texture() = default;
		~Texture() = default;

		void Initialize(Device* devices, const std::filesystem::path& filePath);
		void Initialize(Device* devices, const uint8_t* data, size_t size);
		void Initialize(Device* devices, const Color& color);

		void BindPS(const UINT slot) const noexcept;

	private:
		Device* devices_{};
		ComPtr<ID3D11Resource> texture_;
		ComPtr<ID3D11ShaderResourceView> textureView_;
	};
}
