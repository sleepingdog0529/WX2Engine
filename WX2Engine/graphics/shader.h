/*********************************************************************
 * @file	shader.h
 * @author	SleepingDog0529
 * @date	2021/07/12 17:34
*********************************************************************/
#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <filesystem>
#include "core/assert.h"

namespace wx2 {

	class VertexShader
	{
	public:
		VertexShader(
			ID3D11Device* device,
			const std::filesystem::path& file_name,
			const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout_desc,
			const std::string& entry_point = "main");

		[[nodiscard]] inline ID3D11VertexShader* GetShader() { return shader_.Get(); }
		[[nodiscard]] inline ID3D10Blob* GetBuffer() { return buffer_.Get(); }
		[[nodiscard]] inline ID3D11InputLayout* GetInputLayout() { return input_layout_.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> shader_;
		Microsoft::WRL::ComPtr<ID3D10Blob> buffer_;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout_;
	};

	class PixelShader
	{
	public:
		PixelShader(
			ID3D11Device* device,
			const std::filesystem::path& file_name,
			const std::string& entry_point = "main");

		[[nodiscard]] inline ID3D11PixelShader* GetShader() { return shader_.Get(); }
		[[nodiscard]] inline ID3D10Blob* GetBuffer() { return buffer_.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> shader_;
		Microsoft::WRL::ComPtr<ID3D10Blob> buffer_;
	};

}