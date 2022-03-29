
#include "Share.fx"


/*
DirectX11에서 지원되는 함수들은 Shader에서 지원안되는 경우가 많다.
그러므로 비교나 기타 등등 지원 안되는 함수들은 여기에서 모두 구현하여 사용해야 한다.
구면선형보간같은것들이 여기에 속한다.
*/

float4 VectorLess(float4 _vQ1, float4 _vQ2)
{
	float4 vReturn =
	{
		(_vQ1[0] < _vQ2[0]) ? asfloat((uint) 0xFFFFFFFF) : 0.f,
		(_vQ1[1] < _vQ2[1]) ? asfloat((uint) 0xFFFFFFFF) : 0.f,
		(_vQ1[2] < _vQ2[2]) ? asfloat((uint) 0xFFFFFFFF) : 0.f,
		(_vQ1[3] < _vQ2[3]) ? asfloat((uint) 0xFFFFFFFF) : 0.f
	};

	return vReturn;
}

void VectorPermute(uint PermuteX, uint PermuteY, uint PermuteZ, uint PermuteW
	, in float4 V1, in float4 V2
	, out float4 _vOut)
{
	float4 aPtr[2] = { V1, V2 };
	float4 Result = (float4) 0.f;

	const uint i0 = PermuteX & 3;
	const uint vi0 = PermuteX >> 2;
	Result[0] = aPtr[vi0][i0];

	const uint i1 = PermuteY & 3;
	const uint vi1 = PermuteY >> 2;
	Result[1] = aPtr[vi1][i1];

	const uint i2 = PermuteZ & 3;
	const uint vi2 = PermuteZ >> 2;
	Result[2] = aPtr[vi2][i2];

	const uint i3 = PermuteW & 3;
	const uint vi3 = PermuteW >> 2;
	Result[3] = aPtr[vi3][i3];

	_vOut = Result;
}


float4 VectorShiftLeft(in float4 _V1, in float4 _V2, uint _Elements)
{
	float4 vOut = (float4) 0.f;

	VectorPermute(_Elements, ((_Elements)+1), ((_Elements)+2), ((_Elements)+3), _V1, _V2, vOut);

	return vOut;
}

float4 VectorSelect(float4 _vQ1, float4 _vQ2, float4 _vControl)
{
	uint4 iQ1 = asuint(_vQ1);
	uint4 iQ2 = asuint(_vQ2);
	uint4 iControl = asuint(_vControl);

	int4 iReturn =
	{
		(iQ1[0] & ~iControl[0]) | (iQ2[0] & iControl[0]),
		(iQ1[1] & ~iControl[1]) | (iQ2[1] & iControl[1]),
		(iQ1[2] & ~iControl[2]) | (iQ2[2] & iControl[2]),
		(iQ1[3] & ~iControl[3]) | (iQ2[3] & iControl[3]),
	};

	return asfloat(iReturn);
}

float4 VectorXorInt(float4 _V1, float4 _V2)
{
	uint4 iV1 = { asuint(_V1.x), asuint(_V1.y), asuint(_V1.z), asuint(_V1.w) };
	uint4 iV2 = { 2147483648, 0, 0, 0 };

	uint4 Result =
	{
		iV1[0] ^ iV2[0],
		iV1[1] ^ iV2[1],
		iV1[2] ^ iV2[2],
		iV1[3] ^ iV2[3]
	};

	return float4(asfloat(Result.x), asfloat(Result.y), asfloat(Result.z), asfloat(Result.w));
}


