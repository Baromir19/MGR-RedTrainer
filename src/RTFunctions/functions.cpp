#include "functions.h"
#include "gui/gui.h"
#include "imgui/imgui.h"
#include <cGameUIManager.h>
#include <PlayerManagerImplement.h>
#include <common.h>
#include <injector/injector.hpp>
#include <cItemPossessionBase.h>
#include <cSlowRateManager.h>
#include <cmath>
#include <cCameraGame.h>

int currentMissionCheck = 0;
float posX = 0.0f, posY = 0.0f, posZ = 0.0f;
int arrayEnemyInfo[70] = { 0 };

bool RedTrainer::isFirstFly = true;

///GAME FUNCTIONS

int(*playAnimation)(unsigned int, int, int, int, int); ///playerPtr, animId, animType, aId2, aType2
int(*getItem)(unsigned int);
unsigned int(*setLessSword)(unsigned int, int);
int(*setSound)(int, int);
int(*setBgmFunc)(int);
int(*setMessagePrint)(unsigned int, int, unsigned int, int, int, int, int, int, int);
void(*spawnEnemyFunc)(unsigned int, unsigned int);
unsigned int* (*setCorpsFunc)(unsigned int, int);

///ADDITIONAL FUNCTIONS

void RedTrainer::setText(bool bActive)
{
	ImGui::SameLine();
	if (bActive)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 1.0f, 0.8f, 1.0f));
		ImGui::Text("Enabled");
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.8f, 1.0f));
		ImGui::Text("Disabled");
	}
	ImGui::PopStyleColor();
}

unsigned int RedTrainer::reverseBytes(unsigned int reverseValue) //trash
{
	unsigned int result = 0;

	result |= ((reverseValue & 0x000000FF) << 24);
	result |= ((reverseValue & 0x0000FF00) << 8);
	result |= ((reverseValue & 0x00FF0000) >> 8);
	result |= ((reverseValue & 0xFF000000) >> 24);

	return result;
}

unsigned int RedTrainer::getCurrentAnimation()
{
	auto player = cGameUIManager::Instance.m_pPlayer;
	if (!player)
		return NULL;

	return player->m_pAnimationSlots->m_pStart[0].field_4;
}

unsigned int RedTrainer::getFirstEnemyCurrentAnimation()
{
	if (!cGameUIManager::Instance.m_pPlayer) 
		return NULL;

	Behavior* enemy = (Behavior*)(shared::base + 0x14A5024);
	if (!enemy)
		return NULL;

	return enemy->m_pAnimationSlots->m_pStart[0].field_4;
}

void RedTrainer::setEnemyIdToStruct(unsigned int enemyId, unsigned int enemyType) //caller: 0xA9EEB0, callee: 0xA9EB50
{
	int cObjReadManager = shared::base + 0x177B364;
	((void(__thiscall*)(void*, eObjID, unsigned int))(shared::base + 0x600A60))(&cObjReadManager, (eObjID)enemyId, enemyType);
}

/*void RedTrainer::setEnemyIdToStruct(unsigned int enemyId, int funcNum) //caller: 0xA9EEB0, callee: 0xA9EB50
{
	int setIdFuncAddress = shared::base + funcNum;
	__asm
	{
		push enemyId
		call setIdFuncAddress
		add esp, 4
	}
}*/

///STATS

void RedTrainer::setInvincibility(bool& bInvincibility)
{
	bInvincibility ^= true;
	Core_PlaySound("core_se_sys_decide_s", 0);
	if (bInvincibility)
		injector::WriteMemoryRaw(shared::base + 0x787847, (void*)"\xEB\x56", 2, true);
	else
		injector::WriteMemoryRaw(shared::base + 0x787847, (void*)"\x74\x56", 2, true);
}

void RedTrainer::setInfinityEnergy(bool& bEnergy)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	bEnergy ^= true;
	if (bEnergy)
		injector::MakeNOP(shared::base + 0x7C30BE, 3, true);
	else
		injector::WriteMemoryRaw(shared::base + 0x7C30BE, (void*)"\xD9\x52\x04", 3, true);
}

void RedTrainer::setHealth(int healthValue)
{
	auto player = cGameUIManager::Instance.m_pPlayer;
	if (!player)
	{
		Core_PlaySound("core_se_sys_menu_out", 0);
		return;
	}
	Core_PlaySound("core_se_sys_decide_s", 0);
	player->m_nHealth = healthValue;
}

void RedTrainer::setMoney(int moneyValue)
{
	auto manager = PlayerManagerImplement::Get();
	if (!manager)
	{
		Core_PlaySound("core_se_sys_menu_out", 0);
		return;
	}
	Core_PlaySound("core_se_sys_decide_s", 0);
	manager->m_nBattlePoints = moneyValue;
}

void RedTrainer::setNewItem(char itemId)
{
	auto player = cGameUIManager::Instance.m_pPlayer;

	if (!player)
	{
		Core_PlaySound("core_se_sys_menu_out", 0);
		return;
	}
	Core_PlaySound("core_se_sys_decide_s", 0);
	switch (itemId)
	{
	case 0:
		((void(__thiscall*)(Pl0000*))(shared::base + 0x77CA60))(player);
		break;
	case 1:
		((void(__thiscall*)(Pl0000*))(shared::base + 0x7D9F60))(player);
		break;
	/*case 2: // not even player function xd
		itemGetFunc = shared::base + 0x1E9110; ///SPINE
		break;*/
	default:
		return;
	}
}

///ITEMS

void RedTrainer::setInfinityAddWeapons(bool& bInfAddWeapon)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	bInfAddWeapon ^= true;
	if (bInfAddWeapon)
	{
		injector::MakeNOP(shared::base + 0x54D8D0, 3, true);
		injector::MakeNOP(shared::base + 0x5499F6, 3, true);
	}
	else
	{
		injector::WriteMemoryRaw(shared::base + 0x54D8D0, (void*)"\xFF\x49\x54", 3, true);
		injector::WriteMemoryRaw(shared::base + 0x5499F6, (void*)"\x89\x51\x5C", 3, true);
	}
}

void RedTrainer::setBodyShop(int bodyShopId, short bodyShopType)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	switch (bodyShopId)
	{
	case 1:
		bodyShopId = 2;
		break;
	case 2:
		bodyShopId = 1;
		break;
	default:
		break;
	}

	if (bodyShopType == 4) 
		bodyShopType = 0x103;

	injector::WriteMemory(shared::base + 0x1773860 + bodyShopId * 0x20, bodyShopType); // .data section
}

void RedTrainer::setSwordShop(int swordShopId, short swordShopType)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	switch (swordShopId)
	{
	case 6:
		swordShopId = 7;
		break;
	case 7:
		swordShopId = 6;
		break;
	default:
		break;
	}

	if (swordShopType == 4)
		swordShopType = 0x103;

	injector::WriteMemory(shared::base + 0x1773A00 + swordShopId * 0x20, swordShopType); // .data section, we don't need some virtual protection to change ability to write in protected memory

}

