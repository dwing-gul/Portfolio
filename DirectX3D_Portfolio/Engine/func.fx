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

float gaussian(in int2 _pixelPos, in Texture2D _tex)
{
    float4 output = (float4)0.f;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
			output += _tex[_pixelPos + int2(i - 2, j - 2)] * gaussian5x5[i * 5 + j];
		}
    }

    return output.x;
}

float gaussian_Height(in int2 _pixelPos, in RWTexture2D<float> _tex)
{
	float4 output = (float4) 0.f;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			output += _tex[_pixelPos + int2(i - 2, j - 2)] * gaussian5x5[i * 5 + j];
		}
	}

	return output.y;
}

float4 Sample_CS(in int2 _centerPixelPos, in Texture2D _tex, int2 _resolution)
{
	float4 output = (float4) 0.f;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			int2 pixelPos = _centerPixelPos + int2(i - 2, j - 2);
			
			if (pixelPos.x < 0 || _resolution.x <= pixelPos.x || pixelPos.y < 0 || _resolution.y <= pixelPos.y)
			{
				continue;
			}

			output += _tex[pixelPos] * gaussian5x5[i * 5 + j];
		}
	}

	return output;
}

float Sample_Flatten(in int2 _centerPos, in RWTexture2D<float> _height, int2 _scale)
{
	float output = (float) 0.f;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			int2 pixelPos = _centerPos + int2(i - 2, j - 2);
			
			if (pixelPos.x < _centerPos.x - (_scale.x / 2) || _centerPos.x + (_scale.x / 2) < pixelPos.x ||
				pixelPos.y < _centerPos.y - (_scale.y / 2) || _centerPos.y + (_scale.y / 2) < pixelPos.y)
			{
				continue;
			}

			output += gaussian_Height(pixelPos, _height);
		}
	}
	
	output /= 25.f;

	return output;
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

    return gaussian(uv, g_noise);
}

void CalculateLighting(float3 _viewNormal, float3 _viewPos, inout tLightColor _lightColor, int _lightIndex)
{
	if (0 == g_Light3DBuffer[_lightIndex].lightType)
	{
		float3 viewLightDir = normalize(mul(float4(g_Light3DBuffer[_lightIndex].lightDir.xyz, 0.f), g_viewMatrix).xyz);
	
		_lightColor.diffuse += g_Light3DBuffer[_lightIndex].color.diffuse * saturate(dot(-viewLightDir, _viewNormal));
	
		float3 viewReflect = viewLightDir + 2.f * dot(-viewLightDir, _viewNormal) * _viewNormal;
	
		float3 eye = normalize(_viewPos);
	
		float reflectPower = saturate(dot(-eye, viewReflect));
		reflectPower = pow(reflectPower, 10.f);
        
		_lightColor.specular += g_Light3DBuffer[_lightIndex].color.specular * reflectPower;
		_lightColor.ambient += g_Light3DBuffer[_lightIndex].color.ambient;
	}
	else
	{
		float3 viewLightPos = mul(float4(g_Light3DBuffer[_lightIndex].lightPos.xyz, 1.f), g_viewMatrix).xyz;
		float3 viewLightDir = normalize(_viewPos - viewLightPos);
        
		float _distance = abs(distance(viewLightPos, _viewPos));
		float rangePower = 0.f;
        
		if (1 == g_Light3DBuffer[_lightIndex].lightType)
		{
			rangePower = saturate(1.f - (_distance / g_Light3DBuffer[_lightIndex].range));

			_lightColor.diffuse += g_Light3DBuffer[_lightIndex].color.diffuse * saturate(dot(-viewLightDir, _viewNormal)) * rangePower;
	
			float3 viewReflect = viewLightDir + 2.f * dot(-viewLightDir, _viewNormal) * _viewNormal;
	
			float3 eye = normalize(_viewPos);
	
			float reflectPower = saturate(dot(-eye, viewReflect));
			reflectPower = pow(reflectPower, 20.f);
			
			_lightColor.specular += g_Light3DBuffer[_lightIndex].color.specular * reflectPower;
		}
		else
		{
			float3 viewSpotLightDir = normalize(mul(float4(g_Light3DBuffer[_lightIndex].lightDir.xyz, 0.f), g_viewMatrix).xyz);
			float lightAngle = abs(acos(dot(viewLightDir, viewSpotLightDir)) * (180.f / 3.141592f));
			float areaAngle = g_Light3DBuffer[_lightIndex].angle * (180.f / 3.141592f);
			float anglePower = 1.f - saturate(lightAngle / areaAngle);
			rangePower = saturate(1.f - (_distance / g_Light3DBuffer[_lightIndex].range));
			
			_lightColor.diffuse += g_Light3DBuffer[_lightIndex].color.diffuse * saturate(dot(-viewLightDir, _viewNormal)) * rangePower * anglePower;
	
			float3 viewReflect = viewLightDir + 2.f * dot(-viewLightDir, _viewNormal) * _viewNormal;
	
			float3 eye = normalize(_viewPos);
	
			float reflectPower = saturate(dot(-eye, viewReflect));
			reflectPower = pow(reflectPower, 20.f);
			
		    _lightColor.specular += g_Light3DBuffer[_lightIndex].color.specular * reflectPower;
		}
	}
}

