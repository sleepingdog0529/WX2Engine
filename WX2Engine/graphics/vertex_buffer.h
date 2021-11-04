/*********************************************************************
 * @file	vertex_buffer.h
 * @author	SleepingDog0529
 * @date	2021/07/10 20:58
*********************************************************************/
#pragma once

#include "graphics.h"

namespace wx2
{

	template <typename VertexType>
	class VertexBuffer final
	{
	public:
		VertexBuffer(Graphics* graphics, std::span<VertexType> vertices) :
			graphics_(graphics)
		{
			HRESULT hr;

			D3D11_BUFFER_DESC desc = {};
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.ByteWidth = kBufferStride * vertices.size();
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA data = {};
			data.pSysMem = vertices.data();

			hr = graphics_->GetDevice()->CreateBuffer(&desc, &data, buffer_.GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateBuffer for vertex buffer failed.");
		}

		~VertexBuffer() = default;

		void Bind() const 
		{
			graphics_->GetDeviceContext()->IASetVertexBuffers(
				0, 1, buffer_.GetAddressOf(), &kBufferStride, &kOffset);
		}

	private:
		static constexpr UINT kBufferStride = sizeof(VertexType);
		static constexpr UINT kOffset = 0;

		Graphics* graphics_;
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
	};

}