void MatrixRotationQuaternion(in float4 Quaternion, out matrix _outMat)
{
	float4 Constant1110 = float4(1.f, 1.f, 1.f, 0.f);

	float4 Q0 = Quaternion + Quaternion;
	float4 Q1 = Quaternion * Q0;

	float4 V0 = (float4) 0.f;
	VectorPermute(1, 0, 0, 7, Q1, Constant1110, V0);

	float4 V1 = (float4) 0.f;
	VectorPermute(2, 2, 1, 7, Q1, Constant1110, V1);

	float4 R0 = Constant1110 - V0;
	R0 = R0 - V1;

	V0 = float4(Quaternion[0], Quaternion[0], Quaternion[1], Quaternion[3]);
	V1 = float4(Q0[2], Q0[1], Q0[2], Q0[3]);
	V0 = V0 * V1;

	V1 = float4(Quaternion.w, Quaternion.w, Quaternion.w, Quaternion.w);
	float4 V2 = float4(Q0[1], Q0[2], Q0[0], Q0[3]);
	V1 = V1 * V2;

	float4 R1 = V0 + V1;
	float4 R2 = V0 - V1;

	VectorPermute(1, 4, 5, 2, R1, R2, V0);
	VectorPermute(0, 6, 0, 6, R1, R2, V1);

	matrix M = (matrix) 0.f;
	VectorPermute(0, 4, 5, 3, R0, V0, M._11_12_13_14);
	VectorPermute(6, 1, 7, 3, R0, V0, M._21_22_23_24);
	VectorPermute(4, 5, 2, 3, R0, V1, M._31_32_33_34);
	M._41_42_43_44 = float4(0.f, 0.f, 0.f, 1.f);
	_outMat = M;
}


void MatrixAffineTransformation(in float4 Scaling
	, in float4 RotationOrigin
	, in float4 RotationQuaternion
	, in float4 Translation
	, out matrix _outMat)
{
	matrix MScaling = (matrix) 0.f;
	MScaling._11_22_33 = Scaling.xyz;
	MScaling._44 = 1.f;

	float4 VRotationOrigin = float4(RotationOrigin.xyz, 0.f);
	float4 VTranslation = float4(Translation.xyz, 0.f);

	matrix MRotation = (matrix) 0.f;
	MatrixRotationQuaternion(RotationQuaternion, MRotation);

	matrix M = MScaling;
	M._41_42_43_44 = M._41_42_43_44 - VRotationOrigin;
	M = mul(M, MRotation);
	M._41_42_43_44 = M._41_42_43_44 + VRotationOrigin;
	M._41_42_43_44 = M._41_42_43_44 + VTranslation;
	_outMat = M;
}


float4 QuternionLerp(in float4 _vQ1, in float4 _vQ2, float _fRatio)
{
	float4 vT = float4(_fRatio, _fRatio, _fRatio, _fRatio);

	// Result = Q1 * sin((1.0 - t) * Omega) / sin(Omega) + Q2 * sin(t * Omega) / sin(Omega)
	const float4 OneMinusEpsilon = { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f };

	float fQDot = dot(_vQ1, _vQ2);
	float4 CosOmega = float4(fQDot, fQDot, fQDot, fQDot);

	const float4 Zero = (float4) 0.f;
	float4 Control = VectorLess(CosOmega, Zero);
	float4 Sign = VectorSelect(float4(1.f, 1.f, 1.f, 1.f), float4(-1.f, -1.f, -1.f, -1.f), Control);

	CosOmega = CosOmega * Sign;
	Control = VectorLess(CosOmega, OneMinusEpsilon);

	float4 SinOmega = float4(1.f, 1.f, 1.f, 1.f) - (CosOmega * CosOmega);
	SinOmega = float4(sqrt(SinOmega.x), sqrt(SinOmega.y), sqrt(SinOmega.z), sqrt(SinOmega.w));

	float4 Omega = float4(atan2(SinOmega.x, CosOmega.x)
		, atan2(SinOmega.y, CosOmega.y)
		, atan2(SinOmega.z, CosOmega.z)
		, atan2(SinOmega.w, CosOmega.w));

	float4 SignMask = float4(asfloat(0x80000000U), asfloat(0x80000000U), asfloat(0x80000000U), asfloat(0x80000000U));
	float4 V01 = VectorShiftLeft(vT, Zero, 2);
	SignMask = VectorShiftLeft(SignMask, Zero, 3);

	V01 = VectorXorInt(V01, SignMask);
	V01 = float4(1.0f, 0.0f, 0.0f, 0.0f) + V01;

	float4 InvSinOmega = float4(1.f, 1.f, 1.f, 1.f) / SinOmega;

	float4 S0 = V01 * Omega;
	S0 = float4(sin(S0.x), sin(S0.y), sin(S0.z), sin(S0.w));
	S0 = S0 * InvSinOmega;
	S0 = VectorSelect(V01, S0, Control);

	float4 S1 = float4(S0.y, S0.y, S0.y, S0.y);
	S0 = float4(S0.x, S0.x, S0.x, S0.x);

	S1 = S1 * Sign;

	float4 Result = _vQ1 * S0;
	Result = (_vQ2 * S1) + Result;

	return Result;
}

