#include "header/normal.hlsli"

Texture2D DiffuseTexture : register(t0);
SamplerState Sampler : register(s0);

// WVP�ϊ��s��
cbuffer WVPMatrix : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// ���_�V�F�[�_�[����
struct VSInput
{
	float3 pos : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

// �s�N�Z���V�F�[�_�[����
struct PSInput
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

// ���_�V�F�[�_�[�G���g���|�C���g
PSInput VSMain(VSInput Input)
{
	PSInput Output;

	Output.pos = mul(worldMatrix, float4(Input.pos, 1.0f));
	Output.pos = mul(viewMatrix, Output.pos);
	Output.pos = mul(projectionMatrix, Output.pos);

	Output.texcoord = Input.texcoord;

	return Output;
}

// �s�N�Z���V�F�[�_�[�G���g���|�C���g
float4 PSMain(PSInput Input) : SV_Target
{
	float4 color = DiffuseTexture.Sample(Sampler, Input.texcoord);

	return color;
}