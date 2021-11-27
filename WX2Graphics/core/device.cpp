#include "device.h"

namespace wx2::graphics
{
	void Device::Initialize()
	{
		// 機能レベル
		D3D_FEATURE_LEVEL supportedFeatureLevel = D3D_FEATURE_LEVEL_1_0_CORE;
		const std::array<D3D_FEATURE_LEVEL, 2> featureLevels = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};

		UINT frags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		if (IS_DEBUGGING)
		{
			frags |= D3D11_CREATE_DEVICE_DEBUG;
		}

		// デバイス作成
		const HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			frags,
			featureLevels.data(),
			featureLevels.size(),
			D3D11_SDK_VERSION,
			device_.GetAddressOf(),
			&supportedFeatureLevel,
			deviceContext_.GetAddressOf());
		WX2_COM_ERROR_IF_FAILED(hr, "デバイスの作成に失敗しました。");
	}
}
