#include "header/normal.hlsli"

Texture2D DiffuseTexture : register(t0);
SamplerState Sampler : register(s0);

// WVP変換行列
cbuffer WVPMatrix : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// 頂点シェーダー入力
struct VSInput
{
	float3 pos : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

// ピクセルシェーダー入力
struct PSInput
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

// 頂点シェーダーエントリポイント
PSInput VSMain(VSInput Input)
{
	PSInput Output;

	Output.pos = mul(worldMatrix, float4(Input.pos, 1.0f));
	Output.pos = mul(viewMatrix, Output.pos);
	Output.pos = mul(projectionMatrix, Output.pos);

	Output.texcoord = Input.texcoord;

	return Output;
}

// ピクセルシェーダーエントリポイント
float4 PSMain(PSInput Input) : SV_Target
{
	float4 color = DiffuseTexture.Sample(Sampler, Input.texcoord);

	return color;
}