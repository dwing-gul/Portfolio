#ifndef _ANIMATION
#define _ANIMATION

#include "value.fx"

float4 VectorLess(float4 _Q1, float4 _Q2)
{
	float4 Return =
	{
		(_Q1[0] < _Q2[0]) ? asfloat((uint) 0xFFFFFFFF) : 0.f,
        (_Q1[1] < _Q2[1]) ? asfloat((uint) 0xFFFFFFFF) : 0.f,
        (_Q1[2] < _Q2[2]) ? asfloat((uint) 0xFFFFFFFF) : 0.f,
        (_Q1[3] < _Q2[3]) ? asfloat((uint) 0xFFFFFFFF) : 0.f
	};
    
	return Return;
}

void VectorPermute(uint _PermuteX, uint _PermuteY, uint _PermuteZ, uint _PermuteW, in float4 _V1, in float4 _V2, out float4 _Out)
{
	float4 aPtr[2] = { _V1, _V2 };
	float4 Result = (float4) 0.f;

	const uint i0 = _PermuteX & 3;
	const uint vi0 = _PermuteX >> 2;
	Result[0] = aPtr[vi0][i0];

	const uint i1 = _PermuteY & 3;
	const uint vi1 = _PermuteY >> 2;
	Result[1] = aPtr[vi1][i1];

	const uint i2 = _PermuteZ & 3;
	const uint vi2 = _PermuteZ >> 2;
	Result[2] = aPtr[vi2][i2];

	const uint i3 = _PermuteW & 3;
	const uint vi3 = _PermuteW >> 2;
	Result[3] = aPtr[vi3][i3];

	_Out = Result;
}


float4 VectorShiftLeft(in float4 _V1, in float4 _V2, uint _Elements)
{
	float4 Out = (float4) 0.f;
    
	VectorPermute(_Elements, ((_Elements) + 1), ((_Elements) + 2), ((_Elements) + 3), _V1, _V2, Out);
    
	return Out;
}

float4 VectorSelect(float4 _Q1, float4 _Q2, float4 _Control)
{
	uint4 Q1 = asuint(_Q1);
	uint4 Q2 = asuint(_Q2);
	uint4 Control = asuint(_Control);
        
	int4 Return =
	{
		(Q1[0] & ~Control[0]) | (Q2[0] & Control[0]),
		(Q1[1] & ~Control[1]) | (Q2[1] & Control[1]),
		(Q1[2] & ~Control[2]) | (Q2[2] & Control[2]),
		(Q1[3] & ~Control[3]) | (Q2[3] & Control[3]),
	};
    
	return asfloat(Return);
}

float4 VectorXorInt(float4 _V1, float4 _V2)
{
	uint4 V1 = { asuint(_V1.x), asuint(_V1.y), asuint(_V1.z), asuint(_V1.w) };
	uint4 V2 = { 2147483648, 0, 0, 0 };
    
	uint4 Result =
	{
		V1[0] ^ V2[0],
        V1[1] ^ V2[1],
        V1[2] ^ V2[2],
        V1[3] ^ V2[3]
	};
    
	return float4(asfloat(Result.x), asfloat(Result.y), asfloat(Result.z), asfloat(Result.w));
}


void MatrixRotationQuaternion(in float4 _Quaternion, out matrix _outMatrix)
{
	float4 Constant1110 = float4(1.f, 1.f, 1.f, 0.f);

	float4 Q0 = _Quaternion + _Quaternion;
	float4 Q1 = _Quaternion * Q0;

	float4 V0 = (float4) 0.f;
	VectorPermute(1, 0, 0, 7, Q1, Constant1110, V0);
    
	float4 V1 = (float4) 0.f;
	VectorPermute(2, 2, 1, 7, Q1, Constant1110, V1);
    
	float4 R0 = Constant1110 - V0;
	R0 = R0 - V1;

	V0 = float4(_Quaternion[0], _Quaternion[0], _Quaternion[1], _Quaternion[3]);
	V1 = float4(Q0[2], Q0[1], Q0[2], Q0[3]);
	V0 = V0 * V1;

	V1 = float4(_Quaternion.w, _Quaternion.w, _Quaternion.w, _Quaternion.w);
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
	_outMatrix = M;
}


void MatrixAffineTransformation(in float4 _Scaling, in float4 _RotationOrigin, in float4 _RotationQuaternion, in float4 _Translation, out matrix _outMatrix)
{
	matrix Scaling = (matrix) 0.f;
	Scaling._11_22_33 = _Scaling.xyz;
        
	float4 RotationOrigin = float4(_RotationOrigin.xyz, 0.f);
	float4 Translation = float4(_Translation.xyz, 0.f);
    
	matrix Rotation = (matrix) 0.f;
	MatrixRotationQuaternion(_RotationQuaternion, Rotation);

	matrix M = Scaling;
	M._41_42_43_44 = M._41_42_43_44 - RotationOrigin;
	M = mul(M, Rotation);
	M._41_42_43_44 = M._41_42_43_44 + RotationOrigin;
	M._41_42_43_44 = M._41_42_43_44 + Translation;
	_outMatrix = M;
}


