#include "pch.h"
#include "App.h"
#include "Pallet.h"
#include "Ground.h"
#include "Enemy.h"
#include "Player.h"

App::App()
{
	s_pApp = this;
	CPU_CALLBACK_START(OnStart);
	CPU_CALLBACK_UPDATE(OnUpdate);
	CPU_CALLBACK_EXIT(OnExit);
	CPU_CALLBACK_RENDER(OnRender);
}

App::~App()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void App::OnStart()
{
	// YOUR CODE HERE

	// Resources
	m_font.Create(cpuDevice.GetHeight() <= 512 ? 14 : 28);
	
	m_meshBats.CreateCube(3.0f);
	m_meshPallet.CreateCylinder(0.3f, 1.0f, 12);
	m_meshTerrain.CreateCube(2.0f);
	m_meshBats.CreateCube(0.1f);

	// Shader
	m_materialPlayerBat.color = cpu::ToColor(179, 235, 242);
	m_materialEnemyBat.color = cpu::ToColor(222, 165, 164);
	m_materialPallet.color = cpu::ToColor(128, 128, 128);
	m_materialTerrain.color = cpu::ToColor(244, 237, 222);

	//3D
	m_pPallet = new Pallet;
	m_pPallet->Create(&m_meshPallet, &m_materialPallet);

	m_pPlayer = new Player;
	m_pPlayer->Create(&m_meshBats, &m_materialPlayerBat);

	m_pGround = new Ground;
	m_pGround->Create(&m_meshTerrain, &m_materialTerrain);

	m_pEnemy = new Enemy;
	m_pEnemy->Create(&m_meshBats, &m_materialEnemyBat);

	m_pGround->PlaceWall();

	cpuEngine.GetCamera()->transform.pos.y = 15;
	cpuEngine.GetCamera()->transform.pos.x = m_pPallet->GetEntity()->transform.pos.x;
	cpuEngine.GetCamera()->transform.pos.z = -41;
	cpuEngine.GetCamera()->transform.LookAt(m_pPallet->GetEntity()->transform.pos.x, m_pPallet->GetEntity()->transform.pos.y, m_pPallet->GetEntity()->transform.pos.z);

	XMFLOAT2 cursorpos = { cpuDevice.GetWidth() * 0.5f , cpuDevice.GetHeight() * 0.5f };
	cpuEngine.SetCursor(cursorpos);
	ShowCursor(false);

	m_scorePlayer = 0;
	m_scoreEnemy = 0;
}

void App::OnUpdate()
{
	if (m_isGameFinished == false && m_isGamePaused == false) {
		float dt = cpuTime.delta;
		m_pPlayer->Update(dt);
		m_pEnemy->Update(dt);
		m_pPallet->Update(dt);

		if (m_scorePlayer >= 5 || m_scoreEnemy >= 5) {
			m_isGameFinished = true;
			cpuEngine.GetCamera()->transform.pos.y = -40;
		}

		// pause
		if (cpuInput.IsBackPressed())
		{
			m_isGamePaused = true;
			cpuEngine.GetCamera()->transform.pos.y = -40;
			m_pEnemy->GetFSM()->ToState(CPU_ID(StateEnemyPause));
		}
			

	}
	else if(m_isGameFinished == true) {

		if (cpuInput.IsUpPressed()) {
			m_isGameFinished = false;
			
			Reset();
		}
	}
	else if (m_isGamePaused == true)
	{
		if (cpuInput.IsBackPressed())
		{
			m_isGamePaused = false;
			cpuEngine.GetCamera()->transform.pos.y = 15;
			m_pEnemy->GetFSM()->ToState(CPU_ID(StateEnemyIdle));
			
		}
		if (cpuInput.IsUpPressed())
			cpuEngine.Quit();
	}

	
}

void App::OnExit()
{
	// YOUR CODE HERE
	m_pEnemy->Destroy();
	CPU_DELPTR(m_pEnemy);
	m_pPlayer->Destroy();
	CPU_DELPTR(m_pPlayer);
	m_pPallet->Destroy();
	CPU_DELPTR(m_pPallet);
	m_pGround->Destroy();
	CPU_DELPTR(m_pGround);
}

