#include "pch.h"
#include "base.h"
#include <emmintrin.h>
#include "trainerFunctions.h"
#include "trainerGui/trainerGui.h"

int currentMissionCheck = 0;
float posX = 0.0f, posY = 0.0f, posZ = 0.0f;
int arrayEnemyInfo[70] = { 0 };

bool RedTrainer::isFirstFly = true;
uintptr_t RedTrainer::moduleBase = 0;

///GAME FUNCTIONS

int(*playAnimation)(unsigned int, int, int, int, int); ///playerPtr, animId, animType, aId2, aType2
int(*getItem)(unsigned int);
unsigned int(*setLessSword)(unsigned int, int);
int(*setSound)(int, int);
int(*setBgmFunc)(int);
int(*setMessagePrint)(unsigned int, int, unsigned int, int, int, int, int, int, int);
void(*spawnEnemyFunc)(unsigned int, unsigned int);
unsigned int*(*setCorpsFunc)(unsigned int, int);

///ADDITIONAL FUNCTIONS

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

unsigned int  RedTrainer::reverseBytes(unsigned int reverseValue) //trash
{
	unsigned int result = 0;

	result |= ((reverseValue & 0x000000FF) << 24);
	result |= ((reverseValue & 0x0000FF00) << 8);
	result |= ((reverseValue & 0x00FF0000) >> 8);
	result |= ((reverseValue & 0xFF000000) >> 24);

	return result;
}

void RedTrainer::playSound(int soundPtr) ///For PTR
{
	soundPtr += moduleBase; 
	int setSeSoundFunc = moduleBase + 0xA5E050;
	setSound = (int(__cdecl*)(int, int))setSeSoundFunc; ///Play button click

	__asm {
		push 0
		push soundPtr
		call setSound
		add esp, 8
	}
} 
void RedTrainer::playSound(char soundText[], int secondVar) ///For string
{
	int setSeSoundFunc = moduleBase + 0xA5E050;
	setSound = (int(__cdecl*)(int, int))setSeSoundFunc; ///Play button click

	__asm {
		push 0
		push [soundText]
		call setSound
		add esp, 8
	}
}

void RedTrainer::setBgm(int bgmPtr)
{
	bgmPtr += moduleBase; //0x123CFD8 bgm sam
	int bgmFuncAddr = moduleBase + 0xA5E1B0;
	setBgmFunc = (int(__cdecl*)(int))bgmFuncAddr;

	__asm {
		push bgmPtr
		call setBgmFunc
		add esp, 4
	}
}
void RedTrainer::setBgm(char bgmText[])
{
	int bgmFuncAddr = moduleBase + 0xA5E1B0;
	setBgmFunc = (int(__cdecl*)(int))bgmFuncAddr;

	__asm {
		push [bgmText]
		call setBgmFunc
		add esp, 4
	}
}

void RedTrainer::setEnemyIdToStruct(unsigned int enemyId, unsigned int enemyType) //caller: 0xA9EEB0, callee: 0xA9EB50
{
	int setIdFuncAddress = moduleBase + 0x600A60;
	__asm
	{
		push enemyType
		push enemyId
		call setIdFuncAddress
	}
}
/*void RedTrainer::setEnemyIdToStruct(unsigned int enemyId, int funcNum) //caller: 0xA9EEB0, callee: 0xA9EB50
{
	int setIdFuncAddress = moduleBase + funcNum;
	__asm
	{
		push enemyId
		call setIdFuncAddress
		add esp, 4
	}
}*/

///STATS

void RedTrainer::setInvincibility(bool &bInvincibility)
{
	bInvincibility = !bInvincibility;
	//playSound(0x1256F24); /// change pos
	//playSound(0x12568D0); /// select item sound...
	playSound(0x1257100);
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
	playSound(0x1257100);
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
		playSound(0x12581AC);
		return;
	}
	playSound(0x1257100);
	healthAddress = mem::in::find_DMA(moduleBase + 0x19C1490, { 0x870 });
	mem::in::write((BYTE*)healthAddress, (BYTE*)&healthValue, sizeof(healthValue));
}

