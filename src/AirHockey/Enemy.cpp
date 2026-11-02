#include "pch.h"
#include "Enemy.h"
#include "Pallet.h"
#include "App.h"

Enemy::Enemy()
{
	m_pEntity = nullptr;
	m_pFSM = nullptr; 
	m_pPallet = nullptr;
}

Enemy::~Enemy()
{
}

void Enemy::Create(cpu_mesh* _pMesh, cpu_material* _pMaterial)
{
	m_pEntity = cpuEngine.CreateEntity();
	m_pEntity->pMesh = _pMesh;
	m_pEntity->pMaterial = _pMaterial;
	m_pEntity->transform.Scale(15.0f);
	m_pEntity->transform.sca.z = 7.0f;
	m_pEntity->transform.sca.y = 10.0f;
	m_pEntity->transform.pos.y = 1.0f;
	m_pEntity->transform.pos.z = 25.0f;

	m_pFSM = cpuEngine.CreateFSM(this);
	m_pFSM->Add<StateEnemyIdle>("Idle");
	m_pFSM->Add<StateEnemyDefense>("Defense");
	m_pFSM->Add<StateEnemyOffense>("Offense");
	m_pFSM->Add<StateEnemyPause>("Pause");
	GetFSM()->ToState(CPU_ID(StateEnemyIdle));

	App pApp = App::GetInstance();

	m_pPallet = pApp.GetPallet();
}

void Enemy::Update(float _dt)
{
	SaveLastPositions(_dt);
	StayInBounds();
}

void Enemy::Destroy()
{
	m_pEntity = cpuEngine.Release(m_pEntity);
	m_pFSM = cpuEngine.Release(m_pFSM);
}

void Enemy::StayInBounds()
{
	if (m_pEntity->transform.pos.x > 9.0f)
	{
		m_pEntity->transform.pos.x = 9.0f;
	}
	else if (m_pEntity->transform.pos.x < -9.0f)
	{
		m_pEntity->transform.pos.x = -9.0f;
	}

	if (m_pEntity->transform.pos.z > 40.0f)
	{
		m_pEntity->transform.pos.z = 40.0f;
	}

	else if (m_pEntity->transform.pos.z < 0.0f)
	{
		m_pEntity->transform.pos.z = 0.0f;
	}
}

//Global
void StateEnemyPause::OnEnter(Enemy& _cur, int _from, void* _pParam)
{
}

void StateEnemyPause::OnExecute(Enemy& _cur)
{
}

void StateEnemyPause::OnExit(Enemy& _cur, int _to)
{
}

//Idle
void StateEnemyIdle::OnEnter(Enemy& _cur, int _from, void* _pParam)
{
}

void StateEnemyIdle::OnExecute(Enemy& _cur)
{
	float dt = cpuTime.delta;
	App pApp = App::GetInstance();

	cpu_entity* enemy = _cur.GetEntity();
	cpu_entity* pallet = _cur.GetPallet()->GetEntity();

	float enemyZ = enemy->transform.pos.z;
	float palletZ = pallet->transform.pos.z;

	int randomNumber = pApp.GetRandomNumber(1, 15);
	float acceleration = 3.0f;
		
	if (_cur.GetRight() == true)
	{
		enemy->transform.pos.x += _cur.GetSpeed() * dt * acceleration;

		if (enemy->transform.pos.x >= randomNumber)
		{
			_cur.SetRight(false);
		}
	}
	else
	{
		enemy->transform.pos.x -= _cur.GetSpeed() * dt * acceleration;

		if (enemy->transform.pos.x <= -randomNumber)
		{
			_cur.SetRight(true);
		}
	}

	if (pallet->transform.pos.z >= 0.f)
	{
		if(_cur.GetPallet()->GetDirZ() >= 0.f || _cur.GetPallet()->GetSpeedPallet() <= 3.0f)
			_cur.GetFSM()->ToState(CPU_ID(StateEnemyDefense));
	}


	if (enemy->transform.pos.z >= 25.0f)
	{
		enemy->transform.pos.z -= _cur.GetSpeed() * dt * acceleration;
	}
	else
	{
		enemy->transform.pos.z += _cur.GetSpeed() * dt * acceleration;
	}

	if (enemyZ < palletZ)
	{
		_cur.GetFSM()->ToState(CPU_ID(StateEnemyDefense));

	}
}

