
#include "Share.fx"

struct VS_INPUT_3D
{
	float3	Pos	: POSITION;
	float3	Normal : NORMAL;
	float2	UV	: TEXCOORD;
	float3	Tangent : TANGENT;
	float3	Binormal : BINORMAL;
	float4	BlendWeight : BLENDWEIGHT;
	float4	BlendIndex : BLENDINDICES;
};

struct VS_OUTPUT_SHADOW
{
	float4	Pos : SV_POSITION;
	float4	ProjPos : POSITION;
};

VS_OUTPUT_SHADOW ShadowVS(VS_INPUT_3D input)
{
	VS_OUTPUT_SHADOW output = (VS_OUTPUT_SHADOW)0;

	float3	Pos = SkinningShadow(input.Pos, input.BlendWeight, input.BlendIndex);

	output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);
	output.Pos = output.ProjPos;

	return output;
}

PS_OUTPUT_SINGLE ShadowPS(VS_OUTPUT_SHADOW input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.Color = (float4)(input.ProjPos.z / input.ProjPos.w);
	output.Color.a = 1.f;
	//output.Color.rgb = float3(1.f, 0.f, 0.f);

	return output;
}