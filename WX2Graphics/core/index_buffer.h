/*********************************************************************
 * @file   index_buffer.h
 * @author Tomomi Murakami
 * @date   2021/11/25 23:05
 * @brief  インデックスバッファ
 ********************************************************************/
#pragma once
#include "device.h"
#include <span>
#include <WX2Common.h>

namespace wx2
{
	class IndexBuffer final
	{
	private:
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		IndexBuffer() = default;
		~IndexBuffer() = default;

		WX2_COPYABLE(IndexBuffer);
		WX2_MOVEABLE(IndexBuffer);

		void Initialize(Device* devices, std::span<DWORD> indices);

		void Bind() const noexcept;

		[[nodiscard]] UINT NumIndices() const noexcept { return numIndices_; }

	private:
		Device* devices_{};
		ComPtr<ID3D11Buffer> buffer_{};
		UINT numIndices_ = 0;
	};
}
