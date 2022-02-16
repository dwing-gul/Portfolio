#ifndef _COPYBONE
#define _COPYBONE

#include "value.fx"

RWStructuredBuffer<matrix> g_arrDestMatrix : register(u0); // ������� ������ ����
StructuredBuffer<matrix> g_sourceMatrix : register(t14); // ���� ����

// ======================
// BoneMatrix ���� ���̴�
#define BoneCount g_int_0
#define RowIndex g_int_1
// ======================


[numthreads(1024, 1, 1)]
void CS_CopyBoneMatrix(int3 _threadIndex : SV_DispatchThreadID)
{
	if (_threadIndex.x >= g_int_0)
		return;
    
	g_arrDestMatrix[BoneCount * RowIndex + _threadIndex.x] = g_sourceMatrix[_threadIndex.x];
}

#endif