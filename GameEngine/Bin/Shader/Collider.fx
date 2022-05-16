
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
};

VS_OUTPUT_COLLIDER ColliderVS(VS_INPUT_COLLIDER input)
{
	VS_OUTPUT_COLLIDER output = (VS_OUTPUT_COLLIDER)0;

	// output.Pos �� x, y, z ���� input.Pos�� x, y, z �� ���� w���� 1.f �� ���� �ȴ�.
	output.Pos = mul(float4(input.Pos, 1.f), g_matWVP);

	return output;
}


PS_OUTPUT_SINGLE ColliderPS(VS_OUTPUT_COLLIDER input)
{
	PS_OUTPUT_SINGLE    output = (PS_OUTPUT_SINGLE)0;

	output.Color = g_ColliderColor;

	return output;
}
