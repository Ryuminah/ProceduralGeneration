
#include "Share.fx"

cbuffer Collider : register(b11)
{
	float4  g_ColliderColor;
};


struct VS_INPUT_COLLIDER
{
	// : 뒤에 Semantic 을 작성한다.
	// POSITION 레지스터에 있는 값을 얻어오겠다는 의미이다.
	// 뒤에는 레지스터 번호가 붙을 수 있다. POSITION1 이런식으로 작성이 가능하다.
	// 안붙여줄 경우에는 0번이 자동으로 지정이 된다.
	float3  Pos : POSITION;
};

struct VS_OUTPUT_COLLIDER
{
	// SV 가 붙으면 System Value 이다.
	// 이 변수들은 Shader 코드에서 리턴용으로만 사용이 되고 연산용으로 사용이 불가능하다.
	// 투영 공간으로 변환된 정점을 SV_POSITION 레지스터에 넣어놓으면 PixelShader가 동작되기 전에
	// 화면공간으로의 변환이 일어나게 된다.
	float4  Pos : SV_POSITION;
};

VS_OUTPUT_COLLIDER ColliderVS(VS_INPUT_COLLIDER input)
{
	VS_OUTPUT_COLLIDER output = (VS_OUTPUT_COLLIDER)0;

	// output.Pos 의 x, y, z 에는 input.Pos의 x, y, z 가 들어가고 w에는 1.f 이 들어가게 된다.
	output.Pos = mul(float4(input.Pos, 1.f), g_matWVP);

	return output;
}


PS_OUTPUT_SINGLE ColliderPS(VS_OUTPUT_COLLIDER input)
{
	PS_OUTPUT_SINGLE    output = (PS_OUTPUT_SINGLE)0;

	output.Color = g_ColliderColor;

	return output;
}
