#include "pch.h"
#include "base.h"
#include <emmintrin.h>
#include "trainerFunctions.h"

int currentMissionCheck = 0;
float posX = 0.0f, posY = 0.0f, posZ = 0.0f;

bool RedTrainer::isFirstFly = true;
uintptr_t RedTrainer::moduleBase = 0;

void RedTrainer::setText (bool bActive) 
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

unsigned int  RedTrainer::reverseBytes(unsigned int reverseValue) {
	unsigned int result = 0;

	result |= ((reverseValue & 0x000000FF) << 24);
	result |= ((reverseValue & 0x0000FF00) << 8);
	result |= ((reverseValue & 0x00FF0000) >> 8);
	result |= ((reverseValue & 0xFF000000) >> 24);

	return result;
}

///STATS

void RedTrainer::setInvincibility(bool &bInvincibility)
{
	bInvincibility = !bInvincibility;
	if (bInvincibility)
	{
		mem::in::write((BYTE*)(moduleBase + 0x787847), (BYTE*)"\xEB\x56", 2);
	}
	else
	{
		mem::in::write((BYTE*)(moduleBase + 0x787847), (BYTE*)"\x74\x56", 2);
	}
}

void RedTrainer::setInfinityEnergy(bool &bEnergy)
{
	bEnergy = !bEnergy;
	if (bEnergy)
	{
		mem::in::write((BYTE*)(moduleBase + 0x7C30BE), (BYTE*)"\x90\x90\x90", 3);
	}
	else
	{
		mem::in::write((BYTE*)(moduleBase + 0x7C30BE), (BYTE*)"\xD9\x52\x04", 3);
	}
}

void RedTrainer::setHealth(int healthValue)
{
	uintptr_t healthAddress;
	memcpy(&healthAddress, (BYTE*)(moduleBase + 0x19C1490), sizeof(healthAddress));
	if (healthAddress == NULL)
	{
		return;
	}
	healthAddress = mem::in::find_DMA(moduleBase + 0x19C1490, { 0x870 });
	mem::in::write((BYTE*)healthAddress, (BYTE*)&healthValue, sizeof(healthValue));
}

void RedTrainer::setMoney(int moneyValue)
{
	uintptr_t moneyAddress;
	memcpy(&moneyAddress, (BYTE*)(moduleBase + 0x19C1490), sizeof(moneyAddress));
	if (moneyAddress == NULL)
	{
		return;
	}
	moneyAddress = mem::in::find_DMA(moduleBase + 0x17EA100, { 0xDC });
	mem::in::write((BYTE*)moneyAddress, (BYTE*)&moneyValue, sizeof(moneyValue));
}

///ITEMS

void RedTrainer::setInfinityAddWeapons(bool &bInfAddWeapon)
{
	bInfAddWeapon = !bInfAddWeapon;
	if (bInfAddWeapon)
	{
		mem::in::write((BYTE*)(moduleBase + 0x54D8D0), (BYTE*)"\x90\x90\x90", 3);
		mem::in::write((BYTE*)(moduleBase + 0x5499F6), (BYTE*)"\x90\x90\x90", 3);
	}
	else
	{
		mem::in::write((BYTE*)(moduleBase + 0x54D8D0), (BYTE*)"\xFF\x49\x54", 3);
		mem::in::write((BYTE*)(moduleBase + 0x5499F6), (BYTE*)"\x89\x51\x5C", 3);
	}
}

void RedTrainer::setBodyShop(int bodyShopId, short bodyShopType)
{
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

	if (bodyShopType == 4) {
		bodyShopType = 0x103;
	}

	mem::in::write((BYTE*)(moduleBase + 0x1773860 + bodyShopId * 0x20), (BYTE*)&bodyShopType, sizeof(bodyShopType));
}

void RedTrainer::setSwordShop(int swordShopId, short swordShopType)
{
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

	if (swordShopType == 4) {
		swordShopType = 0x103;
	}

	mem::in::write((BYTE*)(moduleBase + 0x1773A00 + swordShopId * 0x20), (BYTE*)&swordShopType, sizeof(swordShopType));

}

void RedTrainer::setUniqueShop(int uniqueShopId, short uniqueShopType)
{
	if (uniqueShopType == 4) {
		uniqueShopType = 0x103;
	}

	uniqueShopId--;

	mem::in::write((BYTE*)(moduleBase + 0x1773B20 + uniqueShopId * 0x20), (BYTE*)&uniqueShopType, sizeof(uniqueShopType));
}

