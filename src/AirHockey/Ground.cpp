#include "pch.h"
#include "Ground.h"

Ground::Ground()
{
	s_pGround = this;
}

Ground::~Ground()
{

}

void Ground::Create(cpu_mesh* _pMesh, cpu_material* _pMaterial)
{
	for (int i = 0; i < 2; i++)
	{
		m_wallX.push_back(cpuEngine.CreateEntity());
		m_wallX[i]->pMesh = _pMesh;
		m_wallX[i]->pMaterial = _pMaterial;
	}

	for (int i = 0; i < 2; i++)
	{
		m_wallZ.push_back(cpuEngine.CreateEntity());
		m_wallZ[i]->pMesh = _pMesh;
		m_wallZ[i]->pMaterial = _pMaterial;
	}

	m_wallY.push_back(cpuEngine.CreateEntity());
	m_wallY[0]->pMesh = _pMesh;
	m_wallY[0]->pMaterial = _pMaterial;
	
}

void Ground::Destroy()
{
	for (int i = (int)m_wallX.size() - 1; i >= 0; i--)
	{
		cpuEngine.Release(m_wallX[i]);
		m_wallX.erase(m_wallX.begin() + i);
	}
	for (int i = (int)m_wallZ.size() - 1; i >= 0; i--)
	{
		cpuEngine.Release(m_wallZ[i]);
		m_wallZ.erase(m_wallZ.begin() + i);
	}
	for (int i = (int)m_wallY.size() - 1; i >= 0; i--)
	{
		cpuEngine.Release(m_wallY[i]);
		m_wallY.erase(m_wallY.begin() + i);
	}

}

void Ground::Update()
{
}

void Ground::PlaceWall()
{
	//left wall
	m_wallX[0]->transform.pos.x = -13.0f;
	m_wallX[0]->transform.pos.y = 0.0f;
	m_wallX[0]->transform.pos.z = 0.0f;
	m_wallX[0]->transform.sca.z = 23.0f;

	//right wall
	m_wallX[1]->transform.pos.x = 13.0f;
	m_wallX[1]->transform.pos.y = 0.0f;
	m_wallX[1]->transform.pos.z = 0.0f;
	m_wallX[1]->transform.sca.z = 23.0f;

	//top wall
	m_wallZ[0]->transform.pos.x = 0.0f;
	m_wallZ[0]->transform.pos.y = 0.0f;
	m_wallZ[0]->transform.pos.z = 45.0f;
	m_wallZ[0]->transform.sca.x = 7.0f;

	//bottom wall
	m_wallZ[1]->transform.pos.x = 0.0f;
	m_wallZ[1]->transform.pos.y = 0.0f;
	m_wallZ[1]->transform.pos.z = -28.0f;
	m_wallZ[1]->transform.sca.x = 7.0f;

	//ground wall
	m_wallY[0]->transform.pos.x = 0.0f;
	m_wallY[0]->transform.pos.y = -2.0f;
	m_wallY[0]->transform.pos.z = 0.0f;
	m_wallY[0]->transform.sca.z = 20.0f;
	m_wallY[0]->transform.sca.x = 8.0f;
}
