#include "gui.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <Hw.h>
#include <common.h>
#include <Events.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK hkWindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam) > 0)
		return 1L;
	return ::CallWindowProcA(gui::oWindProc, hwnd, uMsg, wParam, lParam);
}

void gui::Init()
{
	Events::OnGameStartupEvent += []()
		{
			if (!bInit)
			{
				oWindProc = (WNDPROC)::SetWindowLongPtr((HWND)Hw::OSWindow, GWLP_WNDPROC, (LONG)hkWindowProc);

				ImGui::CreateContext();
				ImGui_ImplWin32_Init(Hw::OSWindow);
				ImGui_ImplDX9_Init(Hw::GraphicDevice);

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

				bInit = true;
			}
		};
	
	Events::OnEndScene += OnEndScene;
}

void gui::RenderWindow()
{

	if (bShowMain)
	{
		ImGui::Begin("RedTrainer", NULL, ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_AlwaysAutoResize);
		
		ImGui::Text("MAIN MENU");

		if (ImGui::Button("STATS", ImVec2(150, 20)))
		{
			Core_PlaySound("core_se_sys_decide_l", 0);
			bShowWindows[0] ^= true;
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
	}
}

void gui::OnEndScene()
{
	if (!bInit)
		return;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	gui::RenderWindow();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void gui::Shutdown()
{

}