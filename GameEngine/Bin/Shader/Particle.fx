
#include "Share.fx"

cbuffer ParticleCBuffer : register(b11)
{
	float3	g_ParticlePos;
	uint	g_ParticleSpawnCount;
	float3	g_ParticleStartScale;
	uint	g_ParticleSpawnCountMax;
	float3	g_ParticleEndScale;
	float	g_ParticleLifeTimeMin;
	float3	g_ParticleRange;
	float	g_ParticleLifeTimeMax;
	float4	g_ParticleStartColor;
	float4	g_ParticleEndColor;
	float	g_MinSpeed;
	float	g_MaxSpeed;
	int		g_ParticleMove;
	int		g_Particle2D;
	float3	g_ParticleMoveDir;
	float	g_ParticleDefaultZ;
	float3	g_ParticleMoveAngle;
	int		g_ParticleGravity;
};

#define	GRAVITY	9.8f

struct ParticleInfo
{
	float3	WorldPos;
	float3	Dir;
	float	Speed;
	float	AddSpeed;
	float	LifeTime;
	float	MaxLifeTime;
	int		Alive;
	float	FallTime;
	float	FallStartY;
};

struct ParticleInfoShared
{
	uint	SpawnCount;
	uint	CurrentCount;

	float3	StartScale;
	float3	EndScale;
	
	float4	StartColor;
	float4	EndColor;

	int	GravityEnable;
};

RWStructuredBuffer<ParticleInfo>		g_ParticleArray	: register(u0);
RWStructuredBuffer<ParticleInfoShared>	g_ParticleShare	: register(u1);

