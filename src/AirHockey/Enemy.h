#pragma once
#include "pch.h"
#include "Bats.h"

class Pallet;

struct StateEnemyIdle
{
	virtual void OnEnter(Enemy& _cur, int _from, void* _pParam);
	virtual void OnExecute(Enemy& _cur);
	virtual void OnExit(Enemy& _cur, int _to);
};

struct StateEnemyDefense
{
	virtual void OnEnter(Enemy& _cur, int _from, void* _pParam);
	virtual void OnExecute(Enemy& _cur);
	virtual void OnExit(Enemy& _cur, int _to);
};

struct StateEnemyOffense
{
	virtual void OnEnter(Enemy& _cur, int _from, void* _pParam);
	virtual void OnExecute(Enemy& _cur);
	virtual void OnExit(Enemy& _cur, int _to);
};

struct StateEnemyPause
{
	virtual void OnEnter(Enemy& _cur, int _from, void* _pParam);
	virtual void OnExecute(Enemy& _cur);
	virtual void OnExit(Enemy& _cur, int _to);
};

class Enemy : public Bats
{
public:

	Enemy();
	~Enemy();

	StateEnemyPause m_stateEnemyPause;
	StateEnemyIdle m_stateEnemyIdle;
	StateEnemyDefense m_stateEnemyDefense;
	StateEnemyOffense m_stateEnemyOffense;

	void Create(cpu_mesh* _pMesh, cpu_material* _pMaterial) override;
	void Update(float _dt) override;
	void Destroy() override;

	cpu_fsm<Enemy>* GetFSM() { return m_pFSM; }
	bool GetRight() { return m_right; }
	bool GetMoveForward() { return m_moveForward; }
	float GetSpeed() { return m_vitesseEnemy; }
	float GetSpeedShoot() { return m_speedShoot; }
	Pallet* GetPallet() { return m_pPallet; }
	void SetRight(bool _new) { m_right = _new; }
	void SetMoveForward(bool _new) { m_moveForward = _new; }

protected:
	bool m_right = true;
	bool m_moveForward = false;
	float m_vitesseEnemy = 5.0f;
	float m_speedShoot = 10.0f;
	Pallet* m_pPallet;
	cpu_fsm<Enemy>* m_pFSM = nullptr;

	void StayInBounds() override;
};
