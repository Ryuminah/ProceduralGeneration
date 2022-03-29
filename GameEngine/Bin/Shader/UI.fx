
#include "Share.fx"

cbuffer UITransformCBuffer : register(b10)
{
	matrix	g_matUIWVP;
	int		g_UITextureEnable;
	int		g_UIAnimation2DEnable;
	float2	g_UISize;
};

cbuffer UIColorTintCBuffer : register(b11)
{
	float4	g_UIColorTint;
};

cbuffer ProgressBarCBuffer : register(b12)
{
	float	g_ProgressPercent;
	int		g_ProgressDir;
	float2	g_ProgressEmpty;
};

cbuffer UINumberCBuffer : register(b13)
{
	float2	g_NumberStartUV;
	float2	g_NumberEndUV;
};


struct VS_INPUT_UI
{
	float3	Pos : POSITION;
	float2	UV	: TEXCOORD;
};

struct VS_OUTPUT_UI
{
	float4	Pos : SV_POSITION;
	float2	UV : TEXCOORD;
};

float2 ComputeUIAnimation2DUV(float2 UV)
{
	if (g_UIAnimation2DEnable == 0)
		return UV;

	if (g_Animation2DType == Animation2DFrame ||
		g_Animation2DType == Animation2DArray)
		return UV;

	float2	ResultUV;

	if (UV.x <= 0.f)
		ResultUV.x = g_vAnimation2DStartUV.x;

	else
		ResultUV.x = g_vAnimation2DEndUV.x;

	if (UV.y <= 0.f)
		ResultUV.y = g_vAnimation2DStartUV.y;

	else
		ResultUV.y = g_vAnimation2DEndUV.y;

	return ResultUV;
}

VS_OUTPUT_UI UIMainVS(VS_INPUT_UI input)
{
	VS_OUTPUT_UI	output = (VS_OUTPUT_UI)0;

	output.Pos = mul(float4(input.Pos, 1.f), g_matUIWVP);
	output.UV = ComputeUIAnimation2DUV(input.UV);

	return output;
}

VS_OUTPUT_UI UIMouseVS(VS_INPUT_UI input)
{
	VS_OUTPUT_UI	output = (VS_OUTPUT_UI)0;

	float3	Pos = input.Pos;
	
	Pos.y = input.Pos.y - 1.f;

	output.Pos = mul(float4(Pos, 1.f), g_matUIWVP);
	output.UV = ComputeUIAnimation2DUV(input.UV);

	return output;
}


PS_OUTPUT_SINGLE UIImagePS(VS_OUTPUT_UI input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	if (g_UITextureEnable == 1 || g_UIAnimation2DEnable == 1)
		output.Color = g_BaseTexture.Sample(g_AnisotropicSmp, input.UV) * g_UIColorTint;

	else
		output.Color = g_UIColorTint;

	return output;
}

VS_OUTPUT_UI ProgressBarVS(VS_INPUT_UI input)
{
	VS_OUTPUT_UI	output = (VS_OUTPUT_UI)0;

	float3	Pos = input.Pos;

	// 오른쪽 -> 왼쪽
	if (g_ProgressDir == 0)
	{
		if (Pos.x == 1.f)
		{
			Pos.x = g_ProgressPercent;
		}
	}

	// 왼쪽 -> 오른쪽
	if (g_ProgressDir == 1)
	{
		if (Pos.x == 0.f)
		{
			Pos.x = 1.f - g_ProgressPercent;
		}
	}

	// 위 -> 아래
	if (g_ProgressDir == 2)
	{
		if (Pos.y == 1.f)
		{
			Pos.y = g_ProgressPercent;
		}
	}

	// 아래 -> 위
	if (g_ProgressDir == 3)
	{
		if (Pos.y == 0.f)
		{
			Pos.y = 1.f - g_ProgressPercent;
		}
	}

	output.Pos = mul(float4(Pos, 1.f), g_matUIWVP);

	float2	UV = input.UV;

	// 오른쪽 -> 왼쪽
	if (g_ProgressDir == 0)
	{
		if (UV.x == 1.f)
		{
			UV.x = g_ProgressPercent;
		}
	}

	// 왼쪽 -> 오른쪽
	if (g_ProgressDir == 1)
	{
		if (UV.x == 0.f)
		{
			UV.x = 1.f - g_ProgressPercent;
		}
	}

	// 위 -> 아래
	if (g_ProgressDir == 2)
	{
		if (UV.y == 0.f)
		{
			UV.y = 1.f - g_ProgressPercent;
		}
	}

	// 아래 -> 위
	if (g_ProgressDir == 3)
	{
		if (UV.y == 1.f)
		{
			UV.y = g_ProgressPercent;
		}
	}

	output.UV = UV;

	return output;
}


PS_OUTPUT_SINGLE ProgressBarPS(VS_OUTPUT_UI input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	if (g_UITextureEnable == 1)
		output.Color = g_BaseTexture.Sample(g_AnisotropicSmp, input.UV) * g_UIColorTint;

	else
		output.Color = g_UIColorTint;


	return output;
}

VS_OUTPUT_UI UINumberVS(VS_INPUT_UI input)
{
	VS_OUTPUT_UI	output = (VS_OUTPUT_UI)0;

	output.Pos = mul(float4(input.Pos, 1.f), g_matUIWVP);

	if (input.Pos.x == 0.f)
		output.UV.x = g_NumberStartUV.x;

	else
		output.UV.x = g_NumberEndUV.x;

	if (input.Pos.y == 0.f)
		output.UV.y = g_NumberEndUV.y;

	else
		output.UV.y = g_NumberStartUV.y;

	return output;
}


PS_OUTPUT_SINGLE UINumberPS(VS_OUTPUT_UI input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	if (g_UITextureEnable == 1)
		output.Color = g_BaseTexture.Sample(g_AnisotropicSmp, input.UV) * g_UIColorTint;

	else
		output.Color = g_UIColorTint;

	return output;
}
