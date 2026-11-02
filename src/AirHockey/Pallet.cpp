#include "pch.h"
#include "Pallet.h"
#include "App.h"
#include "Ground.h"
#include "Player.h"
#include "Enemy.h"

Pallet::Pallet()
{
	m_palletSpeed = 0;
	m_pEntity = nullptr;
	m_dir.x = 1;
	m_dir.y = 0;
	m_dir.z = 1;
}

Pallet::~Pallet()
{
	
}

void Pallet::Create(cpu_mesh* _pMesh, cpu_material* _pMaterial)
{
	m_pEntity = cpuEngine.CreateEntity();
	m_pEntity->pMesh = _pMesh;
	m_pEntity->pMaterial = _pMaterial;

	m_pEntity->transform.pos.z = -10.0f;
	m_pEntity->transform.pos.y = 0.0f;
}

void Pallet::Update(float _dt)
{
	// base moves
	m_pEntity->transform.pos.x += m_palletSpeed * _dt * m_dir.x;
	m_pEntity->transform.pos.y += m_palletSpeed * _dt * m_dir.y;
	m_pEntity->transform.pos.z += m_palletSpeed * _dt * m_dir.z;

	Deceleration(_dt);
	Bounce(_dt);
}

void Pallet::Destroy()
{
	m_pEntity = cpuEngine.Release(m_pEntity);
}

void Pallet::ResetPalletStats()
{
	m_palletSpeed = 0;
	m_dir.x = 0;
	m_dir.y = 0;
	m_dir.z = 0;
}

void Pallet::Deceleration(float _dt)
{
	if (m_palletSpeed > 0.1f) {
		m_palletSpeed -= (m_palletSpeed * _dt);
	}
	else {
		m_palletSpeed = 0;
	}
}

void Pallet::Bounce(float _dt)
{
	App& pApp = App::GetInstance();
	Ground* pGround = pApp.GetGround();
	
	for (int i = 0; i < pGround->m_wallX.size(); i++)
	{
		if (IsCollindingX(pGround->m_wallX[i]))
		{
			if (pGround->m_wallX[i]->transform.pos.x < 0) {
				m_dir.x = 1;
			}
			else {
				m_dir.x = -1;
			}
		}
	}
	

	for (int i = 0; i < pGround->m_wallZ.size(); i++)
	{
		if (IsCollindingZ(pGround->m_wallZ[i])) 
		{
			bool hasEnemyMarked = true;
			if (m_pEntity->transform.pos.z > 0) {
				hasEnemyMarked = false;
			}
			pApp.Goal(hasEnemyMarked);
		}
	}

	Player* pPlayer = pApp.GetPlayer();
	if (IsCollidingWithBat(pPlayer->GetEntity())) {
		OnBatHit(pPlayer, true);
	}

	Enemy* pEnemy = pApp.GetEnemy();
	if (IsCollidingWithBat(pEnemy->GetEntity())) {
		OnBatHit(pEnemy, false);
	}	
}

void Pallet::OnBatHit(Bats* _bat, bool _isPlayerBat)
{
	float speedToAdd = 0.f;
	XMFLOAT3 newDir = {};

	_bat->GetNormalizedDirVectorAndSpeed(&newDir, &speedToAdd);

	if ((newDir.z < 0 && m_dir.z < 0 && _isPlayerBat == true) || (newDir.z > 0 && m_dir.z > 0  && _isPlayerBat == false)) {
		m_dir.z *= -1;
	} else {
		m_dir = newDir;
		m_palletSpeed += speedToAdd * 2;
	}


}

bool Pallet::IsCollindingX(cpu_entity* _entity)
{
	App& pApp = App::GetInstance();

	return pApp.GetDistanceX(GetEntity(), _entity) <= 3.f;
}

bool Pallet::IsCollindingZ(cpu_entity* _entity)
{
	App& pApp = App::GetInstance();

	return pApp.GetDistanceZ(GetEntity(), _entity) <= 3.f;
}

bool Pallet::IsCollidingWithBat(cpu_entity* _entity)
{
	App& pApp = App::GetInstance();

	bool collideZ = pApp.GetDistanceZ(GetEntity(), _entity) <= 2.5f;
	bool collideX = pApp.GetDistanceX(GetEntity(), _entity) <= 3.f;

	return (collideX && collideZ); 
}

