#include <pch.h>
#include <base.h>
#include <iostream>
#include "trainerFunctions/trainerFunctions.h"
#include "trainerGui/trainerGui.h"
#include "../assets/fonts.h"

bool isKeyPressed = false,
	 isKeyPressed2 = false;
int currentButtonIndex = 0;

HRESULT __stdcall Base::Hooks::EndScene(LPDIRECT3DDEVICE9 pDevice)
{
	Data::pDxDevice9 = pDevice;
	if (!Data::InitImGui)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange | ImGuiConfigFlags_NavEnableKeyboard;
		ImFontConfig fontConfig;
		fontConfig.FontDataOwnedByAtlas = false;
		fontConfig.GlyphExtraSpacing.x = -0.5f;
		io.Fonts->AddFontFromMemoryTTF((void*)mainBoldData, sizeof(mainBoldData), 11.0f, &fontConfig);
		ImGui_ImplWin32_Init(Data::hWindow);
		ImGui_ImplDX9_Init(pDevice);
		Data::InitImGui = true;
	}

	if (!Data::InitImGui) return Data::oEndScene(pDevice);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//ImGui::SetNextWindowPos(ImVec2(0, 0));

	if (GetAsyncKeyState(VK_ADD) & 0x8000)
	{
		isKeyPressed = true;
	}
	else if (!(GetAsyncKeyState(VK_ADD) & 0x8000) && isKeyPressed)
	{
		Data::ShowMenu = !Data::ShowMenu;
		isKeyPressed = false;
	}

	if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000)
	{
		isKeyPressed2 = true;
	}
	else if (!(GetAsyncKeyState(VK_SUBTRACT) & 0x8000) && isKeyPressed2)
	{
		Data::ShowMenu2 = false;
		Data::ShowMenu3 = false;
		isKeyPressed2 = false;
	}

	if (Data::ShowMenu)
	{
		RtGui::mainWindow();
	}

	ImGui::SetNextWindowPos(ImVec2( ImGui::GetIO().DisplaySize.x - 350, 0 ));

	if (Data::ShowMenu2)
	{
		RtGui::statsWindow();
	}

	if (Data::ShowMenu3)
	{
		RtGui::customizationWindow();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	if(Data::ToDetach)
		Base::Detach();
	return Data::oEndScene(pDevice);
}