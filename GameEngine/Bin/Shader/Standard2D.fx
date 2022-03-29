
#include "Share.fx"

/*
int : ����
float : �Ǽ�
float2 ~ 4 : 2, 3, 4���� ����(floatŸ��)
int2 ~ 4 : 2, 3, 4���� ����(intŸ��)
matrix : 4x4 ���
float3x3 : 3x3 ��� floatŸ��
*/

Texture2D	g_GBuffer2	: register(t50);


struct VS_INPUT_COLOR
{
	// : �ڿ� Semantic �� �ۼ��Ѵ�.
	// POSITION �������Ϳ� �ִ� ���� �����ڴٴ� �ǹ��̴�.
	// �ڿ��� �������� ��ȣ�� ���� �� �ִ�. POSITION1 �̷������� �ۼ��� �����ϴ�.
	// �Ⱥٿ��� ��쿡�� 0���� �ڵ����� ������ �ȴ�.
	float3  Pos : POSITION;
	float4  Color : COLOR;
};

struct VS_OUTPUT_COLOR
{
	// SV �� ������ System Value �̴�.
	// �� �������� Shader �ڵ忡�� ���Ͽ����θ� ����� �ǰ� ��������� ����� �Ұ����ϴ�.
	// ���� �������� ��ȯ�� ������ SV_POSITION �������Ϳ� �־������ PixelShader�� ���۵Ǳ� ����
	// ȭ����������� ��ȯ�� �Ͼ�� �ȴ�.
	float4  Pos : SV_POSITION;
	float4  Color : COLOR;
	float2	UV	: TEXCOORD;
};

float2 ComputeAnimation2DUV(float2 UV)
{
	if (g_Animation2DEnable == 0)
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

VS_OUTPUT_COLOR Standard2DColorVS(VS_INPUT_COLOR input)
{
	VS_OUTPUT_COLOR output = (VS_OUTPUT_COLOR)0;

	float3  Pos = input.Pos - g_vPivot * g_vMeshSize;

	// output.Pos �� x, y, z ���� input.Pos�� x, y, z �� ���� w���� 1.f �� ���� �ȴ�.
	output.Pos = mul(float4(Pos, 1.f), g_matWVP);
	output.Color = input.Color;

	return output;
}


PS_OUTPUT_SINGLE Standard2DColorPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT_SINGLE    output = (PS_OUTPUT_SINGLE)0;

	float4	Color = input.Color * g_vMtrlBaseColor + g_vMtrlEmissiveColor;

	output.Color = Color;

	return output;
}

struct VS_INPUT_UV
{
	// : �ڿ� Semantic �� �ۼ��Ѵ�.
	// POSITION �������Ϳ� �ִ� ���� �����ڴٴ� �ǹ��̴�.
	// �ڿ��� �������� ��ȣ�� ���� �� �ִ�. POSITION1 �̷������� �ۼ��� �����ϴ�.
	// �Ⱥٿ��� ��쿡�� 0���� �ڵ����� ������ �ȴ�.
	float3  Pos : POSITION;
	float2  UV : TEXCOORD;
};

struct VS_OUTPUT_UV
{
	// SV �� ������ System Value �̴�.
	// �� �������� Shader �ڵ忡�� ���Ͽ����θ� ����� �ǰ� ��������� ����� �Ұ����ϴ�.
	// ���� �������� ��ȯ�� ������ SV_POSITION �������Ϳ� �־������ PixelShader�� ���۵Ǳ� ����
	// ȭ����������� ��ȯ�� �Ͼ�� �ȴ�.
	float4  Pos : SV_POSITION;
	float2  UV : TEXCOORD;
	float4	ProjPos : POSITION;
};

VS_OUTPUT_UV Standard2DTextureVS(VS_INPUT_UV input)
{
	VS_OUTPUT_UV output = (VS_OUTPUT_UV)0;

	float3  Pos = input.Pos - g_vPivot * g_vMeshSize;

	// output.Pos �� x, y, z ���� input.Pos�� x, y, z �� ���� w���� 1.f �� ���� �ȴ�.
	output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);
	output.Pos = output.ProjPos;
	output.UV = ComputeAnimation2DUV(input.UV);

	return output;
}


PS_OUTPUT_SINGLE Standard2DTexturePS(VS_OUTPUT_UV input)
{
	PS_OUTPUT_SINGLE    output = (PS_OUTPUT_SINGLE)0;

	float4  BaseColor = g_BaseTexture.Sample(g_AnisotropicSmp, input.UV);
	float4	EmissiveColor = g_EmissiveTexture.Sample(g_AnisotropicSmp, input.UV);
	
	float4	result = (float4)0.f;

	result.rgb = BaseColor.rgb * g_vMtrlBaseColor.rgb + EmissiveColor.rgb * g_vMtrlEmissiveColor.rgb;
	result.a = BaseColor.a * g_MtrlOpacity;

	result = PaperBurn2D(result, input.UV);

	result = Distortion(result, input.UV, input.ProjPos);

	output.Color = result;

	return output;
}

VS_OUTPUT_UV BillboardTextureVS(VS_INPUT_UV input)
{
	VS_OUTPUT_UV output = (VS_OUTPUT_UV)0;

	// output.Pos �� x, y, z ���� input.Pos�� x, y, z �� ���� w���� 1.f �� ���� �ȴ�.
	output.ProjPos = mul(float4(input.Pos, 1.f), g_matWVP);
	output.Pos = output.ProjPos;
	output.UV = input.UV;

	return output;
}


PS_OUTPUT_SINGLE BillboardTexturePS(VS_OUTPUT_UV input)
{
	PS_OUTPUT_SINGLE    output = (PS_OUTPUT_SINGLE)0;

	float4  BaseColor = g_BaseTexture.Sample(g_AnisotropicSmp, input.UV);

	// ���� ȭ�鿡���� UV�� �����ش�.
	float2	ScreenUV;
	ScreenUV.x = input.ProjPos.x / input.ProjPos.w * 0.5f + 0.5f;
	ScreenUV.y = input.ProjPos.y / input.ProjPos.w * -0.5f + 0.5f;

	float4  GBuffer2 = g_GBuffer2.Sample(g_AnisotropicSmp, ScreenUV);

	float	Alpha = 1.f;

	if (GBuffer2.w > 0.f)
		Alpha = (GBuffer2.y - input.ProjPos.w) / 0.5f;

	float4	result = (float4)0.f;

	result.rgb = BaseColor.rgb * g_vMtrlBaseColor.rgb;
	result.a = BaseColor.a * g_MtrlOpacity * Alpha;

	output.Color = result;

	return output;
}