struct FrameTrans
{
	float4	Translate;
	float4	Scale;
	float4	Rot;
};

StructuredBuffer<FrameTrans>	g_FrameTransArray	: register(t13);
StructuredBuffer<matrix>		g_OffsetArray		: register(t14);
StructuredBuffer<FrameTrans>	g_ChangeFrameTransArray		: register(t15);
RWStructuredBuffer<matrix>		g_BoneMatrixArray	: register(u0);
RWStructuredBuffer<matrix>		g_BoneSocketMatrixArray	: register(u1);

cbuffer AnimationUpdateCBuffer : register(b11)
{
	int		g_AnimBoneCount;
	int		g_AnimCurrentFrame;
	int		g_AnimNextFrame;
	float	g_AnimRatio;
	int		g_AnimFrameCount;
	int		g_AnimRowIndex;
	int		g_AnimChangeAnimation;
	float	g_AnimChangeRatio;
	int		g_AnimChangeFrameCount;
	float3	g_AnimEmpty;
}

[numthreads(256, 1, 1)]
void AnimationUpdateCS(int3 ThreadID : SV_DispatchThreadID)
{
	if (g_AnimBoneCount <= ThreadID.x)
		return;

	float4	ZeroRot = float4(0.f, 0.f, 0.f, 1.f);
	matrix	matBone = (matrix)0.f;

	uint FrameIndex = ThreadID.x * g_AnimFrameCount + g_AnimCurrentFrame;
	uint FrameNextIndex = ThreadID.x * g_AnimFrameCount + g_AnimNextFrame;

	float4	Scale = lerp(g_FrameTransArray[FrameIndex].Scale, g_FrameTransArray[FrameNextIndex].Scale, g_AnimRatio);
	float4	Pos = lerp(g_FrameTransArray[FrameIndex].Translate, g_FrameTransArray[FrameNextIndex].Translate, g_AnimRatio);
	float4	Rot = QuternionLerp(g_FrameTransArray[FrameIndex].Rot, g_FrameTransArray[FrameNextIndex].Rot, g_AnimRatio);

	if (g_AnimChangeAnimation == 1)
	{
		uint ChangeFrameIndex = ThreadID.x * g_AnimChangeFrameCount;

		Scale = lerp(Scale, g_ChangeFrameTransArray[ChangeFrameIndex].Scale, g_AnimChangeRatio);
		Pos = lerp(Pos, g_ChangeFrameTransArray[ChangeFrameIndex].Translate, g_AnimChangeRatio);
		Rot = QuternionLerp(Rot, g_ChangeFrameTransArray[ChangeFrameIndex].Rot, g_AnimChangeRatio);
	}

	MatrixAffineTransformation(Scale, ZeroRot, Rot, Pos, matBone);

	matrix	matOffset = transpose(g_OffsetArray[ThreadID.x]);

	g_BoneMatrixArray[g_AnimRowIndex * g_AnimBoneCount + ThreadID.x] = mul(matOffset, matBone);
	g_BoneSocketMatrixArray[ThreadID.x] = transpose(matBone);
}
