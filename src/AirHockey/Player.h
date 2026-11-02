#pragma once
#include "Bats.h"

class Player : public Bats
{
public:
	Player();
	~Player();

	void Create(cpu_mesh* _pMesh, cpu_material* _pMaterial) override;
	void Destroy() override;
	void Update(float _dt) override;
	void SetCursorPos(XMFLOAT2 _new) { m_cursorPos = _new; }
protected:
	
	XMFLOAT2 m_cursorPos;
	
	void MovePlayer();
	void StayInBounds() override;
};