float4 QuternionLerp(in float4 _Q1, in float4 _Q2, float _Ratio)
{
	float4 T = float4(_Ratio, _Ratio, _Ratio, _Ratio);
    
    // Result = Q1 * sin((1.0 - t) * Omega) / sin(Omega) + Q2 * sin(t * Omega) / sin(Omega)
	const float4 OneMinusEpsilon = { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f };

	float QDot = dot(_Q1, _Q2);
	float4 CosOmega = float4(QDot, QDot, QDot, QDot);

	const float4 Zero = (float4) 0.f;
	float4 Control = VectorLess(CosOmega, Zero);
	float4 Sign = VectorSelect(float4(1.f, 1.f, 1.f, 1.f), float4(-1.f, -1.f, -1.f, -1.f), Control);
    
	CosOmega = CosOmega * Sign;
	Control = VectorLess(CosOmega, OneMinusEpsilon);

	float4 SinOmega = float4(1.f, 1.f, 1.f, 1.f) - (CosOmega * CosOmega);
	SinOmega = float4(sqrt(SinOmega.x), sqrt(SinOmega.y), sqrt(SinOmega.z), sqrt(SinOmega.w));

	float4 Omega = float4(atan2(SinOmega.x, CosOmega.x), atan2(SinOmega.y, CosOmega.y), atan2(SinOmega.z, CosOmega.z), atan2(SinOmega.w, CosOmega.w));
    
	float4 SignMask = float4(asfloat(0x80000000U), asfloat(0x80000000U), asfloat(0x80000000U), asfloat(0x80000000U));
	float4 V01 = VectorShiftLeft(T, Zero, 2);
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
    
	float4 Result = _Q1 * S0;
	Result = (_Q2 * S1) + Result;
    
	return Result;
}

struct tFrameTransform
{
	float4 translate;
	float4 scale;
	float4 rot;
};

StructuredBuffer<tFrameTransform> g_arrFrameTransform : register(t13);
StructuredBuffer<matrix> g_arrOffset : register(t14);
RWStructuredBuffer<matrix> g_arrFinalMatrix : register(u0);

RWStructuredBuffer<matrix> g_handBoneMatrix : register(u1);

// 오른쪽 뼈 위치를 받아와야함
// 그리고 해당 오브젝트마다 구조화 버퍼를 만들어 줘야함
// ===========================
// Animation3D Compute Shader
#define BoneCount		g_int_0
#define CurFrame		g_int_1
#define NextFrame		g_int_2
#define Ratio			g_float_0

#define HandBonePos		g_int_3
#define AddAnimFrame	g_vec2_0
#define AddRatio		g_float_1
// ===========================

[numthreads(256, 1, 1)]
void CS_Animation3D(int3 _threadIndex : SV_DispatchThreadID)
{
	if (BoneCount <= _threadIndex.x)
		return;
    
    // 오프셋 행렬을 곱하여 최종 본행렬을 만들어낸다.		
	float4 QZero = float4(0.f, 0.f, 0.f, 1.f);
	matrix boneMatrix = (matrix) 0.f;
      
    // Frame Data Index == Bone Count * Frame Count + _iThreadIdx.x
	uint FrameDataIndex = BoneCount * CurFrame + _threadIndex.x;
    uint NextFrameDataIndex = BoneCount * NextFrame + _threadIndex.x;
      
	float4 scale = lerp(g_arrFrameTransform[FrameDataIndex].scale, g_arrFrameTransform[NextFrameDataIndex].scale, Ratio);
	float4 translate = lerp(g_arrFrameTransform[FrameDataIndex].translate, g_arrFrameTransform[NextFrameDataIndex].translate, Ratio);
	float4 rot = QuternionLerp(g_arrFrameTransform[FrameDataIndex].rot, g_arrFrameTransform[NextFrameDataIndex].rot, Ratio);
       
    // 최종 본행렬 연산
	MatrixAffineTransformation(scale, QZero, rot, translate, boneMatrix);
     
	matrix offsetMatrix = transpose(g_arrOffset[_threadIndex.x]);
    
    // 구조화버퍼에 결과값 저장
	g_arrFinalMatrix[_threadIndex.x] = mul(offsetMatrix, boneMatrix);
	
    if (HandBonePos == _threadIndex.x)
    {
        g_handBoneMatrix[0] = boneMatrix;

    }
}


#endif
