#include "../Include/UI.hpp"

#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/imgui_impl_win32.h"
#include "../../libs/imgui/imgui_impl_dx11.h"

#include "../Include/Objects/Object2D.hpp"
#include "../Include/App.hpp"

#include <sstream>

#pragma execution_character_set("utf-8")  // Для отображения на русском языке

UI::UI(App* app)
	:
	pApp(app)
{
	//F_DEBUG(inputs.resize(pApp->dList.dList.size()));
	ImGui::GetStyle().WindowBorderSize = 0.0f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.07f, 0.07f, 0.07f, 0.54f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TabSelected] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
	colors[ImGuiCol_Tab] = ImVec4(0.02f, 0.02f, 0.02f, 0.86f);
	colors[ImGuiCol_Button] = ImVec4(0.55f, 0.47f, 0.03f, 0.91f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.05f, 0.07f, 0.09f, 1.00f);
}

void UI::Update()
{
	// Открыть контекстное меню
	if (pApp->wnd.mouse.RightIsPressed())
	{
		cMenu.pos.x = pApp->wnd.mouse.GetPosX();
		cMenu.pos.y = pApp->wnd.mouse.GetPosY();
		cMenu.show = true;
	}
}

void UI::Render()
{
	ShowMenu();

	SetPanelSizeAndPosition(0, 0.2f, 1.0f, 0.0f, 0.0f);
	ShowPanel();

	SetPanelSizeAndPosition(3, 0.025, 0.22, -0.015f, -0.05f);
	ShowViewPort();

	F_DEBUG(Debug());

	ImGui::ShowDemoWindow();
}


void UI::SetPanelSizeAndPosition(int corner, float width, float height, float x_offset, float y_offset)
{
	ImGuiIO& io = ImGui::GetIO();

	float MenuHeight = pApp->gfx.menuH;
	ImVec2 DispSize = io.DisplaySize;

	float PanelW = round(DispSize.x * width);
	float PanelH = DispSize.y * height;

	ImVec2 PanelSize = ImVec2(
		PanelW,
		PanelH
	);

	ImVec2 PanelPos = ImVec2(
		(corner & 1) ? DispSize.x + round(DispSize.x * x_offset) : round(DispSize.x * x_offset),
		(corner & 2) ? DispSize.y + MenuHeight + DispSize.y * y_offset : MenuHeight + DispSize.y * y_offset
	);

	ImVec2 PanelPivot = ImVec2(
		(corner & 1) ? 1.0f : 0.0f,
		(corner & 2) ? 1.0f : 0.0f
	);

	ImGui::SetNextWindowPos(PanelPos, ImGuiCond_Always, PanelPivot);
	ImGui::SetNextWindowSize(PanelSize);
}

void UI::ShowMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Файл"))
		{
			if (ImGui::MenuItem("Сохранить"))
			{
			}

			ImGui::EndMenu();
		}

		ImGui::SetCursorPos({ pApp->wnd.ScreenWidth - pApp->gfx.menuH, 0 });
		if (ImGui::Button("X", { pApp->gfx.menuH, pApp->gfx.menuH }))
		{
			exit(0);
		}

		ImGui::EndMainMenuBar();
	};
}

void UI::ShowPanel()
{
	if (ImGui::Begin("MainBar", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		if (ImGui::BeginTabBar("Main bar"))
		{
			if (ImGui::BeginTabItem("Проекты"))
			{
				ImGui::EndTabItem();
			}
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

void UI::ShowViewPort()
{
	if (ImGui::Begin("slider viewport", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | 
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize))
	{
		ImGui::PushID("vp_slider");
		if (ImGui::VSliderFloat("", { 18, 160 }, &appScale, 1.0f, 10.0f))
		{
			// set viewport dimensions
			D3D11_VIEWPORT vp;
			vp.Width = float(pApp->wnd.ScreenWidth) * appScale;
			vp.Height = float(pApp->wnd.ScreenHeight) * appScale;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = float(pApp->wnd.ScreenWidth) * pApp->gfx.pK;
			vp.TopLeftY = pApp->gfx.menuH;

			pApp->gfx.pImmediateContext->RSSetViewports(1, &vp);
		}
		ImGui::PopID();
	}
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_NoSharedDelay))
		ImGui::SetTooltip("Размер поля", ImGui::GetStyle().HoverDelayShort);
	ImGui::End();
}

void UI::Debug()
{
	if (ImGui::Begin("Debug", NULL, ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::BeginTabBar("Debug bar"))
		{
			if (ImGui::BeginTabItem("Item"))
			{
				std::ostringstream oss;
				oss << "Mouse pos: x = " << pApp->wnd.mouse.GetPosX() << ", y = " << pApp->wnd.mouse.GetPosY();

				ImGui::Text(oss.str().c_str());

				for (auto& l : pApp->dList.dLayers)
				{
					if (ImGui::TreeNode(l.name.c_str()))
					{
						for (size_t i = 0; i < l.GetSize(); i++)
						{
							ImGui::Text(l[i].id.c_str());
						}				

						if (ImGui::Button("Spawn"))
						{
							l.Insert(Object2D(pApp->rManager["item2.bmp"]));
						}

						ImGui::TreePop();
					}
				}

				ImGui::SliderInt("Camera x", &pApp->camera.dpos.x, -1000, 1000);
				ImGui::SliderInt("Camera y", &pApp->camera.dpos.y, -1000, 1000);

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	ImGui::End();

	if (cMenu.show)
	{
		ImGui::SetNextWindowPos(ImVec2(cMenu.pos.x + 75, cMenu.pos.y));
		if (ImGui::Begin("Меню", &cMenu.show))
		{
			auto cell = pApp->dList.grid.GetCellByHover(cMenu.pos.x, cMenu.pos.y);
			cell->Draw(pApp->gfx);

			std::ostringstream oss;
			oss << "Ячейка (" << cell->x << ", " << cell->y << ")";
			ImGui::Text(oss.str().c_str());

			/*if (ImGui::Button("Добавить объект"))
			{
				pApp->dList.Add(Object2D(pApp->rManager["neuron.bmp"], cell));
				F_DEBUG(inputs.resize(pApp->dList.dList.size()));
				cMenu.show = false;
			}*/
		}

		ImGui::End();
	}
}
