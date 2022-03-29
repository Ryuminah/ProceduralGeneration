
#include "Share.fx"

cbuffer Collider : register(b11)
{
	float4  g_ColliderColor;
};


struct VS_INPUT_COLLIDER
{
	// : �ڿ� Semantic �� �ۼ��Ѵ�.
	// POSITION �������Ϳ� �ִ� ���� �����ڴٴ� �ǹ��̴�.
	// �ڿ��� �������� ��ȣ�� ���� �� �ִ�. POSITION1 �̷������� �ۼ��� �����ϴ�.
	// �Ⱥٿ��� ��쿡�� 0���� �ڵ����� ������ �ȴ�.
	float3  Pos : POSITION;
};

struct VS_OUTPUT_COLLIDER
{
	// SV �� ������ System Value �̴�.
	// �� �������� Shader �ڵ忡�� ���Ͽ����θ� ����� �ǰ� ��������� ����� �Ұ����ϴ�.
	// ���� �������� ��ȯ�� ������ SV_POSITION �������Ϳ� �־������ PixelShader�� ���۵Ǳ� ����
	// ȭ����������� ��ȯ�� �Ͼ�� �ȴ�.
	float4  Pos : SV_POSITION;
	float4	ProjPos : POSITION;
	float3	Normal : NORMAL;
};

VS_OUTPUT_COLLIDER ColliderVS(VS_INPUT_COLLIDER input)
{
	VS_OUTPUT_COLLIDER output = (VS_OUTPUT_COLLIDER)0;

	// output.Pos �� x, y, z ���� input.Pos�� x, y, z �� ���� w���� 1.f �� ���� �ȴ�.
	output.ProjPos = mul(float4(input.Pos, 1.f), g_matWVP);
	output.Pos = output.ProjPos;
	output.Normal = normalize(mul(float4(input.Pos, 0.f), g_matWV).xyz);

	return output;
}


PS_OUTPUT_SINGLE ColliderPS(VS_OUTPUT_COLLIDER input)
{
	PS_OUTPUT_SINGLE    output = (PS_OUTPUT_SINGLE)0;

	output.Color = g_ColliderColor;

	return output;
}

PS_OUTPUT_GBUFFER Collider3DPS(VS_OUTPUT_COLLIDER input)
{
	PS_OUTPUT_GBUFFER    output = (PS_OUTPUT_GBUFFER)0;

	output.Albedo = g_ColliderColor;// *(result.Dif + result.Amb) + result.Spc + result.Emv;
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
