
#include "Share.fx"



struct PS_OUTPUT_LIGHTACC
{
	float4	Dif	: SV_TARGET;
	float4	Spc	: SV_TARGET1;
	float4	Emv	: SV_TARGET2;
};


struct VS_INPUT_LIGHTACC
{
	float3	Pos : POSITION;
};

struct VS_OUTPUT_LIGHTACC
{
	float4	Pos : SV_POSITION;
	float4	ProjPos : POSITION;
};

Texture2D       g_AlbedoTarget	   : register(t10);
Texture2D       g_GBuffer1Target   : register(t11);
Texture2D       g_GBuffer2Target   : register(t12);
Texture2D       g_GBuffer3Target   : register(t13);

Texture2D       g_LightDifTex   : register(t14);
Texture2D       g_LightSpcTex   : register(t15);
Texture2D       g_LightEmvTex   : register(t16);

Texture2D       g_ShadowMapTex   : register(t17);

cbuffer Shadow : register(b12)
{
	matrix	g_matShadow;
	matrix	g_matShadowInvVP;
	float	g_ShadowBias;
	float3	g_ShadowEmpty;
};



LightResult ComputeLightGBuffer(float3 Pos, float3 Normal, float2 UV, float SpecularPower,
	float4 MtrlColor)
{
	LightResult	result = (LightResult)0.f;

	float3	LightDir = (float3)0.f;
	float	Attn = 1.f;

	if (g_LightLightType == LightTypeDir)
	{
		LightDir = -g_LightDir;
		LightDir = normalize(LightDir);
	}

	if (g_LightLightType == LightTypePoint)
	{
		LightDir = g_LightPos - Pos;
		LightDir = normalize(LightDir);

		float	Dist = distance(g_LightPos, Pos);

		if (Dist > g_LightDistance)
			Attn = 0.f;

		else
		{
			Attn = 1.f / (g_LightAtt1 + g_LightAtt2 * Dist + g_LightAtt3 * (Dist * Dist));
		}
	}

	if (g_LightLightType == LightTypeSpot)
	{
	}

	// Color를 얻어온다.
	float4	MtrlDif = ConvertColor(MtrlColor.x);
	float4	MtrlAmb = ConvertColor(MtrlColor.y);
	float4	MtrlSpc = ConvertColor(MtrlColor.z);
	float4	MtrlEmv = ConvertColor(MtrlColor.w);

	float	Intensity = max(0.f, dot(Normal, LightDir));

	result.Dif = g_LightDiffuse * MtrlDif * Intensity * Attn;
	result.Amb = g_LightAmbient * MtrlAmb * Attn;

	// 비금속 표면에 대한 Specular 처리를 위해 반사벡터를 구한다.
	float3	Reflect = 2.f * dot(Normal, LightDir) * Normal - LightDir;
	Reflect = normalize(Reflect);

	float3	View = -Pos;
	View = normalize(View);

	result.Spc = g_LightSpecular * MtrlSpc *
		pow(max(0.f, dot(View, Reflect)), SpecularPower) * Attn;

	result.Emv = MtrlEmv;

	return result;
}



VS_OUTPUT_LIGHTACC LightDirVS(uint VertexID : SV_VertexID)
{
	VS_OUTPUT_LIGHTACC	output = (VS_OUTPUT_LIGHTACC)0;

	output.ProjPos = float4(g_NullPos[VertexID], 0.f, 1.f);
	output.Pos = output.ProjPos;

	return output;
}

VS_OUTPUT_LIGHTACC LightAccVS(VS_INPUT_LIGHTACC input)
{
	VS_OUTPUT_LIGHTACC	output = (VS_OUTPUT_LIGHTACC)0;

	// output.Pos 의 x, y, z 에는 input.Pos의 x, y, z 가 들어가고 w에는 1.f 이 들어가게 된다.
	//float3	ViewPos = mul(float4(input.Pos, 1.f), g_matWV).xyz;

	//if (ViewPos.z < 0.f)
	//	ViewPos.z = 1.5f;

	//output.ProjPos = mul(float4(ViewPos, 1.f), g_matProj);
	output.ProjPos = mul(float4(input.Pos, 1.f), g_matWVP);

	if (output.ProjPos.w < 0.f)
	{
		output.ProjPos.w = 1.f;
		output.ProjPos.z = 0.1f;
	}

	output.Pos = output.ProjPos;

	return output;
}

