#include <pch.h>
#include <base.h>
#include <iostream>
#include "trainerFunctions/trainerFunctions.h"
#include "trainerGui/trainerGui.h"
#include "../assets/fonts.h"

bool isKeyPressed = false,
	 isKeyPressed2 = false,
	 isKeyPressed3 = false,
	 isFly = false;

int currentButtonIndex = 0;

int timeToSpawnMil() ///Time to first spawn enemy
{
	if ((RtGui::enemyId & 0xF0000) == 0x20000)
	{
		switch (RtGui::enemyId & 0x00FF0)
		{
		case 0x10:
		case 0x20:
		case 0x40:
		case 0x90:
		case 0xa0:
		case 0x120:
		case 0x140:
		case 0x160:
		case 0x1a0:
		case 0x330:
		case 0x400:
		case 0x410:
			return 200;
		case 0x30:
		case 0x60:
		case 0x70:
		case 0x80:
		case 0x100: 
		case 0x130:
		case 0x150:
		case 0x170:
		case 0x310:
			return 500;
		case 0x190:
			return 1000;
		case 0x600:
			return 1500;
		case 0x180:
		case 0x200:
		case 0x700:
			return 2000;
		default:
			return 2500;
		}
	}

	return 200;
}

HRESULT __stdcall Base::Hooks::EndScene(LPDIRECT3DDEVICE9 pDevice)
{
	Data::pDxDevice9 = pDevice;
	if (!Data::InitImGui)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange | ImGuiConfigFlags_NavEnableKeyboard;
		ImFontConfig fontConfig;
		fontConfig.FontDataOwnedByAtlas = false;
		fontConfig.GlyphExtraSpacing.x = -0.5f;
		io.Fonts->AddFontFromMemoryTTF((void*)mainBoldData, sizeof(mainBoldData), 11.0f, &fontConfig);
		ImGui_ImplWin32_Init(Data::hWindow);
		ImGui_ImplDX9_Init(pDevice);
		Data::InitImGui = true;
	}

	if (!Data::InitImGui) return Data::oEndScene(pDevice);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RtGui::playerAnimationValue = RedTrainer::getCurrentAnimation();

	///TIMER PART
	if (RtGui::toSpawn && 
		((RtGui::previousEnemyId == RtGui::enemyId && RtGui::previousTypeId == RtGui::enemyTypeId) ||
		(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - RtGui::spawnTimer).count() >= timeToSpawnMil()))
	   )
	{
		RtGui::toSpawn = !RtGui::toSpawn;
		RedTrainer::spawnEnemy(RtGui::enemyId, RtGui::enemyTypeId, RtGui::enemyFlagId);
		RtGui::previousEnemyId = RtGui::enemyId;
		RtGui::previousTypeId = RtGui::enemyTypeId;
	}

	if (RtGui::toSetWeaponById && 
		(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - RtGui::customizationTimer).count() >= 100))
	{
		RtGui::toSetWeaponById = !RtGui::toSetWeaponById;
		RedTrainer::setDynamicWeapon(0, 0);
	}

	///VIRTUAL KEY PART
	if ((GetAsyncKeyState(VK_ADD) || GetAsyncKeyState(VK_F1)) && 0x8000)
	{
		isKeyPressed = true;
	}
	else if (!((GetAsyncKeyState(VK_ADD) || GetAsyncKeyState(VK_F1)) && 0x8000) && isKeyPressed)
	{
		//RedTrainer::playSound(0x12A66C8);
		RtGui::hideSecondWindow();
		Data::ShowMenu = !Data::ShowMenu;
		isKeyPressed = false;
	}

	if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000)
	{
		isKeyPressed2 = true;
	}
	else if (!(GetAsyncKeyState(VK_SUBTRACT) & 0x8000) && isKeyPressed2)
	{
		RtGui::hideSecondWindow();
		isKeyPressed2 = false;
	}

	if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
	{
		isKeyPressed3 = true;
	}
	else if (!(GetAsyncKeyState(VK_NUMPAD5) & 0x8000) && isKeyPressed3)
	{
		isKeyPressed3 = false;
		RtGui::bFly = !RtGui::bFly;
		RedTrainer::setSpeed(1.0f);
	}

	if (RtGui::bFly) 
	{
		RedTrainer::setFly();
	}
	else if (!RtGui::bFly)
	{
		RedTrainer::isFirstFly = true;
	}

	///MENU PART
	if (Data::ShowMenu)
	{
		RtGui::mainWindow();
	}

	ImGui::SetNextWindowPos(ImVec2( ImGui::GetIO().DisplaySize.x - 350, 0 ));

	if (Data::ShowMenu2)
	{
		RtGui::statsWindow();
	}

	if (Data::ShowMenu3)
	{
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 490, 0));
		RtGui::itemsWindow();
	}

	if (Data::ShowMenu4)
	{
		RtGui::customizationWindow();
	}

	if (Data::ShowMenu5)
	{
		RtGui::movementWindow();
	}

	if (Data::ShowMenu6)
	{
		RtGui::missionWindow();
	}

	if (Data::ShowMenu7)
	{
		RtGui::testWindow();
	}

	if (Data::ShowMenu8)
	{
		RtGui::otherWindow();
	}

	if (Data::ShowMenu9)
	{
		RtGui::enemyWindow();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	if(Data::ToDetach)
		Base::Detach();
	return Data::oEndScene(pDevice);
}