
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

struct VS_OUTPUT_3D
{
	float4	Pos	: SV_POSITION;
	float4	ProjPos : POSITION;
	float3	ViewPos : POSITION1;
	float3	Normal : NORMAL;
	float2	UV	: TEXCOORD;
	float3	Tangent : TANGENT;
	float3	Binormal : BINORMAL;
};

cbuffer LandScape : register(b11)
{
	float	g_LandScapeDetailLevel;
	int		g_LandScapeSplatCount;
	float2	g_LandScapeEmpty;
};

Texture2DArray	g_SplatAlbedo	: register(t30);
Texture2DArray	g_SplatNormal	: register(t31);
Texture2DArray	g_SplatSpecular	: register(t32);
Texture2DArray	g_SplatAlpha	: register(t33);

float3 ComputeLandScapeBumpNormal(float3 Normal, float3 Tangent, float3 Binormal, float2 DetailUV,
	float2 SplatUV)
{
	float3	result = Normal;
	

	if (g_MtrlBumpEnable == 1)
	{
		float4  NormalColor = g_NormalTexture.Sample(g_AnisotropicSmp, DetailUV);

		for (int i = 0; i < g_LandScapeSplatCount; ++i)
		{
			float4	SplatNormal = g_SplatNormal.Sample(g_AnisotropicSmp, float3(DetailUV, i));

			float4	SplatAlpha = g_SplatAlpha.Sample(g_AnisotropicSmp, float3(SplatUV, i));

			NormalColor.rgb = NormalColor.rgb * (1.f - SplatAlpha.r) + SplatNormal.rgb * SplatAlpha.r;
		}

		float3	ConvertNormal = NormalColor.xyz * 2.f - 1.f;
		ConvertNormal.z = 1.f;
		ConvertNormal = normalize(ConvertNormal);

		float3x3 mat =
		{
			Tangent,
			Binormal,
			Normal
		};

		result = normalize(mul(ConvertNormal, mat));
	}

	return result;
}

VS_OUTPUT_3D LandScapeVS(VS_INPUT_3D input)
{
	VS_OUTPUT_3D	output = (VS_OUTPUT_3D)0;

	// output.Pos 의 x, y, z 에는 input.Pos의 x, y, z 가 들어가고 w에는 1.f 이 들어가게 된다.
	output.ProjPos = mul(float4(input.Pos, 1.f), g_matWVP);
	output.Pos = output.ProjPos;

	output.ViewPos = mul(float4(input.Pos, 1.f), g_matWV).xyz;

	// Bump 처리를 할때 
	// Z축
	output.Normal = normalize(mul(float4(input.Normal, 0.f), g_matWV).xyz);

	// X축
	output.Tangent = normalize(mul(float4(input.Tangent, 0.f), g_matWV).xyz);

	// Y축
	output.Binormal = normalize(mul(float4(input.Binormal, 0.f), g_matWV).xyz);

	output.UV = input.UV;

	return output;
}

PS_OUTPUT_GBUFFER LandScapePS(VS_OUTPUT_3D input)
{
	PS_OUTPUT_GBUFFER	output = (PS_OUTPUT_GBUFFER)0;

	float2	DetailUV = input.UV * g_LandScapeDetailLevel;

	float4  BaseColor = g_BaseTexture.Sample(g_AnisotropicSmp, DetailUV);

	for (int i = 0; i < g_LandScapeSplatCount; ++i)
	{
		float4	SplatAlbedo = g_SplatAlbedo.Sample(g_AnisotropicSmp, float3(DetailUV, i));

		float4	SplatAlpha = g_SplatAlpha.Sample(g_AnisotropicSmp, float3(input.UV, i));

		BaseColor.rgb = BaseColor.rgb * (1.f - SplatAlpha.r) + SplatAlbedo.rgb * SplatAlpha.r;
	}

	/*LightResult result = ComputeLight(input.ViewPos, input.Normal, input.Tangent,
		input.Binormal, input.UV);*/

	output.Albedo = BaseColor;// *(result.Dif + result.Amb) + result.Spc + result.Emv;
	output.GBuffer1.xyz = ComputeLandScapeBumpNormal(input.Normal, input.Tangent, input.Binormal, DetailUV, input.UV);
	output.GBuffer1.w = 1.f;

	output.GBuffer2.x = input.ProjPos.z / input.ProjPos.w;
	output.GBuffer2.y = input.ProjPos.w;
	output.GBuffer2.z = g_vMtrlSpecularColor.w;
	output.GBuffer2.w = g_MtrlReceiveDecal;

	output.GBuffer4.xyz = input.Tangent;
	output.GBuffer4.a = 1.f;

	output.GBuffer5.xyz = input.Binormal;
	output.GBuffer5.a = 1.f;

	output.GBuffer3.x = ConvertColor(g_vMtrlBaseColor);
	output.GBuffer3.y = ConvertColor(g_vMtrlAmbientColor);

	float4	MtrlSpc = g_vMtrlSpecularColor;

	if (g_MtrlSpcTex)
		MtrlSpc = g_SpecularTexture.Sample(g_AnisotropicSmp, DetailUV);

	for (int i = 0; i < g_LandScapeSplatCount; ++i)
	{
		float4	SplatSpc = g_SplatSpecular.Sample(g_AnisotropicSmp, float3(DetailUV, i));

		float4	SplatAlpha = g_SplatAlpha.Sample(g_AnisotropicSmp, float3(input.UV, i));

		MtrlSpc.rgb = MtrlSpc.rgb * (1.f - SplatAlpha.r) + SplatSpc.rgb * SplatAlpha.r;
	}

	output.GBuffer3.z = ConvertColor(MtrlSpc);

	float4	MtrlEmv = g_vMtrlEmissiveColor;

	if (g_MtrlEmvTex)
		MtrlEmv = g_EmissiveTexture.Sample(g_AnisotropicSmp, input.UV);

	output.GBuffer3.w = ConvertColor(MtrlEmv);

	return output;
}
