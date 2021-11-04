/*********************************************************************
 * @file	constant_buffer.h
 * @author	SleepingDog0529
 * @date	2021/07/19 15:50
*********************************************************************/
#pragma once
#include "graphics.h"

namespace wx2 
{
	class Graphics;

	template <typename ConstantType>
	class ConstantBuffer final
	{
	public:
		ConstantType data;

		ConstantBuffer(Graphics* graphics) :
			graphics_(graphics)
		{
			HRESULT hr;

			D3D11_BUFFER_DESC desc = {};
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.ByteWidth = static_cast<UINT>(sizeof(ConstantType) + (16 - (sizeof(ConstantType) % 16)));
			desc.StructureByteStride = 0;

			hr = graphics_->GetDevice()->CreateBuffer(&desc, 0, buffer_.GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateBuffer failed.");
		}

		~ConstantBuffer() = default;

		void Bind() const 
		{
			HRESULT hr;
			ID3D11DeviceContext* device_context = graphics_->GetDeviceContext();

			D3D11_MAPPED_SUBRESOURCE resource = {};
			hr = device_context->Map(buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
			WX2_ASSERT(SUCCEEDED(hr), "Failed to change ConstantBuffer.");

			CopyMemory(resource.pData, &data, sizeof(ConstantType));
			device_context->Unmap(buffer_.Get(), 0);

			device_context->VSSetConstantBuffers(
				0, 1, buffer_.GetAddressOf());
		}

	private:
		Graphics* graphics_;
		ComPtr<ID3D11Buffer> buffer_;
	};

}