float CalTessLevel(in float3 _worldCamPos, float3 _edgePos, float _min, float _max, float _maxLevel)
{
	float len = length(_edgePos - _worldCamPos);
	
	float level = floor((_maxLevel - 2) * ((1.f - saturate((len - _min) / (_max - _min)))) + 1.f);
	
	if (len > _max)
	{
		level = 0.f;
	}

	level = pow(2.f, level);
	
	return level;

}

int IntersectsLay(float3 _vertices[3], float3 _start, float3 _dir, out float3 _crossPoint, out float _result)
{
	float3 edge[2] = { (float3) 0.f, (float3) 0.f };

	edge[0] = _vertices[1] - _vertices[0];
	edge[1] = _vertices[2] - _vertices[0];
	
	float3 normal = normalize(cross(edge[0], edge[1]));
	float b = dot(normal, _dir);
	
	float3 w0 = _start - _vertices[0];
	float a = -dot(normal, w0);
	float distRatio = a / b;
	
	_result = distRatio;
	
	float3 ray = _start + distRatio * _dir;
	
	_crossPoint = ray;
	
	float uu, uv, vv, wu, wv, inverseD;
	uu = dot(edge[0], edge[0]);
	uv = dot(edge[0], edge[1]);
	vv = dot(edge[1], edge[1]);
	
	float3 w = ray - _vertices[0];
	wu = dot(w, edge[0]);
	wv = dot(w, edge[1]);
	inverseD = uv * uv - uu * vv;
	inverseD = 1.f / inverseD;
	
	float u = (uv * wv - vv * wu) * inverseD;
	if (u < 0.f || 1.f < u)
	{
		_result = 0.f;
		return 0;
	}
	
	float v = (uv * wu - uu * wv) * inverseD;
	if (v < 0.f || 1.f < (u + v))
	{
		_result = 0.f;
		return 0;
	}
	
	return 1;
}

matrix GetBoneMatrix(int _boneIndex, int _rowIndex)
{
	return g_arrBoneMatrix[(g_boneCount * _rowIndex) + _boneIndex];
}

void Skinning(inout float3 _pos, inout float3 _tangent, inout float3 _binormal, inout float3 _normal, inout float4 _weight, inout float4 _indices, int _rowIndex)
{
	tSkinningInfo info = (tSkinningInfo) 0.f;
    
	if (_rowIndex == -1)
		return;

	for (int i = 0; i < 4; ++i)
	{
		if (0.f == _weight[i])
			continue;

		matrix matBone = GetBoneMatrix((int) _indices[i], _rowIndex);

		info.pos += (mul(float4(_pos, 1.f), matBone) * _weight[i]).xyz;
		info.tangent += (mul(float4(_tangent, 0.f), matBone) * _weight[i]).xyz;
		info.binormal += (mul(float4(_binormal, 0.f), matBone) * _weight[i]).xyz;
		info.normal += (mul(float4(_normal, 0.f), matBone) * _weight[i]).xyz;
	}
    
	_pos = info.pos;
	_tangent = normalize(info.tangent);
	_binormal = normalize(info.binormal);
	_normal = normalize(info.normal);
}


void Skinning(inout float3 _pos, inout float4 _weight, inout float4 _indices, int _rowIndex)
{
	tSkinningInfo info = (tSkinningInfo) 0.f;
    
	for (int i = 0; i < 4; ++i)
	{
		if (0.f == _weight[i])
			continue;

		matrix matBone = GetBoneMatrix((int) _indices[i], _rowIndex);

		info.pos += (mul(float4(_pos, 1.f), matBone) * _weight[i]).xyz;
	}
    
	_pos = info.pos;
}

float encode(float4 _value)
{
	uint rgba = (uint(_value.x * 255.f) << 24) + (uint(_value.y * 255.f) << 16) + (uint(_value.z * 255.f) << 8) + uint(_value.w * 255.f);
	return asfloat(rgba);
}

float4 decode(float _value)
{
	uint rgba = asint(_value);
    
	float r = (float) (rgba >> 24) / 255.f;
	float g = (float) ((rgba & 0x00ff0000) >> 16) / 255.f;
	float b = (float) ((rgba & 0x0000ff00) >> 8) / 255.f;
	float a = (float) (rgba & 0x000000ff) / 255.f;
    
	return float4(r, g, b, a);
}
#endif