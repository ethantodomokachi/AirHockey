#include "pch.h"
#include "Bats.h"

Bats::Bats()
{
}

Bats::~Bats()
{
}

void Bats::GetNormalizedDirVectorAndSpeed(XMFLOAT3* _dir, float* _speed)
{
	if (m_previousPos.size() == 0){
		_dir->x = 0;
		_dir->y = 0;
		_dir->z = 0;

		*_speed = 0;
		return;
	}

	XMFLOAT3 A = m_previousPos[0];
	XMFLOAT3 B = { m_pEntity->transform.pos.x, m_pEntity->transform.pos.y, m_pEntity->transform.pos.z };

	_dir->x = B.x - A.x;
	_dir->y = B.y - A.y;
	_dir->z = B.z - A.z;

	float norm = sqrtf((_dir->x * _dir->x) + (_dir->y * _dir->y) + (_dir->z * _dir->z));

	*_speed = norm / 0.5f;

	if (norm == 0) {
		_dir->x = 0;
		_dir->y = 0;
		_dir->z = 0;
	}
	else {
		_dir->x /= norm;
		_dir->y /= norm;
		_dir->z /= norm;
	}
}

void Bats::SaveLastPositions(float _dt)
{
	if (m_previousPos.size() == 5) {
		m_previousPos.erase(m_previousPos.begin());
	}

	m_saveTimer -= _dt;
	if (m_saveTimer <= 0) {
		m_previousPos.push_back({ m_pEntity->transform.pos.x, m_pEntity->transform.pos.y, m_pEntity->transform.pos.z });
	}
}
