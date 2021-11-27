#include "index_buffer.h"

namespace wx2::graphics
{
	void IndexBuffer::Initialize(
		Device* devices,
		const std::span<DWORD> indices)
	{
		WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");
		devices_ = devices;

		buffer_.Reset();

		auto* device = devices_->GetDevice();

		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = static_cast<UINT>(sizeof(DWORD) * indices.size());
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA srd{};
		srd.pSysMem = indices.data();

		const HRESULT hr = device->CreateBuffer(&bd, &srd, buffer_.GetAddressOf());
		WX2_COM_ERROR_IF_FAILED(hr, "インデックスバッファの作成に失敗しました。");
	}

	void IndexBuffer::Bind() const noexcept
	{
		auto* deviceContext = devices_->GetDeviceContext();

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceContext->IASetIndexBuffer(buffer_.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	}
}
