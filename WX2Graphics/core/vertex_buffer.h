/*********************************************************************
 * @file   vertex_buffer.h
 * @author Tomomi Murakami
 * @date   2021/11/26 18:34
 * @brief  頂点バッファ
 ********************************************************************/
#pragma once
#include "device.h"
#include <span>
#include <WX2Common.h>

namespace wx2
{
	template <typename VertexType>
	class VertexBuffer
	{
	private:
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		VertexBuffer() = default;
		~VertexBuffer() = default;

		WX2_COPYABLE(VertexBuffer);
		WX2_MOVEABLE(VertexBuffer);

		void Initialize(Device* devices, std::span<VertexType> vertices)
		{
			WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");
			devices_ = devices;

			buffer_.Reset();

			auto* device = devices_->GetDevice();

			D3D11_BUFFER_DESC bd{};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = static_cast<UINT>(sizeof(VertexType) * vertices.size());
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA srd{};
			srd.pSysMem = vertices.data();

			const HRESULT hr = device->CreateBuffer(&bd, &srd, buffer_.GetAddressOf());
			WX2_COM_ERROR_IF_FAILED(hr, "頂点バッファの作成に失敗しました。");
		}

		void Bind() const noexcept
		{
			constexpr UINT stride = sizeof(VertexType);
			constexpr UINT offset = 0;

			auto* deviceContext = devices_->GetDeviceContext();

			deviceContext->IASetVertexBuffers(0, 1, buffer_.GetAddressOf(), &stride, &offset);
		}

	private:
		Device* devices_{};
		ComPtr<ID3D11Buffer> buffer_{};
	};
}