void StateEnemyIdle::OnExit(Enemy& _cur, int _to)
{
}

//Defense
void StateEnemyDefense::OnEnter(Enemy& _cur, int _from, void* _pParam)
{
}

void StateEnemyDefense::OnExecute(Enemy& _cur)
{
	float dt = cpuTime.delta;

	cpu_entity* enemy = _cur.GetEntity();
	cpu_entity* pallet = _cur.GetPallet()->GetEntity();

	float enemyX = enemy->transform.pos.x;
	float palletX = pallet->transform.pos.x;

	float enemyZ = enemy->transform.pos.z;
	float palletZ = pallet->transform.pos.z;

	float palletSpeed = _cur.GetPallet()->GetSpeedPallet();
	float dirX = _cur.GetPallet()->GetDirX();
	float dirZ = _cur.GetPallet()->GetDirZ();

	float positionFuturPalet = palletX;

	float distanceZ = abs(enemyZ - palletZ);

	//on verifie si le palet va suffisament vite et qu'il a une direction z
	if (palletSpeed > 0.001f && abs(dirZ) > 0.f)
	{
		//on calcule le temps qu'il faudra pour que le palet arrive à sa position future
		float time = (enemyZ - palletZ) / (palletSpeed * dirZ);

		//si le temps est positif (approche de l'ennemi) on recupere sa position future
		if (time > 0.0f)
		{
			positionFuturPalet = palletX + palletSpeed * dirX * time;
		}
	}

	float differenceX = positionFuturPalet - enemyX;

	//position en x de l'ennemi par rapport au palet
	if (differenceX > 1.0f)
	{
		enemy->transform.pos.x += _cur.GetSpeed() * dt;
	}
	else if (differenceX < -1.0f)
	{
		enemy->transform.pos.x -= _cur.GetSpeed() * dt;
	}

	if (pallet->transform.pos.z < 0.0f)
	{
		_cur.GetFSM()->ToState(CPU_ID(StateEnemyIdle));
	}

	if (enemyZ < palletZ)
	{
		enemy->transform.pos.z += _cur.GetSpeed() * _cur.GetSpeedShoot() * 2 * dt;

	}
	else
	{
		_cur.GetFSM()->ToState(CPU_ID(StateEnemyOffense));
	}
}

void StateEnemyDefense::OnExit(Enemy& _cur, int _to)
{
}

//Offense
void StateEnemyOffense::OnEnter(Enemy& _cur, int _from, void* _pParam)
{
}

void StateEnemyOffense::OnExecute(Enemy& _cur)
{
	float dt = cpuTime.delta;
	App pApp = App::GetInstance();

	cpu_entity* enemy = _cur.GetEntity();
	cpu_entity* pallet = _cur.GetPallet()->GetEntity();

	float enemyZ = enemy->transform.pos.z;
	float palletZ = pallet->transform.pos.z;
	float enemyX = enemy->transform.pos.x;
	float palletX = pallet->transform.pos.x;

	float distanceZ = pApp.GetDistanceZ(enemy, pallet);

	//sens de x
	float differenceX = palletX - enemyX;

	if (differenceX > 1.0f)
	{
		enemy->transform.pos.x += _cur.GetSpeedShoot() * 2 * dt;
	}
	else if (differenceX < -1.0f)
	{
		enemy->transform.pos.x -= _cur.GetSpeedShoot() * 2 * dt;
	}

	if (_cur.GetMoveForward() == true )
	{
		//l'enemi avance sur z (tire)
		enemy->transform.pos.z -= _cur.GetSpeed() * _cur.GetSpeedShoot() * dt;

		if (distanceZ <= 3.0f)
		{
			_cur.SetMoveForward(false);
			_cur.GetFSM()->ToState(CPU_ID(StateEnemyIdle));
		}
	}
	else
	{
		//l'ennemi recul sur z (élan) 
		enemy->transform.pos.z += _cur.GetSpeed() * _cur.GetSpeedShoot() * dt;

		if (distanceZ <= 20.0f || enemy->transform.pos.z >= 40.0f)
		{
			_cur.SetMoveForward(true);
		}
	}

	if (enemyZ < palletZ)
	{
		_cur.GetFSM()->ToState(CPU_ID(StateEnemyDefense));
	}
}

void StateEnemyOffense::OnExit(Enemy& _cur, int _to)
{
}

