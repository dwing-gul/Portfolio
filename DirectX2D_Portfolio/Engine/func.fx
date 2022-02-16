#ifndef _FUNC
#define _FUNC
#include "value.fx"

static float gaussian5x5[25] =
{
    0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
    0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
    0.023792, 0.094907, 0.150342, 0.094907, 0.023792,
    0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
    0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
};

float gaussian(in int2 _uv, in Texture2D _tex)
{
    float4 output = (float4)0.f;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            output += _tex[_uv + int2(i - 2, j - 2)] * gaussian5x5[i * 5 + j];
        }
    }

    return output.x;
}

float Rand(in float _key)
{
    float2 uv = float2(_key + g_accTime, g_accTime);
    uv.y += sin(uv.x * 2.f * 3.141592f);

    if (uv.x > 0)
        uv.x = frac(uv.x);
    else
        uv.x = 1.f - abs(uv.x);

    if (uv.y > 0)
        uv.y = frac(uv.y);
    else
        uv.y = 1.f - abs(uv.y);

    uv *= g_noiseResolution;

    return gaussian(uv, g_noiseTexture);
}
#endif