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
	selected_layers.resize(pApp->dList.dLayers.size());

	ImGui::GetStyle().WindowBorderSize = 0.0f;
	ImGui::GetStyle().TabBorderSize = 1.0f;
	ImGui::GetStyle().TabBarBorderSize = 1.0f;
	ImGui::GetStyle().FrameRounding = 8.0f;
	ImGui::GetStyle().GrabRounding = 5.0f;

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
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.17f, 0.18f, 0.19f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.26f, 0.29f, 0.40f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.34f, 0.06f, 0.06f, 0.91f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.09f, 0.11f, 0.13f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.42f, 0.13f, 0.13f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.22f, 0.05f, 0.05f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
}

void UI::Update(float dt)
{	
	// Если курсор в рабочей области
	if (pApp->wnd.mouse.GetPosX() >= 0 &&
		pApp->wnd.mouse.GetPosY() >= pApp->gfx.menuH)
	{
		pApp->dList.CheckHover(pApp->wnd.mouse.GetPosX(), pApp->wnd.mouse.GetPosY());  // Подсветка ячеек
	}
	else
	{
		pApp->dList.hoveredCell = nullptr;
	}

	// Если нажали ЛКМ
	if (pApp->wnd.mouse.LeftIsPressed())
	{
		cMenu.pos.x = pApp->wnd.mouse.GetPosX();
		cMenu.pos.y = pApp->wnd.mouse.GetPosY();

		if (!cMenu.flag_pressed)
		{
			// Начисляем время удержания
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

	// Если отпустили ЛКМ
	if (pApp->wnd.mouse.LeftIsReleased())
	{
		cMenu.flag_pressed = false;
		cMenu.counter = 0.0f;
		pApp->dList.selected = pLayer;
	}
	
	// Если зажата ЛКМ
	if (cMenu.flag_pressed)
	{
		cMenu.pos.x = pApp->wnd.mouse.GetPosX();
		cMenu.pos.y = pApp->wnd.mouse.GetPosY();

		cMenu.second_ptr = pApp->dList.grid.GetCellByHover(cMenu.pos.x, cMenu.pos.y);
	}

	// Dragging objects
	if (cMenu.flag_pressed && cMenu.first_ptr != nullptr && cMenu.second_ptr != nullptr)
	{
		// Если ячейка, в которую идёт перетаскивание не равна исходной
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

			// Если исходная ячейка содержит в себе объект
			if (valid)
			{	
				auto pos1 = cMenu.first_ptr->GetIdx();
				auto pos2 = cMenu.second_ptr->GetIdx();

				// Дельта
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

				// Если был подсвечен слой
				if (pApp->dList.selected != nullptr)
				{
					// Убоать подсветку во время перемещения объекта
					pApp->dList.selected = nullptr;
				}
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

	// Popups
	/* ==== Окно подтверждения удаления слоя ==== */
	if (isDeleteLayer)
	{
		ImGui::OpenPopup("delete_layer");

		if (ImGui::BeginPopup("delete_layer"))
		{
			ImGui::Text("Это последний нейрон в слое.");
			ImGui::Text("Удалить слой?");

			if (ImGui::Button("Да", { 100.0f, 25.0f }))
			{
				isDeleteLayer = false;
				pApp->dList.Delete(pLayer);
				pLayer = nullptr;
			}

			ImGui::SameLine();

			if (ImGui::Button("Нет", { 100.0f, 25.0f }))
			{
				isDeleteLayer = false;
			}

			ImGui::EndPopup();
		}
	}
	/**********************************************/

	/* ==== Окно подтверждения добавления слоя ==== */
	if (isAddLayer)
	{
		ImGui::OpenPopup("Добавление слоя");

		if (ImGui::BeginPopupModal("Добавление слоя", NULL, ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Выберите тип слоя:");

			static bool selected_type[3] = {};
			std::string names[3] = { "Входной слой", "Скрытый слой", "Выходной слой" };
			for (int i = 0; i < (int)LayerType::All; i++)
			{
				if (ImGui::Checkbox(names[i].c_str(), &selected_type[i]))
				{
					memset(selected_type, 0, 3 * sizeof(bool));
					selected_type[i] = true;
				}
			}

			// Добавить слой
			if (ImGui::Button("OK", ImVec2(120, 0)))
			{	
				// Выбираем тип слоя
				LayerType type = LayerType::All;
				for (int i = 0; i < 3; i++)
				{
					if (selected_type[i])
					{
						type = LayerType(i);
						break;
					}
				}

				// Если не выбран тип, то закрыть окно
				if (type == LayerType::All)
				{
					isAddLayer = false;
					ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
					return;
				}

				// Если слой не скрытого типа И
				// Если слой уже существует
				if (type != LayerType::Hidden && CheckExLayer(type))
				{
					isAddLayer = false;
					ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
					return;
				}

				/* Настройки для перемещения слоёв */
				pos2d curPos;
				pos2d posSet;
				int hiddenLayersCounter = 0;
				DrawLayer* pCurLayer  = nullptr;
				DrawLayer* pNextLayer = nullptr;
				Cell*	   CellToSet  = nullptr;
				/***********************************/

				DrawLayer newLayer(pApp->rManager, pApp->dList.grid);
				newLayer.type = type;

				// Добавляем слой по типу
				switch (type)
				{
				case LayerType::Input:  // Если входной слой
					newLayer.name = "Входной слой";
					newLayer.Add(Object2D(pApp->rManager["res-1"]));
					newLayer.dLayer[0].cell = pApp->dList.grid.GetCellByPos(0, 0);

					pApp->dList.Add(std::move(newLayer)); // Добавить новый слой
					break;
				case LayerType::Hidden: // Если скрытый слой
					// Считаем кол-во скрытых слоёв
					for (auto& l : pApp->dList.dLayers)
					{
						if (l.type == type) 
						{
							hiddenLayersCounter++;
						}
					}
					newLayer.name = std::string("Скрытый слой ") + std::to_string(hiddenLayersCounter);
					newLayer.Add(Object2D(pApp->rManager["res-2"]));
					newLayer.dLayer[0].cell = pApp->dList.grid.GetCellByPos(0, 0);

					pApp->dList.Add(std::move(newLayer)); // Добавить новый слой

					// Получаем текущий и предыдущий слои
					pCurLayer = &pApp->dList.dLayers[pApp->dList.dLayers.size() - 2];
					pNextLayer = &pApp->dList.dLayers[pApp->dList.dLayers.size() - 1];

					if (pCurLayer->type == LayerType::Output)
					{
						// Поменять местами последний и предпоследний слои
						std::swap(pApp->dList.dLayers[pApp->dList.dLayers.size() - 2], 
								  pApp->dList.dLayers[pApp->dList.dLayers.size() - 1]);

						// Сдвигаем последний слой вправо на 5 ячеек
						for (auto& l : pNextLayer->dLayer)
						{
							curPos = l.cell->GetIdx();
							curPos.x += 5;

							l.SetCell(pApp->dList.grid.GetCellByPos(curPos.x, curPos.y));
						}

						// Если в последнем слое чётное кол-во нейронов
						if (pNextLayer->dLayer.size() % 2 == 0)
						{
							size_t idx = int(pNextLayer->dLayer.size() / 2) - 1;
							CellToSet = pApp->dList.grid.GetLowerCell(pNextLayer->dLayer[idx].cell);
						}
						else // Иначе
						{
							size_t idx = int(std::round(pNextLayer->dLayer.size() / 2));
							CellToSet = pNextLayer->dLayer[idx].cell;
						}

						// Сдвигаем нейрон на 5 ячеек влево
						posSet = CellToSet->GetIdx();
						posSet.x -= 5;

						CellToSet = pApp->dList.grid.GetCellByPos(posSet.x, posSet.y);
						pCurLayer->dLayer[0].SetCell(CellToSet);
					}
					else
					{
						// Если в последнем слое чётное кол-во нейронов
						if (pCurLayer->dLayer.size() % 2 == 0)
						{
							size_t idx = int(pCurLayer->dLayer.size() / 2) - 1;
							CellToSet = pApp->dList.grid.GetLowerCell(pCurLayer->dLayer[idx].cell);
						}
						else // Иначе
						{
							size_t idx = int(std::round(pCurLayer->dLayer.size() / 2));
							CellToSet = pCurLayer->dLayer[idx].cell;
						}

						// Сдвигаем нейрон на 5 ячеек вправо
						posSet = CellToSet->GetIdx();
						posSet.x += 5;

						CellToSet = pApp->dList.grid.GetCellByPos(posSet.x, posSet.y);
						pNextLayer->dLayer[0].SetCell(CellToSet);
					}

					selected_layers.resize(pApp->dList.dLayers.size());
					break;
				case LayerType::Output:
					break;
				}
				
				pLayer = nullptr;
				isAddLayer = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				isAddLayer = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
	/************************************************/

	F_DEBUG(Debug());
	F_DEBUG(ImGui::ShowDemoWindow());
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
				if (ImGui::TreeNode(pApp->dList.projectName.c_str()))  // Имя проекта
				{
					if (ImGui::BeginPopupContextItem())
					{
						ImGui::Text("Добавить слой?");

						if (ImGui::Button("OK"))
						{
							isAddLayer = true;							
						}

						ImGui::EndPopup();
					}

					auto& layers = pApp->dList.dLayers;  // Указатель на текущий слой
					for (size_t i = 0; i < layers.size(); i++)  // Цикл по слоям 
					{
						bool curr = selected_layers[i];
						if (ImGui::Selectable(layers[i].name.c_str(), curr))  // Выбранные слои
						{
							pLayer = &layers[i];  // Указатель на тек. слой
							pApp->dList.selected = pLayer;

							if (!ImGui::GetIO().KeyCtrl)
							{
								for (auto& s : selected_layers)
								{
									s = false;
								}
							}

							selected_layers[i] = true;
						}

						// Для выбранного слоя показываем содержимое
						if (selected_layers[i])
						{
							for (size_t j = 0; j < layers[i].GetSize(); j++)
							{
								ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20.0f);
								ImGui::Text(layers[i][j].id.c_str());
							}
						}
					}

					ImGui::TreePop();
				}
				else
				{
					pLayer = nullptr;
					pApp->dList.selected = nullptr;
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
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12);
	if (ImGui::Begin("slider viewport", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | 
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize))
	{
		ImGui::PushID("vp_slider");
		if (ImGui::VSliderFloat("", { 18, pApp->gfx.GetHeight() * 0.2f}, &appScale, 1.0f, 5.0f))
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
	ImGui::PopStyleVar();
}

// Показать верхнюю панель
void UI::ShowTopPanel()
{
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.7f);
	if (ImGui::Begin("Top panel", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar))
	{
		if (pLayer != nullptr)
		{
			// Имя выбранного слоя
			ImGui::Text(pLayer->name.c_str());
			ImGui::SameLine();

			// Добавить нейрон
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
			if (ImGui::Button("+", { 26, 0 }))
			{
				std::string res;
				std::ostringstream nId;

				switch (pLayer->type)
				{
				case LayerType::Input:
					res = "res-1";
					nId << "Neuron_0_";
					break;
				case LayerType::Hidden:
					res = "res-2";
					nId << "Neuron_1_";
					break;
				case LayerType::Output:
					res = "res-3";
					nId << "Neuron_2_";
					break;
				default:
					throw std::exception("Error layer type");
					break;
				}

				nId << pLayer->dLayer.size();

				// Вставить нейрон
				pLayer->Insert(Object2D(pApp->rManager[res], nullptr, nId.str()));
			}

			ImGui::SameLine();

			// Убрать нейрон / слой
			if (ImGui::Button("-", { 26, 0 }))
			{
				// Убрать нейрон
				if (pLayer->dLayer.size() - 1 != 0)
				{
					pLayer->Erase();
				}
				else // Удалить слой, если больше нет нейронов
				{
					isDeleteLayer = true;
				}
			}
	
			ImGui::PopStyleVar();
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

#ifndef NDEBUG

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

				if (pLayer != nullptr)
				{
					ImGui::Text((std::string("Layer name: ") + pLayer->name).c_str());
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

#endif // NDEBUG

bool UI::CheckExLayer(LayerType type)
{
	for (auto& l : pApp->dList.dLayers)
	{
		if (l.type == type)
		{
			return true;
		}
	}

	return false;
}
