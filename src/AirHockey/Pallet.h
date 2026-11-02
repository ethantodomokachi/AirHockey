#pragma once
#include "pch.h"

class Bats;

class Pallet
{
public :
	Pallet();
	~Pallet();

	void Create(cpu_mesh* _pMesh, cpu_material* _pMaterial);
	void Update(float _dt);
	void Destroy();

	cpu_entity* GetEntity() { return m_pEntity; }
	float GetSpeedPallet() { return m_palletSpeed; }
	float GetDirZ() const{ return m_dir.z;}
	float GetDirX() const { return m_dir.x; }
	void ResetPalletStats();

protected:
	float m_palletSpeed;
	cpu_entity* m_pEntity;
	XMFLOAT3 m_dir;
	
	void Deceleration(float _dt);
	void Bounce(float _dt);
	void OnBatHit(Bats* _bat, bool _isPlayerBat);

	bool IsCollindingX(cpu_entity* _entity);

	bool IsCollindingZ(cpu_entity* _entity);

	bool IsCollidingWithBat(cpu_entity* _entity);
	
};