void RedTrainer::setUniqueShop(int uniqueShopId, short uniqueShopType)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	if (uniqueShopType == 4)
		uniqueShopType = 0x103;

	uniqueShopId--;

	injector::WriteMemory(shared::base + 0x1773B20 + uniqueShopId * 0x20, uniqueShopType);
}

void RedTrainer::setWigShop(int wigShopId, short wigShopType)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	if (wigShopType == 4)
		wigShopType = 0x103;

	wigShopId--;

	injector::WriteMemory(shared::base + 0x1773B80 + wigShopId * 0x20, wigShopType);
}

void RedTrainer::setLifeFuelShop(int lfShopId, short lfShopType)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	if (lfShopType == 4)
		lfShopType = 0x103;

	injector::WriteMemory(shared::base + 0x1773BE0 + lfShopId * 0x20, lfShopType);
}

void RedTrainer::setSkillsShop(int skillShopId, short skillShopType)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	if (skillShopType == 4)
		skillShopType = 0x103;

	injector::WriteMemory(shared::base + 0x1773D00 + skillShopId * 0x20, skillShopType);
}

void RedTrainer::getWeaponsCount(char& weaponCount)
{
	//Core_PlaySound("core_se_sys_decide_s", 0);
	weaponCount = (*(lib::StaticArray<cItemPossessionBase*, 64>*)(shared::base + 0x1486EA0)).m_nSize;
}

void RedTrainer::setAddWeapons(char weaponNum, int weaponValue)
{
	lib::StaticArray<cItemPossessionBase*, 64> items = *(lib::StaticArray<cItemPossessionBase*, 64>*)(shared::base + 0x1486EA0);
	if (!&items)
	{
		Core_PlaySound("core_se_sys_decide_s", 0);
		Core_PlaySound("core_se_sys_menu_out", 0);
		return;
	}
	Core_PlaySound("core_se_sys_decide_s", 0);
	items.m_pStart[weaponNum]->set(weaponValue);
}

///CUSTOMIZATION

void RedTrainer::setPlayerType(int playerTypeId)
{
	bool bPlayerType = true;
	Core_PlaySound("core_se_sys_decide_s", 0);

	if (playerTypeId > 2)
		playerTypeId += 5;

	if (playerTypeId == 10)
		bPlayerType = false;

	if (bPlayerType)
	{
		injector::WriteMemoryRaw(shared::base + 0x823EC1, (void*)"\xB8\x00\x00\x00\x00", 5, true);
		injector::WriteMemoryRaw(shared::base + 0x823EC2, (void*)playerTypeId, 4, true);
	}
	else
	{
		injector::WriteMemoryRaw(shared::base + 0x823EC1, (void*)"\xE8\x1A\xFF\xFE\xFF", 5, true);
	}
}

void RedTrainer::setAttackType(int attackType) {

	Core_PlaySound("core_se_sys_decide_s", 0);

	unsigned int playerAttackTypeAddr;

	int playerType = *(int*)(shared::base + 0x17EA030);

	if (playerType == 8) 
		playerAttackTypeAddr = shared::base + 0x129EBB4;

	if (playerType != 8 && playerType != 9) 
		playerAttackTypeAddr = shared::base + 0x129CA1C;

	switch (attackType)
	{
		//RAIDEN
	case 0:
		injector::WriteMemory(playerAttackTypeAddr, shared::base + 0x7E6E90, true);
		break;
		//SAM
	case 1:
		injector::WriteMemory(playerAttackTypeAddr, shared::base + 0x46BC60, true);
		break;
		//BOSS SAM
	case 2:
		injector::WriteMemory(playerAttackTypeAddr, shared::base + 0x1EE70, true);
		break;
		//ARMSTRONG
	case 3:
		injector::WriteMemory(playerAttackTypeAddr, shared::base + 0x1B3060, true);
		break;
	default:
		break;
	}
}

void RedTrainer::setPlayerSword(int playerSwordId)
{
	bool bPlayerSword = true;
	Core_PlaySound("core_se_sys_decide_s", 0);

	if (playerSwordId > 7)
		playerSwordId++;

	if (playerSwordId == 13)
		bPlayerSword = false;

	if (bPlayerSword)
	{
		injector::WriteMemoryRaw(shared::base + 0x823EDE, (void*)"\xBD\x00\x00\x00\x00\x90", 6, true);
		injector::WriteMemory<unsigned char>(shared::base + 0x823EDF, playerSwordId, true);
	}
	else
	{
		injector::WriteMemoryRaw(shared::base + 0x823EDE, (void*)"\x8B\x44\x24\x0C\x8B\xE8", 6, true);
	}
}

void RedTrainer::setPlayerBody(int playerBodyId)
{
	bool bPlayerBody = true;
	Core_PlaySound("core_se_sys_decide_s", 0);

	if (playerBodyId == 16)
		bPlayerBody = false;

	if (bPlayerBody)
	{
		injector::WriteMemoryRaw(shared::base + 0x823FA2, (void*)"\xB8\x00\x00\x00\x00", 5, true);
		injector::WriteMemory<unsigned char>(shared::base + 0x823FA3, playerBodyId, true);
	}
	else
		injector::WriteMemoryRaw(shared::base + 0x823FA2, (void*)"\xE8\x79\x34\xDA\xFF", 5, true);
}

void RedTrainer::setPlayerHair(int playerHairId)
{
	bool bPlayerHair = true;
	Core_PlaySound("core_se_sys_decide_s", 0);

	if (playerHairId == 4)
		bPlayerHair = false;

	if (bPlayerHair)
	{
		injector::WriteMemoryRaw(shared::base + 0x5C7CAC, (void*)"\xB8\x00\x00\x00\x00\xEB\x1C", 7, true);
		injector::WriteMemory<unsigned char>(shared::base + 0x5C7CAD, playerHairId, true);
	}
	else
	{
		injector::WriteMemoryRaw(shared::base + 0x5C7CAC, (void*)"\x75\x1C\xB9\x40\x91\xFE\x01", 7, true);
	}
}

void RedTrainer::setInvisibility(bool& bInvisible)
{
	auto player = cGameUIManager::Instance.m_pPlayer;
	if (!player)
	{
		Core_PlaySound("core_se_sys_menu_out", 0);
		return;
	}
	bInvisible = !bInvisible;
	Core_PlaySound("core_se_sys_decide_s", 0);
	player->m_fLostDistRate = bInvisible ? 0.0f : 30.0f;
}


void RedTrainer::samRipper(bool& samRipperEnabled)
{
	Core_PlaySound("core_se_sys_decide_s", 0);

	samRipperEnabled = !samRipperEnabled;

	if (samRipperEnabled) {

		//Attack effects from Raiden
		injector::WriteMemory<unsigned int>(shared::base + 0x129EBB4, shared::base + 0x7E6E90, true);

		//Red ripper gauge and button (not works in Sam DLC, don't touch)
		injector::WriteMemory<unsigned int>(shared::base + 0x129EDC8, shared::base + 0x7C3370, true);

		//Raiden ripper activation button (Raiden update)
		injector::WriteMemory<unsigned int>(shared::base + 0x129EAE4, shared::base + 0x8104B0, true);

	}
	else {
		//Attack effects
		injector::WriteMemory<unsigned int>(shared::base + 0x129EBB4, shared::base + 0x46BC60, true);

		//Red ripper gauge and button (not works in Sam DLC, don't touch)
		injector::WriteMemory<unsigned int>(shared::base + 0x129EDC8, shared::base + 0x6C3700, true);

		//Raiden ripper activation button (Raiden update)
		injector::WriteMemory<unsigned int>(shared::base + 0x129EAE4, shared::base + 0x69D3D0, true);
	}
}

