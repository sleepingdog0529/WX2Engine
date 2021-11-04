#include "shader.h"
#include <d3dcompiler.h>
#include "directx_helper.h"
#include "util/file_explorer.h"

namespace wx2 
{

	static constexpr const char* kShaderPath = ".\\src\\shader";

	VertexShader::VertexShader(
		ID3D11Device* device,
		const std::filesystem::path& file_name,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout_desc,
		const std::string& entry_point)
	{
		HRESULT hr;

		const auto shader_path = FindFileInDirectory(kShaderPath, file_name);
		WX2_ASSERT(!shader_path.empty(), "The shader file was not found. file\"{}\"", file_name.string());

		UINT flags = D3DCOMPILE_SKIP_OPTIMIZATION;
#ifdef _DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif

		Microsoft::WRL::ComPtr<ID3DBlob> error_blob;
		hr = D3DCompileFromFile(
			shader_path.c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entry_point.c_str(),
			"vs_5_0",
			flags,
			0,
			buffer_.GetAddressOf(),
			error_blob.GetAddressOf());
		WX2_ASSERT(SUCCEEDED(hr), "D3DCompileFromFile failed.\" msg\"{}\", file\"{}", 
			DirectXHelper::ConvertBlobMessageToString(error_blob.Get()), shader_path.string());

		hr = device->CreateVertexShader(
			buffer_->GetBufferPointer(),
			buffer_->GetBufferSize(),
			NULL,
			shader_.GetAddressOf());
		WX2_ASSERT(SUCCEEDED(hr), "CreateVertexShader failed.\" file\"{}", file_name.string());

		hr = device->CreateInputLayout(
			layout_desc.data(),
			layout_desc.size(),
			buffer_->GetBufferPointer(),
			buffer_->GetBufferSize(),
			input_layout_.GetAddressOf());
		WX2_ASSERT(SUCCEEDED(hr), "CreateInputLayout failed.\" file\"{}", file_name.string());

		WX2_LOG_INFO("The shaders have been compiled.\" file\"{}", file_name.string());
	}

	PixelShader::PixelShader(
		ID3D11Device* device,
		const std::filesystem::path& file_name,
		const std::string& entry_point)
	{
		HRESULT hr;

		const auto shader_path = FindFileInDirectory(kShaderPath, file_name);
		WX2_ASSERT(!shader_path.empty(), "The shader file was not found. file\"{}\"", file_name.string());

		UINT flags = D3DCOMPILE_SKIP_OPTIMIZATION;
#ifdef _DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif

		Microsoft::WRL::ComPtr<ID3DBlob> error_blob;
		hr = D3DCompileFromFile(
			shader_path.c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entry_point.c_str(),
			"ps_5_0",
			flags,
			0,
			buffer_.GetAddressOf(),
			error_blob.GetAddressOf());
		WX2_ASSERT(SUCCEEDED(hr), "D3DCompileFromFile failed.\" msg\"{}\", file\"{}",
			DirectXHelper::ConvertBlobMessageToString(error_blob.Get()), shader_path.string());

		hr = device->CreatePixelShader(
			buffer_->GetBufferPointer(),
			buffer_->GetBufferSize(),
			NULL,
			shader_.GetAddressOf());
		WX2_ASSERT(SUCCEEDED(hr), "CreatePixelShader failed.\" file\"{}", file_name.string());

		WX2_LOG_INFO("The shaders have been compiled.\" file\"{}", file_name.string());
	}

}