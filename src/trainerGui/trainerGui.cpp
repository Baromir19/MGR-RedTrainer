#include "pch.h"
#include "base.h"
#include "trainerGui.h"
#include "trainerFunctions/trainerFunctions.h"

bool bInvicibility   = false,
	 bEnergy		 = false,
	 bNoDamage		 = false,
	 bNoKilled		 = false,
	 bNoAlert		 = false,
	 bInfAddWeapon   = false,
	 RtGui::bFly	 = false;

char missionName[0x20] = "Subphase",
	 difficultyValue = 0,
	 cRaidenType = 0,
	 cMenuType = 0,
	 addWeaponCount = 0,
	 addWeaponNum = 0;

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
	swordType = 12,
	bodyType = 16,
	hairType = 4,
	battlePointsValue = 0,
	maxComboValue = 0,
	killsValue 	= 0,
	zandzutsuKillsValue = 0,
	addWeaponValue = 0;

unsigned int enemyId = 0x00020140,
			 enemyTypeId = 0x00000000,
			 enemySetTypeId = 0x00000000,
			 enemyFlagId = 0x00000000,
			 renderType = 0x00000000;

float speedValue  = 0.0f,
	  battleTimer = 0.0f,
	  rFilter	  = 1.0f,
	  gFilter	  = 1.0f, 
	  bFilter	  = 1.0f,
	  xSize		  = 1.0f,
	  ySize		  = 1.0f, 
	  zSize		  = 1.0f;

const char* cPlayerTypes[] = { "Raiden", "First Raiden", "Camera mode", "Sam", "BladeWolf", "Disabled" };
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
		RtGui::hideSecondWindow();
		Base::Data::ShowMenu2 = !Base::Data::ShowMenu2;
	}

	if (ImGui::Button("ITEMS", ImVec2(150, 20)))
	{
		addWeaponNum = 0;
		RedTrainer::getWeaponsCount(addWeaponCount);
		RtGui::hideSecondWindow();
		Base::Data::ShowMenu3 = !Base::Data::ShowMenu3;
	}

	if (ImGui::Button("CUSTOMIZATION", ImVec2(150, 20)))
	{
		RtGui::hideSecondWindow();
		Base::Data::ShowMenu4 = !Base::Data::ShowMenu4;
	}

	if (ImGui::Button("MOVEMENT", ImVec2(150, 20)))
	{
		RtGui::hideSecondWindow();
		Base::Data::ShowMenu5 = !Base::Data::ShowMenu5;
	}

	if (ImGui::Button("MISSION", ImVec2(150, 20)))
	{
		RtGui::hideSecondWindow();
		Base::Data::ShowMenu6 = !Base::Data::ShowMenu6;
	}

	if (ImGui::Button("RAIDEN FLAGS", ImVec2(150, 20)))
	{
		RtGui::hideSecondWindow();
		Base::Data::ShowMenu7 = !Base::Data::ShowMenu7;
	}

	if (ImGui::Button("OTHER", ImVec2(150, 20)))
	{
		RtGui::hideSecondWindow();
		Base::Data::ShowMenu8 = !Base::Data::ShowMenu8;
	}

	if (ImGui::Button("ENEMY", ImVec2(150, 20)))
	{
		RtGui::hideSecondWindow();
		Base::Data::ShowMenu9 = !Base::Data::ShowMenu9;
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
	ImGui::InputScalar("      ", ImGuiDataType_S32, &moneyValue);

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
	ImGui::InputScalar("      ", ImGuiDataType_Float, &speedValue);

	if (ImGui::Button("FLY HACK", ImVec2(150, 20)))
		bFly = !bFly;
	RedTrainer::setText(bFly);

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

void RtGui::raidenFlagsWindow()
{
	renderStyle(1);

	ImGui::Begin("RaidenFlagsWindow", NULL, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::Text("RAIDEN FLAGS");

	if (ImGui::Button("SET FLAG", ImVec2(150, 20)))
		RedTrainer::setFlag(cRaidenType);
	ImGui::SameLine();
	if (ImGui::BeginCombo(" ", cRaidenFlags[cRaidenType], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular))
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
	ImGui::InputScalar("  ", ImGuiDataType_U32, &renderType, NULL, NULL, "%X");

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

	if (ImGui::Button("SET ALL ENEMIES", ImVec2(200, 20)))
		RedTrainer::setAllEnemies(1, enemyId, enemyTypeId, enemySetTypeId, enemyFlagId);

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
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.83f, 1.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.90f, 1.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 1.0f, 1.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.5f, 0.83f, 1.0f, 0.3f));
	}
	else
	{
		ImGui::PopStyleColor(6);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	}
}