///MOVEMENT

void RedTrainer::setSpeed(float speedValue)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	cSlowRateManager::Instance.m_SlowRateUnit[0].m_fSlowRate = speedValue;
}

void RedTrainer::setAnimFromAnimMapRaiden(int animID) {
	Core_PlaySound("core_se_sys_decide_s", 0);
	injector::WriteMemory(shared::base + 0x7982F1, animID, true);
}

void RedTrainer::setFly() ///NOT OPTIMIZED!!!
{
	auto player = cGameUIManager::Instance.m_pPlayer;

	if (!player)
	{
		RedTrainer::bFly = false;
		Core_PlaySound("core_se_sys_menu_out", 0);
		return;
	}

	if (isFirstFly) 
	{
		Core_PlaySound("core_se_sys_decide_s", 0);
		setSpeed(0.0f);
		isFirstFly = false;
	}

	auto& pos = player->m_vecTransPos;

	// optimized version
	/*
	float yaw = cCameraGame::Instance.field_360.y;
	float pitch = cCameraGame::Instance.field_360.x;

	if (shared::IsKeyPressed(VK_NUMPAD2))
	{
		pos.x -= sin(yaw) * 0.5f;
		pos.z -= cos(yaw) * 0.5f;
	}
	else if (shared::IsKeyPressed(VK_NUMPAD8))
	{
		pos.x += sin(yaw) * 0.5f;
		pos.z += cos(yaw) * 0.5f;
	}

	if (shared::IsKeyPressed(VK_NUMPAD4))
	{
		pos.x -= sin(yaw - DegreeToRadian(90)) * 0.5f;
		pos.z -= cos(yaw - DegreeToRadian(90)) * 0.5f;
	}
	else if (shared::IsKeyPressed(VK_NUMPAD6))
	{
		pos.x += sin(yaw - DegreeToRadian(90)) * 0.5f;
		pos.z += cos(yaw - DegreeToRadian(90)) * 0.5f;
	}

	if (shared::IsKeyPressed(VK_NUMPAD1))
		pos.y -= 0.5f;
	else if (shared::IsKeyPressed(VK_NUMPAD7))
		pos.y += 0.5f;
 */

	if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
		pos.x -= 0.5f;
	else if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
		pos.y += 0.5f;
	if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
		pos.z -= 0.5f;
	else if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
		pos.z += 0.5f;

	if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
		pos.y -= 0.5f;
	else if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000)
		pos.y += 0.5f;
}

void RedTrainer::setPlayerAnimation(int animId, int animType, int animIdOld, int animTypeOld, bool isSelectable)
{
	auto player = cGameUIManager::Instance.m_pPlayer;

	if (!player)
	{
		Core_PlaySound("core_se_sys_menu_out", 0);
		return;
	}

	Core_PlaySound("core_se_sys_decide_s", 0);

	if (isSelectable) ///Raiden
	{
		switch (animId)
		{
		case 0: ///Still
			animId = 0;
			break;
		case 1:
			animId = 1;
			break;
		case 2:
			animId = 72;
			break;

		case 3: ///Walk
			animId = 2;
			break;
		case 4:
			animId = 3;
			break;

		case 5: ///Landing
			animId = 11;
			break;
		case 6:
			animId = 91;
			break;

		case 7: ///Jump
			animId = 4;
			break;
		case 8:
			animId = 5;
			break;

		case 9: ///Flip
			animId = 10;
			break;
		case 10:
			animId = 15;
			break;
		case 11:
			animId = 16;
			break;
		case 12:
			animId = 17;
			break;
		case 13:
			animId = 18;
			break;
		case 14:
			animId = 19;
			break;
		case 15:
			animId = 20;
			break;
		case 16:
			animId = 62;
			break;
		case 17:
			animId = 204;
			break;
		case 18:
			animId = 205;
			break;
		case 19:
			animId = 206;
			break;
		case 20:
			animId = 210;
			break;
		case 21:
			animId = 213;
			break;
		case 22:
			animId = 240;
			break;
		case 23:
			animId = 241;
			break;
		case 24:
			animId = 247;
			break;
		case 25:
			animId = 272;
			break;
		case 26:
			animId = 273;
			break;
		case 27:
			animId = 274;
			break;
		case 28:
			animId = 292;
			break;

		case 29: ///Sprint
			animId = 56;
			break;
		case 30:
			animId = 59;
			break;
		case 31:
			animId = 122;
			break;
		case 32:
			animId = 233;
			break;
		case 33:
			animId = 249;
			break;

		case 34: ///Death
			animId = 60;
			break;
		case 35:
			animId = 63;
			break;
		case 36:
			animId = 64;
			break;
		case 37:
			animId = 65;
			break;
		case 38:
			animId = 66;
			break;
		case 39:
			animId = 218;
			break;
		case 40:
			animId = 219;
			break;
		case 41:
			animId = 234;
			break;

		case 42: ///Hit
			animId = 77;
			break;
		case 43:
			animId = 154;
			break;
		case 44:
			animId = 197;
			break;
		case 45:
			animId = 198;
			break;
		case 46:
			animId = 199;
			break;
		case 47:
			animId = 200;
			break;
		case 48:
			animId = 201;
			break;
		case 49:
			animId = 202;
			break;
		case 50:
			animId = 203;
			break;
		case 51:
			animId = 207;
			break;
		case 52:
			animId = 208;
			break;
		case 53:
			animId = 209;
			break;
		case 54:
			animId = 211;
			break;
		case 55:
			animId = 212;
			break;
		case 56:
			animId = 214;
			break;
		case 57:
			animId = 215;
			break;
		case 58:
			animId = 216;
			break;
		case 59:
			animId = 242;
			break;

		case 60: ///Hand
			animId = 23;
			break;
		case 61:
			animId = 51;
			break;
		case 62:
			animId = 83;
			break;
		case 63:
			animId = 83;
			animType = 1;
			break;
		case 64:
			animId = 97;
			break;
		case 65:
			animId = 99;
			break;
		case 66:
			animId = 100;
			break;
		case 67:
			animId = 101;
			break;
		case 68:
			animId = 102;
			break;
		case 69:
			animId = 108;
			break;
		case 70:
			animId = 109;
			break;
		case 71:
			animId = 110;
			break;
		case 72:
			animId = 111;
			break;
		case 73:
			animId = 112;
			break;
		case 74:
			animId = 116;
			break;
		case 75:
			animId = 123;
			break;
		case 76:
			animId = 217;
			break;
		case 77:
			animId = 255;
			break;

		case 78: ///Leg
			animId = 81;
			break;
		case 79:
			animId = 82;
			break;
		case 80:
			animId = 93;
			break;
		case 81:
			animId = 94;
			break;
		case 82:
			animId = 104;
			break;
		case 83:
			animId = 105;
			break;
		case 84:
			animId = 107;
			break;
		case 85:
			animId = 113;
			break;
		case 86:
			animId = 115;
			break;
		case 87:
			animId = 117;
			break;
		case 88:
			animId = 118;
			break;
		case 89:
			animId = 119;
			break;
		case 90:
			animId = 121;
			break;

		case 91: ///Additional weapon animation
			animId = 124;
			break;
		case 92:
			animId = 127;
			break;
		case 93:
			animId = 128;
			break;
		case 94:
			animId = 129;
			break;
		case 95:
			animId = 130;
			break;
		case 96:
			animId = 131;
			break;
		case 97:
			animId = 136;
			break;
		case 98:
			animId = 138;
			break;

		case 99: ///Block animation
			animId = 180;
			break;
		case 100:
			animId = 183;
			break;
		case 101:
			animId = 185;
			break;
		case 102:
			animId = 193;
			break;
		case 103:
			animId = 194;
			break;
		case 104:
			animId = 195;
			break;
		case 105:
			animId = 191;
			break;
		case 106:
			animId = 192;
			break;

		case 107: ///Parry
			animId = 184;
			break;
		case 108:
			animId = 190;
			break;

		case 109: ///Victory
			animId = 306;
			break;
		case 110:
			animId = 307;
			break;

		case 111: ///Hacking
			animId = 42;
			break;
		case 112:
			animId = 43;
			break;
		case 113:
			animId = 44;
			break;

		case 114: ///Codec
			animId = 33;
			break;
		case 115:
			animId = 34;
			break;

		case 116: ///Other
			animId = 189;
			break;
		case 117:
			animId = 196;
			break;
		case 118:
			animId = 321;
			break;
		case 119:
			animId = 251;
			break;
		case 120:
			animId = 288;
			break;
		case 121:
			animId = 319;
			break;
		case 122:
			animId = 67;
			break;
		case 123:
			animId = 61;
			break;

		default:
			return;
		}
	}

	player->setState(animId, animType, animIdOld, animTypeOld);
}

