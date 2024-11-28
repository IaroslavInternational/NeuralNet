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
	memset(buffer, '\0', 6*sizeof(char));

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

void UI::Update(float dt)
{
	if (pApp->wnd.mouse.LeftIsPressed())
	{
		cMenu.pos.x = pApp->wnd.mouse.GetPosX();
		cMenu.pos.y = pApp->wnd.mouse.GetPosY();

		if (!cMenu.flag_pressed)
		{
			if (cMenu.counter > 0.065f)
			{
				cMenu.flag_pressed = true;
				cMenu.first_ptr = pApp->dList.grid.GetCellByHover(cMenu.pos.x, cMenu.pos.y);
			}
			else
			{
				cMenu.counter += dt;
			}
		}
	}

	if (pApp->wnd.mouse.LeftIsReleased())
	{
		cMenu.flag_pressed = false;
	}
	
	if (cMenu.flag_pressed)
	{
		cMenu.pos.x = pApp->wnd.mouse.GetPosX();
		cMenu.pos.y = pApp->wnd.mouse.GetPosY();

		cMenu.second_ptr = pApp->dList.grid.GetCellByHover(cMenu.pos.x, cMenu.pos.y);
	}

	// Dragging objects
	if (cMenu.flag_pressed && cMenu.first_ptr != nullptr && cMenu.second_ptr != nullptr)
	{
		if (cMenu.first_ptr != cMenu.second_ptr)
		{
			bool valid = false;
			for (auto& layer : pApp->dList.dLayers)
			{
				for (auto& obj : layer.dLayer)
				{
					if (obj.GetCell() == cMenu.first_ptr)
					{
						valid = true;
						break;
					}
				}
			}

			if (valid)
			{
				auto pos1 = cMenu.first_ptr->GetIdx();
				auto pos2 = cMenu.second_ptr->GetIdx();

				pos2d res;
				res.x = pos2.x - pos1.x;
				res.y = pos2.y - pos1.y;

				for (auto& layer : pApp->dList.dLayers)
				{
					for (auto& obj : layer.dLayer)
					{
						obj.SetCell(pApp->dList.grid.GetCellByPos(obj.cell->x + res.x, obj.cell->y + res.y));
					}
				}

				cMenu.first_ptr = cMenu.second_ptr;
				cMenu.second_ptr = nullptr;
				cMenu.counter = 0.0f;
			}
		}	
	}
}

void UI::Render()
{
	ShowMenu();

	SetPanelSizeAndPosition(0, 0.2f, 1.0f, 0.0f, 0.0f);
	ShowPanel();

	SetPanelSizeAndPosition(3, 0.025, 0.22, -0.015f, -0.05f);
	ShowViewPort();

	SetPanelSizeAndPosition(0, 0.8f, 0.05f, 0.2f, 0.0f);
	ShowTopPanel();

	F_DEBUG(Debug());

	ImGui::ShowDemoWindow();
}

// Windows

// Установить размер окна
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

// Показать меню
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

// Показать левую панель
void UI::ShowPanel()
{
	if (ImGui::Begin("MainBar", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		if (ImGui::BeginTabBar("Main bar"))
		{
			if (ImGui::BeginTabItem("Проекты"))
			{
				ImGui::Text(pApp->dList.projectName.c_str());

				auto& layers = pApp->dList.dLayers;
				for (size_t i = 0; i < layers.size(); i++)
				{
					if (ImGui::TreeNode(layers[i].name.c_str()))
					{
						ImGui::InputText("Resource", buffer, 6);

						if (ImGui::Button("Spawn"))
						{
							std::ostringstream nId;
							nId << "N_" << i << "_" << layers[i].GetSize();

							layers[i].Insert(Object2D(pApp->rManager[std::string(buffer)], nullptr, nId.str()));
						}

						for (size_t j = 0; j < layers[i].GetSize(); j++)
						{
							ImGui::Text(layers[i][j].id.c_str());
						}

						ImGui::TreePop();
					}
				}

				ImGui::EndTabItem();
			}
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

// Показать меню ViewPort
void UI::ShowViewPort()
{
	if (ImGui::Begin("slider viewport", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | 
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize))
	{
		ImGui::PushID("vp_slider");
		if (ImGui::VSliderFloat("", { 18, 160 }, &appScale, 1.0f, 5.0f))
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

// Показать верхнюю панель
void UI::ShowTopPanel()
{
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.7f);
	if (ImGui::Begin("Top panel", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
		if (ImGui::Button("+", {26.0f, 26.0f}))
		{

		}

		ImGui::SameLine();

		if (ImGui::Button("-", { 26.0f, 26.0f }))
		{

		}
		ImGui::PopStyleVar();
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

void UI::Debug()
{
	if (ImGui::Begin("Debug", NULL, ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::BeginTabBar("Debug bar"))
		{
			if (ImGui::BeginTabItem("Device"))
			{
				std::ostringstream oss, oss_cell;
				oss << "Mouse pos: x = " << pApp->wnd.mouse.GetPosX() << ", y = " << pApp->wnd.mouse.GetPosY();
				oss << "\n" << "Mouse Holded: " << cMenu.counter;
				ImGui::Text(oss.str().c_str());

				if (pApp->dList.hoveredCell != nullptr)
				{
					oss_cell << "Cell: (" << pApp->dList.hoveredCell->x << ", " << pApp->dList.hoveredCell->y << ")";
					ImGui::Text(oss_cell.str().c_str());
				}

				ImGui::Checkbox("Camera move", &pApp->camera.isActive);
				ImGui::SliderInt("Camera x", &pApp->camera.dpos.x, -1000, 1000);
				ImGui::SliderInt("Camera y", &pApp->camera.dpos.y, -1000, 1000);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Gfx"))
			{	
				if (ImGui::Checkbox("Msaa x1", &pApp->dList.msaa[0]))
				{
					pApp->dList.msaa[1] = false;
					pApp->dList.msaa[2] = false;
				}
				
				if (ImGui::Checkbox("Msaa x2", &pApp->dList.msaa[1]))
				{
					pApp->dList.msaa[0] = true;
				}

				if (ImGui::Checkbox("Msaa x3", &pApp->dList.msaa[2]))
				{
					pApp->dList.msaa[0] = true;
					pApp->dList.msaa[1] = true;
				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	ImGui::End();

	/*if (cMenu.show)
	{
		ImGui::SetNextWindowPos(ImVec2(cMenu.pos.x + 75, cMenu.pos.y));
		if (ImGui::Begin("Меню", &cMenu.show))
		{
			auto cell = pApp->dList.grid.GetCellByHover(cMenu.pos.x, cMenu.pos.y);
			cell->Draw(pApp->gfx);

			std::ostringstream oss;
			oss << "Ячейка (" << cell->x << ", " << cell->y << ")";
			ImGui::Text(oss.str().c_str());

			if (ImGui::Button("Добавить объект"))
			{
				pApp->dList.Add(Object2D(pApp->rManager["neuron.bmp"], cell));
				F_DEBUG(inputs.resize(pApp->dList.dList.size()));
				cMenu.show = false;
			}
		}

		ImGui::End();
	}*/
}
