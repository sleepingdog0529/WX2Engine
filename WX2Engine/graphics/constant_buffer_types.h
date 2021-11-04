/*********************************************************************
 * @file	constant_buffer_types.h
 * @author	SleepingDog0529
 * @date	2021/07/12 17:32
 * @brief   定数バッファ型
*********************************************************************/
#pragma once

#define WX2_ALIGN16 _declspec(align(16))

namespace wx2 
{

	struct CBVSMesh
	{
		XMMATRIX world = XMMatrixIdentity();
	};

	struct CBVSScene
	{
		XMMATRIX view_proj = XMMatrixIdentity();
	};

	struct CBPSMaterial
	{
		WX2_ALIGN16 XMFLOAT3 BaseColor = { 1.0f, 1.0f, 1.0f };
		WX2_ALIGN16 XMFLOAT3 Ambient = { 1.0f, 1.0f, 1.0f };
		WX2_ALIGN16 XMFLOAT3 Diffuse = { 1.0f, 1.0f, 1.0f };
		WX2_ALIGN16 XMFLOAT3 Specular = { 0.5f, 0.5f, 0.5f };
		WX2_ALIGN16 float SpecularPower = 16.0f;
		WX2_ALIGN16 XMFLOAT3 Emissive = { 0.0f, 0.0f, 0.0f };
		WX2_ALIGN16 XMFLOAT3 Reflection = { 0.5f, 0.5f, 0.5f };
	};

	struct CBPSPointLight
	{
		WX2_ALIGN16 XMFLOAT4 Position = { 0.0f, 0.0f, 0.0f, 0 };
		WX2_ALIGN16 XMFLOAT4 Color = { 1.0f, 1.0f, 1.0f, 0 };
		WX2_ALIGN16 XMFLOAT4 Range = { 5.0f, 0, 0, 0 };
	};

	struct CBPSScene
	{
		WX2_ALIGN16 XMFLOAT3 EyePosition = { 0.0f, 0.0f, 0.0f };
		WX2_ALIGN16 XMFLOAT3 AmbientLight = { 0.6f, 0.6f, 0.8f };
		WX2_ALIGN16 XMFLOAT3 DirectionLightDirection = { -1.0f, -1.0f, -1.0f };
		WX2_ALIGN16 XMFLOAT3 DirectionLightColor = { 1.0f, 1.0f, 1.0f };
		WX2_ALIGN16 float DirectionLightPower = 1.0f;
		WX2_ALIGN16 XMFLOAT3 PointLightPosition = { 0.0f, 0.0f, 0.0f };
		WX2_ALIGN16 XMFLOAT3 PointLightColor = { 1.0f, 1.0f, 1.0f };
		WX2_ALIGN16 float PointLightPower = 5.0f;
	};

}