void RedTrainer::setMoney(int moneyValue)
{
	uintptr_t moneyAddress;
	memcpy(&moneyAddress, (BYTE*)(moduleBase + 0x19C1490), sizeof(moneyAddress));
	if (moneyAddress == NULL)
	{
		playSound(0x12581AC);
		return;
	}
	playSound(0x1257100);
	moneyAddress = mem::in::find_DMA(moduleBase + 0x17EA100, { 0xDC });
	mem::in::write((BYTE*)moneyAddress, (BYTE*)&moneyValue, sizeof(moneyValue));
}

void RedTrainer::setNewItem(char itemId)
{
	int playerPtr = 0, itemGetFunc = 0;
	memcpy(&playerPtr, (BYTE*)(moduleBase + 0x19C1490), sizeof(playerPtr));

	if (playerPtr == NULL)
	{
		playSound(0x12581AC);
		return;
	}
	playSound(0x1257100);
	switch (itemId)
	{
	case 0:
		itemGetFunc = moduleBase + 0x77CA60; ///ADD MAX HEALTH
		break;
	case 1:
		itemGetFunc = moduleBase + 0x7D9F60; ///ADD MAX FUELS
		break;
	case 2:
		itemGetFunc = moduleBase + 0x1E9110; ///SPINE
		break;
	default:
		return;
	}

	getItem = (int(*)(unsigned int))itemGetFunc;

	__asm {
		mov ecx, playerPtr
		call getItem
	}
}

///ITEMS

void RedTrainer::setInfinityAddWeapons(bool &bInfAddWeapon)
{
	playSound(0x1257100);
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
	playSound(0x1257100);
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
	playSound(0x1257100);
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
	playSound(0x1257100);
	if (uniqueShopType == 4) {
		uniqueShopType = 0x103;
	}

	uniqueShopId--;

	mem::in::write((BYTE*)(moduleBase + 0x1773B20 + uniqueShopId * 0x20), (BYTE*)&uniqueShopType, sizeof(uniqueShopType));
}

void RedTrainer::setWigShop(int wigShopId, short wigShopType)
{
	playSound(0x1257100);
	if (wigShopType == 4) {
		wigShopType = 0x103;
	}

	wigShopId--;

	mem::in::write((BYTE*)(moduleBase + 0x1773B80 + wigShopId * 0x20), (BYTE*)&wigShopType, sizeof(wigShopType));
}

void RedTrainer::setLifeFuelShop(int lfShopId, short lfShopType)
{
	playSound(0x1257100);
	if (lfShopType == 4) {
		lfShopType = 0x103;
	}

	mem::in::write((BYTE*)(moduleBase + 0x1773BE0 + lfShopId * 0x20), (BYTE*)&lfShopType, sizeof(lfShopType));
}

void RedTrainer::setSkillsShop(int skillShopId, short skillShopType)
{
	playSound(0x1257100);
	if (skillShopType == 4) {
		skillShopType = 0x103;
	}

	mem::in::write((BYTE*)(moduleBase + 0x1773D00 + skillShopId * 0x20), (BYTE*)&skillShopType, sizeof(skillShopType));
}

void RedTrainer::getWeaponsCount(char &weaponCount)
{
	//playSound(0x1257100);
	mem::in::read((BYTE*)(moduleBase + 0x1486EA8), (BYTE*)&weaponCount, sizeof(weaponCount));
}

