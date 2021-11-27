cbuffer WVPMatrix : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct PSInput
{
	float4 pos : SV_POSITION;
};

PSInput VSMain(float4 pos : POSITION) : SV_POSITION
{
	PSInput psIn;

	psIn.pos = mul(worldMatrix, pos);
	psIn.pos = mul(viewMatrix, psIn.pos);
	psIn.pos = mul(projectionMatrix, psIn.pos);

	return psIn;
}

float4 PSMain(PSInput psIn) : SV_Target
{
	float4 color = float4(1.0f, 0.0f, 1.0f, 1.0f);

	return color;
}