void RedTrainer::setWigShop(int wigShopId, short wigShopType)
{
	if (wigShopType == 4) {
		wigShopType = 0x103;
	}

	wigShopId--;

	mem::in::write((BYTE*)(moduleBase + 0x1773B80 + wigShopId * 0x20), (BYTE*)&wigShopType, sizeof(wigShopType));
}

void RedTrainer::setLifeFuelShop(int lfShopId, short lfShopType)
{
	if (lfShopType == 4) {
		lfShopType = 0x103;
	}

	mem::in::write((BYTE*)(moduleBase + 0x1773BE0 + lfShopId * 0x20), (BYTE*)&lfShopType, sizeof(lfShopType));
}

void RedTrainer::setSkillsShop(int skillShopId, short skillShopType)
{
	if (skillShopType == 4) {
		skillShopType = 0x103;
	}

	mem::in::write((BYTE*)(moduleBase + 0x1773D00 + skillShopId * 0x20), (BYTE*)&skillShopType, sizeof(skillShopType));
}

void RedTrainer::getWeaponsCount(char &weaponCount)
{
	mem::in::read((BYTE*)(moduleBase + 0x1486EA8), (BYTE*)&weaponCount, sizeof(weaponCount));
}

void RedTrainer::setAddWeapons(char weaponNum, int weaponValue, char weaponCount)
{
	uintptr_t weaponAddress;
	memcpy(&weaponAddress, (BYTE*)(moduleBase + 0x1486EB0 + weaponNum * 4), sizeof(weaponAddress));
	if (weaponAddress == NULL)
	{
		return;
	}
	if ((weaponCount == 9 || weaponCount == 11) && (weaponNum == 0 || weaponNum == 1))
	{
		weaponAddress = mem::in::find_DMA(moduleBase + 0x1486EB0 + weaponNum * 4, { 0x5C }); ///Rocket launchers
	}
	else
	{
		weaponAddress = mem::in::find_DMA(moduleBase + 0x1486EB0 + weaponNum * 4, { 0x6 }); ///ItemBlocker
		mem::in::write((BYTE*)weaponAddress, (BYTE*)"\x00", sizeof("\x00"));
		weaponAddress += 0x4E; ///Items
	}
	mem::in::write((BYTE*)weaponAddress, (BYTE*)&weaponValue, sizeof(weaponValue));
}

///CUSTOMIZATION

void RedTrainer::setPlayerType(int playerTypeId)
{
	bool bPlayerType = true;

	if (playerTypeId > 2)
		playerTypeId += 5;

	if (playerTypeId == 10)
		bPlayerType = false;

	if (bPlayerType)
	{
		mem::in::write((BYTE*)(moduleBase + 0x823EC1), (BYTE*)"\xB8\x00\x00\x00\x00", 5);
		mem::in::write((BYTE*)(moduleBase + 0x823EC2), (BYTE*)&playerTypeId, 1);
	}
	else
	{
		mem::in::write((BYTE*)(moduleBase + 0x823EC1), (BYTE*)"\xE8\x1A\xFF\xFE\xFF", 5);
	}
}

void RedTrainer::setPlayerSword(int playerSwordId)
{
	bool bPlayerSword = true;

	if (playerSwordId > 7)
		playerSwordId++;

	if (playerSwordId == 13)
		bPlayerSword = false;

	if (bPlayerSword)
	{
		mem::in::write((BYTE*)(moduleBase + 0x823EDE), (BYTE*)"\xBD\x00\x00\x00\x00\x90", 6);
		mem::in::write((BYTE*)(moduleBase + 0x823EDF), (BYTE*)&playerSwordId, 1);
	}
	else
	{
		mem::in::write((BYTE*)(moduleBase + 0x823EDE), (BYTE*)"\x8B\x44\x24\x0C\x8B\xE8", 6);
	}
}

void RedTrainer::setPlayerBody(int playerBodyId)
{
	bool bPlayerBody = true;

	if (playerBodyId == 16)
		bPlayerBody = false;

	if (bPlayerBody)
	{
		mem::in::write((BYTE*)(moduleBase + 0x823FA2), (BYTE*)"\xB8\x00\x00\x00\x00", 5);
		mem::in::write((BYTE*)(moduleBase + 0x823FA3), (BYTE*)&playerBodyId, 1);
	}
	else
	{
		mem::in::write((BYTE*)(moduleBase + 0x823FA2), (BYTE*)"\xE8\x79\x34\xDA\xFF", 5);
	}
}

