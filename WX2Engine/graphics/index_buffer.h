/*********************************************************************
 * @file	index_buffer.h
 * @author	SleepingDog0529
 * @date	2021/07/11 16:51
*********************************************************************/
#pragma once
#include "graphics.h"

namespace wx2
{

	class IndexBuffer
	{
	public:
		IndexBuffer(Graphics* graphics, std::span<UINT> indices) :
			graphics_(graphics),
			num_indices_(indices.size())
		{
			HRESULT hr;

			D3D11_BUFFER_DESC desc = {};
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.ByteWidth = kBufferStride * num_indices_;
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA data = {};
			data.pSysMem = indices.data();

			hr = graphics_->GetDevice()->CreateBuffer(&desc, &data, buffer_.GetAddressOf());
			WX2_ASSERT(SUCCEEDED(hr), "CreateBuffer for index buffer failed.");
		}

		virtual ~IndexBuffer() = default;

		WX2_DISALLOW_COPY_AND_MOVE(IndexBuffer);

		void Bind() const
		{
			graphics_->GetDeviceContext()->IASetIndexBuffer(
				buffer_.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		}

		std::size_t NumIndices() const { return num_indices_; }

	private:
		static constexpr UINT kBufferStride = sizeof(UINT);

		Graphics* graphics_;
		std::size_t num_indices_;
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
	};

}