#ifndef _RAYCAST
#define _RAYCAST

#include "value.fx"
#include "func.fx"

RWStructuredBuffer<tRaycastOut> OUTPUT : register(u0);
RWStructuredBuffer<int> INDEX : register(u1);

#define HEIGHT_MAP g_texture_0

#define CAM_POS g_vec4_0
#define CAM_DIR g_vec4_1

#define FACE_X g_int_0
#define FACE_Z g_int_1

[numthreads(32, 32, 1)]
void CS_Raycast(int3 _threadID : SV_DispatchThreadID)
{
	int2 id = _threadID.xy;
	
	float3 pos[3] = { (float3) 0.f, (float3) 0.f, (float3) 0.f };

	if (0 == id.x % 2)
	{
		pos[0].x = id.x / 2;
		pos[0].z = id.y;
		
		pos[1].x = pos[0].x + 1;
		pos[1].z = id.y;
		
		pos[2].x = pos[0].x;
		pos[2].z = id.y + 1;
	}
	else
	{
		pos[0].x = (id.x / 2) + 1;
		pos[0].z = id.y + 1;
		
		pos[1].x = pos[0].x - 1;
		pos[1].z = id.y + 1;
		
		pos[2].x = pos[0].x;
		pos[2].z = id.y;
	}
	
	for (int i = 0; i < 3; ++i)
	{
		float2 uv = float2(saturate(pos[i].x / (float) FACE_X), saturate(1.f - pos[i].z / (float) FACE_Z));
		pos[i].y = HEIGHT_MAP.SampleLevel(g_sampler_0, uv, 0).x;
	}
	
	float3 crossPoint = (float3) 0.f;
	float dist = 0.f;
	
	if (IntersectsLay(pos, CAM_POS.xyz, CAM_DIR.xyz, crossPoint, dist))
	{
		int distance = int(10000 * dist);
		int distOut = 0;
		
		InterlockedAdd(INDEX[0], 1);
		InterlockedMin(OUTPUT[INDEX[0] - 1].dist, distance, distOut);
		
		if (distOut < distance)
		{
			return;
		}
		
		OUTPUT[INDEX[0] - 1].UV = float2(saturate(crossPoint.x / (float) FACE_X), saturate((1.f - crossPoint.z / (float) FACE_Z)));
		OUTPUT[INDEX[0] - 1].success = 1;
	}
}

#endif