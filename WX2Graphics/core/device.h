/*********************************************************************
 * @file   device.h
 * @author Tomomi Murakami
 * @date   2021/11/25 21:39
 * @brief  デバイス
 ********************************************************************/
#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <WX2Common.h>

namespace wx2::graphics
{
	class Device final
	{
	private:
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		Device() = default;
		virtual ~Device() = default;

		WX2_DISALLOW_COPY_AND_MOVE(Device);

		void Initialize();

		[[nodiscard]] ID3D11Device* GetDevice() const noexcept { return device_.Get(); }
		[[nodiscard]] ID3D11DeviceContext* GetDeviceContext() const noexcept { return deviceContext_.Get(); }

	private:
		ComPtr<ID3D11Device> device_{};
		ComPtr<ID3D11DeviceContext> deviceContext_{};
	};
}