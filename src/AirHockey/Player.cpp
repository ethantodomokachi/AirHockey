#include "pch.h"
#include "Player.h"
#include "App.h"

Player::Player()
{
	
	m_pEntity = nullptr;
	m_cursorPos = { cpuDevice.GetWidth() * 0.5f , cpuDevice.GetHeight() * 0.5f };
}

Player::~Player()
{
}

void Player::Create(cpu_mesh* _pMesh, cpu_material* _pMaterial)
{
	m_pEntity = cpuEngine.CreateEntity();
	m_pEntity->transform.Scale(15.0f);
	m_pEntity->transform.sca.z = 7.0f;
	m_pEntity->transform.sca.y = 10.0f;
	m_pEntity->transform.pos.y = 1.0f;
	m_pEntity->transform.pos.z = -15.0f;
	m_pEntity->pMesh = _pMesh;
	m_pEntity->pMaterial = _pMaterial;
	m_saveTimer = m_saveDelay;
}

void Player::Destroy()
{
	m_pEntity = cpuEngine.Release(m_pEntity);
}

void Player::Update(float _dt)
{
	MovePlayer();
	SaveLastPositions(_dt);
	StayInBounds();

}

void Player::MovePlayer()
{
	XMFLOAT2 lastCursorPos = m_cursorPos;
	cpuEngine.GetCursor(m_cursorPos);

	m_pEntity->transform.pos.x += (m_cursorPos.x - lastCursorPos.x) * 0.03f;
	m_pEntity->transform.pos.z -= (m_cursorPos.y - lastCursorPos.y) * 0.05f;
}

void Player::StayInBounds()
{
	if (m_pEntity->transform.pos.x > 9.0f)
	{
		m_pEntity->transform.pos.x = 9.0f;
	}
	else if (m_pEntity->transform.pos.x < -9.0f)
	{
		m_pEntity->transform.pos.x = -9.0f;
	}

	if (m_pEntity->transform.pos.z < -25.0f)
	{
		m_pEntity->transform.pos.z = -25.0f;
	}

	else if (m_pEntity->transform.pos.z > 0.0f)
	{
		m_pEntity->transform.pos.z = 0.0f;
	}
}
