#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "ParticleUpdateShader.h"

class StructuredBuffer;

class Particle :
	public Component
{
private :
	Ptr<Mesh>				  m_mesh;
	Ptr<Material>			  m_material;
	Ptr<ParticleUpdateShader> m_updateShader;
	vector<Ptr<Texture>>	  m_vectorParticleTexture;
	UINT					  m_textureIndex;
	StructuredBuffer*		  m_particleBuffer;

	Vector4					  m_startColor;
	Vector4					  m_endColor;
	Vector3					  m_startScale;
	Vector3					  m_endScale;
	float					  m_startSpeed;
	float					  m_endSpeed;
	float					  m_minLifeTime;
	float					  m_maxLifeTime;
	UINT					  m_spawnRate;
	UINT					  m_maxParticleCount;
	Vector3					  m_area;
	float					  m_accTime;

public :
	virtual void start();
	virtual void update();
	virtual void lateUpdate();
	void render();

private :
	UINT CalSpawnCount();
	virtual void SaveToScene(FILE* _file);
	virtual void LoadFromScene(FILE* _file);

public :
	void SetTextureIndex(UINT _index) { m_textureIndex = _index; }
	void SetStartColor(Vector4 _color) { m_startColor = _color; }
	void SetEndColor(Vector4 _color) { m_endColor = _color; }
	void SetStartScale(Vector3 _scale) { m_startScale = _scale; }
	void SetEndScale(Vector3 _scale) { m_endScale = _scale; }
	void SetStartSpeed(float _speed) { m_startSpeed = _speed; }
	void SetEndSpeed(float _speed) { m_endSpeed = _speed; }
	void SetMinLifeTime(float _life) { m_minLifeTime = _life; }
	void SetMaxLifeTime(float _life) { m_maxLifeTime = _life; }
	void SetSpawnRate(UINT _rate) { m_spawnRate = _rate; }
	void SetMaxParticleCount(UINT _count) { m_maxParticleCount = _count; }
	void SetArea(Vector3 _area) { m_area = _area; }

	UINT GetTextureIndex() { return m_textureIndex; }
	Vector4 GetStartColor() { return m_startColor; }
	Vector4 GetEndColor() { return m_endColor; }
	Vector3 GetStartScale() { return m_startScale; }
	Vector3 GetEndScale() { return m_endScale; }
	float GetStartSpeed() { return m_startSpeed; }
	float GetEndSpeed() { return m_endSpeed; }
	float GetMinLifeTime() { return m_minLifeTime; }
	float GetMaxLifeTime() { return m_maxLifeTime; }
	UINT GetSpawnRate() { return m_spawnRate; }
	UINT GetMaxParticleCount() { return m_maxParticleCount; }
	Vector3 GetArea() { return m_area; }
	vector<Ptr<Texture>>& GetVectorTexture() { return m_vectorParticleTexture; }

	CLONE(Particle)

public :
	Particle();
	Particle(const Particle& _origin);
	~Particle();
};

