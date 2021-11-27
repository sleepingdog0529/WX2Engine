/*********************************************************************
 * @file   vertex_shader.h
 * @author Tomomi Murakami
 * @date   2021/11/27 10:36
 * @brief  頂点シェーダー
 ********************************************************************/
#pragma once
#include <filesystem>
#include <span>
#include "device.h"

namespace wx2::graphics
{
	class VertexShader final
	{
	private:
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		VertexShader() = default;
		~VertexShader() = default;

		WX2_DISALLOW_COPY_AND_MOVE(VertexShader);

		void Initialize(
			Device* devices,
			const std::filesystem::path& filePath,
			std::span<D3D11_INPUT_ELEMENT_DESC> layoutDescs);

		void Bind() const noexcept;

	private:
		Device* devices_{};
		ComPtr<ID3D11VertexShader> shader_{};
		ComPtr<ID3D10Blob> buffer_{};
		ComPtr<ID3D11InputLayout> inputLayout_{};
	};
}