void RedTrainer::setWithoutSword(bool& isActive)
{
	auto player = cGameUIManager::Instance.m_pPlayer;

	if (!player)
	{
		Core_PlaySound("core_se_sys_menu_out", 0);
		return;
	}

	isActive = !isActive;

	Core_PlaySound("core_se_sys_decide_s", 0);
	player->SetSwordLost(isActive);
}

///MISSION

void RedTrainer::setMission(short missionId, char missionName[])
{
	if (!strlen(missionName))
		return;

	Core_PlaySound("core_se_sys_decide_s", 0);

	mem::in::read((BYTE*)(shared::base + 0x1764670), (BYTE*)&currentMissionCheck, sizeof(currentMissionCheck));

	if (currentMissionCheck)
	{
		injector::WriteMemoryRaw(shared::base + 0x1764670), (BYTE*)&missionId, sizeof(missionId));
		injector::WriteMemoryRaw(shared::base + 0x1764674), (BYTE*)&missionName[0], 0x20);
	}
	else
	{
		injector::WriteMemoryRaw(shared::base + 0x1766004), (BYTE*)&missionId, sizeof(missionId));
		injector::WriteMemoryRaw(shared::base + 0x1766008), (BYTE*)&missionName[0], 0x20);
	}

	currentMissionCheck = 0;
}

void RedTrainer::setDifficulty(char difficultyValue)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	injector::WriteMemoryRaw(shared::base + 0x1764430), (BYTE*)&difficultyValue, sizeof(difficultyValue));
}

void RedTrainer::setNoDamage(bool& bNoDamage)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	bNoDamage = !bNoDamage;
	if (bNoDamage)
	{
		injector::WriteMemoryRaw(shared::base + 0x81B488), (BYTE*)"\xEB\x05", 2);
		injector::WriteMemoryRaw(shared::base + 0x1776218), (BYTE*)"\x00", 1);
	}
	else
	{
		injector::WriteMemoryRaw(shared::base + 0x81B488), (BYTE*)"\x74\x05", 2);
	}
}

void RedTrainer::setNoKilled(bool& bNoKilled) //doesnt work
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	bNoKilled = !bNoKilled;
	if (bNoKilled)
	{
		injector::WriteMemoryRaw(shared::base + 0x81B494), (BYTE*)"\xEB\x05", 2);
		injector::WriteMemoryRaw(shared::base + 0x177621C), (BYTE*)"\x00\x00\x00\x00", 4);
	}
	else
	{
		injector::WriteMemoryRaw(shared::base + 0x81B494), (BYTE*)"\x74\x05", 2);
	}
}

void RedTrainer::setNoAlert(bool& bNoAlert) //doesnt work
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	bNoAlert = !bNoAlert;
	if (bNoAlert)
	{
		injector::WriteMemoryRaw(shared::base + 0x82C417), (BYTE*)"\x90\x90\x90\x90\x90\x90", 6);
		injector::WriteMemoryRaw(shared::base + 0x1776174), (BYTE*)"\x00\x00\x00\x00", 4);
	}
	else
	{
		injector::WriteMemoryRaw(shared::base + 0x82C417), (BYTE*)"\x01\x05\x74\x61\x30\x02", 6);
	}
}

void RedTrainer::setBattleTimer(float timerValue)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	injector::WriteMemoryRaw(shared::base + 0x1776204), (BYTE*)&timerValue, sizeof(timerValue));
}

void RedTrainer::setBattlePoints(int battlePointsValue)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	injector::WriteMemoryRaw(shared::base + 0x1776208), (BYTE*)&battlePointsValue, sizeof(battlePointsValue));
}

void RedTrainer::setMaxCombo(int maxComboValue)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	injector::WriteMemoryRaw(shared::base + 0x1776210), (BYTE*)&maxComboValue, sizeof(maxComboValue));
}

void RedTrainer::setKills(int killsValue)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	injector::WriteMemoryRaw(shared::base + 0x1776214), (BYTE*)&killsValue, sizeof(killsValue));
}

void RedTrainer::setZandzutsuKills(int zandzutsuKillsValue)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	injector::WriteMemoryRaw(shared::base + 0x177620C), (BYTE*)&zandzutsuKillsValue, sizeof(zandzutsuKillsValue));
}

///RAIDEN FLAGS

