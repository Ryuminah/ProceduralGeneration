
#include "Share.fx"

/*
int : 정수
float : 실수
float2 ~ 4 : 2, 3, 4차원 벡터(float타입)
int2 ~ 4 : 2, 3, 4차원 벡터(int타입)
matrix : 4x4 행렬
float3x3 : 3x3 행렬 float타입
*/

Texture2D	g_GBuffer2	: register(t50);


struct VS_INPUT_COLOR
{
	// : 뒤에 Semantic 을 작성한다.
	// POSITION 레지스터에 있는 값을 얻어오겠다는 의미이다.
	// 뒤에는 레지스터 번호가 붙을 수 있다. POSITION1 이런식으로 작성이 가능하다.
	// 안붙여줄 경우에는 0번이 자동으로 지정이 된다.
	float3  Pos : POSITION;
	float4  Color : COLOR;
};

struct VS_OUTPUT_COLOR
{
	// SV 가 붙으면 System Value 이다.
	// 이 변수들은 Shader 코드에서 리턴용으로만 사용이 되고 연산용으로 사용이 불가능하다.
	// 투영 공간으로 변환된 정점을 SV_POSITION 레지스터에 넣어놓으면 PixelShader가 동작되기 전에
	// 화면공간으로의 변환이 일어나게 된다.
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

	// output.Pos 의 x, y, z 에는 input.Pos의 x, y, z 가 들어가고 w에는 1.f 이 들어가게 된다.
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
	// : 뒤에 Semantic 을 작성한다.
	// POSITION 레지스터에 있는 값을 얻어오겠다는 의미이다.
	// 뒤에는 레지스터 번호가 붙을 수 있다. POSITION1 이런식으로 작성이 가능하다.
	// 안붙여줄 경우에는 0번이 자동으로 지정이 된다.
	float3  Pos : POSITION;
	float2  UV : TEXCOORD;
};

struct VS_OUTPUT_UV
{
	// SV 가 붙으면 System Value 이다.
	// 이 변수들은 Shader 코드에서 리턴용으로만 사용이 되고 연산용으로 사용이 불가능하다.
	// 투영 공간으로 변환된 정점을 SV_POSITION 레지스터에 넣어놓으면 PixelShader가 동작되기 전에
	// 화면공간으로의 변환이 일어나게 된다.
	float4  Pos : SV_POSITION;
	float2  UV : TEXCOORD;
	float4	ProjPos : POSITION;
};

VS_OUTPUT_UV Standard2DTextureVS(VS_INPUT_UV input)
{
	VS_OUTPUT_UV output = (VS_OUTPUT_UV)0;

	float3  Pos = input.Pos - g_vPivot * g_vMeshSize;

	// output.Pos 의 x, y, z 에는 input.Pos의 x, y, z 가 들어가고 w에는 1.f 이 들어가게 된다.
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

	// output.Pos 의 x, y, z 에는 input.Pos의 x, y, z 가 들어가고 w에는 1.f 이 들어가게 된다.
	output.ProjPos = mul(float4(input.Pos, 1.f), g_matWVP);
	output.Pos = output.ProjPos;
	output.UV = input.UV;

	return output;
}


PS_OUTPUT_SINGLE BillboardTexturePS(VS_OUTPUT_UV input)
{
	PS_OUTPUT_SINGLE    output = (PS_OUTPUT_SINGLE)0;

	float4  BaseColor = g_BaseTexture.Sample(g_AnisotropicSmp, input.UV);

	// 현재 화면에서의 UV를 구해준다.
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
