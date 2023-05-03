#include "pch.h"
#include "base.h"
#include "trainerGui.h"
#include "trainerFunctions/trainerFunctions.h"

bool bInvicibility = false,
bEnergy = false,
bNoDamage = false,
bNoKilled = false,
bNoAlert = false,
bInfAddWeapon = false,
isHasSword = false,
RtGui::bFly = false,
RtGui::bTestWindow = false,
bInvisible = false,
RtGui::toSpawn = false;

char missionName[0x20] = "Subphase",
	 messageChar[0xFF] = { 0 },
	 bgmChar[0x40] = "bgm_Paused_Exit",
	 difficultyValue = 0,
	 cRaidenType = 0,
	 cMenuType = 0,
	 addWeaponCount = 0,
	 addWeaponNum = 0,
	 cItemId = 0,
	 funcType = 0,
	 numberOfFunctionArgs = 0;

short missionId	= 0x1d,
	  cBodyShop	= 0,
	  cSwordShop = 0,
	  cUniqueWeaponShop = 0,
	  cWigShop = 0,
	  cLifeFuelShop = 0,
	  cSkillShop = 0;

int healthValue	= 1600,
	moneyValue = 0,
	bodyTypeShop = 1,
	swordTypeShop = 0,
	uniqueWeaponTypeShop = 1,
	wigTypeShop	= 1,
	lifeFuelTypeShop = 0,
	skillTypeShop = 0,
	playerType = 5,
	attackType = 0,
	swordType = 12,
	bodyType = 16,
	hairType = 4,
	battlePointsValue = 0,
	maxComboValue = 0,
	killsValue 	= 0,
	zandzutsuKillsValue = 0,
	addWeaponValue = 0,
	animationIdSelectable = 0,
	animationId = 0, ///FOR TEST
	animationTime = 0,
	animationIdOld = 0,
	animationTimeOld = 0,
	messageNum = 0,
	messagePrint = 0,
	messagePosition = 0,
	animationID = 392,
	enemyDamageType = 2,
	callFunctionAddress = 0,
	functionArgumentA = 0,
	functionArgumentB = 0,
	functionArgumentC = 0,
	functionArgumentD = 0,
	functionArgumentE = 0,
	functionArgumentF = 0;

unsigned int RtGui::enemyId = 0x00000000,
			 RtGui::enemyTypeId = 0x00000000,
			 RtGui::enemySetTypeId = 0x00000000,
			 RtGui::enemyFlagId = 0x00000000,
			 renderType = 0x00000000,
			 messageId = 0,
			 bgmPtr = 0x0,
			 RtGui::previousEnemyId = 0x1,
			 RtGui::previousTypeId = 0xBEC001,
			 RtGui::playerAnimationValue = 0x0;

float speedValue  = 0.0f,
	  battleTimer = 0.0f,
	  rFilter	  = 1.0f,
	  gFilter	  = 1.0f, 
	  bFilter	  = 1.0f,
	  xSize		  = 1.0f,
	  ySize		  = 1.0f, 
	  zSize		  = 1.0f;

std::chrono::steady_clock::time_point RtGui::spawnTimer;

const char* cNewItemType[] = { "Max life", "Max fuel", "Spine" };
const char* cPlayerTypes[] = { "Raiden", "First Raiden", "Camera mode", "Sam", "BladeWolf", "Disabled" };
const char* cAttackTypes[] = { "Raiden", "Sam", "Boss Sam", "Armstrong" };
const char* cSwordTypes[]  = { "HF Blade", "Stun Blade", "Armor Breaker", "Long Sword", "Wooden Sword", "Murasama",
							  "Fox Blade", "HF Machete", "First Blade", "Sam's Murasama", "Chainsaw", "Invisible", "Disabled" };
const char* cUniqueWeaponsTypes[] = { "Default", "Pole-Arm", "Sai", "Pincer Blades", "Disabled" };
const char* cBodyTypes[]   = { "Default", "Blue Body", "Red Body", "Yellow Body", "Desperado", "Suit", "Mariachi",
							 "Original Body", "MGS4", "Gray Fox", "White Armor", "Inferno Armor", "Commando Armor", 
							 "Raiden", "First Body", "Sam :)", "Disabled" };
const char* cHairTypes[]   = { "Default", "Wig A", "Wig B", "Wig C", "Disabled" };
const char* cAddWeaponTypesR[] = { "RPG", "Stinger", "Grenade", "Smoke grenade", "Chaff grenade", "EMP grenade", "Freepaper", "Dambole", "Drumcan", "Energy pack", "Life pack" }; ///Raiden
const char* cAddWeaponTypesS[] = { "RPG", "Stinger", "Grenade", "Smoke grenade", "EMP grenade", "Dambole", "Drumcan", "Energy pack", "Life pack" }; ///Sam
const char* cAddWeaponTypesB[] = { "Smoke Grenade", "EMP Grenade", "Freepaper", "Energy pack", "Life pack", "Blade knife" }; ///Bladewolf