void RedTrainer::setPlayerHair(int playerHairId)
{
	bool bPlayerHair = true;

	if (playerHairId == 4)
		bPlayerHair = false;

	if (bPlayerHair)
	{
		mem::in::write((BYTE*)(moduleBase + 0x5C7CAC), (BYTE*)"\xB8\x00\x00\x00\x00\xEB\x1C", 7);
		mem::in::write((BYTE*)(moduleBase + 0x5C7CAD), (BYTE*)&playerHairId, 1);
	}
	else
	{
		mem::in::write((BYTE*)(moduleBase + 0x5C7CAC), (BYTE*)"\x75\x1C\xB9\x40\x91\xFE\x01", 7);
	}
}

///MOVEMENT

void RedTrainer::setSpeed(float speedValue)
{
	mem::in::write((BYTE*)(moduleBase + 0x17E93EC), (BYTE*)&speedValue, sizeof(speedValue));
}

void RedTrainer::setFly() //NOT OPTIMIZED!!!
{
	memcpy(&posX, (BYTE*)(moduleBase + 0x19C1490), sizeof(posX));

	if (posX == NULL)
	{
		return;
	}
	
	if (isFirstFly) {
		setSpeed(0.0f);
		isFirstFly = false;
	}

	uintptr_t posAddress = mem::in::find_DMA(moduleBase + 0x19C1490, { 0x50 });
	memcpy(&posX, (BYTE*)posAddress, sizeof(posX));
	memcpy(&posY, (BYTE*)posAddress + 4, sizeof(posY));
	memcpy(&posZ, (BYTE*)posAddress + 8, sizeof(posZ));

	if (GetAsyncKeyState(VK_NUMPAD2) && 0x8000)
	{
		posX -= 0.5f;
		mem::in::write((BYTE*)posAddress, (BYTE*)&posX, sizeof(posX));
	}

	if (GetAsyncKeyState(VK_NUMPAD8) && 0x8000)
	{
		posX += 0.5f;
		mem::in::write((BYTE*)posAddress, (BYTE*)&posX, sizeof(posX));
	}

	if (GetAsyncKeyState(VK_NUMPAD4) && 0x8000)
	{
		posZ -= 0.5f;
		mem::in::write((BYTE*)(posAddress + 8), (BYTE*)&posZ, sizeof(posZ));
	}

	if (GetAsyncKeyState(VK_NUMPAD6) && 0x8000)
	{
		posZ += 0.5f;
		mem::in::write((BYTE*)(posAddress + 8), (BYTE*)&posZ, sizeof(posZ));
	}

	if (GetAsyncKeyState(VK_NUMPAD1) && 0x8000)
	{
		posY -= 0.5f;
		mem::in::write((BYTE*)(posAddress + 4), (BYTE*)&posY, sizeof(posY));
	}

	if (GetAsyncKeyState(VK_NUMPAD7) && 0x8000)
	{
		posY += 0.5f;
		mem::in::write((BYTE*)(posAddress + 4), (BYTE*)&posY, sizeof(posY));
	}

}

///MISSION

void RedTrainer::setMission(short missionId, char missionName[]) 
{
	if (missionName == NULL)
		return;

	mem::in::read((BYTE*)(moduleBase + 0x1764670), (BYTE*)&currentMissionCheck, sizeof(currentMissionCheck));

	if (currentMissionCheck) 
	{
		mem::in::write((BYTE*)(moduleBase + 0x1764670), (BYTE*)&missionId, sizeof(missionId));
		mem::in::write((BYTE*)(moduleBase + 0x1764674), (BYTE*)&missionName[0], 0x20);
	}
	else
	{
		mem::in::write((BYTE*)(moduleBase + 0x1766004), (BYTE*)&missionId, sizeof(missionId));
		mem::in::write((BYTE*)(moduleBase + 0x1766008), (BYTE*)&missionName[0], 0x20);
	}

	currentMissionCheck = 0;
}

void RedTrainer::setDifficulty(char difficultyValue)
{
	mem::in::write((BYTE*)(moduleBase + 0x1764430), (BYTE*)&difficultyValue, sizeof(difficultyValue));
}

void RedTrainer::setNoDamage(bool &bNoDamage)
{
	bNoDamage = !bNoDamage;
	if (bNoDamage)
	{
		mem::in::write((BYTE*)(moduleBase + 0x81B488), (BYTE*)"\xEB\x05", 2);
		mem::in::write((BYTE*)(moduleBase + 0x1776218), (BYTE*)"\x00", 1);
	}
	else
	{
		mem::in::write((BYTE*)(moduleBase + 0x81B488), (BYTE*)"\x74\x05", 2);
	}
}

