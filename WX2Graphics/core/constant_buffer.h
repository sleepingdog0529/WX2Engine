/*********************************************************************
 * @file   constant_buffer.h
 * @author Tomomi Murakami
 * @date   2021/11/25 22:20
 * @brief  �萔�o�b�t�@
 ********************************************************************/
#pragma once
#include "device.h"

namespace wx2::graphics
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
			WX2_ASSERT_MSG(devices, "�f�o�C�X��nullptr�ł����B");
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
			WX2_COM_ERROR_IF_FAILED(hr, "�萔�o�b�t�@�̍쐬�Ɏ��s���܂����B");
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
			WX2_COM_ERROR_IF_FAILED(hr, "�萔�o�b�t�@�̃}�b�v�Ɏ��s���܂����B");

			std::memcpy(msr.pData, &data, sizeof(data));
;			deviceContext->Unmap(buffer_.Get(), 0);
		}

		void VSBind(const UINT startSlot) noexcept
		{
			auto* deviceContext = devices_->GetDeviceContext();

			deviceContext->VSSetConstantBuffers(startSlot, 1, buffer_.GetAddressOf());
		}

		void PSBind(const UINT startSlot) noexcept
		{
			auto* deviceContext = devices_->GetDeviceContext();

			deviceContext->PSSetConstantBuffers(startSlot, 1, buffer_.GetAddressOf());
		}

	private:
		Device* devices_{};
		ComPtr<ID3D11Buffer> buffer_{};
	};
}