#include "header/normal.hlsli"

Texture2D DiffuseTexture : register(t0);
SamplerState Sampler : register(s0);

cbuffer WVPMatrix : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VSInput
{
	float3 pos : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

PSInput VSMain(VSInput Input)
{
	PSInput Output;

	Output.pos = mul(worldMatrix, float4(Input.pos, 1.0f));
	Output.pos = mul(viewMatrix, Output.pos);
	Output.pos = mul(projectionMatrix, Output.pos);

	Output.texcoord = Input.texcoord;

	return Output;
}

float4 PSMain(PSInput Input) : SV_Target
{
	float4 color = DiffuseTexture.Sample(Sampler, Input.texcoord);

	return color;
}