void RedTrainer::setFlag(char raidenFlag)
{
	bool isSizeOver = false;
	int setBitsFromFlag = 0;
	int memoryBuffer = 0;
	Core_PlaySound("core_se_sys_decide_s", 0);

	if (raidenFlag > 0x1F)
	{
		isSizeOver = !isSizeOver;

		if (raidenFlag > 0x40)
		{
			return;
		}
	}

	if (raidenFlag > 0x35)
	{
		raidenFlag += 3;
		if (raidenFlag > 0x3A)
		{
			raidenFlag += 2;
		}
	}

	if (!isSizeOver)
	{
		raidenFlag = 0x1F - raidenFlag;
		setBitsFromFlag = 1 << raidenFlag;
		mem::in::read((BYTE*)(shared::base + 0x17EA090), (BYTE*)&memoryBuffer, sizeof(memoryBuffer));
		memoryBuffer = memoryBuffer ^ setBitsFromFlag;
		injector::WriteMemoryRaw(shared::base + 0x17EA090), (BYTE*)&memoryBuffer, sizeof(memoryBuffer));
	}
	else
	{
		raidenFlag -= 0x20;
		raidenFlag = 0x1F - raidenFlag;
		setBitsFromFlag = 1 << raidenFlag;
		mem::in::read((BYTE*)(shared::base + 0x17EA094), (BYTE*)&memoryBuffer, sizeof(memoryBuffer));
		memoryBuffer = memoryBuffer ^ setBitsFromFlag;
		injector::WriteMemoryRaw(shared::base + 0x17EA094), (BYTE*)&memoryBuffer, sizeof(memoryBuffer));
	}
}

void RedTrainer::setRender(unsigned int renderType)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	injector::WriteMemoryRaw(shared::base + 0x17EA09C), (BYTE*)&renderType, sizeof(renderType));
}

///OTHER

void RedTrainer::setFilter(float filterValue, char filterOffset)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	injector::WriteMemoryRaw(shared::base + 0x1ADD604 + filterOffset), (BYTE*)&filterValue, sizeof(filterValue));
}

void RedTrainer::setSize(float sizeValue, char sizeOffset)
{
	uintptr_t sizeAddress;
	memcpy(&sizeAddress, (BYTE*)(shared::base + 0x19C1490), sizeof(sizeAddress));

	if (sizeAddress == NULL)
	{
		Core_PlaySound("core_se_sys_menu_out", 0);
		return;
	}

	Core_PlaySound("core_se_sys_decide_s", 0);
	sizeAddress = mem::in::find_DMA(shared::base + 0x19C1490, { 0x70 + (unsigned int)sizeOffset });
	injector::WriteMemoryRaw(sizeAddress), (BYTE*)&sizeValue, sizeof(sizeValue));
};

void RedTrainer::setMenuType(char menuType)
{
	if (menuType > 2)
	{
		++menuType;
		if (menuType > 4)
		{
			++menuType;
		}
		if (menuType > 0xD)
		{
			++menuType;
		}
	}
	injector::WriteMemoryRaw(shared::base + 0x17E9F9C), (BYTE*)&menuType, sizeof(menuType));
}

void RedTrainer::printMessage(unsigned int messageId, int messageNum, int messagePrint, int messagePosition, char messageChar[])
{
	int setPtrChkMsg = shared::base + 0x19C3D08;
	int setMsgFuncAddr = shared::base + 0x8E2DB0;
	Core_PlaySound("core_se_sys_decide_s", 0);
	//mem::in::read((BYTE*)(shared::base + 0x19C3D08), (BYTE*)&setPtrChkMsg, sizeof(setPtrChkMsg));

	setMessagePrint = (int(*)(unsigned int, int, unsigned int, int, int, int, int, int, int))setMsgFuncAddr;

	__asm {
		push messagePosition
		push 1
		push 0
		push 0
		push messageNum
		push messagePrint
		push messageId
		push 0
		mov ecx, setPtrChkMsg
		call setMessagePrint
	}
}

///ENEMY

void RedTrainer::setAllEnemies(int typeEnemyActive, unsigned int enemyId, unsigned int enemyType, unsigned int enemySetType, unsigned int enemyFlag)
{
	Core_PlaySound("core_se_sys_decide_s", 0);
	if (typeEnemyActive && 0x1) //delete write functions
	{
		//enemyId = reverseBytes(enemyId);
		//enemyType = reverseBytes(enemyType);
		//enemySetType = reverseBytes(enemySetType);
		//enemyFlag = reverseBytes(enemyFlag);

		mem::in::set_nop((BYTE*)(shared::base + 0x8A1960), 0x11); //Id setter to nop
		injector::WriteMemoryRaw(shared::base + 0x8A1960), (BYTE*)"\xC7\x46\x0C", 3);
		injector::WriteMemoryRaw(shared::base + 0x8A1963), (BYTE*)&enemyId, 4);

		mem::in::set_nop((BYTE*)(shared::base + 0x8A1A4D), 0x11); //Type to nop
		injector::WriteMemoryRaw(shared::base + 0x8A1A4D), (BYTE*)"\xC7\x46\x44", 3);
		injector::WriteMemoryRaw(shared::base + 0x8A1A50), (BYTE*)&enemyType, 4);

		mem::in::set_nop((BYTE*)(shared::base + 0x8A1AC5), 0x11); //Flag to nop
		injector::WriteMemoryRaw(shared::base + 0x8A1AC5), (BYTE*)"\xC7\x46\x50", 3);
		injector::WriteMemoryRaw(shared::base + 0x8A1AC8), (BYTE*)&enemyFlag, 4);
	}

	if (typeEnemyActive && 0x2) //SetType to nop
	{
		mem::in::set_nop((BYTE*)(shared::base + 0x8A1A75), 0x11);
		injector::WriteMemoryRaw(shared::base + 0x8A1A75), (BYTE*)"\xC7\x46\x48", 3);
		injector::WriteMemoryRaw(shared::base + 0x8A1A78), (BYTE*)&enemySetType, 4);
	}
	/*
	if (typeEnemyActive && 0x4) //Set enemy to player pos
	{
		memcpy(&posX, (BYTE*)(shared::base + 0x19C1490), sizeof(posX));

		if (posX == NULL)
		{
			Core_PlaySound("core_se_sys_menu_out", 0);
			return;
		}

		uintptr_t posAddress = mem::in::find_DMA(shared::base + 0x19C1490, { 0x50 });
		memcpy(&posX, (BYTE*)posAddress, sizeof(posX));
		memcpy(&posY, (BYTE*)posAddress + 4, sizeof(posY));
		memcpy(&posZ, (BYTE*)posAddress + 8, sizeof(posZ));

		mem::in::set_nop((BYTE*)(shared::base + 0x8A1999), 0x25);
		injector::WriteMemoryRaw(shared::base + 0x8A1999), (BYTE*)"\xC7\x46\x1C", 3);
		injector::WriteMemoryRaw(shared::base + 0x8A199C), (BYTE*)&posX, 4);
		injector::WriteMemoryRaw(shared::base + 0x8A19A0), (BYTE*)"\xC7\x46\x20", 3);
		injector::WriteMemoryRaw(shared::base + 0x8A19A3), (BYTE*)&posY, 4);
		injector::WriteMemoryRaw(shared::base + 0x8A19A7), (BYTE*)"\xC7\x46\x24", 3);
		injector::WriteMemoryRaw(shared::base + 0x8A19AA), (BYTE*)&posZ, 4);
	}*/

	if (!typeEnemyActive) //reset
	{
		injector::WriteMemoryRaw(shared::base + 0x8A1960), (BYTE*)"\x8B\x17\x8D\x4E\x0C\x51\x50\x8B\x82\xE8\x00\x00\x00\x8B\xCF\xFF\xD0", 0x11); //reset id
		injector::WriteMemoryRaw(shared::base + 0x8A1A4D), (BYTE*)"\x8B\x17\x8D\x4E\x44\x51\x50\x8B\x82\xD8\x00\x00\x00\x8B\xCF\xFF\xD0", 0x11); //reset type
		injector::WriteMemoryRaw(shared::base + 0x8A1A75), (BYTE*)"\x8B\x17\x8D\x4E\x48\x51\x50\x8B\x82\xD8\x00\x00\x00\x8B\xCF\xFF\xD0", 0x11); //reset setType
		injector::WriteMemoryRaw(shared::base + 0x8A1AC5), (BYTE*)"\x8B\x17\x8D\x4E\x50\x51\x50\x8B\x82\xE8\x00\x00\x00\x8B\xCF\xFF\xD0", 0x11); //reset flag
		//injector::WriteMemoryRaw(shared::base + 0x8A1999), (BYTE*)"\x8B\x17\x8B\x82\x9C\x00\x00\x00\x68\xD0\xFC\x09\x02\x53\x8B\xCF\xFF\xD0\x83\xF8\xFF\x74\x0E\x8B\x17\x55\x50\x8B\x82\xC4\x00\x00\x00\x8B\xCF\xFF\xD0", 0x25); //reset pos
	}
}

