#include "pch.h"
#include "base.h"
#include "trainerFunctions.h"

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

//TODO
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

/*void RedTrainer::spawnEnemy()
{
	positionAddress = mem::FindDMAAddy(moduleBase + 0x19C1490, { 0x50 });

	memcpy(&pos[0], (BYTE*)positionAddress, sizeof(pos[0]));
	memcpy(&pos[1], (BYTE*)positionAddress + 4, sizeof(pos[1]));
	memcpy(&pos[2], (BYTE*)positionAddress + 8, sizeof(pos[2]));
	//DWORD workThread = GetThreadByNum(procId, -2);

	CreateEnemy(0x20140, 2, 0, 0, pos);
}*/

/*uintptr_t* localPlayerPtr = (uintptr_t*)(moduleBase + 0x19C1490);

if (localPlayerPtr)
{
	if (bHealth) {
		uintptr_t healthAddress = mem::FindDMAAddy(moduleBase + 0x19C1490, { 0x870 });
		mem::Patch((BYTE*)healthAddress, (BYTE*)&healthValue, sizeof(healthValue));
		std::cout << "Health value = " << std::dec << healthValue << std::endl;
		bHealth = false;
	}
}*/