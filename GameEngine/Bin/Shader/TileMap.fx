
#include "Share.fx"

cbuffer TileMapCBuffer : register(b10)
{
	float2	g_TileImageSize;
	int		g_TileStartX;
	int		g_TileStartY;
	int		g_TileEndX;
	int		g_TileEndY;
	float2	g_TileSize;
};

struct VS_INPUT_TILEMAP
{
	float3	Pos : POSITION;
	float2	UV : TEXCOORD;
	uint	InstanceID : SV_InstanceID;
};

struct VS_OUTPUT_TILEMAP
{
	float4	Pos : SV_POSITION;
	float4	ProjPos : POSITION;
	float2	UV : TEXCOORD;
	float4	Color : COLOR;
	float4	EmvColor : COLOR1;
	float	Opacity : TEXCOORD1;
};

struct TileInfo
{
	matrix	matWVP;
	float2	FrameStart;
	float2	FrameEnd;
	float4	Color;
	float4	EmvColor;
	float	Opacity;
	float3	TileInfoEmpty;
};

StructuredBuffer<TileInfo>		g_TileArrayInput	: register(t30);

VS_OUTPUT_TILEMAP TileMapVS(VS_INPUT_TILEMAP input)
{
	VS_OUTPUT_TILEMAP	output = (VS_OUTPUT_TILEMAP)0;

	output.ProjPos = mul(float4(input.Pos, 1.f), g_TileArrayInput[input.InstanceID].matWVP);
	output.Pos = output.ProjPos;
	output.Color = g_TileArrayInput[input.InstanceID].Color;
	output.EmvColor = g_TileArrayInput[input.InstanceID].EmvColor;
	output.Opacity = g_TileArrayInput[input.InstanceID].Opacity;

	if (input.UV.x > 0.f)
		output.UV.x = g_TileArrayInput[input.InstanceID].FrameEnd.x / g_TileImageSize.x;

	else
		output.UV.x = g_TileArrayInput[input.InstanceID].FrameStart.x / g_TileImageSize.x;

	if (input.UV.y > 0.f)
		output.UV.y = g_TileArrayInput[input.InstanceID].FrameEnd.y / g_TileImageSize.y;

	else
		output.UV.y = g_TileArrayInput[input.InstanceID].FrameStart.y / g_TileImageSize.y;

	return output;
}

PS_OUTPUT_SINGLE TileMapPS(VS_OUTPUT_TILEMAP input)
{
	PS_OUTPUT_SINGLE    output = (PS_OUTPUT_SINGLE)0;

	float4  BaseColor = g_BaseTexture.Sample(g_AnisotropicSmp, input.UV);
	float4	EmissiveColor = g_EmissiveTexture.Sample(g_AnisotropicSmp, input.UV);

	float4	result = (float4)0.f;

	result.rgb = BaseColor.rgb * input.Color.rgb * input.Color.a + EmissiveColor.rgb * input.EmvColor.rgb;
	result.a = BaseColor.a * input.Opacity;

	result = PaperBurn2D(result, input.UV);

	result = Distortion(result, input.UV, input.ProjPos);

	output.Color = result;

	return output;
}
