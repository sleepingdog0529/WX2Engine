/*********************************************************************
 * @file   pixel_shader.h
 * @author Tomomi Murakami
 * @date   2021/11/27 15:24
 * @brief  ピクセルシェーダー
 ********************************************************************/
#pragma once
#include <filesystem>
#include <span>
#include "device.h"

namespace wx2::graphics
{
	class PixelShader final
	{
	private:
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		PixelShader() = default;
		~PixelShader() = default;

		WX2_DISALLOW_COPY_AND_MOVE(PixelShader);

		void Initialize(
			Device* devices,
			const std::filesystem::path& filePath);

		void Bind() const noexcept;

	private:
		Device* devices_{};
		ComPtr<ID3D11PixelShader> shader_{};
		ComPtr<ID3D10Blob> buffer_{};
	};
}