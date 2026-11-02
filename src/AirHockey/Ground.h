#pragma once
#include "pch.h"

class Ground
{
public:

	Ground();
	~Ground();

	void Create(cpu_mesh* _pMesh, cpu_material* _pMaterial);
	void Destroy();
	void Update();
	void PlaceWall();

	std::vector<cpu_entity*> m_wallX;
	std::vector<cpu_entity*> m_wallZ;
	std::vector<cpu_entity*> m_wallY;

private:

	inline static Ground* s_pGround = nullptr;
	
};
 