PS_OUTPUT_LIGHTACC LightAccPS(VS_OUTPUT_LIGHTACC input)
{
	PS_OUTPUT_LIGHTACC    output = (PS_OUTPUT_LIGHTACC)0;
	
	float2	UV;
	UV.x = input.ProjPos.x / input.ProjPos.w * 0.5f + 0.5f;
	UV.y = input.ProjPos.y / input.ProjPos.w * -0.5f + 0.5f;

	float4	Albedo = g_AlbedoTarget.Sample(g_PointSmp, UV);

	if (Albedo.a == 0.f)
		clip(-1);

	float4	GBuffer1 = g_GBuffer1Target.Sample(g_PointSmp, UV);
	float4	GBuffer2 = g_GBuffer2Target.Sample(g_PointSmp, UV);
	float4	GBuffer3 = g_GBuffer3Target.Sample(g_PointSmp, UV);

	// uv를 이용해서 위치를 구해준다.
	float4	ProjPos;
	ProjPos.x = UV.x * 2.f - 1.f;
	ProjPos.y = UV.y * -2.f + 1.f;
	ProjPos.z = GBuffer2.x;
	ProjPos.w = 1.f;

	// 2 * 3 = 6  이것에서 6을 3을 곱하기 전으로 만들려면 어떻게 해야 할까요?
	// 2 * 3 / 3 = 6 / 3
	ProjPos *= GBuffer2.y;

	// 로컬정점 * 월드 * 뷰 * 투영 = ProjPos
	// 로컬정점 * 월드 * 뷰 * 투영 / 투영 = PrjoPos / 투영
	// 로컬정점 * 월드 * 뷰 = ViewPos
	float3	ViewPos = mul(ProjPos, g_matInvProj).xyz;

	float3	ViewNormal = GBuffer1.xyz;

	LightResult result = ComputeLightGBuffer(ViewPos, ViewNormal, UV, GBuffer2.z, GBuffer3);

	if (GBuffer1.w == 0.f)
	{
		output.Dif = float4(1.f, 1.f, 1.f, 1.f);
		output.Spc = float4(0.f, 0.f, 0.f, 0.f);
		output.Emv = float4(0.f, 0.f, 0.f, 0.f);
	}

	else
	{
		output.Dif = result.Dif + result.Amb;
		output.Dif.w = 1.f;
		output.Spc = result.Spc;
		output.Spc.w = 1.f;
		output.Emv = result.Emv;
		output.Emv.w = 1.f;
	}

	return output;
}

PS_OUTPUT_SINGLE LightCombinePS(VS_OUTPUT_NULLBUFFER input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float4	Albedo = g_AlbedoTarget.Sample(g_PointSmp, input.UV);

	if (Albedo.a == 0.f)
		clip(-1);

	float4	LightDif = g_LightDifTex.Sample(g_PointSmp, input.UV);
	float4	LightSpc = g_LightSpcTex.Sample(g_PointSmp, input.UV);
	float4	LightEmv = g_LightEmvTex.Sample(g_PointSmp, input.UV);

	// 그림자 부분을 판단한다.
	float4	GBuffer2 = g_GBuffer2Target.Sample(g_PointSmp, input.UV);

	// uv를 이용해서 위치를 구해준다.
	float4	ProjPos;
	ProjPos.x = input.UV.x * 2.f - 1.f;
	ProjPos.y = input.UV.y * -2.f + 1.f;
	ProjPos.z = GBuffer2.x;
	ProjPos.w = 1.f;

	// 2 * 3 = 6  이것에서 6을 3을 곱하기 전으로 만들려면 어떻게 해야 할까요?
	// 2 * 3 / 3 = 6 / 3
	ProjPos *= GBuffer2.y;

	// 로컬정점 * 월드 * 뷰 * 투영 = ProjPos
	// 로컬정점 * 월드 * 뷰 * 투영 / 투영 = PrjoPos / 투영
	// 로컬정점 * 월드 * 뷰 = ViewPos
	float3	WorldPos = mul(ProjPos, g_matShadowInvVP).xyz;

	float4	ShadowPos = mul(float4(WorldPos, 1.f), g_matShadow);

	//float	CurrentDepth = ShadowPos.z / ShadowPos.w;

	float2	ShadowUV;
	ShadowUV.x = ShadowPos.x / ShadowPos.w * 0.5f + 0.5f;
	ShadowUV.y = ShadowPos.y / ShadowPos.w * -0.5f + 0.5f;

	float4	ShadowMap = g_ShadowMapTex.Sample(g_PointSmp, ShadowUV);

	if (ShadowMap.a > 0.f)
	{
		// 그림자 부분일 경우 어둡게 한다.
		if (ShadowPos.z - g_ShadowBias > ShadowMap.r * ShadowPos.w)
		{
			LightDif *= 0.2f;
			LightSpc *= 0.f;
		}
	}


	output.Color.xyz = Albedo.xyz * LightDif.xyz + LightSpc.xyz + LightEmv.xyz;
	output.Color.a = Albedo.a;

	return output;
}

VS_OUTPUT_NULLBUFFER LightCombineRenderVS(uint VertexID : SV_VertexID)
{
	VS_OUTPUT_NULLBUFFER	output = (VS_OUTPUT_NULLBUFFER)0;

	output.ProjPos = float4(g_NullPos[VertexID], 0.f, 1.f);
	output.Pos = output.ProjPos;
	output.UV = g_NullUV[VertexID];

	return output;
}

PS_OUTPUT_SINGLE LightCombineRenderPS(VS_OUTPUT_NULLBUFFER input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float4	BaseColor = g_BaseTexture.Sample(g_AnisotropicSmp, input.UV);

	if (BaseColor.a == 0.f)
		clip(-1);

	output.Color = BaseColor;

	return output;
}
