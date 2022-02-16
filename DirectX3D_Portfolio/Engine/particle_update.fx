#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"
#include "func.fx"

#define particleCount g_int_0
#define g_startSpeed g_float_0
#define g_endSpeed g_float_1
#define g_minLifeTime g_float_2
#define g_maxLifeTime g_float_3

#define g_objPos g_vec4_0
#define g_area g_vec4_1

RWStructuredBuffer<tParticle> g_Particle : register(u0);
RWStructuredBuffer<tSharedData> g_shared : register(u1);

[numthreads(1024, 1, 1)]
void CS_ParticleUpdate(int3 _threadID : SV_DispatchThreadID)
{
	if (particleCount <= _threadID.x)
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
			float key = (float)_threadID.x / (float)particleCount;

			float4 rand = (float4)0.f;
			rand.x = Rand(key);
			rand.y = Rand(rand.x);
			rand.z = Rand(rand.y);
			rand.w = Rand(rand.z);

			rand = rand * 2.f - 1.f;

			g_Particle[_threadID.x].worldPos = g_objPos.xyz + (g_area.xyz * rand.xyz);
			g_Particle[_threadID.x].worldDir = normalize(rand.xyz);
			g_Particle[_threadID.x].curTime = 0.f;
			g_Particle[_threadID.x].maxTime = g_minLifeTime + (g_maxLifeTime - g_minLifeTime) * rand.w;
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
		float curSpeed = (g_endSpeed - g_startSpeed) * lifeRatio + g_startSpeed;

		g_Particle[_threadID.x].worldPos += g_Particle[_threadID.x].worldDir * curSpeed * g_deltaTime;
	}
}

#endif