[numthreads(64, 1, 1)]
void ParticleUpdate(uint3 ThreadID : SV_DispatchThreadID)
{
	g_ParticleShare[0].SpawnCount = g_ParticleSpawnCount;
	g_ParticleShare[0].StartScale = g_ParticleStartScale;
	g_ParticleShare[0].EndScale = g_ParticleEndScale;
	g_ParticleShare[0].StartColor = g_ParticleStartColor;
	g_ParticleShare[0].EndColor = g_ParticleEndColor;
	g_ParticleShare[0].GravityEnable = g_ParticleGravity;

	if (g_ParticleSpawnCountMax <= ThreadID.x)
		return;

	if (g_ParticleArray[ThreadID.x].Alive == 0)
	{
		int	SpawnCount = g_ParticleShare[0].SpawnCount;
		int	Exchange = 0;

		if (g_ParticleShare[0].SpawnCount == 1)
		{
			int	InputValue = SpawnCount - 1;

			// 함수 인자에 in, out, inout 을 붙일 수 있다.
			// in은 일반적인 함수 인자로 함수 외부에서 값을 넘겨줄때 사용한다.
			// out은 반쪽짜리 레퍼런스이다. 이 함수가 결과값을 return 말고
			// out으로 되어 있는 인자에 값을 넣어주면 그 값을 이 함수를 호출한
			// 부분에서 가져다가 사용할 수 있다.
			// 단, out은 함수 내에서 연산할때 사용하면 책임지지 않는다.
			// inout은 레퍼런스와 동일.
			// 아래 함수는 3번 인자에 1번의 원본값을 넣어준다.
			// 그러면서 1번인자의 값을 2번인자의 값으로 채워준다.
			InterlockedExchange(g_ParticleShare[0].SpawnCount, InputValue, Exchange);

			if (Exchange == SpawnCount)
				g_ParticleArray[ThreadID.x].Alive = 1;
		}

		if (g_ParticleArray[ThreadID.x].Alive == 0)
			return;

		// 살려야 하는 파티클이므로 파티클 정보를 계산한다.
		float	key = ThreadID.x / (float)g_ParticleSpawnCountMax;

		// Rand는 무조건 0 ~ 1 사이의 값을 얻어온다.
		float3	RandomPos = float3(Rand(key), Rand(key * 2.f), Rand(key * 3.f));

		// -0.5를 해주어서 -0.5 ~ 0.5 사이로 만들어주고 크기를 곱한다.
		float3	PosRange = (RandomPos - 0.5f) * g_ParticleRange;

		g_ParticleArray[ThreadID.x].WorldPos = g_ParticlePos + PosRange;

		float	ConvertY = g_ParticleArray[ThreadID.x].WorldPos.y - g_CameraBottom;

		float	RatioZ = ConvertY / (g_Resolution.y * 2.f);

		if (g_ParticleDefaultZ >= 0.7f)
		{
			// Min : 0.7f   Max : 0.99999f 
			g_ParticleArray[ThreadID.x].WorldPos.z = 
				(0.99999f - 0.7f) * RatioZ + 0.7f;
		}

		// Default
		else if (g_ParticleDefaultZ >= 0.3f)
		{
			// Min : 0.3f   Max : 0.69999f 
			g_ParticleArray[ThreadID.x].WorldPos.z = 
				(0.69999f - 0.3f) * RatioZ + 0.3f;
		}

		// Particle
		else
		{
			// Min : 0.f   Max : 0.29999f 
			g_ParticleArray[ThreadID.x].WorldPos.z = 
				0.29999f * RatioZ;
		}

		g_ParticleArray[ThreadID.x].FallTime = 0.f;
		g_ParticleArray[ThreadID.x].FallStartY = 
			g_ParticleArray[ThreadID.x].WorldPos.y;

		g_ParticleArray[ThreadID.x].MaxLifeTime = RandomPos.x * (g_ParticleLifeTimeMax - g_ParticleLifeTimeMin) + g_ParticleLifeTimeMin;
		g_ParticleArray[ThreadID.x].LifeTime = 0.f;

		if (g_ParticleMove == 1)
		{
			g_ParticleArray[ThreadID.x].Speed = RandomPos.x * (g_MaxSpeed - g_MinSpeed) + g_MinSpeed;

			float2	RandomDir = RandomPos.xy * 2.f - 1.f;
			g_ParticleArray[ThreadID.x].Dir = normalize(normalize(float3(RandomDir, 0.f)) + g_ParticleMoveDir);
			//g_ParticleArray[ThreadID.x].Dir = normalize(PosRange);
		}
	}

	// 현재 파티클이 살아있는 파티클이라면 파티클 정보를 갱신한다.
	else
	{
		g_ParticleArray[ThreadID.x].LifeTime += g_DeltaTime;

		if (g_ParticleMove == 1)
		{
			g_ParticleArray[ThreadID.x].WorldPos.xy +=
				g_ParticleArray[ThreadID.x].Dir.xy * 
				g_ParticleArray[ThreadID.x].Speed * g_DeltaTime;

			if (g_ParticleShare[0].GravityEnable)
			{
				g_ParticleArray[ThreadID.x].FallTime += g_DeltaTime * 10.f;

				float	Velocity = 0.1f * g_ParticleArray[ThreadID.x].Speed *
					g_ParticleArray[ThreadID.x].FallTime;

				g_ParticleArray[ThreadID.x].WorldPos.y =
					g_ParticleArray[ThreadID.x].FallStartY +
					(Velocity - 0.5f * GRAVITY * g_ParticleArray[ThreadID.x].FallTime * g_ParticleArray[ThreadID.x].FallTime);
			}

			float	ConvertY = g_ParticleArray[ThreadID.x].WorldPos.y - g_CameraBottom;

			float	RatioZ = ConvertY / (g_Resolution.y * 2.f);

			if (g_ParticleDefaultZ >= 0.7f)
			{
				// Min : 0.7f   Max : 0.99999f 
				g_ParticleArray[ThreadID.x].WorldPos.z =
					(0.99999f - 0.7f) * RatioZ + 0.7f;
			}

			// Default
			else if (g_ParticleDefaultZ >= 0.3f)
			{
				// Min : 0.3f   Max : 0.69999f 
				g_ParticleArray[ThreadID.x].WorldPos.z =
					(0.69999f - 0.3f) * RatioZ + 0.3f;
			}

			// Particle
			else
			{
				// Min : 0.f   Max : 0.29999f 
				g_ParticleArray[ThreadID.x].WorldPos.z =
					0.29999f * RatioZ;
			}
		}

		if (g_ParticleArray[ThreadID.x].LifeTime >=
			g_ParticleArray[ThreadID.x].MaxLifeTime)
		{
			g_ParticleArray[ThreadID.x].Alive = 0;
		}
	}
}


StructuredBuffer<ParticleInfo>		g_ParticleArrayInput	: register(t30);
StructuredBuffer<ParticleInfoShared>	g_ParticleShareInput	: register(t31);

struct VS_INPUT_PARTICLE
{
	float3	Pos : POSITION;
	uint	InstanceID : SV_InstanceID;
};