void RedTrainer::enemyNoDamageTo(int damageType) {

	Core_PlaySound("core_se_sys_decide_s", 0);

	//Enemy startup change
	injector::WriteMemoryRaw(shared::base + 0x6C7C80), (BYTE*)&damageType, sizeof(damageType));
}

void RedTrainer::spawnEnemy(unsigned int enemyId, unsigned int enemyType, unsigned int enemyFlag)
{
	int setCorpsFuncAddr = shared::base + 0x8A4360;
	int criticalSectPtr = shared::base + 0x1878CD0; //Get ptr from func
	//int binXmlPtr = shared::base + 0x123E684;
	//int setArrayXmlFunc = shared::base + 0x8A1790;
	//int someFuncAddr = shared::base + 0x681330;
	//int lockFunc = shared::base + 0x67C950;
	//int setFunc = shared::base + 0x67C960;

	///CHECK FOR CREATION

	memcpy(&posX, (BYTE*)(shared::base + 0x19C1490), sizeof(posX));

	if (posX == NULL)
	{
		Core_PlaySound("core_se_sys_menu_out", 0);
		return;
	}

	uintptr_t posAddress = mem::in::find_DMA(shared::base + 0x19C1490, { 0x50 });
	memcpy(&posX, (BYTE*)posAddress, sizeof(posX));
	memcpy(&posY, (BYTE*)posAddress + 4, sizeof(posY));
	memcpy(&posZ, (BYTE*)posAddress + 8, sizeof(posZ));

	///CALL BINXML FUNC
	/*
	RedTrainer::setAllEnemies(0x1, enemyId, enemyType, 0, enemyFlag);

	__asm
	{
		push 0xA1
		lea eax, [binXmlPtr]
		push eax
		lea eax, [arrayEnemyInfo]
		push eax
		push 0xE60000
		push 0
		mov ecx, criticalSectPtr
		call setArrayXmlFunc
	}

	RedTrainer::setAllEnemies(0);
	*/

	///SET ARRAY

	arrayEnemyInfo[0] = 1;
	//arrayEnemyInfo[1] = 0xFFFFFFFF;
	arrayEnemyInfo[3] = enemyId;
	memcpy(&arrayEnemyInfo[7], &posX, sizeof(posX));
	memcpy(&arrayEnemyInfo[8], &posY, sizeof(posY));
	memcpy(&arrayEnemyInfo[9], &posZ, sizeof(posZ));
	//	memcpy(&arrayEnemyInfo[13], &posX, sizeof(posX));
	//	memcpy(&arrayEnemyInfo[14], &posY, sizeof(posY));
	//	memcpy(&arrayEnemyInfo[15], &posZ, sizeof(posZ));
	arrayEnemyInfo[17] = enemyType;
	//arrayEnemyInfo[18] = 0;
	arrayEnemyInfo[20] = enemyFlag;
	arrayEnemyInfo[22] = -1;
	arrayEnemyInfo[23] = -1;
	arrayEnemyInfo[25] = -1;
	arrayEnemyInfo[26] = -1;
	arrayEnemyInfo[29] = -1;
	arrayEnemyInfo[30] = -1;
	arrayEnemyInfo[31] = -1;
	//	memcpy(&arrayEnemyInfo[55], &posX, sizeof(posX));
	//	memcpy(&arrayEnemyInfo[56], &posY, sizeof(posY));
	//	memcpy(&arrayEnemyInfo[57], &posZ, sizeof(posZ));
	arrayEnemyInfo[59] = 1;
	arrayEnemyInfo[60] = -1;
	arrayEnemyInfo[62] = 0xFF * 0x1000000 + 0x00 * 0x10000 + 0x00 * 0x100 + 0xFF;

	setCorpsFunc = (unsigned int* (*)(unsigned int, int))setCorpsFuncAddr;

	///CALL SPAWN FUNC

	__asm
	{
		push eax
		push ebx
		push ecx
		push edx
		push edi
		push esi

		mov edi, criticalSectPtr
		//add edi, 20h
		lea esi, [edi + 50h]

		lea eax, [arrayEnemyInfo]
		push eax
		mov ecx, criticalSectPtr
		call setCorpsFunc//spfunc

		/*add code
		mov ebx, eax
		mov ecx, ebx
		mov eax, [ecx+48h]
		mov edi, eax
		mov ecx, 0
		mov edx, [edi]
		mov edx, [edx+264h]
		lea eax, [arrayEnemyInfo]
		mov [edi+80Ch], ecx
		push eax
		mov ecx, edi
		call edx

		mov ecx, [edi+83Ch]
		mov eax, 1
		shl eax, 6
		mov [eax+esi+24h], ecx
		mov edx, 1
		shl edx, 6
		lea eax, [edx+esi-10h]
		xor ecx, ecx
		mov [eax+64h], ecx
		lea edi, [eax+30h]
		mov eax, 1
		shl eax, 6
		mov [eax+esi+2Ch], ecx
		mov ecx, edi
		call someFuncAddr

		mov  ecx, edi
		call lockFunc
		mov ecx, ebx
		lea eax, [ecx+2Ch]
		push eax
		mov ecx, edi
		call setFunc
		inc dword ptr[esi+4]
		inc dword ptr[esi]
		*/
		pop esi
		pop edi
		pop edx
		pop ecx
		pop ebx
		pop eax
	}
}

///TEST

void(*playerImplementFunc)(int);
void RedTrainer::playerImplement(int plId) //trash
{
	int plImplAddr = shared::base + 0x841040;//0x840850;
	int plImplement = shared::base + 0x12A6BDC;
	playerImplementFunc = (void(*)(int))plImplAddr;

	__asm
	{
		lea ecx, plImplement
		call playerImplementFunc//spfunc
	}
}

