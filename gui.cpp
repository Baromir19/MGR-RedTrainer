#include "gui.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

#include <Hw.h>
#include <cItemPossessionBase.h>
#include <cItemPossessionWeaponGun.h>
#include <cGameUIManager.h>

#include "assets/fonts.h"

#include "RTFunctions/functions.h"

static WNDPROC oWndProc = NULL;

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
	return ::CallWindowProcA(oWndProc, hwnd, uMsg, wParam, lParam);
}

void gui::OnReset::Before()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void gui::OnReset::After()
{
	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::OnEndScene()
{
	static bool init = false;

	if (!init)
	{
		oWndProc = (WNDPROC)::SetWindowLongPtr(Hw::OSWindow, GWLP_WNDPROC, (LONG)hkWindowProc);

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Hw::OSWindow);
		ImGui_ImplDX9_Init(Hw::GraphicDevice);

		gui::LoadStyle();

		setGUIWindows();

		init = true;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	gui::RenderWindow();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void gui::LoadStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange | ImGuiConfigFlags_NavEnableKeyboard;
	ImFontConfig fontConfig;
	fontConfig.FontDataOwnedByAtlas = false;
	fontConfig.GlyphExtraSpacing.x = -0.5f;
	io.Fonts->AddFontFromMemoryTTF((void*)mainBoldData, sizeof(mainBoldData), 11.0f, &fontConfig);

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

void gui::setGUIWindows()
{
	windows.push_back({[](bWindowStruct* window)
		{
			ImGui::Begin("StatsWindow", NULL, ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_AlwaysAutoResize);
			//ImGui::GetStyle().WindowRounding = 0.0f;

			ImGui::SetWindowPos(ImVec2(window->vecWindowPos.x, window->vecWindowPos.y), ImGuiCond_Always);

			ImGui::Text("STATS");

			if (ImGui::Button("INVINCIBILITY", ImVec2(150, 20)))
				RedTrainer::setInvincibility(RedTrainer::bInvincibility);
			RedTrainer::setText(RedTrainer::bInvincibility);

			if (ImGui::Button("INFINITE ENERGY", ImVec2(150, 20)))
				RedTrainer::setInfinityEnergy(RedTrainer::bEnergy);
			RedTrainer::setText(RedTrainer::bEnergy);

			{
				static int healthValue = 0;

				if (ImGui::Button("HEALTH", ImVec2(150, 20)))
					RedTrainer::setHealth(healthValue);
				ImGui::SameLine();
				ImGui::InputScalar(" ", ImGuiDataType_S32, &healthValue);
			}

			{
				static int moneyValue = 0;

				if (ImGui::Button("MONEY", ImVec2(150, 20)))
					RedTrainer::setMoney(moneyValue);
				ImGui::SameLine();
				ImGui::InputScalar("  ", ImGuiDataType_S32, &moneyValue);
			}

			ImGui::End();
		} });
	windows.push_back({ [](bWindowStruct* window)
		{
			ImGui::Begin("ItemsWindow", NULL, ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_AlwaysAutoResize);
			//ImGui::GetStyle().WindowRounding = 0.0f;

			ImGui::SetWindowPos(ImVec2(window->vecWindowPos.x, window->vecWindowPos.y), ImGuiCond_Always);

			ImGui::Text("ITEMS");

			if (ImGui::Button("INFINITE UTILITY", ImVec2(150, 20)))
				RedTrainer::setInfinityAddWeapons(RedTrainer::bInfAddWeapon);
			RedTrainer::setText(RedTrainer::bInfAddWeapon);

			auto asShopSelector = [](int *type, int* unlockType, const char* label, const char* itemsSeparatedByZeros) -> void
				{
					ImGui::PushID(label);
					ImGui::Combo("##1", type, itemsSeparatedByZeros);
					ImGui::SameLine();
					ImGui::Indent(180);
					ImGui::Combo("##2", unlockType, [](void* data, int idx, const char** out_text) -> bool
						{
							if (idx >= 4)
							{
								*out_text = "Selected\000";
								return true;
							}
							else
							{
								const char* items_separated_by_zeros = (const char*)data;
								int items_count = 0;
								const char* p = items_separated_by_zeros;
								while (*p)
								{
									if (idx == items_count)
										break;
									p += strlen(p) + 1;
									items_count++;
								}
								if (!*p)
									return false;
								if (out_text)
									*out_text = p;
								return true;
							}

						}, (void*)"Hidden\000New\000To buy\000Buyed\000Selected\000", 5);
					ImGui::Unindent(180);
					ImGui::PopID();
				};

			{
				static int addWeaponNum = 0;

				char weaponNum[256];

				lib::StaticArray<cItemPossessionBase*, 64> &items = *(lib::StaticArray<cItemPossessionBase*, 64>*)(shared::base + 0x1486EA0);

				weaponNum[0] = '\0';

				char* p = &weaponNum[0];

				for (auto& item : items)
				{
					strcpy(p, item->m_ItemName);
					p += strlen(item->m_ItemName) + 1;
				}

				*p = '\0';
				
				switch (*(int*)(shared::base + 0x17EA030))
				{
				case 0: // Raiden
					ImGui::Combo("##ItemSetSelector", &addWeaponNum, weaponNum);
					break;
				case 8: // Sam
					ImGui::Combo("##ItemSetSelector", &addWeaponNum, weaponNum);
					break;
				case 9: // Bladewolf
					ImGui::Combo("##ItemSetSelector", &addWeaponNum, weaponNum);
					break;
				default:
					ImGui::Indent(5);
					ImGui::Text("Nothing");
					ImGui::Unindent(5);
					break;
				}
				ImGui::SameLine();
				ImGui::Indent(180);
				auto item = items.m_pBegin[addWeaponNum];
				bool isGun = strcmp(item->getClassName(), "cItemPossessionWeaponGun") == 0;
				auto& possession = isGun ? ((cItemPossessionWeaponGun*)item)->m_nGunPossession : item->m_nBasePossession;
				// auto& maxPossession = isGun ? ((cItemPossessionWeaponGun*)item)->m_nGunMaxPossession : item->m_nBaseMaxPossession;
				ImGui::InputScalar("  ", ImGuiDataType_S32, &possession);
				ImGui::Unindent(360);
			}

			ImGui::Indent(180);

			{
				static int bodyTypeShop = 0;
				static int cBodyShop = 0;

				if (ImGui::Button("BODY SHOP", ImVec2(150, 20)))
					RedTrainer::setBodyShop(bodyTypeShop, cBodyShop);
				ImGui::SameLine();
				ImGui::Indent(180);
				asShopSelector(&bodyTypeShop, (int*)(shared::base + 0x1773860 + bodyTypeShop * 0x20), "##BodySelector", "Default\000Blue Body\000Red Body\000Yellow Body\000Desperado\000Suit\000Mariachi\000Original Body\000MGS4\000Gray Fox\000White Armor\000Inferno Armor\000Commando Armor\000Raiden\000First Body\000Sam :)\000");
				cBodyShop = *(int*)(shared::base + 0x1773860 + bodyTypeShop * 0x20);
				ImGui::Unindent(180);
			}

			{
				static int swordTypeShop = 0;
				static int cSwordShop = 0;

				if (ImGui::Button("SWORD SHOP", ImVec2(150, 20)))
					RedTrainer::setSwordShop(swordTypeShop, cSwordShop);
				ImGui::SameLine();
				ImGui::Indent(180);
				asShopSelector(&swordTypeShop, (int*)(shared::base + 0x1773A00 + swordTypeShop * 0x20), "##SwordSelector", "HF Blade\000Stun Blade\000Armor Breaker\000Long Sword\000Wooden Sword\000Murasama\000Fox Blade\000HF Machete\000First Blade\000Sam's Murasama\000Chainsaw\000Invisible\000");
				cSwordShop = *(int*)(shared::base + 0x1773A00 + swordTypeShop * 0x20);
				ImGui::Unindent(180);
			}

			{
				static int uniqueWeaponTypeShop = 0;
				static int cUniqueWeaponShop = 0;

				if (ImGui::Button("UNIQUE SHOP", ImVec2(150, 20)))
					RedTrainer::setUniqueShop(uniqueWeaponTypeShop, cUniqueWeaponShop);
				ImGui::SameLine();
				ImGui::Indent(180);
				asShopSelector(&uniqueWeaponTypeShop, (int*)(shared::base + 0x1773B20 + uniqueWeaponTypeShop * 0x20), "##UniqueWeaponSelector", "Pole-Arm\000Sai\000Pincer Blades\000");
				cUniqueWeaponShop = *(int*)(shared::base + 0x1773B20 + uniqueWeaponTypeShop * 0x20);
				ImGui::Unindent(180);
			}

			{
				static int wigTypeShop = 0;
				static int cWigShop = 0;

				if (ImGui::Button("WIG SHOP", ImVec2(150, 20)))
					RedTrainer::setWigShop(wigTypeShop, cWigShop);
				ImGui::SameLine();
				ImGui::Indent(180);
				asShopSelector(&wigTypeShop, (int*)(shared::base + 0x1773B80 + wigTypeShop * 0x20), "##WigSelector", "Wig A\000Wig B\000Wig C\000");
				cWigShop = *(int*)(shared::base + 0x1773B80 + wigTypeShop * 0x20);
				ImGui::Unindent(180);
			}

			{
				static int lifeFuelTypeShop = 0;
				static int cLifeFuelShop = 0;

				if (ImGui::Button("INDICATORS SHOP", ImVec2(150, 20)))
					RedTrainer::setLifeFuelShop(lifeFuelTypeShop, cLifeFuelShop);
				ImGui::SameLine();
				ImGui::Indent(180);
				asShopSelector(&lifeFuelTypeShop, (int*)(shared::base + 0x1773BE0 + lifeFuelTypeShop * 0x20), "##LifeFuelSelector", "Life 1\000Life 2\000Life 3\000Life 4\000Fuel 1\000Fuel 2\000Fuel 3\000Fuel 4\000Fuel 5\000");
				cLifeFuelShop = *(int*)(shared::base + 0x1773BE0 + lifeFuelTypeShop * 0x20);
				ImGui::Unindent(180);
			}

			{
				static int skillTypeShop = 0;
				static int cSkillShop = 0;

				if (ImGui::Button("SKILLS SHOP", ImVec2(150, 20)))
					RedTrainer::setSkillsShop(skillTypeShop, cSkillShop);
				ImGui::SameLine();
				ImGui::Indent(180);
				asShopSelector(&skillTypeShop, (int*)(shared::base + 0x1773D00 + skillTypeShop * 0x20), "##SkillSelector", "Aerial Parry\000Defensive Offense\000Sky High\000Sweep Kick\000Thunder Strike\000Falling Lightning\000Quick Draw\000Lightning Strike\000Stormbringer\000Marches du ciel\000Lumiere du ciel\000Cercle de l'ange\000Turbuience\000Down Burst\000");
				cSkillShop = *(int*)(shared::base + 0x1773D00 + skillTypeShop * 0x20);
				ImGui::Unindent(180);
			}

			ImGui::Unindent(180);

			ImGui::End();
		} });
	windows.push_back({ [](bWindowStruct* window)
		{
			ImGui::Begin("CustWindow", NULL, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize);
			//ImGui::GetStyle().WindowRounding = 0.0f;

			ImGui::SetWindowPos(ImVec2(window->vecWindowPos.x, window->vecWindowPos.y), ImGuiCond_Always);

			ImGui::Text("CUSTOMIZATION");

			{
				static int playerType = 0;

				if (ImGui::Button("PLAYER TYPE", ImVec2(150, 20)))
					RedTrainer::setPlayerType(playerType);
				ImGui::SameLine();
				//ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(500, 20));
				ImGui::Combo("##PlayerTypeCombo", &playerType, "Raiden\000First Raiden\000Camera mode\000Sam\000BladeWolf\000Disabled\000");
			}


			{
				static int attackType = 0;

				if (ImGui::Button("ATTACK TYPE", ImVec2(150, 20)))
					RedTrainer::setAttackType(attackType);
				ImGui::SameLine();
				//ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(500, 20));
				ImGui::Combo("##AttackTypeCombo", &attackType, "Raiden\000Sam\000Boss Sam\000Armstrong\000");
			}


			{
				static int swordType = 0;

				if (ImGui::Button("PLAYER SWORD", ImVec2(150, 20)))
					RedTrainer::setPlayerSword(swordType);
				ImGui::SameLine();
				ImGui::Combo("##PlayerSwordCombo", &swordType, "HF Blade\000Stun Blade\000Armor Breaker\000Long Sword\000Wooden Sword\000Murasama\000Fox Blade\000HF Machete\000First Blade\000Sam's Murasama\000Chainsaw\000Invisible\000");
			}

			ImGui::SameLine();
			if (ImGui::Button(" ", ImVec2(150, 20)))
				bTestWindow = !bTestWindow;

			{
				static int bodyType = 0;

				if (ImGui::Button("PLAYER BODY", ImVec2(150, 20)))
					RedTrainer::setPlayerBody(bodyType);
				ImGui::SameLine();
				ImGui::Combo("##PlayerBodyCombo", &bodyType, "Default\000Blue Body\000Red Body\000Yellow Body\000Desperado\000Suit\000Mariachi\000Original Body\000MGS4\000Gray Fox\000White Armor\000Inferno Armor\000Commando Armor\000Raiden\000First Body\000Sam :)\000");
			}

			{
				static int hairType = 0;

				if (ImGui::Button("PLAYER HAIR", ImVec2(150, 20)))
					RedTrainer::setPlayerHair(hairType);
				ImGui::SameLine();
				ImGui::Combo("##PlayerHairCombo", &hairType, "Default\000Wig A\000Wig B\000Wig C\000");
			}

			if (ImGui::Button("INVISIBLE", ImVec2(150, 20)))
				RedTrainer::setInvisibility(RedTrainer::bInvisible);
			RedTrainer::setText(RedTrainer::bInvisible);

			if (ImGui::Button("SAM RIPPER", ImVec2(150, 20)))
				RedTrainer::samRipper(RedTrainer::bSamRipper);
			RedTrainer::setText(RedTrainer::bSamRipper);

			ImGui::End();
			} });
	windows.push_back({ [](bWindowStruct* window)
		{
			ImGui::Begin("MoveWindow", NULL, ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_AlwaysAutoResize);
					//ImGui::GetStyle().WindowRounding = 0.0f;

			ImGui::SetWindowPos({ window->vecWindowPos.x, window->vecWindowPos.y }, ImGuiCond_Always);

			ImGui::Text("MOVEMENT");

			{
				static float speedValue = 1.f;

				if (ImGui::Button("GAME SPEED", ImVec2(150, 20)))
					RedTrainer::setSpeed(speedValue);
				ImGui::SameLine();
				ImGui::InputScalar(" ", ImGuiDataType_Float, &speedValue);
			}

			{
				static int animationID = 0;

				if (ImGui::Button("STRONG ATTACK", ImVec2(150, 20)))
					RedTrainer::setAnimFromAnimMapRaiden(animationID);
				ImGui::SameLine();
				ImGui::InputScalar("AnimMap", ImGuiDataType_S32, &animationID);
			}

			if (ImGui::Button("FLY HACK", ImVec2(150, 20)))
				RedTrainer::bFly ^= true;
			RedTrainer::setText(RedTrainer::bFly);

			{
				static int animationIdSelectable = 0;

				if (ImGui::Button("ANIMATION", ImVec2(150, 20)))
					RedTrainer::setPlayerAnimation(animationIdSelectable, 0, 0, 0, 1);
				ImGui::SameLine();
				ImGui::Combo("##AnimationSelectionCombo", &animationIdSelectable, "Still1\000Still2\000Still3\000Walk1\000Walk2\000Landing1\000Landing2\000Jump1\000Jump2\000Flip1\000Flip2\000Flip3\000Flip4\000Flip5\000Flip6\000Flip7\000Flip8\000Flip9\000Flip10\000Flip11\000Flip12\000Flip13\000Flip14\000Flip15\000Flip16\000Flip17\000Flip18\000Flip19\000Flip20\000Sprint1\000Sprint2\000Sprint3\000Sprint4\000Sprint5\000Death1\000Death2\000Death3\000Death4\000Death5\000Death6\000Death7\000Death8\000Hit1\000Hit2\000Hit3\000Hit4\000Hit5\000Hit6\000Hit7\000Hit8\000Hit9\000Hit10\000Hit11\000Hit12\000Hit13\000Hit14\000Hit15\000Hit16\000Hit17\000Hit18\000Hand1\000Hand2\000Hand3\000Hand4\000Hand5\000Hand6\000Hand7\000Hand8\000Hand9\000Hand10\000Hand11\000Hand12\000Hand13\000Hand14\000Hand15\000Hand16\000Hand17\000Hand18\000Leg1\000Leg2\000Leg3\000Leg4\000Leg5\000Leg6\000Leg7\000Leg8\000Leg9\000Leg10\000Leg11\000Leg12\000Leg13\000Add weapon1\000Add weapon2\000Add weapon3\000Add weapon4\000Add weapon5\000Add weapon6\000Add weapon7\000Add weapon8\000Block1\000Block2\000Block3\000Block4\000Block5\000Block6\000Block7\000Block8\000Parry1\000Parry2\000Victory1\000Victory2\000Hacking1\000Hacking2\000Hacking3\000Codec1\000Codec2\000Red eyes\000Get sword\000Lost sword\000Joke\000Wasd\000Monsoon kill\000MG Ray\000Spying\000");
			}

			{
				static bool setWithoutSword = false;

				if (ImGui::Button("LOST SWORD", ImVec2(150, 20)))
					RedTrainer::setWithoutSword(setWithoutSword);
				//RedTrainer::setText(isHasSword);
			}

				ImGui::Text("ANIMATION");
				ImGui::SameLine();
				ImGui::Text("%d", RedTrainer::getCurrentAnimation()); //%x to hex

				ImGui::Text("ENEMY/BOSS ANIMATION");
				ImGui::SameLine();
				ImGui::Text("%d", RedTrainer::getFirstEnemyCurrentAnimation()); //%x to hex

				ImGui::End();
			}
		});
}