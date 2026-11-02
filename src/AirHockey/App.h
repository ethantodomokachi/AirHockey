#pragma once

class Player;
class Pallet;
class Ground;
class Enemy;

class App
{
public:
	App();
	~App();
	static App& GetInstance() { return *s_pApp; }

	void OnStart();
	void OnUpdate();
	void OnExit();
	void OnRender(int _pass);

	Ground* GetGround() { return m_pGround; }
	Player* GetPlayer() { return m_pPlayer; }
	Enemy* GetEnemy() { return m_pEnemy; }
	Pallet* GetPallet() { return m_pPallet; }

	float GetDistanceX(cpu_entity* _entity1, cpu_entity* _entity2);
	float GetDistanceZ(cpu_entity* _entity1, cpu_entity* _entity2);	
	int GetRandomNumber(int _min, int _max);
	void Goal(bool _player);

private:
	inline static App* s_pApp = nullptr;

	void IncreaseScore(bool _player);
	void ResetPlayers();
	void Reset();
	
	// resources
	cpu_font m_font;
	cpu_mesh m_meshBats;
	cpu_mesh m_meshPallet;
	cpu_mesh m_meshTerrain;
	cpu_texture m_texturePlayerBat;
	cpu_texture m_textureEnemyBat;
	cpu_texture m_texturePallet;

	// Shader
	cpu_material m_materialPlayerBat;
	cpu_material m_materialEnemyBat;
	cpu_material m_materialPallet;
	cpu_material m_materialTerrain;
  
	//3D
	Pallet* m_pPallet;
	Ground* m_pGround;
	Player* m_pPlayer;
	Enemy* m_pEnemy;

	//score
	int m_scorePlayer;
	int m_scoreEnemy;

	bool m_isGameFinished = false;
	bool m_isGamePaused = false;
};