void RedTrainer::setAddWeapons(char weaponNum, int weaponValue, char weaponCount)
{
	uintptr_t weaponAddress;
	memcpy(&weaponAddress, (BYTE*)(moduleBase + 0x1486EB0 + weaponNum * 4), sizeof(weaponAddress));
	if (weaponAddress == NULL)
	{
		playSound(0x12581AC);
		return;
	}
	playSound(0x1257100);
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
	playSound(0x1257100);

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
	playSound(0x1257100);

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
	playSound(0x1257100);

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
	playSound(0x1257100);

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

void RedTrainer::setInvisibility(bool &bInvisible)
{
	uintptr_t invisibleAddress;
	memcpy(&invisibleAddress, (BYTE*)(moduleBase + 0x19C1490), sizeof(invisibleAddress));
	if (invisibleAddress == NULL)
	{
		playSound(0x12581AC);
		return;
	}
	bInvisible = !bInvisible;
	int invisibleVar = bInvisible;
	playSound(0x1257100);
	invisibleAddress = mem::in::find_DMA(moduleBase + 0x19C1490, { 0x18C });
	mem::in::write((BYTE*)invisibleAddress, (BYTE*)&invisibleVar, sizeof(invisibleVar));
}

///MOVEMENT

void RedTrainer::setSpeed(float speedValue)
{
	playSound(0x1257100);
	mem::in::write((BYTE*)(moduleBase + 0x17E93EC), (BYTE*)&speedValue, sizeof(speedValue));
}

void RedTrainer::setFly() ///NOT OPTIMIZED!!!
{
	memcpy(&posX, (BYTE*)(moduleBase + 0x19C1490), sizeof(posX));

	if (posX == NULL)
	{
		RtGui::bFly = !RtGui::bFly;
		playSound(0x12581AC);
		return;
	}
	
	if (isFirstFly) {
		playSound(0x1257100);
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

void RedTrainer::setPlayerAnimation(int animId, int animType, int animIdOld, int animTypeOld, bool isSelectable)
{
	int playerPtr = 0;
	memcpy(&playerPtr, (BYTE*)(moduleBase + 0x19C1490), sizeof(playerPtr));

	if (playerPtr == NULL)
	{
		playSound(0x12581AC);
		return;
	}

	playSound(0x1257100);
	int playAnimFuncAddr = moduleBase + 0x68CAF0;
	playAnimation = (int(*)(unsigned int, int, int, int, int))playAnimFuncAddr;

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

	__asm {
		push animTypeOld
		push animIdOld
		push animType
		push animId
		mov ecx, playerPtr
		call playAnimation
	}
}

void RedTrainer::setWithoutSword(bool &isActive)
{
	int playerPtr = 0;
	memcpy(&playerPtr, (BYTE*)(moduleBase + 0x19C1490), sizeof(playerPtr));

	if (playerPtr == NULL)
	{
		playSound(0x12581AC);
		return;
	}

	isActive = !isActive;

	playSound(0x1257100);
	int playerSwordType = 1;
	int setLessSwordAddr = moduleBase + 0x77E210;
	setLessSword = (unsigned int(*)(unsigned int, int))setLessSwordAddr;

	__asm {
		push playerSwordType
		mov ecx, playerPtr
		call setLessSword
	}
}

///MISSION

void RedTrainer::setMission(short missionId, char missionName[]) 
{
	if (missionName == NULL)
		return;

	playSound(0x1257100);

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
	playSound(0x1257100);
	mem::in::write((BYTE*)(moduleBase + 0x1764430), (BYTE*)&difficultyValue, sizeof(difficultyValue));
}

void RedTrainer::setNoDamage(bool &bNoDamage)
{
	playSound(0x1257100);
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
	playSound(0x1257100);
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
	playSound(0x1257100);
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
	playSound(0x1257100);
	mem::in::write((BYTE*)(moduleBase + 0x1776204), (BYTE*)&timerValue, sizeof(timerValue));
}

void RedTrainer::setBattlePoints(int battlePointsValue)
{
	playSound(0x1257100);
	mem::in::write((BYTE*)(moduleBase + 0x1776208), (BYTE*)&battlePointsValue, sizeof(battlePointsValue));
}

void RedTrainer::setMaxCombo(int maxComboValue)
{
	playSound(0x1257100);
	mem::in::write((BYTE*)(moduleBase + 0x1776210), (BYTE*)&maxComboValue, sizeof(maxComboValue));
}

void RedTrainer::setKills(int killsValue)
{
	playSound(0x1257100);
	mem::in::write((BYTE*)(moduleBase + 0x1776214), (BYTE*)&killsValue, sizeof(killsValue));
}

void RedTrainer::setZandzutsuKills(int zandzutsuKillsValue)
{
	playSound(0x1257100);
	mem::in::write((BYTE*)(moduleBase + 0x177620C), (BYTE*)&zandzutsuKillsValue, sizeof(zandzutsuKillsValue));
}

///RAIDEN FLAGS

void RedTrainer::setFlag(char raidenFlag)
{
	bool isSizeOver = false;
	int setBitsFromFlag = 0;
	int memoryBuffer = 0;
	playSound(0x1257100);

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
	playSound(0x1257100);
	mem::in::write((BYTE*)(moduleBase + 0x17EA09C), (BYTE*)&renderType, sizeof(renderType));
}

///OTHER

void RedTrainer::setFilter(float filterValue, char filterOffset)
{
	playSound(0x1257100);
	mem::in::write((BYTE*)(moduleBase + 0x1ADD604 + filterOffset), (BYTE*)&filterValue, sizeof(filterValue));
}

void RedTrainer::setSize(float sizeValue, char sizeOffset)
{
	uintptr_t sizeAddress;
	memcpy(&sizeAddress, (BYTE*)(moduleBase + 0x19C1490), sizeof(sizeAddress));
	
	if (sizeAddress == NULL)
	{
		playSound(0x12581AC);
		return;
	}

	playSound(0x1257100);
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

void RedTrainer::printMessage(unsigned int messageId, int messageNum, int messagePrint, int messagePosition, char messageChar[])
{
	int setPtrChkMsg = moduleBase + 0x19C3D08;
	int setMsgFuncAddr = moduleBase + 0x8E2DB0;
	playSound(0x1257100);
	//mem::in::read((BYTE*)(moduleBase + 0x19C3D08), (BYTE*)&setPtrChkMsg, sizeof(setPtrChkMsg));
	
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
	playSound(0x1257100);
	if (typeEnemyActive && 0x1) //delete write functions
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

		mem::in::set_nop((BYTE*)(moduleBase + 0x8A1AC5), 0x11); //Flag to nop
		mem::in::write((BYTE*)(moduleBase + 0x8A1AC5), (BYTE*)"\xC7\x46\x50", 3);
		mem::in::write((BYTE*)(moduleBase + 0x8A1AC8), (BYTE*)&enemyFlag, 4);
	}

	if (typeEnemyActive && 0x2) //SetType to nop
	{
		mem::in::set_nop((BYTE*)(moduleBase + 0x8A1A75), 0x11); 
		mem::in::write((BYTE*)(moduleBase + 0x8A1A75), (BYTE*)"\xC7\x46\x48", 3);
		mem::in::write((BYTE*)(moduleBase + 0x8A1A78), (BYTE*)&enemySetType, 4);
	}
	/*
	if (typeEnemyActive && 0x4) //Set enemy to player pos
	{
		memcpy(&posX, (BYTE*)(moduleBase + 0x19C1490), sizeof(posX));

		if (posX == NULL)
		{
			playSound(0x12581AC);
			return;
		}

		uintptr_t posAddress = mem::in::find_DMA(moduleBase + 0x19C1490, { 0x50 });
		memcpy(&posX, (BYTE*)posAddress, sizeof(posX));
		memcpy(&posY, (BYTE*)posAddress + 4, sizeof(posY));
		memcpy(&posZ, (BYTE*)posAddress + 8, sizeof(posZ));

		mem::in::set_nop((BYTE*)(moduleBase + 0x8A1999), 0x25);
		mem::in::write((BYTE*)(moduleBase + 0x8A1999), (BYTE*)"\xC7\x46\x1C", 3);
		mem::in::write((BYTE*)(moduleBase + 0x8A199C), (BYTE*)&posX, 4);
		mem::in::write((BYTE*)(moduleBase + 0x8A19A0), (BYTE*)"\xC7\x46\x20", 3);
		mem::in::write((BYTE*)(moduleBase + 0x8A19A3), (BYTE*)&posY, 4);
		mem::in::write((BYTE*)(moduleBase + 0x8A19A7), (BYTE*)"\xC7\x46\x24", 3);
		mem::in::write((BYTE*)(moduleBase + 0x8A19AA), (BYTE*)&posZ, 4);
	}*/
	
	if (!typeEnemyActive) //reset
	{
		mem::in::write((BYTE*)(moduleBase + 0x8A1960), (BYTE*)"\x8B\x17\x8D\x4E\x0C\x51\x50\x8B\x82\xE8\x00\x00\x00\x8B\xCF\xFF\xD0", 0x11); //reset id
		mem::in::write((BYTE*)(moduleBase + 0x8A1A4D), (BYTE*)"\x8B\x17\x8D\x4E\x44\x51\x50\x8B\x82\xD8\x00\x00\x00\x8B\xCF\xFF\xD0", 0x11); //reset type
		mem::in::write((BYTE*)(moduleBase + 0x8A1A75), (BYTE*)"\x8B\x17\x8D\x4E\x48\x51\x50\x8B\x82\xD8\x00\x00\x00\x8B\xCF\xFF\xD0", 0x11); //reset setType
		mem::in::write((BYTE*)(moduleBase + 0x8A1AC5), (BYTE*)"\x8B\x17\x8D\x4E\x50\x51\x50\x8B\x82\xE8\x00\x00\x00\x8B\xCF\xFF\xD0", 0x11); //reset flag
		//mem::in::write((BYTE*)(moduleBase + 0x8A1999), (BYTE*)"\x8B\x17\x8B\x82\x9C\x00\x00\x00\x68\xD0\xFC\x09\x02\x53\x8B\xCF\xFF\xD0\x83\xF8\xFF\x74\x0E\x8B\x17\x55\x50\x8B\x82\xC4\x00\x00\x00\x8B\xCF\xFF\xD0", 0x25); //reset pos
	}
}

///TEST

void RedTrainer::spawnEnemy(unsigned int enemyId, unsigned int enemyType, unsigned int enemyFlag)
{
	int setCorpsFuncAddr = moduleBase + 0x8A4360;
	int criticalSectPtr = moduleBase + 0x1878CD0; //Get ptr from func
	//int binXmlPtr = moduleBase + 0x123E684;
	//int setArrayXmlFunc = moduleBase + 0x8A1790;
	//int someFuncAddr = moduleBase + 0x681330;
	//int lockFunc = moduleBase + 0x67C950;
	//int setFunc = moduleBase + 0x67C960;

	///CHECK FOR CREATION

	memcpy(&posX, (BYTE*)(moduleBase + 0x19C1490), sizeof(posX));

	if (posX == NULL)
	{
		playSound(0x12581AC);
		return;
	}

	uintptr_t posAddress = mem::in::find_DMA(moduleBase + 0x19C1490, { 0x50 });
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

	setCorpsFunc = (unsigned int*(*)(unsigned int, int))setCorpsFuncAddr;

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
		lea esi, [edi+50h]

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

void(*playerImplementFunc)(int);
void RedTrainer::playerImplement(int plId) //trash
{
	int plImplAddr = moduleBase + 0x841040;//0x840850;
	int plImplement = moduleBase + 0x12A6BDC;
	playerImplementFunc = (void(*)(int))plImplAddr;

	__asm
	{
		lea ecx, plImplement
		call playerImplementFunc//spfunc
	}
}

///TRASH

//itemGetFunc = moduleBase + 0x54ABB0;
//itemGetFunc = moduleBase + 0x1EA950;
//itemGetFunc = moduleBase + 0x54C2F0;

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
/*
void RedTrainer::spawnEnemyA(unsigned int enemyId, unsigned int enemyType, unsigned int enemyFlag)
{
	setAllEnemies(0x5, enemyId, enemyType, 0, enemyFlag);

	int spawnFuncAddr = moduleBase + 0x8A6F70;//0x8A5F90;
	int criticalSectPtr = moduleBase + 0x1878CD0;

	//spawnFunc?


	__asm
	{
		push 0
		mov ecx, criticalSectPtr
		call spawnFuncAddr//spfunc
	}

	setAllEnemies(0);
}*/