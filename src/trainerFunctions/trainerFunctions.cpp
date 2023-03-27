#include "pch.h"
#include "base.h"
#include "trainerFunctions.h"

bool RedTrainer::isFirstFly = true;

float posX = 0.0f, posY = 0.0f, posZ = 0.0f;

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
	uintptr_t healthAddress = mem::in::find_DMA(moduleBase + 0x19C1490, { 0x870 });
	mem::in::write((BYTE*)healthAddress, (BYTE*)&healthValue, sizeof(healthValue));
}

void RedTrainer::setMoney(int moneyValue)
{
	uintptr_t moneyAddress = mem::in::find_DMA(moduleBase + 0x17EA100, { 0xDC });
	mem::in::write((BYTE*)moneyAddress, (BYTE*)&moneyValue, sizeof(moneyValue));
}

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

void RedTrainer::setMission(short missionId, char missionName[]) 
{
	if (missionName == NULL)
		return;
	mem::in::write((BYTE*)(moduleBase + 0x1764670), (BYTE*)&missionId, sizeof(missionId));
	mem::in::write((BYTE*)(moduleBase + 0x1764674), (BYTE*)&missionName[0], 0x20);
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

/*void RedTrainer::spawnEnemy()
{
	positionAddress = mem::FindDMAAddy(moduleBase + 0x19C1490, { 0x50 });

	memcpy(&pos[0], (BYTE*)positionAddress, sizeof(pos[0]));
	memcpy(&pos[1], (BYTE*)positionAddress + 4, sizeof(pos[1]));
	memcpy(&pos[2], (BYTE*)positionAddress + 8, sizeof(pos[2]));
	//DWORD workThread = GetThreadByNum(procId, -2);

	CreateEnemy(0x20140, 2, 0, 0, pos);
}*/
