#include "../Include/UI.hpp"

#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/imgui_impl_win32.h"
#include "../../libs/imgui/imgui_impl_dx11.h"

#include "../Include/Objects/Object2D.hpp"
#include "../Include/App.hpp"

#pragma execution_character_set("utf-8")  // Для отображения на русском языке

UI::UI(App* app)
	:
	pApp(app)
{
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

void UI::Render()
{
	ShowMenu();

	SetPanelSizeAndPosition(0, 0.2f, 1.0f, 0.0f, 0.0f);
	ShowPanel();

	SetPanelSizeAndPosition(3, 0.025, 0.22, -0.015f, -0.05f);
	ShowViewPort();

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
				for (auto& obj : pApp->dList.dList)
				{
					ImGui::SliderInt((std::string("x ") + obj.first).c_str(), &obj.second.position.x, 0, 700);
					ImGui::SliderInt((std::string("y ") + obj.first).c_str(), &obj.second.position.y, 0, 700);
					ImGui::Separator();
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Test"))
			{
				if (ImGui::Button("Spawn"))
				{
					pApp->dList.Add(Object2D(0, 0, pApp->rManager["item2.bmp"]), pApp->camera.dpos);
				}

				ImGui::SliderInt("Grid Size", (int*)&pApp->dList.grid.padding, 1, 200);
				ImGui::SliderInt("Camera x", &pApp->camera.dpos.x, 0, 1300);
				ImGui::SliderInt("Camera y", &pApp->camera.dpos.y, 0, 700);

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
