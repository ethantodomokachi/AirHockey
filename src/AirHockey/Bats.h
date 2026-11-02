#pragma once

#include "pch.h"

class Bats
{
public :
	Bats();
	~Bats();

	virtual void Create(cpu_mesh* _pMesh, cpu_material* _pMaterial) = 0;
	virtual void Destroy() = 0;
	virtual void Update(float _dt) = 0;

	void GetNormalizedDirVectorAndSpeed(XMFLOAT3* _dir, float* _speed);

	cpu_entity* GetEntity() { return m_pEntity; }

protected :
	cpu_entity* m_pEntity;

	std::vector<XMFLOAT3> m_previousPos;
	float m_saveTimer = 0.f;
	const float m_saveDelay = 0.1f;

	void SaveLastPositions(float _dt);
	virtual void StayInBounds() = 0;
};