//0 - cdecl, 1 - thiscall, 2 - stdcall, 3 - fastcall. Args = 0~6
void RedTrainer::callGameFunction(int funcAddress, char functionType, char numOfArgs, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6)
{
	funcAddress += shared::base;

	///TODO: for() pushing args from stack. Fastcall?

	switch (numOfArgs)
	{
	case 6:
		__asm
		{
			push arg6
		}

	case 5:
		__asm
		{
			push arg5
		}

	case 4:
		__asm
		{
			push arg4
		}

	case 3:
		__asm
		{
			push arg3
		}

	case 2:
		__asm
		{
			push arg2
		}

	case 1:
		if (functionType != 1) //funcType is not thiscall?
		{
			__asm
			{
				push arg1
			}
		}
		else
		{
			__asm
			{
				mov ecx, arg1
			}
		}

	default:
		break;
	}

	__asm
	{
		call funcAddress
	}

	if (!functionType) //is funcType cdecl?
	{
		int addEsp = numOfArgs * 4;
		__asm
		{
			add esp, addEsp
		}
	}
}

//0 = pauseBg
void RedTrainer::setBackground(char bgType, bool enabled)
{
	int resultVar = 0;

	if (bgType == 0 && enabled)
	{
		//unsigned int bgPointerVariable = 0x17EA138 + shared::base;
		unsigned int pauseMenuBgFunc = 0x594370 + shared::base;

		__asm
		{
			call pauseMenuBgFunc
			mov resultVar, eax
		}

		injector::WriteMemoryRaw(0x17EA138 + shared::base), (BYTE*)&resultVar, sizeof(resultVar));

		return;
	}
}

void RedTrainer::setDynamicWeapon(bool isEnabled, unsigned int weaponId)
{
	if (isEnabled) //????????????? ?? ???????????? ?????? + ???????? ?????? ?? ????
	{
		///Changed weapon type
		injector::WriteMemoryRaw(shared::base + 0x7885C0), (BYTE*)"\xBF", 1);
		injector::WriteMemoryRaw(shared::base + 0x7885C1), (BYTE*)&weaponId, sizeof(weaponId));

		///Disabling verification func 
		mem::in::set_nop((BYTE*)(shared::base + 0x592DE9), 2);

		///WeaponMenuType 2 -> 4
		//mem::in::read((BYTE*)(shared::base + 0x8294B5), (BYTE*)&buffer, sizeof(buffer));
		injector::WriteMemoryRaw(shared::base + 0x8294AD), (BYTE*)"\x83\x05", 2);
		injector::WriteMemoryRaw(shared::base + 0x8294B3), (BYTE*)"\x02", 1);
		mem::in::set_nop((BYTE*)(shared::base + 0x8294B4), 5);

		///Change weaponMenuType shared::base + 17EA124 to 2 (write pointer to object in global var)
		injector::WriteMemoryRaw(shared::base + 0x17EA124), (BYTE*)"\x02", 1);

		///Set menu to weapon menu setMenuType(7)
		setMenuType(7);
	}

	if (!isEnabled)
	{
		unsigned int funcAddr = shared::base + 0x19C2D78;
		///Change bytes to bytes before call
		//weapon menu type
		injector::WriteMemoryRaw(shared::base + 0x8294AD), (BYTE*)"\x01\x1D", 2);
		injector::WriteMemoryRaw(shared::base + 0x8294B5), (BYTE*)&funcAddr, sizeof(funcAddr));
		injector::WriteMemoryRaw(shared::base + 0x8294B3), (BYTE*)"\x89\x1D", 2);

		//jne
		injector::WriteMemoryRaw(shared::base + 0x592DE9), (BYTE*)"\x75\x07", 2);

		//weapon id
		injector::WriteMemoryRaw(shared::base + 0x7885C0), (BYTE*)"\x75\x05\x83\xCF\xFF", 5);
	}

	/*
	int resultVar = 0;

	//unsigned int bgPointerVariable = 0x17EA138 + shared::base;
	unsigned int weaponSelectMenuFunc = 0x5926A0 + shared::base;

	__asm
	{
		call weaponSelectMenuFunc
		mov resultVar, eax
	}

	injector::WriteMemoryRaw(0x17EA14C + shared::base), (BYTE*)&resultVar, sizeof(resultVar));
	*/
}

///TRASH

	//shared::base + 0x7885C0 = \x75\x05\x83\xCF\xFF //+5 byte
	//to "\xBF" + var weaponId //\x00\x04\x03\x00"
	//shared::base + 0x592DE9 = \x75\x07
	//to NOP 2 bytes
	//shared::base + 0x8294AD = \x01\x1D //write
	//to \x83\x05
	//shared::base + 0x8294B3 = \x89\x1D + copy next 4 bytes
	//to \x02 + 5 NOP bytes
	//01 1D + 24 A1 07 02 
	//89 1D + 78 2D 25 02

//itemGetFunc = shared::base + 0x54ABB0;
//itemGetFunc = shared::base + 0x1EA950;
//itemGetFunc = shared::base + 0x54C2F0;

/*
METAL GEAR RISING REVENGEANCE.exe+8A703E - 8B 46 08              - mov eax,[esi+08]
METAL GEAR RISING REVENGEANCE.exe+8A7041 - 40                    - inc eax
METAL GEAR RISING REVENGEANCE.exe+8A7042 - 90                    - nop
METAL GEAR RISING REVENGEANCE.exe+8A7043 - 90                    - nop
METAL GEAR RISING REVENGEANCE.exe+8A7044 - 90                    - nop
METAL GEAR RISING REVENGEANCE.exe+8A7045 - 90                    - nop
METAL GEAR RISING REVENGEANCE.exe+8A7046 - 90                    - nop
METAL GEAR RISING REVENGEANCE.exe+8A7047 - 90                    - nop
METAL GEAR RISING REVENGEANCE.exe+8A7048 - 89 44 24 24           - mov [esp+24],eax
METAL GEAR RISING REVENGEANCE.exe+8A704C - 3B 46 08              - cmp eax,[esi+08]


typedef int(__cdecl *pSetCorpsFunc)(unsigned int*, __m128, int);
pSetCorpsFunc setCorpsFunc = (pSetCorpsFunc)(0x8A4360 + RedTrainer::shared::base);

unsigned int corpsAddr = 0x8A4360 + RedTrainer::shared::base;
std::vector<DWORD> arrayEnemyInfo(69, 0);
DWORD* pArrayEnemyInfo = arrayEnemyInfo.data();
__m128 xmmReg = _mm_set_ps(0.0f, 0.0f, 0.0f, 0.0f);
uintptr_t criticalSectPtr = 0;

static void __declspec(naked) emSetCorpsCrEnemyPtr(unsigned int* critSegment, __m128 xmmReg, int arrayPtr)
{
	__asm
	{
		push[esp + 04h] // arrayPtr
		movdqa xmm4, [esp + 08h] //push xmm4 // xmmReg
		push eax // critSegment

		// Call __cdecl
		call corpsAddr

		add esp, 4 // critSegment<eax> is also used for return value
		movdqa[esp], xmm4 //pop xmm4 // xmmReg
		add esp, 4 // arrayPtr
		retn 04h
	}
}*/