void App::OnRender(int _pass)
{
	// YOUR CODE HERE
	switch (_pass)
	{
	case CPU_PASS_UI_END:
		
		//Debug
		//if (m_pEnemy->GetFSM()->GetName() == "Idle")
		//{
		//	std::string info = "Idle ";
		//	cpuDevice.DrawText(&m_font, info.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), 10, CPU_TEXT_CENTER);

		//}
		//if (m_pEnemy->GetFSM()->GetName() == "Defense")
		//{
		//	std::string info = "Defense ";
		//	cpuDevice.DrawText(&m_font, info.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), 10, CPU_TEXT_CENTER);

		//}
		//if (m_pEnemy->GetFSM()->GetName() == "Offense")
		//{
		//	std::string info = "Offense ";
		//	cpuDevice.DrawText(&m_font, info.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), 10, CPU_TEXT_CENTER);

		//}

		//std::string info = "PosZE ";
		//info += CPU_STR(m_pEnemy->GetEntity()->transform.pos.z);
		//cpuDevice.DrawText(&m_font, info.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), 30, CPU_TEXT_CENTER);

		//std::string info1 = "PosZP ";
		//info1 += CPU_STR(m_pPallet->GetEntity()->transform.pos.z);
		//cpuDevice.DrawText(&m_font, info1.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), 50, CPU_TEXT_CENTER);

		if (m_isGameFinished == false && m_isGamePaused == false) {
			std::string score = "Joueur : " + std::to_string(m_scorePlayer);
			cpuDevice.DrawText(&m_font, score.c_str(), 10, 20);
			score = "Robot : " + std::to_string(m_scoreEnemy);
			cpuDevice.DrawText(&m_font, score.c_str(), cpuDevice.GetWidth() - 160, 20);
			break;
		}
		else if (m_isGameFinished == true){
			std::string end;
			std::string quit = "Appuyez sur Flèche du haut pour rejouer ou Echap pour quitter le jeu.";
			if (m_scoreEnemy > m_scorePlayer) {
				end = "Dommage, vous avez perdu " + std::to_string(m_scoreEnemy) + " à " + std::to_string(m_scorePlayer);
			}
			else {
				end = "Bravo, vous avez gagné " + std::to_string(m_scorePlayer) + " à " + std::to_string(m_scoreEnemy);
			}

			cpuDevice.DrawText(&m_font, end.c_str(), cpuDevice.GetWidth() * 0.5f, cpuDevice.GetHeight() * 0.5f, CPU_TEXT_CENTER);
			cpuDevice.DrawText(&m_font, quit.c_str(), cpuDevice.GetWidth() * 0.5f, cpuDevice.GetHeight() * 0.5f + 160, CPU_TEXT_CENTER);

		}
		else if (m_isGamePaused == true)
		{
			std::string info1= "Pause";
			cpuDevice.DrawText(&m_font, info1.c_str(), cpuDevice.GetWidth() * 0.5f, cpuDevice.GetHeight() * 0.5f, CPU_TEXT_CENTER);
			std::string info2 = "Appuyé sur Flèche du haut pour quitter la partie ou Echap pour revenir au jeu.";
			cpuDevice.DrawText(&m_font, info2.c_str(), cpuDevice.GetWidth() * 0.5f, cpuDevice.GetHeight() * 0.5f + 160, CPU_TEXT_CENTER);
		}
		
	}
}

float App::GetDistanceX(cpu_entity* _entity1, cpu_entity* _entity2)
{
	float distance1 = _entity1->transform.pos.x;
	float distance2 = _entity2->transform.pos.x;
	float distance = abs(distance1 - distance2);

	return distance;
}

float App::GetDistanceZ(cpu_entity* _entity1, cpu_entity* _entity2)
{
	float distance1 = _entity1->transform.pos.z;
	float distance2 = _entity2->transform.pos.z;
	float distance = abs(distance1 - distance2);

	return distance;
}

int App::GetRandomNumber(int _min, int _max)
{
	int range = _max - _min + 1;
	return (rand() % range) + _min;
}

void App::Goal(bool _player)
{
	IncreaseScore(_player);

	m_pEnemy->GetFSM()->ToState(CPU_ID(StateEnemyIdle));
	m_pPallet->ResetPalletStats();
	ResetPlayers();

	XMFLOAT2 cursorpos = { cpuDevice.GetWidth() * 0.5f , cpuDevice.GetHeight() * 0.5f };
	m_pPlayer->SetCursorPos(cursorpos);
	cpuEngine.SetCursor(cursorpos);

	if (_player) {
		m_pPallet->GetEntity()->transform.pos.x = 0;
		m_pPallet->GetEntity()->transform.pos.z = -10;
	}
	else {
		m_pPallet->GetEntity()->transform.pos.x = 0;
		m_pPallet->GetEntity()->transform.pos.z = 10;
	}
}

void App::IncreaseScore(bool _player)
{
	if (_player) {
		m_scoreEnemy++;
	}
	else {
		m_scorePlayer++;
	}
}

void App::ResetPlayers()
{
	m_pEnemy->GetEntity()->transform.pos.x = 0;
	m_pEnemy->GetEntity()->transform.pos.z = 20;

	m_pPlayer->GetEntity()->transform.pos.x = 0;
	m_pPlayer->GetEntity()->transform.pos.z = -20;

}

void App::Reset()
{
	m_pEnemy->GetFSM()->ToState(CPU_ID(StateEnemyIdle));
	m_pPallet->ResetPalletStats();

	ResetPlayers();

	XMFLOAT2 cursorpos = { cpuDevice.GetWidth() * 0.5f , cpuDevice.GetHeight() * 0.5f };
	m_pPlayer->SetCursorPos(cursorpos);
	cpuEngine.SetCursor(cursorpos);

	m_pPallet->GetEntity()->transform.pos.x = 0;
	m_pPallet->GetEntity()->transform.pos.z = -10;

	m_scoreEnemy = 0;
	m_scorePlayer = 0;

	cpuEngine.GetCamera()->transform.pos.y = 15;
}
