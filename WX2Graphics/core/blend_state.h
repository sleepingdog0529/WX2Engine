/*********************************************************************
 * @file   blend_state.h
 * @author Tomomi Murakami
 * @date   2021/11/25 20:39
 * @brief  ブレンドステート
 ********************************************************************/
#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <WX2Common.h>
#include "device.h"

namespace wx2
{
	class BlendState final
	{
	public:
		enum class Mode
		{
			Default = 0,
			Additive,
			Subtractive,
			Multiplicative
		};

	private:
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		static constexpr int NUM_BLEND_STATE = 4;

	public:
		BlendState() = default;
		~BlendState() = default;

		WX2_DISALLOW_COPY(BlendState);
		WX2_DISALLOW_MOVE(BlendState);

		void Initialize(Device* devices);

		void Bind(const Mode mode) const noexcept;

	private:
		Device* devices_{};
		ComPtr<ID3D11BlendState> blendState_[NUM_BLEND_STATE]{};
	};
}