const char* cLifeFuelUpgrades[] = { "Life 1", "Life 2", "Life 3", "Life 4", "Fuel 1", "Fuel 2", "Fuel 3", "Fuel 4", "Fuel 5" };
const char* cSkillTypes[] = { "Aerial Parry", "Defensive Offense", "Sky High", "Sweep Kick", "Thunder Strike", "Falling Lightning", 
							  "Quick Draw", "Lightning Strike", "Stormbringer", "Marches du ciel", "Lumiere du ciel", 
							  "Cercle de l'ange", "Turbuience", "Down Burst"};
const char* cShopItemTypes[] = { "Hidden", "New", "To buy", "Buyed", "Selected" };
const char* cDifficultyValues[] = { "Easy", "Normal", "Hard", "Very hard", "Revengeance" };
const char* cRaidenFlags[] = {
"UI", "Answer a call", "Zangeki mode", "Auto HPup", "Battle end demo", "Battle emotioned end", "Missile ninja run",
"Walk only", "Attack off", "Ignore low parkour", "Ignore cliff parkour", "Ignore wall parkour", "Ignore sliding parkour",
"L1 attack off", "Active Y pound", "Active B push", "Zangeki off", "Ninjarun off", "Guard off", "Gecko trail", "Datsu off",
"Jump off", "Map auto load off", "Player no die", "No overheat", "Navigation on", "Rippermode off",
"Onehand", "Oneshot off", "OS Bottmode", "Inf zangeki", "QTE UI disable", "No XA", "No move", "No divekill",
"Off battlecollision", "Display result", "End UI disable", "Item no drop", "No exp add", "Reverse X camera", "Reverse Y camera", 
"Camdir in zangeki", "CODEC point view", "Slider ninjarun", "Weapon select", "Kogekko play", "Soldier monologue", "Depression Raiden",
"HP1 Raiden", "No attack", "Gun manipulate", "Set ripper", "Player no lockon", "Set mask", "Disable zangeki", "Story tutorial", "XY Attack off", "TGS Mode"
};
const char* cMenuTypes[] = { 
"Lock menu", "Game", "Pause", "CODEC", "Pause cutscene", "Mission failed 3", 
"Mission failed 2", "Weapon menu", "System settings", "Combo list", "Resume", "Menu", "VR-missions" };
const char* cAnimationTypesRaiden[] = { 
"Still1", "Still2", "Still3", //0, 1, 72
"Walk1", "Walk2", //2, 3
"Landing1", "Landing2", //11, 91
"Jump1", "Jump2", //4, 5
"Flip1", "Flip2", "Flip3", "Flip4", "Flip5", "Flip6", "Flip7", //10, 15, 16, 17, 18, 19, 20,
"Flip8", "Flip9", "Flip10", "Flip11", "Flip12", "Flip13", "Flip14", "Flip15", //62, 204, 205, 206, 210, 213, 240, 241
"Flip16", "Flip17", "Flip18", "Flip19", "Flip20", //247, 272, 273, 274, 292
"Sprint1", "Sprint2", "Sprint3", "Sprint4", "Sprint5", //56, 59, 122, 233, 249
"Death1", "Death2", "Death3", "Death4", "Death5", "Death6", "Death7", "Death8", //60, 63, 64, 65, 66, 218, 219, 234
"Hit1", "Hit2", "Hit3", "Hit4", "Hit5", "Hit6", "Hit7", "Hit8", "Hit9", //77, 154, 197, 198, 199, 200, 201, 202, 203
"Hit10", "Hit11", "Hit12", "Hit13", "Hit14", "Hit15", "Hit16", "Hit17", "Hit18", //207, 208, 209, 211, 212, 214, 215, 216, 242
"Hand1", "Hand2", "Hand3", "Hand4", "Hand5", "Hand6", "Hand7", "Hand8", "Hand9", //23, 51, 83(0), 83(1), 97, 99, 100, 101, 102
"Hand10", "Hand11", "Hand12", "Hand13", "Hand14", "Hand15", "Hand16", "Hand17", "Hand18",//108, 109, 110, 111, 112, 116, 123, 217, 255
"Leg1", "Leg2", "Leg3", "Leg4", "Leg5", "Leg6", "Leg7", "Leg8", //81, 82, 93, 94, 104, 105, 107, 113
"Leg9", "Leg10", "Leg11", "Leg12", "Leg13", //115, 117, 118, 119, 121
"Add weapon1", "Add weapon2", "Add weapon3", "Add weapon4", "Add weapon5", "Add weapon6", "Add weapon7", "Add weapon8", //124, 127, 128, 129, 130, 131, 136, 138
"Block1", "Block2", "Block3", "Block4", "Block5", "Block6", "Block7", "Block8", //180, 183, 185, 193, 194, 195, 191, 192
"Parry1", "Parry2", //184, 190
"Victory1", "Victory2", //306, 307
"Hacking1", "Hacking2", "Hacking3", //42, 43, 44
"Codec1", "Codec2", //33, 34
"Red eyes", "Get sword", "Lost sword", "Joke", "Wasd", "Monsoon kill", //189, 196, 321, 251, 288, 319
"MG Ray", "Spying" //67, 61
};
const char* cFunctionTypes[] = { "cdecl", "thiscall", "stdcall", "fastcall" };