struct VS_OUTPUT_PARTICLE
{
	uint	InstanceID : TEXCOORD;
};

VS_OUTPUT_PARTICLE ParticleVS(VS_INPUT_PARTICLE input)
{
	VS_OUTPUT_PARTICLE	output = (VS_OUTPUT_PARTICLE)0;

	output.InstanceID = input.InstanceID;

	return output;
}

struct GS_PARTICLE_OUTPUT
{
	float4	Pos	: SV_Position;
	float4	Color : COLOR;
	float2	UV	: TEXCOORD;
	float4	ProjPos : POSITION;
};

static float4	g_ParticleLocalPos[4] =
{
	{-0.5f, 0.5f, 0.f, 0.f},
	{0.5f, 0.5f, 0.f, 0.f},
	{0.5f, -0.5f, 0.f, 0.f},
	{-0.5f, -0.5f, 0.f, 0.f}
};

[maxvertexcount(6)]
void ParticleGS(point VS_OUTPUT_PARTICLE input[1],
	inout TriangleStream<GS_PARTICLE_OUTPUT> OutputStream)
{
	uint	InstanceID = input[0].InstanceID;

	if (g_ParticleArrayInput[InstanceID].Alive == 0)
		return;

	GS_PARTICLE_OUTPUT	OutputArray[4] =
	{
		(GS_PARTICLE_OUTPUT)0.f,
		(GS_PARTICLE_OUTPUT)0.f,
		(GS_PARTICLE_OUTPUT)0.f,
		(GS_PARTICLE_OUTPUT)0.f
	};

	OutputArray[0].UV = float2(0.f, 0.f);
	OutputArray[1].UV = float2(1.f, 0.f);
	OutputArray[2].UV = float2(1.f, 1.f);
	OutputArray[3].UV = float2(0.f, 1.f);

	// UpdateShader에서 파티클의 월드좌표를 만들어놨다.
	// 그렇기 때문에 이 월드좌표를 뷰행렬 * 투영행렬을 해서 투영공간으로 변환한다.
	float	Ratio = g_ParticleArrayInput[InstanceID].LifeTime /
		g_ParticleArrayInput[InstanceID].MaxLifeTime;

	float3	Scale = lerp(g_ParticleShareInput[0].StartScale,
		g_ParticleShareInput[0].EndScale,
		float4(Ratio, Ratio, Ratio, Ratio)).xyz;

	float4	Color = lerp(g_ParticleShareInput[0].StartColor,
		g_ParticleShareInput[0].EndColor,
		float4(Ratio, Ratio, Ratio, Ratio));

	// 4개의 정점정보를 만들어준다.
	for (int i = 0; i < 4; ++i)
	{
		float3	WorldPos = (float3)0.f;
		
		WorldPos.xy = g_ParticleArrayInput[InstanceID].WorldPos.xy +
			g_ParticleLocalPos[i].xy * Scale.xy;
		WorldPos.z = g_ParticleArrayInput[InstanceID].WorldPos.z;

		OutputArray[i].ProjPos = mul(float4(WorldPos, 1.f), g_matVP);
		OutputArray[i].Pos = OutputArray[i].ProjPos;
		OutputArray[i].Color = Color;
	}

	OutputStream.Append(OutputArray[0]);
	OutputStream.Append(OutputArray[1]);
	OutputStream.Append(OutputArray[2]);
	OutputStream.RestartStrip();

	OutputStream.Append(OutputArray[0]);
	OutputStream.Append(OutputArray[2]);
	OutputStream.Append(OutputArray[3]);
	OutputStream.RestartStrip();
}

float4 ParticlePS(GS_PARTICLE_OUTPUT input) : SV_Target
{
	float4	Color = (float4)0.f;

	float4  BaseColor = g_BaseTexture.Sample(g_AnisotropicSmp, input.UV);
	float4	EmissiveColor = g_EmissiveTexture.Sample(g_AnisotropicSmp, input.UV);

	Color.rgb = BaseColor.rgb * g_vMtrlBaseColor.rgb * input.Color.rgb + EmissiveColor.rgb * g_vMtrlEmissiveColor.rgb;
	Color.a = BaseColor.a * g_MtrlOpacity * input.Color.a;

	Color = Distortion(Color, input.UV, input.ProjPos);

	return Color;
}

