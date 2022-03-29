
#include "Share.fx"

cbuffer RenderTargetCBuffer : register(b11)
{
	matrix	g_matTargetWVP;
};

struct VS_INPUT_TARGET
{
	float3	Pos : POSITION;
	float2	UV	: TEXCOORD;
};

struct VS_OUTPUT_TARGET
{
	float4	Pos : SV_POSITION;
	float2	UV : TEXCOORD;
};


VS_OUTPUT_TARGET RenderTargetVS(VS_INPUT_TARGET input)
{
	VS_OUTPUT_TARGET	output = (VS_OUTPUT_TARGET)0;

	output.Pos = mul(float4(input.Pos, 1.f), g_matTargetWVP);
	output.UV = input.UV;

	return output;
}

PS_OUTPUT_SINGLE RenderTargetPS(VS_OUTPUT_TARGET input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	float4	Color = g_BaseTexture.Sample(g_AnisotropicSmp, input.UV);

	if (Color.a == 0.f)
		clip(-1);
	
	output.Color = Color;

	return output;
}