void RedTrainer::setNoKilled(bool &bNoKilled) //doesnt work
{
	bNoKilled = !bNoKilled;
	if (bNoKilled)
	{
		mem::in::write((BYTE*)(moduleBase + 0x81B494), (BYTE*)"\xEB\x05", 2);
		mem::in::write((BYTE*)(moduleBase + 0x177621C), (BYTE*)"\x00\x00\x00\x00", 4);
	}
	else
	{
		mem::in::write((BYTE*)(moduleBase + 0x81B494), (BYTE*)"\x74\x05", 2);
	}
}

void RedTrainer::setNoAlert(bool &bNoAlert) //doesnt work
{
	bNoAlert = !bNoAlert;
	if (bNoAlert)
	{
		mem::in::write((BYTE*)(moduleBase + 0x82C417), (BYTE*)"\x90\x90\x90\x90\x90\x90", 6);
		mem::in::write((BYTE*)(moduleBase + 0x1776174), (BYTE*)"\x00\x00\x00\x00", 4);
	}
	else
	{
		mem::in::write((BYTE*)(moduleBase + 0x82C417), (BYTE*)"\x01\x05\x74\x61\x30\x02", 6);
	}
}

void RedTrainer::setBattleTimer(float timerValue)
{
	mem::in::write((BYTE*)(moduleBase + 0x1776204), (BYTE*)&timerValue, sizeof(timerValue));
}

void RedTrainer::setBattlePoints(int battlePointsValue)
{
	mem::in::write((BYTE*)(moduleBase + 0x1776208), (BYTE*)&battlePointsValue, sizeof(battlePointsValue));
}

void RedTrainer::setMaxCombo(int maxComboValue)
{
	mem::in::write((BYTE*)(moduleBase + 0x1776210), (BYTE*)&maxComboValue, sizeof(maxComboValue));
}

void RedTrainer::setKills(int killsValue)
{
	mem::in::write((BYTE*)(moduleBase + 0x1776214), (BYTE*)&killsValue, sizeof(killsValue));
}

void RedTrainer::setZandzutsuKills(int zandzutsuKillsValue)
{
	mem::in::write((BYTE*)(moduleBase + 0x177620C), (BYTE*)&zandzutsuKillsValue, sizeof(zandzutsuKillsValue));
}

///RAIDEN FLAGS

void RedTrainer::setFlag(char raidenFlag)
{
	bool isSizeOver = false;
	int setBitsFromFlag = 0;
	int memoryBuffer = 0;

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
		mem::in::read((BYTE*)(moduleBase + 0x17EA090), (BYTE*)&memoryBuffer, sizeof(memoryBuffer));
		memoryBuffer = memoryBuffer ^ setBitsFromFlag;
		mem::in::write((BYTE*)(moduleBase + 0x17EA090), (BYTE*)&memoryBuffer, sizeof(memoryBuffer));
	}
	else
	{
		raidenFlag -= 0x20;
		raidenFlag = 0x1F - raidenFlag;
		setBitsFromFlag = 1 << raidenFlag;
		mem::in::read((BYTE*)(moduleBase + 0x17EA094), (BYTE*)&memoryBuffer, sizeof(memoryBuffer));
		memoryBuffer = memoryBuffer ^ setBitsFromFlag;
		mem::in::write((BYTE*)(moduleBase + 0x17EA094), (BYTE*)&memoryBuffer, sizeof(memoryBuffer));
	}
}

void RedTrainer::setRender(unsigned int renderType)
{
	mem::in::write((BYTE*)(moduleBase + 0x17EA09C), (BYTE*)&renderType, sizeof(renderType));
}

///OTHER

void RedTrainer::setFilter(float filterValue, char filterOffset)
{
	mem::in::write((BYTE*)(moduleBase + 0x1ADD604 + filterOffset), (BYTE*)&filterValue, sizeof(filterValue));
}

void RedTrainer::setSize(float sizeValue, char sizeOffset)
{
	uintptr_t sizeAddress;
	memcpy(&sizeAddress, (BYTE*)(moduleBase + 0x19C1490), sizeof(sizeAddress));
	if (sizeAddress == NULL)
	{
		return;
	}
	sizeAddress = mem::in::find_DMA(moduleBase + 0x19C1490, { 0x70 + (unsigned int)sizeOffset });
	mem::in::write((BYTE*)(sizeAddress), (BYTE*)&sizeValue, sizeof(sizeValue));
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
	mem::in::write((BYTE*)(moduleBase + 0x17E9F9C), (BYTE*)&menuType, sizeof(menuType));
}