void RtGui::mainWindow() 
{
	renderStyle(1);

	ImGui::Begin("RedTrainer", NULL, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::Text("MAIN MENU");

	if (ImGui::Button("STATS", ImVec2(150, 20)))
	{
		RedTrainer::playSound(0x12570E8);
		RtGui::hideSecondWindow();
		Base::Data::ShowMenu2 = !Base::Data::ShowMenu2;
	}

	if (ImGui::Button("ITEMS", ImVec2(150, 20)))
	{
		addWeaponNum = 0;
		RedTrainer::playSound(0x12570E8);
		RedTrainer::getWeaponsCount(addWeaponCount);
		RtGui::hideSecondWindow();
		Base::Data::ShowMenu3 = !Base::Data::ShowMenu3;
	}

	if (ImGui::Button("CUSTOMIZATION", ImVec2(150, 20)))
	{
		RedTrainer::playSound(0x12570E8);
		RtGui::hideSecondWindow();
		Base::Data::ShowMenu4 = !Base::Data::ShowMenu4;
	}

	if (ImGui::Button("MOVEMENT", ImVec2(150, 20)))
	{
		RedTrainer::playSound(0x12570E8);
		RtGui::hideSecondWindow();
		Base::Data::ShowMenu5 = !Base::Data::ShowMenu5;
	}

	if (ImGui::Button("MISSION", ImVec2(150, 20)))
	{
		RedTrainer::playSound(0x12570E8);
		RtGui::hideSecondWindow();
		Base::Data::ShowMenu6 = !Base::Data::ShowMenu6;
	}

	if (ImGui::Button("OTHER", ImVec2(150, 20)))
	{
		RedTrainer::playSound(0x12570E8);
		RtGui::hideSecondWindow();
		Base::Data::ShowMenu8 = !Base::Data::ShowMenu8;
	}

	if (ImGui::Button("ENEMY", ImVec2(150, 20)))
	{
		RedTrainer::playSound(0x12570E8);
		RtGui::hideSecondWindow();
		Base::Data::ShowMenu9 = !Base::Data::ShowMenu9;
	}

	if (bTestWindow) {
		if (ImGui::Button("TEST WINDOW", ImVec2(150, 20)))
		{
			RedTrainer::playSound(0x12570E8);
			RtGui::hideSecondWindow();
			Base::Data::ShowMenu7 = !Base::Data::ShowMenu7;
		}
	}

	ImGui::End();
	renderStyle(0);
}

void RtGui::statsWindow()
{
	renderStyle(1);

	ImGui::Begin("StatsWindow", NULL, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::Text("STATS");

	if (ImGui::Button("INVINCIBILITY", ImVec2(150, 20)))
		RedTrainer::setInvincibility(bInvicibility);
	RedTrainer::setText(bInvicibility);

	if (ImGui::Button("INFINITE ENERGY", ImVec2(150, 20)))
		RedTrainer::setInfinityEnergy(bEnergy);
	RedTrainer::setText(bEnergy);

	if (ImGui::Button("HEALTH", ImVec2(150, 20)))
		RedTrainer::setHealth(healthValue);
	ImGui::SameLine();
	ImGui::InputScalar(" ", ImGuiDataType_S32, &healthValue);

	if (ImGui::Button("MONEY", ImVec2(150, 20)))
		RedTrainer::setMoney(moneyValue);
	ImGui::SameLine();
	ImGui::InputScalar("  ", ImGuiDataType_S32, &moneyValue);

	if (ImGui::Button("GET ITEM", ImVec2(150, 20)))
		RedTrainer::setNewItem(cItemId);
	ImGui::SameLine();
	if (ImGui::BeginCombo("a", cNewItemType[cItemId], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cNewItemType); i++) {
			bool is_selected = (cItemId == i);
			if (ImGui::Selectable(cNewItemType[i], is_selected)) {
				cItemId = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::End();
	renderStyle(0);
}

void RtGui::itemsWindow()
{
	renderStyle(1);

	ImGui::Begin("ItemsWindow", NULL, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::Text("ITEMS");

	if (ImGui::Button("INFINITE UTILITY", ImVec2(150, 20)))
		RedTrainer::setInfinityAddWeapons(bInfAddWeapon);
	RedTrainer::setText(bInfAddWeapon);

	if (ImGui::Button("SET ITEM", ImVec2(150, 20)))
		RedTrainer::setAddWeapons(addWeaponNum, addWeaponValue, addWeaponCount);
	ImGui::SameLine();
	ImGui::Indent(155);
	if (addWeaponCount == 11) 
	{
		if (ImGui::BeginCombo("Raiden", cAddWeaponTypesR[addWeaponNum], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
		{
			for (int i = 0; i < IM_ARRAYSIZE(cAddWeaponTypesR); i++) {
				bool is_selected = (addWeaponNum == i);
				if (ImGui::Selectable(cAddWeaponTypesR[i], is_selected)) {
					addWeaponNum = i;
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else if (addWeaponCount == 9)
	{
		if (ImGui::BeginCombo("Sam", cAddWeaponTypesS[addWeaponNum], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
		{
			for (int i = 0; i < IM_ARRAYSIZE(cAddWeaponTypesS); i++) {
				bool is_selected = (addWeaponNum == i);
				if (ImGui::Selectable(cAddWeaponTypesS[i], is_selected)) {
					addWeaponNum = i;
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else if (addWeaponCount == 6)
	{
		if (ImGui::BeginCombo("BF", cAddWeaponTypesB[addWeaponNum], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
		{
			for (int i = 0; i < IM_ARRAYSIZE(cAddWeaponTypesB); i++) {
				bool is_selected = (addWeaponNum == i);
				if (ImGui::Selectable(cAddWeaponTypesB[i], is_selected)) {
					addWeaponNum = i;
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else
	{
		ImGui::Indent(5);
		ImGui::Text("Nothing");
		ImGui::Unindent(5);
	}
	ImGui::SameLine();
	ImGui::Indent(155);
	ImGui::InputScalar("  ", ImGuiDataType_S32, &addWeaponValue);
	ImGui::Unindent(310);

	if (ImGui::Button("BODY SHOP", ImVec2(150, 20)))
		RedTrainer::setBodyShop(bodyTypeShop, cBodyShop);
	ImGui::SameLine();
	ImGui::Indent(155);
	if (ImGui::BeginCombo("a", cBodyTypes[bodyTypeShop], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cBodyTypes) - 4; i++) {
			bool is_selected = (bodyTypeShop == i);
			if (ImGui::Selectable(cBodyTypes[i], is_selected)) {
				bodyTypeShop = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::Indent(155);
	if (ImGui::BeginCombo("1", cShopItemTypes[cBodyShop], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cShopItemTypes); i++) {
			bool is_selected = (cBodyShop == i);
			if (ImGui::Selectable(cShopItemTypes[i], is_selected)) {
				cBodyShop = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Unindent(310);

	if (ImGui::Button("SWORD SHOP", ImVec2(150, 20)))
		RedTrainer::setSwordShop(swordTypeShop, cSwordShop);
	ImGui::SameLine();
	ImGui::Indent(155);
	if (ImGui::BeginCombo("b", cSwordTypes[swordTypeShop], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cSwordTypes) - 5; i++) {
			bool is_selected = (swordTypeShop == i);
			if (ImGui::Selectable(cSwordTypes[i], is_selected)) {
				swordTypeShop = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::Indent(155);
	if (ImGui::BeginCombo("2", cShopItemTypes[cSwordShop], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cShopItemTypes); i++) {
			bool is_selected = (cSwordShop == i);
			if (ImGui::Selectable(cShopItemTypes[i], is_selected)) {
				cSwordShop = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Unindent(310);

	if (ImGui::Button("UNIQUE SHOP", ImVec2(150, 20)))
		RedTrainer::setUniqueShop(uniqueWeaponTypeShop, cUniqueWeaponShop);
	ImGui::SameLine();
	ImGui::Indent(155);
	if (ImGui::BeginCombo("c", cUniqueWeaponsTypes[uniqueWeaponTypeShop], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cUniqueWeaponsTypes) - 2; i++) {
			bool is_selected = (uniqueWeaponTypeShop == i + 1);
			if (ImGui::Selectable(cUniqueWeaponsTypes[i + 1], is_selected)) {
				uniqueWeaponTypeShop = i + 1;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::Indent(155);
	if (ImGui::BeginCombo("3", cShopItemTypes[cUniqueWeaponShop], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cShopItemTypes); i++) {
			bool is_selected = (cUniqueWeaponShop == i);
			if (ImGui::Selectable(cShopItemTypes[i], is_selected)) {
				cUniqueWeaponShop = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Unindent(310);

	if (ImGui::Button("WIG SHOP", ImVec2(150, 20)))
		RedTrainer::setWigShop(wigTypeShop, cWigShop);
	ImGui::SameLine();
	ImGui::Indent(155);
	if (ImGui::BeginCombo("d", cHairTypes[wigTypeShop], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cHairTypes) - 2; i++) {
			bool is_selected = (wigTypeShop == i + 1);
			if (ImGui::Selectable(cHairTypes[i + 1], is_selected)) {
				wigTypeShop = i + 1;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::Indent(155);
	if (ImGui::BeginCombo("4", cShopItemTypes[cWigShop], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cShopItemTypes); i++) {
			bool is_selected = (cWigShop == i);
			if (ImGui::Selectable(cShopItemTypes[i], is_selected)) {
				cWigShop = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Unindent(310);

	if (ImGui::Button("INDICATORS SHOP", ImVec2(150, 20)))
		RedTrainer::setLifeFuelShop(lifeFuelTypeShop, cLifeFuelShop);
	ImGui::SameLine();
	ImGui::Indent(155);
	if (ImGui::BeginCombo("e", cLifeFuelUpgrades[lifeFuelTypeShop], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cLifeFuelUpgrades); i++) {
			bool is_selected = (lifeFuelTypeShop == i);
			if (ImGui::Selectable(cLifeFuelUpgrades[i], is_selected)) {
				lifeFuelTypeShop = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::Indent(155);
	if (ImGui::BeginCombo("5", cShopItemTypes[cLifeFuelShop], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cShopItemTypes); i++) {
			bool is_selected = (cLifeFuelShop == i);
			if (ImGui::Selectable(cShopItemTypes[i], is_selected)) {
				cLifeFuelShop = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Unindent(310);

	if (ImGui::Button("SKILLS SHOP", ImVec2(150, 20)))
		RedTrainer::setSkillsShop(skillTypeShop, cSkillShop);
	ImGui::SameLine();
	ImGui::Indent(155);
	if (ImGui::BeginCombo("f", cSkillTypes[skillTypeShop], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cSkillTypes); i++) {
			bool is_selected = (skillTypeShop == i);
			if (ImGui::Selectable(cSkillTypes[i], is_selected)) {
				skillTypeShop = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::Indent(155);
	if (ImGui::BeginCombo("6", cShopItemTypes[cSkillShop], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cShopItemTypes); i++) {
			bool is_selected = (cSkillShop == i);
			if (ImGui::Selectable(cShopItemTypes[i], is_selected)) {
				cSkillShop = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Unindent(310);

	ImGui::End();
	renderStyle(0);
}

void RtGui::customizationWindow()
{
	renderStyle(1);

	ImGui::Begin("CustWindow", NULL, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::Text("CUSTOMIZATION");

	if (ImGui::Button("PLAYER TYPE", ImVec2(150, 20)))
		RedTrainer::setPlayerType(playerType);
	ImGui::SameLine();
	//ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(500, 20));
	if (ImGui::BeginCombo("  ", cPlayerTypes[playerType], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cPlayerTypes); i++) {
			bool is_selected = (playerType == i);
			if (ImGui::Selectable(cPlayerTypes[i], is_selected)) {
				playerType = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	
	
	if (ImGui::Button("ATTACK TYPE", ImVec2(150, 20)))
		RedTrainer::setAttackType(attackType);
	ImGui::SameLine();
	//ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(500, 20));
	if (ImGui::BeginCombo("1", cAttackTypes[attackType], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cAttackTypes); i++) {
			bool is_selected = (attackType == i);
			if (ImGui::Selectable(cAttackTypes[i], is_selected)) {
				attackType = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	

	if (ImGui::Button("PLAYER SWORD", ImVec2(150, 20)))
		RedTrainer::setPlayerSword(swordType);
	ImGui::SameLine();
	if (ImGui::BeginCombo("   ", cSwordTypes[swordType], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cSwordTypes); i++) {
			bool is_selected = (swordType == i);
			if (ImGui::Selectable(cSwordTypes[i], is_selected)) {
				swordType = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	if (ImGui::Button(" ", ImVec2(150, 20)))
		bTestWindow = !bTestWindow;

	if (ImGui::Button("PLAYER BODY", ImVec2(150, 20)))
		RedTrainer::setPlayerBody(bodyType);
	ImGui::SameLine();
	if (ImGui::BeginCombo("    ", cBodyTypes[bodyType], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cBodyTypes); i++) {
			bool is_selected = (bodyType == i);
			if (ImGui::Selectable(cBodyTypes[i], is_selected)) {
				bodyType = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("PLAYER HAIR", ImVec2(150, 20)))
		RedTrainer::setPlayerHair(hairType);
	ImGui::SameLine();
	if (ImGui::BeginCombo("     ", cHairTypes[hairType], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cHairTypes); i++) {
			bool is_selected = (hairType == i);
			if (ImGui::Selectable(cHairTypes[i], is_selected)) {
				hairType = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("INVISIBLE", ImVec2(150, 20)))
		RedTrainer::setInvisibility(bInvisible);
	RedTrainer::setText(bInvisible);

	ImGui::End();
	renderStyle(0);
}

void RtGui::movementWindow()
{
	renderStyle(1);

	ImGui::Begin("MoveWindow", NULL, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::Text("MOVEMENT");

	if (ImGui::Button("GAME SPEED", ImVec2(150, 20)))
		RedTrainer::setSpeed(speedValue);
	ImGui::SameLine();
	ImGui::InputScalar(" ", ImGuiDataType_Float, &speedValue);
	
	
	if (ImGui::Button("SET STRONG ATTACK", ImVec2(150, 20)))
		RedTrainer::setAnimFromAnimMapRaiden(animationID);
	ImGui::SameLine();
	ImGui::InputScalar("AnimMap", ImGuiDataType_S32, &animationID);


	if (ImGui::Button("FLY HACK", ImVec2(150, 20)))
		bFly = !bFly;
	RedTrainer::setText(bFly);

	if (ImGui::Button("ANIMATION", ImVec2(150, 20)))
		RedTrainer::setPlayerAnimation(animationIdSelectable, 0, 0, 0, 1);
	ImGui::SameLine();
	if (ImGui::BeginCombo("  ", cAnimationTypesRaiden[animationIdSelectable], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cAnimationTypesRaiden); i++) {
			bool is_selected = (animationIdSelectable == i);
			if (ImGui::Selectable(cAnimationTypesRaiden[i], is_selected)) {
				animationIdSelectable = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("LOST SWORD", ImVec2(150, 20)))
		RedTrainer::setWithoutSword(isHasSword);
	//RedTrainer::setText(isHasSword);

	ImGui::Text("ANIMATION");
	ImGui::SameLine();
	ImGui::Text("%d", playerAnimationValue); //%x to hex

	ImGui::End();
	renderStyle(0);
}

void RtGui::missionWindow()
{
	renderStyle(1);

	ImGui::Begin("MissionWindow", NULL, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::Text("MISSIONS");

	if (ImGui::Button("SET MISSION", ImVec2(150, 20)))
		RedTrainer::setMission(missionId, missionName);
	ImGui::InputText(" ", missionName, sizeof(missionName)); //Mission name
	ImGui::SameLine();
	ImGui::Indent(150);
	ImGui::InputScalar("  ", ImGuiDataType_U16, &missionId, NULL, NULL, "%X"); //MissionId
	ImGui::Unindent(150);
	
	if (ImGui::Button("SET DIFFICULTY", ImVec2(150, 20)))
		RedTrainer::setDifficulty(difficultyValue);
	ImGui::SameLine();
	if (ImGui::BeginCombo("        ", cDifficultyValues[difficultyValue], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cDifficultyValues); i++) {
			bool is_selected = (difficultyValue == i);
			if (ImGui::Selectable(cDifficultyValues[i], is_selected)) {
				difficultyValue = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("NO DAMAGE", ImVec2(150, 20)))
		RedTrainer::setNoDamage(bNoDamage);
	RedTrainer::setText(bNoDamage);

	if (ImGui::Button("NO KILLED", ImVec2(150, 20)))
		RedTrainer::setNoKilled(bNoKilled);
	RedTrainer::setText(bNoKilled);

	if (ImGui::Button("NO ALERT", ImVec2(150, 20)))
		RedTrainer::setNoAlert(bNoAlert);
	RedTrainer::setText(bNoAlert);

	if (ImGui::Button("BATTLE TIMER", ImVec2(150, 20)))
		RedTrainer::setBattleTimer(battleTimer);
	ImGui::SameLine();
	ImGui::InputScalar("   ", ImGuiDataType_Float, &battleTimer);

	if (ImGui::Button("BATTLEPOINTS", ImVec2(150, 20)))
		RedTrainer::setBattlePoints(battlePointsValue);
	ImGui::SameLine();
	ImGui::InputScalar("    ", ImGuiDataType_S32, &battlePointsValue);

	if (ImGui::Button("KILLS", ImVec2(150, 20)))
		RedTrainer::setKills(killsValue);
	ImGui::SameLine();
	ImGui::InputScalar("     ", ImGuiDataType_S32, &killsValue);

	if (ImGui::Button("ZANDZUTSU", ImVec2(150, 20)))
		RedTrainer::setZandzutsuKills(zandzutsuKillsValue);
	ImGui::SameLine();
	ImGui::InputScalar("      ", ImGuiDataType_S32, &zandzutsuKillsValue);

	if (ImGui::Button("MAX COMBO", ImVec2(150, 20)))
		RedTrainer::setMaxCombo(maxComboValue);
	ImGui::SameLine();
	ImGui::InputScalar("       ", ImGuiDataType_S32, &maxComboValue);

	ImGui::End();
	renderStyle(0);
}

void RtGui::testWindow() ///FOR TEST
{
	renderStyle(1);

	ImGui::Begin("RaidenFlagsWindow", NULL, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::Text("TEST");

	if (ImGui::Button("ANIMATION TEST", ImVec2(150, 20)))
		RedTrainer::setPlayerAnimation(animationId, animationTime, animationIdOld, animationTimeOld, 0);
	ImGui::InputScalar(" ", ImGuiDataType_U32, &animationId);
	ImGui::SameLine();
	ImGui::Indent(155);
	ImGui::InputScalar("  ", ImGuiDataType_U32, &animationTime);
	ImGui::Unindent(155);

	ImGui::InputScalar("   ", ImGuiDataType_U32, &animationIdOld);
	ImGui::SameLine();
	ImGui::Indent(155);
	ImGui::InputScalar("    ", ImGuiDataType_U32, &animationTimeOld);
	ImGui::Unindent(155);

	if (ImGui::Button("PRINT TEST", ImVec2(150, 20)))
		RedTrainer::printMessage(messageId, messageNum, messagePrint, messagePosition, messageChar);
	ImGui::InputScalar("     ", ImGuiDataType_U32, &messageId);
	ImGui::SameLine();
	ImGui::Indent(155);
	ImGui::InputScalar("      ", ImGuiDataType_S32, &messageNum); /// MSG Display Time
	ImGui::Unindent(155);

	ImGui::InputScalar("       ", ImGuiDataType_S32, &messagePrint);
	ImGui::SameLine();
	ImGui::Indent(155);
	ImGui::InputScalar("        ", ImGuiDataType_S32, &messagePosition);
	ImGui::Unindent(155);

	/*if (ImGui::Button("SET BGM", ImVec2(150, 20)))
		RedTrainer::setBgm(bgmPtr);
	ImGui::SameLine();
	ImGui::Indent(155);
	ImGui::InputScalar("         ", ImGuiDataType_U32, &bgmPtr, NULL, NULL, "%X");
	ImGui::Unindent(155);*/

	if (ImGui::Button("SET BGM STRING", ImVec2(150, 20)))
		RedTrainer::setBgm(bgmChar);
	ImGui::SameLine();
	ImGui::Indent(155);
	ImGui::InputText("          ", bgmChar, sizeof(bgmChar)); //Mission name
	ImGui::Unindent(155);

//	if (ImGui::Button("SP TEST", ImVec2(200, 20)))
//		RedTrainer::spawnEnemyA(enemyId, enemyTypeId, enemyFlagId);

	if (ImGui::Button("SP TEST W", ImVec2(200, 20)))
		RedTrainer::spawnEnemy(enemyId, enemyTypeId, enemyFlagId);

	if (ImGui::Button("SET STRUCT", ImVec2(200, 20)))
		RedTrainer::setEnemyIdToStruct(enemyId, enemyTypeId);

//	if (ImGui::Button("SET STRUCT2", ImVec2(200, 20)))
//		RedTrainer::setEnemyIdToStruct(enemyId, 0xA9EB50);

	ImGui::Text("ENEMY ID");
	ImGui::SameLine();
	ImGui::Indent(150);
	ImGui::InputScalar("           ", ImGuiDataType_U32, &enemyId, NULL, NULL, "%X");
	ImGui::Unindent(150);

	if (ImGui::Button("CALL FUNCTION", ImVec2(150, 20)))
		RedTrainer::callGameFunction(callFunctionAddress, funcType, numberOfFunctionArgs, functionArgumentA, functionArgumentB, functionArgumentC, functionArgumentD, functionArgumentE, functionArgumentF);
	ImGui::SameLine();
	ImGui::Indent(150);
	ImGui::InputScalar("funcAddress", ImGuiDataType_U32, &callFunctionAddress, NULL, NULL, "%X");
	ImGui::Unindent(150);

	if (ImGui::BeginCombo("FuncType", cFunctionTypes[funcType], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (char i = 0; i < IM_ARRAYSIZE(cFunctionTypes); i++) {
			bool is_selected = (funcType == i);
			if (ImGui::Selectable(cFunctionTypes[i], is_selected)) {
				funcType = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::Indent(150);
	ImGui::InputScalar("numberOfArgs", ImGuiDataType_U8, &numberOfFunctionArgs, NULL, NULL, "%X");
	ImGui::Unindent(150);

	ImGui::InputScalar("arg1", ImGuiDataType_U32, &functionArgumentA, NULL, NULL, "%X");
	ImGui::SameLine();
	ImGui::Indent(150);
	ImGui::InputScalar("arg2", ImGuiDataType_U32, &functionArgumentB, NULL, NULL, "%X");
	ImGui::Unindent(150);

	ImGui::InputScalar("arg3", ImGuiDataType_U32, &functionArgumentC, NULL, NULL, "%X");
	ImGui::SameLine();
	ImGui::Indent(150);
	ImGui::InputScalar("arg4", ImGuiDataType_U32, &functionArgumentD, NULL, NULL, "%X");
	ImGui::Unindent(150);

	ImGui::InputScalar("arg5", ImGuiDataType_U32, &functionArgumentE, NULL, NULL, "%X");
	ImGui::SameLine();
	ImGui::Indent(150);
	ImGui::InputScalar("arg6", ImGuiDataType_U32, &functionArgumentF, NULL, NULL, "%X");
	ImGui::Unindent(150);

//	if (ImGui::Button("TEST FUNC", ImVec2(200, 20)))
//		RedTrainer::playerImplement(0);

	ImGui::End();
	renderStyle(0);
}

void RtGui::otherWindow() 
{
	renderStyle(1);

	ImGui::Begin("OtherWindow", NULL, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::Text("OTHER");

	//if (ImGui::Button("CREATE ENEMY?", ImVec2(150, 20)))
		//RedTrainer::spawnEnemy(0x20140, 0, 0, 0);

	if (ImGui::Button("RED FILTER", ImVec2(150, 20)))
		RedTrainer::setFilter(rFilter, 0);
	ImGui::SameLine();
	ImGui::InputScalar(" ", ImGuiDataType_Float, &rFilter);

	if (ImGui::Button("GREEN FILTER", ImVec2(150, 20)))
		RedTrainer::setFilter(gFilter, 4);
	ImGui::SameLine();
	ImGui::InputScalar("  ", ImGuiDataType_Float, &gFilter);

	if (ImGui::Button("BLUE FILTER", ImVec2(150, 20)))
		RedTrainer::setFilter(bFilter, 8);
	ImGui::SameLine();
	ImGui::InputScalar("   ", ImGuiDataType_Float, &bFilter);

	if (ImGui::Button("PLAYER SIZE X", ImVec2(150, 20)))
		RedTrainer::setSize(xSize, 0);
	ImGui::SameLine();
	ImGui::InputScalar("    ", ImGuiDataType_Float, &xSize);

	if (ImGui::Button("PLAYER SIZE Y", ImVec2(150, 20)))
		RedTrainer::setSize(ySize, 4);
	ImGui::SameLine();
	ImGui::InputScalar("     ", ImGuiDataType_Float, &ySize);

	if (ImGui::Button("PLAYER SIZE Z", ImVec2(150, 20)))
		RedTrainer::setSize(zSize, 8);
	ImGui::SameLine();
	ImGui::InputScalar("      ", ImGuiDataType_Float, &zSize);

	if (ImGui::Button("SET MENU", ImVec2(150, 20)))
		RedTrainer::setMenuType(cMenuType);
	ImGui::SameLine();
	if (ImGui::BeginCombo("       ", cMenuTypes[cMenuType], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cMenuTypes); i++) {
			bool is_selected = (cMenuType == i);
			if (ImGui::Selectable(cMenuTypes[i], is_selected)) {
				cMenuType = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("SET FLAG", ImVec2(150, 20)))
		RedTrainer::setFlag(cRaidenType);
	ImGui::SameLine();
	if (ImGui::BeginCombo("        ", cRaidenFlags[cRaidenType], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cRaidenFlags); i++) {
			bool is_selected = (cRaidenType == i);
			if (ImGui::Selectable(cRaidenFlags[i], is_selected)) {
				cRaidenType = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("RENDER", ImVec2(150, 20)))
		RedTrainer::setRender(renderType);
	ImGui::SameLine();
	ImGui::InputScalar("         ", ImGuiDataType_U32, &renderType, NULL, NULL, "%X");

	ImGui::End();
	renderStyle(0);
}

void RtGui::enemyWindow()
{
	renderStyle(1);

	ImGui::Begin("EnemyWindow", NULL, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("ENEMY");

	if (ImGui::Button("NO DAMAGE TYPE", ImVec2(150, 20)))
		RedTrainer::enemyNoDamageTo(enemyDamageType);
	ImGui::SameLine();
	ImGui::InputScalar("      ", ImGuiDataType_S32, &enemyDamageType);

	if (ImGui::Button("SET ALL ENEMIES", ImVec2(200, 20)))
		RedTrainer::setAllEnemies(0x1, enemyId, enemyTypeId, enemySetTypeId, enemyFlagId);

	if (ImGui::Button("SPAWN", ImVec2(200, 20)))
	{
		if ((enemyId != previousEnemyId || enemyTypeId != previousTypeId) && !toSpawn)
		{
			RedTrainer::setEnemyIdToStruct(enemyId, enemyTypeId);
			RtGui::spawnTimer = std::chrono::high_resolution_clock::now();
		}

		RtGui::toSpawn = true;
	}

	ImGui::Text("ENEMY ID");
	ImGui::SameLine();
	ImGui::Indent(150);
	ImGui::InputScalar(" ", ImGuiDataType_U32, &enemyId, NULL, NULL, "%X");
	ImGui::Unindent(150);

	ImGui::Text("ENEMY TYPE");
	ImGui::SameLine();
	ImGui::Indent(150);
	ImGui::InputScalar("  ", ImGuiDataType_U32, &enemyTypeId, NULL, NULL, "%X");
	ImGui::Unindent(150);

	/*
	ImGui::Text("ENEMY STYPE");
	ImGui::SameLine();
	ImGui::Indent(150);
	ImGui::InputScalar("   ", ImGuiDataType_U32, &enemySetTypeId, NULL, NULL, "%X");
	ImGui::Unindent(150);
	*/

	ImGui::Text("ENEMY FLAG");
	ImGui::SameLine();
	ImGui::Indent(150);
	ImGui::InputScalar("    ", ImGuiDataType_U32, &enemyFlagId, NULL, NULL, "%X");
	ImGui::Unindent(150);

	if (ImGui::Button("RESET", ImVec2(200, 20)))
		RedTrainer::setAllEnemies(0);

	ImGui::End();
	renderStyle(0);
}

void RtGui::hideSecondWindow()
{
	Base::Data::ShowMenu2 = false;
	Base::Data::ShowMenu3 = false;
	Base::Data::ShowMenu4 = false;
	Base::Data::ShowMenu5 = false;
	Base::Data::ShowMenu6 = false;
	Base::Data::ShowMenu7 = false;
	Base::Data::ShowMenu8 = false;
	Base::Data::ShowMenu9 = false;
}

void RtGui::renderStyle(bool isPush)
{
	if (isPush)
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
		//ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.83f, 1.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.90f, 1.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 1.0f, 1.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.5f, 0.83f, 1.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_NavHighlight, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	}
	else
	{
		ImGui::PopStyleColor(7);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	}
}
