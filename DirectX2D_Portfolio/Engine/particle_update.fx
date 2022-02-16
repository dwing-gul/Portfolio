#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE
#include "func.fx"

#define PARTICLE_COUNT	g_int_0
#define START_SPEED		g_float_0
#define END_SPEED		g_float_1
#define MIN_LIFE		g_float_2
#define MAX_LIFE		g_float_3
#define OBJ_POS			g_vector4_0
#define AREA			g_vector4_1

RWStructuredBuffer<tParticle> g_Particle : register(u0);
RWStructuredBuffer<tSharedData> g_shared : register(u1);

[numthreads(1024, 1, 1)]
void CS_ParticleUpdate(int3 _threadID :SV_DispatchThreadID)
{
	if (PARTICLE_COUNT <= _threadID.x)
		return;

	if (!g_Particle[_threadID.x].alive)
	{
		int originValue = g_shared[0].spawnCount;
		int exchange = 0;

		while (0 < originValue)
		{
			int inputValue = g_shared[0].spawnCount - 1;
			InterlockedCompareExchange(g_shared[0].spawnCount, originValue, inputValue, exchange);

			if (exchange == originValue)
			{
				g_Particle[_threadID.x].alive = 1;
				break;
			}

			originValue = inputValue;
		}

		if (g_Particle[_threadID.x].alive)
		{
			float key = (float)_threadID.x / (float)PARTICLE_COUNT;

			float4 rand = (float4)0.f;
			rand.x = Rand(key);
			rand.y = Rand(rand.x);
			rand.z = Rand(rand.y);
			rand.w = Rand(rand.z);

			rand = rand * 2.f - 1.f;

			g_Particle[_threadID.x].worldPos = OBJ_POS.xyz + (AREA.xyz * rand.xyz);
			g_Particle[_threadID.x].worldDir = normalize(rand.xyz);
			g_Particle[_threadID.x].curTime = 0.f;
			g_Particle[_threadID.x].maxTime = MIN_LIFE + (MAX_LIFE - MIN_LIFE) * rand.w;
		}
	}
	else
	{
		g_Particle[_threadID.x].curTime += g_deltaTime;

		if (g_Particle[_threadID.x].maxTime < g_Particle[_threadID.x].curTime)
		{
			g_Particle[_threadID.x].alive = 0;
			return;
		}

		float lifeRatio = g_Particle[_threadID.x].curTime / g_Particle[_threadID.x].maxTime;
		float curSpeed = (END_SPEED - START_SPEED) * lifeRatio + START_SPEED;

		g_Particle[_threadID.x].worldPos += g_Particle[_threadID.x].worldDir * curSpeed * g_deltaTime;
	}
}

#endif