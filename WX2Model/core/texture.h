/*********************************************************************
 * @file   texture.h
 * @author Tomomi Murakami
 * @date   2021/11/29 17:20
 * @brief  テクスチャ
 ********************************************************************/
#pragma once
#include <filesystem>
#include <span>
#include <WX2Mathematics.h>
#include <WX2Graphics.h>

namespace wx2
{
	class Texture final
	{
	private:
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		Texture(Device* devices, std::string_view filePath);
		Texture(Device* devices, std::span<uint8_t> data);
		Texture(Device* devices, const Color& color);
		~Texture() = default;

		void Bind(ShaderType type, UINT slot) const noexcept;

	private:
		Device* devices_;
		ComPtr<ID3D11Resource> texture_;
		ComPtr<ID3D11ShaderResourceView> textureView_;
	};
}
