/*********************************************************************
 * @file   constant_buffer.h
 * @author Tomomi Murakami
 * @date   2021/11/25 22:20
 * @brief  定数バッファ
 ********************************************************************/
#pragma once
#include "device.h"
#include "shader_type.h"

namespace wx2
{
	template <typename DataType>
	class ConstantBuffer final
	{
	private:
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		ConstantBuffer() = default;
		~ConstantBuffer() = default;

		DataType data{};

		void Initialize(Device* devices)
		{
			WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");
			devices_ = devices;

			buffer_.Reset();

			auto* device = devices_->GetDevice();

			D3D11_BUFFER_DESC bd{};
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.ByteWidth = static_cast<UINT>(sizeof(DataType) + (16 - (sizeof(DataType) % 16)));
			bd.StructureByteStride = 0;

			const HRESULT hr = device->CreateBuffer(&bd, nullptr, buffer_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "定数バッファの作成に失敗しました。");
		}

		void ApplyChange()
		{
			auto* deviceContext = devices_->GetDeviceContext();

			D3D11_MAPPED_SUBRESOURCE msr{};

			const HRESULT hr = deviceContext->Map(
				buffer_.Get(),
				0,
				D3D11_MAP_WRITE_DISCARD,
				0,
				&msr);
			WX2_COM_ERROR_IF_FAILED(hr, "定数バッファのマップに失敗しました。");

			std::memcpy(msr.pData, &data, sizeof(data));
			deviceContext->Unmap(buffer_.Get(), 0);
		}

		void Bind(const ShaderType& type, const UINT slot) noexcept
		{
			auto* deviceContext = devices_->GetDeviceContext();

			switch (type)
			{
			case ShaderType::Pixel:
				deviceContext->PSSetConstantBuffers(slot, 1, buffer_.GetAddressOf());
				break;
			case ShaderType::Vertex:
				deviceContext->VSSetConstantBuffers(slot, 1, buffer_.GetAddressOf());
				break;
			case ShaderType::Geometry:
				deviceContext->GSSetConstantBuffers(slot, 1, buffer_.GetAddressOf());
				break;
			case ShaderType::Hull:
				deviceContext->HSSetConstantBuffers(slot, 1, buffer_.GetAddressOf());
				break;
			case ShaderType::Domain:
				deviceContext->DSSetConstantBuffers(slot, 1, buffer_.GetAddressOf());
				break;
			case ShaderType::Compute:
				deviceContext->CSSetConstantBuffers(slot, 1, buffer_.GetAddressOf());
				break;
			default:
				WX2_LOG_ERROR("不正なシェーダータイプです。");
				break;
			}
		}

	private:
		Device* devices_{};
		ComPtr<ID3D11Buffer> buffer_{};
	};
}