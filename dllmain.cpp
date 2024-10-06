#include "pch.h"
#include <assert.h>
#include "gui.h"
#include <Events.h>
#include <common.h>
#include "RTFunctions/functions.h"

#include "imgui/imgui.h"

class Plugin
{
public:
	static inline void InitGUI()
	{
		Events::OnDeviceReset.before += gui::OnReset::Before;
		Events::OnDeviceReset.after += gui::OnReset::After;
		Events::OnPresent += gui::OnEndScene; 
		/* // Or if you want to switch it to Present
		Events::OnPresent += gui::OnEndScene;
		*/
	}

	Plugin()
	{
		InitGUI();

		Events::OnTickEvent += RedTrainer::setFly;

		// and here's your code
	}
} plugin;

void menuClick()
{
	Core_PlaySound("core_se_sys_decide_l", 0);
}

void gui::RenderWindow()
{
	if (shared::IsKeyPressed(VK_ADD, false) || shared::IsKeyPressed(VK_F1, false))
		bShowMain ^= true;

	if (bShowMain)
	{
		ImGui::Begin("RedTrainer", NULL, ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_AlwaysAutoResize);

		auto mainPos = ImGui::GetWindowPos();

		ImGui::Text("MAIN MENU");

		if (ImGui::Button("STATS", ImVec2(150, 20)))
		{
			menuClick();
			HideAllWindows(&windows[0]);
			windows[0].vecWindowPos = { ImGui::GetWindowWidth() + mainPos.x, ImGui::GetMousePos().y};
			windows[0].bShow ^= true;
		}

		if (ImGui::Button("ITEMS", ImVec2(150, 20)))
		{
			menuClick();
			HideAllWindows(&windows[1]);
			windows[1].vecWindowPos = { ImGui::GetWindowWidth() + mainPos.x, ImGui::GetMousePos().y };
			windows[1].bShow ^= true;
		}

		if (ImGui::Button("CUSTOMIZATION", ImVec2(150, 20)))
		{
			menuClick();
			HideAllWindows(&windows[2]);
			windows[2].vecWindowPos = { ImGui::GetWindowWidth() + mainPos.x, ImGui::GetMousePos().y };
			windows[2].bShow ^= true;
		}

		if (ImGui::Button("MOVEMENT", ImVec2(150, 20)))
		{
			menuClick();
			HideAllWindows(&windows[3]);
			windows[3].vecWindowPos = { ImGui::GetWindowWidth() + mainPos.x, ImGui::GetMousePos().y };
			windows[3].bShow ^= true;
		}

		if (ImGui::Button("MISSION", ImVec2(150, 20)))
		{
			menuClick();
			HideAllWindows();
			windows[4].bShow ^= true;
		}

		if (ImGui::Button("OTHER", ImVec2(150, 20)))
		{
			menuClick();
			HideAllWindows();
			windows[5].bShow ^= true;
		}

		if (ImGui::Button("ENEMY", ImVec2(150, 20)))
		{
			menuClick();
			HideAllWindows();
			windows[6].bShow ^= true;
		}

		if (bTestWindow) {
			if (ImGui::Button("TEST WINDOW", ImVec2(150, 20)))
			{
				menuClick();
				HideAllWindows();
				windows[7].bShow ^= true;
			}
		}

		ImGui::End();

		for (auto& window : windows)
			window.Draw();
	}
}