/*__declspec(naked) DWORD* __stdcall emSetCorpsCrEnemyPtr(DWORD* criticalSectPtr, __m128 xmmReg, int arrayEnemyInfo)
{
	__asm
	{
		push ebp
		mov ebp, esp
		mov ecx, criticalSectPtr
		movdqa xmm4, xmmReg
		push arrayEnemyInfo
		call [corpsAddr]
		leave
		ret
	}
}*/

/*

void RedTrainer::spawnEnemy(int enemyId, int setType, int type, int flag)
{
	uintptr_t posAddress = mem::in::find_DMA(shared::base + 0x19C1490, { 0x50 });
	memcpy(&posX, (BYTE*)posAddress, sizeof(posX));
	memcpy(&posY, (BYTE*)posAddress + 4, sizeof(posY));
	memcpy(&posZ, (BYTE*)posAddress + 8, sizeof(posZ));

	///TODO: Modify the function to fill the array with your own values


	corpsAddr = 0x8A4360 + shared::base;
	criticalSectPtr = shared::base + 0x1878CD0;

	arrayEnemyInfo[0] = 1;
	arrayEnemyInfo[1] = 0x035A0000;
	arrayEnemyInfo[3] = enemyId;
	arrayEnemyInfo[7] = posX;
	arrayEnemyInfo[8] = posY;
	arrayEnemyInfo[9] = posZ;
	arrayEnemyInfo[13] = posX;
	arrayEnemyInfo[14] = posY;
	arrayEnemyInfo[15] = posZ;
	arrayEnemyInfo[17] = setType;
	arrayEnemyInfo[18] = type;
	arrayEnemyInfo[20] = flag;
	arrayEnemyInfo[22] = -1;
	arrayEnemyInfo[23] = -1;
	arrayEnemyInfo[25] = -1;
	arrayEnemyInfo[26] = -1;
	arrayEnemyInfo[29] = -1;
	arrayEnemyInfo[30] = -1;
	arrayEnemyInfo[31] = -1;
	arrayEnemyInfo[55] = posX;
	arrayEnemyInfo[56] = posY;
	arrayEnemyInfo[57] = posZ;
	arrayEnemyInfo[59] = 1;
	arrayEnemyInfo[62] = 0xFF * 0x1000000 + 0x00 * 0x10000 + 0x00 * 0x100 + 0xFF;

/*
	*(DWORD *)(arrayEnemyInfo) = 1;
	//*(WORD *)(arrayEnemyInfo + 1) = 0;
	*(DWORD *)(arrayEnemyInfo + 1) = 0x035A0000;
	*(DWORD *)(arrayEnemyInfo + 2) = 0;	   //mission Id
	*(DWORD *)(arrayEnemyInfo + 3) = enemyId;// setEnemyId = -1
	*(float *)(arrayEnemyInfo + 4) = 0.0;   // baseRot
	*(float *)(arrayEnemyInfo + 5) = 0.0;
	*(float *)(arrayEnemyInfo + 6) = 0.0;
	*(float *)(arrayEnemyInfo + 7) = posX;   // pos X
	*(float *)(arrayEnemyInfo + 8) = posY;
	*(float *)(arrayEnemyInfo + 9) = posZ;   // Pos Z
	*(float *)(arrayEnemyInfo + 13) = posX;   // pos transL X
	*(float *)(arrayEnemyInfo + 14) = posY;
	*(float *)(arrayEnemyInfo + 15) = posZ;   // pos transL Z
	*(float *)(arrayEnemyInfo + 16) = 0.0;   // rotation?
	*(DWORD *)(arrayEnemyInfo + 17) = setType;     // setType
	*(DWORD *)(arrayEnemyInfo + 18) = type;     // type
	*(DWORD *)(arrayEnemyInfo + 19) = 0;     // set rotation
	*(DWORD *)(arrayEnemyInfo + 20) = flag;     // set flag
	*(DWORD *)(arrayEnemyInfo + 21) = 0;
	*(DWORD *)(arrayEnemyInfo + 22) = -1;     // pathNo
	*(DWORD *)(arrayEnemyInfo + 23) = -1;     // waypointNO
	*(DWORD *)(arrayEnemyInfo + 24) = 0;     // setWait
	*(DWORD *)(arrayEnemyInfo + 25) = -1;    // parentsId
	*(DWORD *)(arrayEnemyInfo + 26) = -1;    // partsNo
	*(DWORD *)(arrayEnemyInfo + 27) = 0;     // hashNo
	*(DWORD *)(arrayEnemyInfo + 28) = 0;     // param
	*(DWORD *)(arrayEnemyInfo + 29) = -1;    // bazierNo
	*(DWORD *)(arrayEnemyInfo + 30) = -1;    // someConst
	*(DWORD *)(arrayEnemyInfo + 31) = -1;    // itemId
	*(DWORD *)(arrayEnemyInfo + 32) = 0;     // groupPos
	*(float *)(arrayEnemyInfo + 55) = posX;     // initialPosX
	*(float *)(arrayEnemyInfo + 56) = posY;     // initialPosY
	*(float *)(arrayEnemyInfo + 57) = posZ;    // initialPosZ
	*(DWORD *)(arrayEnemyInfo + 59) = 1;		//firstArg
	*(DWORD *)(arrayEnemyInfo + 61) = 0;
	*(DWORD *)(arrayEnemyInfo + 62) = 0xFF * 0x1000000 + 0x00 * 0x10000 + 0x00 * 0x100 + 0xFF;		//free0 //DropItemNormal //dropItemStealth //VisceraYableNo
	//*(float *)(arrayEnemyInfo + 63) = 0.0;    // ReflexViewAngY
	//*(float *)(arrayEnemyInfo + 64) = 0.0;   // ReflexViewAngX
	//*(float *)(arrayEnemyInfo + 65) = 0.0;   // ReflexDist
	//*(float *)(arrayEnemyInfo + 66) = 0.0;   // ScoutViewAngY
	//*(float *)(arrayEnemyInfo + 67) = 0.0;   // ScoutViewAngX
	//*(float *)(arrayEnemyInfo + 68) = 0.0;   // ScoutViewDist

	//DWORD workThread = GetThreadByNum(procId, -2);

	//setCorpsFunc = (pSetCorpsFunc)(0x8A4360 + shared::base);
	//setCorpsFunc((unsigned int*)criticalSectPtr, xmmReg, arrayEnemyInfo[0]);

	emSetCorpsCrEnemyPtr((unsigned int*)criticalSectPtr, xmmReg, (int)pArrayEnemyInfo);
}*/
/*
void RedTrainer::spawnEnemyA(unsigned int enemyId, unsigned int enemyType, unsigned int enemyFlag)
{
	setAllEnemies(0x5, enemyId, enemyType, 0, enemyFlag);

	int spawnFuncAddr = shared::base + 0x8A6F70;//0x8A5F90;
	int criticalSectPtr = shared::base + 0x1878CD0;

	//spawnFunc?


	__asm
	{
		push 0
		mov ecx, criticalSectPtr
		call spawnFuncAddr//spfunc
	}

	setAllEnemies(0);
}*/