///ENEMY

void RedTrainer::setAllEnemies(bool bEnemyActive, unsigned int enemyId, unsigned int enemyType, unsigned int enemySetType, unsigned int enemyFlag)
{
	if (bEnemyActive) //delete write functions
	{
		//enemyId = reverseBytes(enemyId);
		//enemyType = reverseBytes(enemyType);
		//enemySetType = reverseBytes(enemySetType);
		//enemyFlag = reverseBytes(enemyFlag);

		mem::in::set_nop((BYTE*)(moduleBase + 0x8A1960), 0x11); //Id setter to nop
		mem::in::write((BYTE*)(moduleBase + 0x8A1960), (BYTE*)"\xC7\x46\x0C", 3);
		mem::in::write((BYTE*)(moduleBase + 0x8A1963), (BYTE*)&enemyId, 4);

		mem::in::set_nop((BYTE*)(moduleBase + 0x8A1A4D), 0x11); //Type to nop
		mem::in::write((BYTE*)(moduleBase + 0x8A1A4D), (BYTE*)"\xC7\x46\x44", 3);
		mem::in::write((BYTE*)(moduleBase + 0x8A1A50), (BYTE*)&enemyType, 4);

		mem::in::set_nop((BYTE*)(moduleBase + 0x8A1A75), 0x11); //SetType to nop
		mem::in::write((BYTE*)(moduleBase + 0x8A1A75), (BYTE*)"\xC7\x46\x48", 3);
		mem::in::write((BYTE*)(moduleBase + 0x8A1A78), (BYTE*)&enemySetType, 4);

		mem::in::set_nop((BYTE*)(moduleBase + 0x8A1AC5), 0x11); //Flag to nop
		mem::in::write((BYTE*)(moduleBase + 0x8A1AC5), (BYTE*)"\xC7\x46\x50", 3);
		mem::in::write((BYTE*)(moduleBase + 0x8A1AC8), (BYTE*)&enemyFlag, 4);
	}
	else //reset
	{
		mem::in::write((BYTE*)(moduleBase + 0x8A1960), (BYTE*)"\x8B\x17\x8D\x4E\x0C\x51\x50\x8B\x82\xE8\x00\x00\x00\x8B\xCF\xFF\xD0", 0x11); //reset id
		mem::in::write((BYTE*)(moduleBase + 0x8A1A4D), (BYTE*)"\x8B\x17\x8D\x4E\x44\x51\x50\x8B\x82\xD8\x00\x00\x00\x8B\xCF\xFF\xD0", 0x11); //reset type
		mem::in::write((BYTE*)(moduleBase + 0x8A1A75), (BYTE*)"\x8B\x17\x8D\x4E\x48\x51\x50\x8B\x82\xD8\x00\x00\x00\x8B\xCF\xFF\xD0", 0x11); //reset setType
		mem::in::write((BYTE*)(moduleBase + 0x8A1AC5), (BYTE*)"\x8B\x17\x8D\x4E\x50\x51\x50\x8B\x82\xE8\x00\x00\x00\x8B\xCF\xFF\xD0", 0x11); //reset flag
	}
}

///TRASH

/*typedef int(__cdecl *pSetCorpsFunc)(unsigned int*, __m128, int);
pSetCorpsFunc setCorpsFunc = (pSetCorpsFunc)(0x8A4360 + RedTrainer::moduleBase);

unsigned int corpsAddr = 0x8A4360 + RedTrainer::moduleBase;
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
	uintptr_t posAddress = mem::in::find_DMA(moduleBase + 0x19C1490, { 0x50 });
	memcpy(&posX, (BYTE*)posAddress, sizeof(posX));
	memcpy(&posY, (BYTE*)posAddress + 4, sizeof(posY));
	memcpy(&posZ, (BYTE*)posAddress + 8, sizeof(posZ));

	///TODO: Modify the function to fill the array with your own values


	corpsAddr = 0x8A4360 + moduleBase;
	criticalSectPtr = moduleBase + 0x1878CD0;

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

	//setCorpsFunc = (pSetCorpsFunc)(0x8A4360 + moduleBase);
	//setCorpsFunc((unsigned int*)criticalSectPtr, xmmReg, arrayEnemyInfo[0]);

	emSetCorpsCrEnemyPtr((unsigned int*)criticalSectPtr, xmmReg, (int)pArrayEnemyInfo);
}*/