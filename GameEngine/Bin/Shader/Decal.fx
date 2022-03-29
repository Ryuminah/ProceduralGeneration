
#include "Share.fx"

struct VS_OUTPUT_DECAL
{
	float4	Pos : SV_POSITION;
	float4	ProjPos : POSITION;
};

struct VS_OUTPUT_DECALDEBUG
{
	float4	Pos : SV_POSITION;
	float4	ProjPos : POSITION;
	float3	Normal	: NORMAL;
};

struct PS_OUTPUT_DECAL
{
	float4	Albedo	 : SV_TARGET;
	float4	Normal   : SV_TARGET1;
	float4	Material : SV_TARGET2;
};


Texture2D       g_GBufferDepth	   : register(t10);
Texture2D       g_GBufferTangent   : register(t11);
Texture2D       g_GBufferBinormal   : register(t12);

VS_OUTPUT_DECAL DecalVS(float3 Pos : POSITION)
{
	VS_OUTPUT_DECAL	output = (VS_OUTPUT_DECAL)0;

	output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);
	output.Pos = output.ProjPos;

	return output;
}

PS_OUTPUT_DECAL DecalPS(VS_OUTPUT_DECAL input)
{
	PS_OUTPUT_DECAL	output = (PS_OUTPUT_DECAL)0;

	float2	ScreenUV = input.ProjPos.xy / input.ProjPos.w;
	ScreenUV = ScreenUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

	float4	Tangent = g_GBufferTangent.Sample(g_PointSmp, ScreenUV);
	float4	Depth = g_GBufferDepth.Sample(g_PointSmp, ScreenUV);

	if (Tangent.a == 0.f)
		clip(-1);

	else if (Depth.w == 0)
		clip(-1);

	float4	Pos;
	Pos.x = (ScreenUV.x * 2.f - 1.f) * Depth.y;
	Pos.y = (ScreenUV.y * -2.f + 1.f) * Depth.y;
	Pos.z = Depth.x * Depth.y;
	Pos.w = Depth.y;

	// 로컬공간으로 변환한다.
	float3	LocalPos = mul(Pos, g_matInvWVP).xyz;

	clip(0.5f - abs(LocalPos));

	float2	UV = LocalPos.xz + float2(0.5f, 0.5f);
	UV.y = 1.f - UV.y;

	float4	Color = g_BaseTexture.Sample(g_AnisotropicSmp, UV);

	if (Color.a == 0.f)
		clip(-1);

	float4	Binormal = g_GBufferBinormal.Sample(g_PointSmp, ScreenUV);
	float3	VtxNormal = normalize(cross(Tangent.xyz, Binormal.xyz));

	output.Normal.xyz = ComputeBumpNormal(VtxNormal, Tangent.xyz, Binormal.xyz, UV);

	float	Alpha = Color.a * g_MtrlOpacity;

	output.Albedo.xyz = Color.xyz;
	output.Albedo.a = Alpha;

	output.Material.x = ConvertColor(g_vMtrlBaseColor);
	output.Material.y = ConvertColor(g_vMtrlAmbientColor);

	float4	MtrlSpc = g_vMtrlSpecularColor;

	if (g_MtrlSpcTex)
		MtrlSpc = g_SpecularTexture.Sample(g_AnisotropicSmp, UV).rrrr;

	output.Material.z = ConvertColor(MtrlSpc);

	float4	MtrlEmv = g_vMtrlEmissiveColor;

	if (g_MtrlEmvTex)
		MtrlEmv = g_EmissiveTexture.Sample(g_AnisotropicSmp, UV);

	output.Material.w = ConvertColor(MtrlEmv);

	return output;
}


VS_OUTPUT_DECALDEBUG DecalDebugVS(float3 Pos : POSITION)
{
	VS_OUTPUT_DECALDEBUG	output = (VS_OUTPUT_DECALDEBUG)0;

	output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);
	output.Pos = output.ProjPos;
	output.Normal = normalize(mul(float4(Pos, 0.f), g_matWV).xyz);

	return output;
}

PS_OUTPUT_GBUFFER DecalDebugPS(VS_OUTPUT_DECALDEBUG input)
{
	PS_OUTPUT_GBUFFER    output = (PS_OUTPUT_GBUFFER)0;

	output.Albedo = float4(0.f, 1.f, 0.f, 1.f);
	output.GBuffer1.xyz = input.Normal;
	output.GBuffer1.w = 0.f;

	output.GBuffer2.x = input.ProjPos.z / input.ProjPos.w;
	output.GBuffer2.y = input.ProjPos.w;
	output.GBuffer2.z = 0.f;
	output.GBuffer2.w = 0.f;

	output.GBuffer3.x = ConvertColor(float4(1.f, 1.f, 1.f, 1.f));
	output.GBuffer3.y = ConvertColor(float4(0.f, 0.f, 0.f, 0.f));

	output.GBuffer3.z = ConvertColor(float4(0.f, 0.f, 0.f, 0.f));

	output.GBuffer3.w = ConvertColor(float4(0.f, 0.f, 0.f, 0.f));

	return output;
}

