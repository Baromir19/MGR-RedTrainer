#include "pch.h"
#include "base.h"
#include "trainerGui.h"
#include "trainerFunctions/trainerFunctions.h"

bool bInvicibility = false,
bEnergy = false,
RtGui::bFly = false;

int healthValue = 1600,
moneyValue = 0,
playerType = 5,
swordType = 12,
bodyType = 16,
hairType = 4;

float speedValue = 0.0f;

const char* cPlayerTypes[] = { "Raiden", "First Raiden", "Camera mode", "Sam", "BladeWolf", "Disabled" };
const char* cSwordTypes[] = { "HF Blade", "Stun Blade", "Armor Breaker", "Long Sword", "Wooden Sword", "Murasama",
							"Fox Blade", "HF Machete", "First Blade", "Sam's Murasama", "Chainsaw", "Invisible", "Disabled" };
const char* cBodyTypes[] = { "Default", "Blue Body", "Red Body", "Yellow Body", "Desperado", "Suit", "Mariachi",
							"Original Body", "MGS4", "Gray Fox", "White Armor", "Inferno Armor", "Commando Armor", "Raiden", "First Body", "Sam :)", "Disabled" };
const char* cHairTypes[] = { "Default", "Wig A", "Wig B", "Wig C", "Disabled" };

void RtGui::mainWindow() 
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
	ImGui::Begin("RedTrainer", NULL, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.83f, 1.0f, 0.3f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.90f, 1.0f, 0.4f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

	ImGui::Text("MAIN MENU");

	if (ImGui::Button("STATS", ImVec2(150, 20)))
		Base::Data::ShowMenu2 = !Base::Data::ShowMenu2;
	if (ImGui::Button("CUSTOMIZATION", ImVec2(150, 20)))
		Base::Data::ShowMenu3 = !Base::Data::ShowMenu3;
	if (ImGui::Button("MOVEMENT", ImVec2(150, 20)))
		Base::Data::ShowMenu4 = !Base::Data::ShowMenu4;

	ImGui::PopStyleColor(5);
	ImGui::PopStyleVar();
	ImGui::End();
	ImGui::PopStyleColor();
}

void RtGui::statsWindow()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
	ImGui::Begin("StatsWindow", NULL, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.83f, 1.0f, 0.3f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.90f, 1.0f, 0.4f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

	ImGui::Text("STATS");

	if (ImGui::Button("INVINCIBILITY", ImVec2(150, 20)))
		RedTrainer::setInvincibility(bInvicibility);
	RedTrainer::setText(bInvicibility);

	if (ImGui::Button("INFINITY ENERGY", ImVec2(150, 20)))
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

	ImGui::PopStyleColor(5);
	ImGui::PopStyleVar();
	ImGui::End();
	ImGui::PopStyleColor();
}

void RtGui::customizationWindow()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
	ImGui::Begin("CustWindow", NULL, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.83f, 1.0f, 0.3f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.90f, 1.0f, 0.4f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.5f, 0.83f, 1.0f, 0.3f));

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

	ImGui::PopStyleColor(6);
	ImGui::PopStyleVar();
	ImGui::End();
	ImGui::PopStyleColor();
}

void RtGui::movementWindow()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
	ImGui::Begin("MoveWindow", NULL, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.83f, 1.0f, 0.3f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.90f, 1.0f, 0.4f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.5f, 0.83f, 1.0f, 0.3f));

	ImGui::Text("MOVEMENT");

	if (ImGui::Button("GAME SPEED", ImVec2(150, 20)))
		RedTrainer::setSpeed(speedValue);
	ImGui::SameLine();
	ImGui::InputScalar("      ", ImGuiDataType_Float, &speedValue);

	if (ImGui::Button("FLY HACK", ImVec2(150, 20)))
		bFly = !bFly;
	RedTrainer::setText(bFly);

	ImGui::PopStyleColor(6);
	ImGui::PopStyleVar();
	ImGui::End();
	ImGui::PopStyleColor();
}

void RtGui::missionWindow()
{

}

void RtGui::otherWindow() 
{

}

