#include "../Include/UI.hpp"

#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/imgui_impl_win32.h"
#include "../../libs/imgui/imgui_impl_dx11.h"

#include "../Include/Objects/Object2D.hpp"
#include "../Include/App.hpp"

#include <sstream>
#include <fstream>
#include <filesystem>

#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "../../libs/stb_image.h"
#include "../../libs/json.hpp"

#define N_OFFSET 4

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
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);

	InputLayerCounter  = GetLayersAmount(LayerType::Input);
	OutputLayerCounter = GetLayersAmount(LayerType::Output);
	HiddenLayerCounter = GetLayersAmount(LayerType::Hidden);

#ifndef N_DEBUG
	for (auto& i : pApp->rManager.textures)
	{
		pTextures.emplace_back(nullptr);
		LoadTextureFromFile(std::string("assets\\" + i.second.path).c_str(), &pTextures.back(), nullptr, nullptr);
	}
#endif
}

void UI::Update(float dt)
{	
	if (pApp->wnd.onClose)
	{
		if (isChanges)
		{
			SaveAll();
		}
		exit(0);
	}

	cMenu.ctrl_pressed  = pApp->wnd.kbd.KeyIsPressed(VK_CONTROL);  // Если нажат Ctrl
	cMenu.shift_pressed = pApp->wnd.kbd.KeyIsPressed(VK_SHIFT);    // Если нажат Shift
	 
	KeyProc('A', &cMenu.a_pressed, &isAddNeuron);         // Если нажата A - добавить нейрон
	KeyProc('D', &cMenu.d_pressed, &isDeleteNeuron);      // Если нажата D - удалить нейрон
	KeyProc('H', &cMenu.h_pressed, &isAddHiddenLayer);    // Если нажата H - добавить скрытый слой
	KeyProc('G', &cMenu.g_pressed, &isDeleteHiddenLayer); // Если нажата G - удалить скрытый слой
	KeyProc('S', &cMenu.s_pressed, &isSave);			  // Если нажата S - сохранить всё
 
	// Если курсор в рабочей области
	if (pApp->wnd.mouse.GetPosX() >= 0 &&
		pApp->wnd.mouse.GetPosY() >= pApp->gfx.menuH &&
		pApp->wnd.mouse.IsInWindow())
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

		SpawnThread(&UI::FindLayer);  // Ищем слой в новом потоке
	}

	// Если отпустили ЛКМ
	if (pApp->wnd.mouse.LeftIsReleased())
	{
		cMenu.flag_pressed = false;
		cMenu.counter = 0.0f;
		pApp->dList.selected = pLayer;
		dragLayer = nullptr;
		invalid_cell = false;
	}
	
	if (pApp->wnd.mouse.RightIsPressed() && pLayer != nullptr)
	{
		cMenu.pos.x = pApp->wnd.mouse.GetPosX();
		cMenu.pos.y = pApp->wnd.mouse.GetPosY();

		// Если есть объект в слое
		for (auto& obj : pLayer->dLayer)
		{
			if (obj.GetCell() == pApp->dList.hoveredCell)
			{
				ShowLayerInfo = true;
				break;
			}
		}
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
			bool valid_from = false;

			for (auto& layer : pApp->dList.dLayers)
			{
				for (auto& obj : layer.dLayer)
				{
					if (obj.GetCell() == cMenu.first_ptr)
					{
						if (dragLayer == nullptr)
						{
							dragLayer = &layer;
						}

						valid_from = true;
						break;
					}
				}
			}

			// Если исходная ячейка содержит в себе объект
			if (valid_from)
			{	
				auto pos1 = cMenu.first_ptr->GetIdx();
				auto pos2 = cMenu.second_ptr->GetIdx();

				// Дельта
				pos2d res;
				res.x = pos2.x - pos1.x;
				res.y = pos2.y - pos1.y;

				if (cMenu.shift_pressed)
				{
					// Если слои перекрываются
					for (auto& obj : dragLayer->dLayer)
					{
						Cell* c = pApp->dList.grid.GetCellByPos(obj.cell->x + res.x, obj.cell->y + res.y);

						for (auto& layer : pApp->dList.dLayers)
						{
							if (&layer != &(*dragLayer))
							{
								for (auto& o : layer.dLayer)
								{
									if (o.GetCell() == c)
									{
										invalid_cell = true;
										break;
									}
								}
							}

							if (invalid_cell)
							{
								break;
							}
						}
					}
					
					// Если нет перекрытия слоёв - двигаем слой
					if (!invalid_cell)
					{
						for (auto& obj : dragLayer->dLayer)
						{
							obj.SetCell(pApp->dList.grid.GetCellByPos(obj.cell->x + res.x, obj.cell->y + res.y));
						}
					}
				}
				else if (cMenu.ctrl_pressed)
				{
					for (auto& layer : pApp->dList.dLayers)
					{
						for (auto& obj : layer.dLayer)
						{
							obj.SetCell(pApp->dList.grid.GetCellByPos(obj.cell->x + res.x, obj.cell->y + res.y));
						}
					}
				}

				isChanges = true;
				cMenu.first_ptr = cMenu.second_ptr;
				cMenu.second_ptr = nullptr;
				cMenu.counter = 0.0f;

				// Если был подсвечен слой
				if (pApp->dList.selected != nullptr && (cMenu.shift_pressed || cMenu.ctrl_pressed))
				{
					// Убрать подсветку во время перемещения объекта
					pApp->dList.selected = nullptr;
				}
			}
		}
	}

	// Добавление/убавление объектов
	if (cMenu.ctrl_pressed)
	{
		if (cMenu.a_pressed)  // Если нажата кнопка A
		{
			AddNeuron();
			isAddNeuron = true;
			isChanges = true;
		}
		else if (cMenu.d_pressed) // Если нажата кнопка D
		{
			DeleteNeuron();
			isDeleteNeuron = true;
			isChanges = true;
		}
		else if (cMenu.h_pressed)  // Если нажата кнопка H
		{
			AddHiddenLayer();
			isAddHiddenLayer = true;
			isChanges = true;
		}
		else if (cMenu.g_pressed)  // Если нажата кнопка G
		{
			DeleteHiddenLayer();
			isDeleteHiddenLayer = true;
			isChanges = true;
		}
		else if (cMenu.s_pressed)  // Если нажата кнопка S
		{
			SpawnThread(&UI::SaveAll);
		}
	}
}

void UI::Render()
{
	SetPanelSizeAndPosition(0, 0.2f, 1.0f, 0.0f, 0.0f);
	ShowPanel();

	SetPanelSizeAndPosition(3, 0.025, 0.22, -0.015f, -0.05f);
	ShowViewPort();

	SetPanelSizeAndPosition(0, 0.8f, 0.05f, 0.2f, 0.0f);
	ShowTopPanel();

	ShowOutputs();

	// Popups
	/* ==== Окно подтверждения удаления слоя ==== */
	if (isDeleteLayer)
	{
		ImGui::OpenPopup("delete_layer");

		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
		if (ImGui::BeginPopup("delete_layer"))
		{			
			ImGui::Text("Удалить слой?");

			if (ImGui::Button("Да", { 100, 0 }))
			{
				auto deletedType = pLayer->type;

				pApp->dList.Delete(pLayer);
				for (size_t i = pApp->dList.GetIdByPtr(pLayer); i < pApp->dList.dLayers.size(); i++)
				{
					RenameLayer(&pApp->dList.dLayers[i], RenameState::Down);
					ShiftLayer(&pApp->dList.dLayers[i], -N_OFFSET - 1);
				}

				switch (deletedType)
				{
				case LayerType::Input:
					InputLayerCounter--;
					break;
				case LayerType::Hidden:
					HiddenLayerCounter--;
					break;
				case LayerType::Output:
					OutputLayerCounter--;
					break;
				default:
					break;
				}
				
				pLayer = nullptr;
				isDeleteLayer = false;
			}

			ImGui::SameLine();

			if (ImGui::Button("Нет", { 100, 0 }))
			{
				isDeleteLayer = false;
			}

			ImGui::EndPopup();
		}
		ImGui::PopStyleColor();
	}
	/**********************************************/

	/* ==== Окно подтверждения добавления слоя ==== */
	if (isAddLayer)
	{
		ImGui::OpenPopup("Добавление слоя");

		if (ImGui::BeginPopupModal("Добавление слоя", NULL, ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Выберите тип слоя:");

			static LayerType type = LayerType::All;
			static bool selected_type[3] = {};
			static const std::string names[3] = { "Входной слой", "Скрытый слой", "Выходной слой" };

			for (int i = 0; i < (int)LayerType::All; i++)
			{
				// (Если слой скрытого типа ИЛИ
				// Если слой не существует) И
				// Если слоёв больше 0
				if ((LayerType(i) == LayerType::Hidden || !CheckExLayer(LayerType(i))) && pApp->dList.dLayers.size() != 0)
				{
					if (ImGui::Checkbox(names[i].c_str(), &selected_type[i]))
					{
						memset(selected_type, 0, 3 * sizeof(bool));
						selected_type[i] = true;
						type = LayerType(i);
					}
				}
				else if (pApp->dList.dLayers.size() == 0)  // Если слоёв нет - можем добавить только входной слой
				{
					selected_type[0] = true;
					ImGui::Checkbox(names[0].c_str(), &selected_type[0]);
					type = LayerType::Input;

					break;
				}
			}

			// Добавить слой
			if (ImGui::Button("Добавить", ImVec2(120, 0)))
			{	
				std::string baseName = "Neuron_";
				std::ostringstream oss;

				// Если не выбран тип, то закрыть окно
				if (type == LayerType::All)
				{
					isAddLayer = false;
					ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
					return;
				}

				/* Настройки для перемещения слоёв */
				pos2d curPos;
				pos2d posSet;
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
					newLayer.Add(std::move(Object2D(pApp->rManager["res-1"], nullptr, "Neuron_0_0")));
					newLayer.dLayer[0].cell = pApp->dList.grid.GetCellByPos(0, 0);

					pApp->dList.Insert(std::move(newLayer), 0);

					if (pApp->dList.dLayers.size() > 1)
					{
						for (size_t i = pApp->dList.GetIdByPtr(&pApp->dList.dLayers[1]); i < pApp->dList.dLayers.size(); i++)
						{
							RenameLayer(&pApp->dList.dLayers[i], RenameState::Up);
						}
					}

					if (HiddenLayerCounter + OutputLayerCounter == 0)
					{
						break;
					}

					// Получаем текущий и предыдущий слои
					pCurLayer = &pApp->dList.dLayers[1];
					pNextLayer = &pApp->dList.dLayers[0];

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

					// Сдвигаем нейрон на N_OFFSET + 1 ячеек влево
					posSet = CellToSet->GetIdx();
					posSet.x -= (N_OFFSET + 1);

					CellToSet = pApp->dList.grid.GetCellByPos(posSet.x, posSet.y);
					pNextLayer->dLayer[0].SetCell(CellToSet);

					InputLayerCounter++;
					break;
				case LayerType::Hidden: // Если скрытый слой
					AddHiddenLayer();
					break;
				case LayerType::Output:
					newLayer.name = "Выходной слой";
					newLayer.Add(std::move(Object2D(pApp->rManager["res-3"], nullptr, std::string("Neuron_") + std::to_string(HiddenLayerCounter + 1) + std::string("_0"))));
					newLayer.dLayer[0].cell = pApp->dList.grid.GetCellByPos(0, 0);

					pApp->dList.Add(std::move(newLayer)); // Добавить новый слой

					// Получаем текущий и предыдущий слои
					pCurLayer = &pApp->dList.dLayers[pApp->dList.dLayers.size() - 2];
					pNextLayer = &pApp->dList.dLayers[pApp->dList.dLayers.size() - 1];

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

					// Сдвигаем нейрон на N_OFFSET + 1 ячеек вправо
					posSet = CellToSet->GetIdx();
					posSet.x += (N_OFFSET + 1);

					CellToSet = pApp->dList.grid.GetCellByPos(posSet.x, posSet.y);
					pNextLayer->dLayer[0].SetCell(CellToSet);
					
					OutputLayerCounter++;
					break;
				}
				
				selected_layers.resize(pApp->dList.dLayers.size());
				pLayer = nullptr;
				isAddLayer = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Закрыть", ImVec2(120, 0)))
			{
				isAddLayer = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
	/************************************************/

	/* ==== Окно информации о слое ==== */
	if (ShowLayerInfo)
	{
		if (pLayer == nullptr)
		{
			return;
		}

		ImGui::OpenPopup(pLayer->name.c_str());

		// Ставим окно рядом с курсором
		if (!posSet)
		{
			ImGui::SetNextWindowPos({ (float)cMenu.pos.x, (float)cMenu.pos.y });
			posSet = true;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.08f, 0.08f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.2f, 0.2f, 0.2f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.2f, 0.2f, 1.00f));

		if (ImGui::Begin(pLayer->name.c_str(), &ShowLayerInfo, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
		{
			std::ostringstream oss;
			oss << "Кол-во нейронов: " << pLayer->dLayer.size();

			ImGui::Text(oss.str().c_str());

			oss.str("");
			oss.clear();

			if (pLayer->type != LayerType::Input && HiddenLayerCounter != 0)
			{
				oss << "Кол-во входящих синапсов: " << pLayer->GetSize() * (pLayer - 1)->GetSize();
				ImGui::Text(oss.str().c_str());
			}

			oss.str("");
			oss.clear();

			if (pLayer->type != LayerType::Output && HiddenLayerCounter != 0)
			{
				oss << "Кол-во исходящих синапсов: " << pLayer->GetSize() * (pLayer + 1)->GetSize();
				ImGui::Text(oss.str().c_str());
			}	

			if (ImGui::Button("Удалить"))
			{
				isDeleteLayer = true;
				ShowLayerInfo = false;
			}

			if (pObj != nullptr)
			{
				ImGui::Separator();

				ImGui::Text("Нейрон:");

				oss.str("");
				oss.clear();

				oss << pObj->id;
			
				ImGui::Text(oss.str().c_str());
			}
						
			ImGui::End();
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}
	else
	{
		posSet = false;
	}
	/************************************/

	F_DEBUG(Debug());
	F_DEBUG(ImGui::ShowDemoWindow());
}

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

void UI::AddNeuron()
{
	if (pLayer != nullptr) 
	{
		isDeleteLayer = false;
		ShowLayerInfo = false;

		std::ostringstream res;
		std::ostringstream nId;

		res << "res-" << (int)pLayer->type + 1;
		nId << "Neuron_";

		switch (pLayer->type)
		{
		case LayerType::Input:
			nId << "0_";
			break;
		case LayerType::Hidden:
			nId << pApp->dList.GetIdByPtr(pLayer) << "_";
			break;
		case LayerType::Output:
			nId << pApp->dList.dLayers.size() - 1 << "_";
			break;
		default:
			throw std::exception("Error layer type");
			break;
		}

		nId << pLayer->dLayer.size();

		// Вставить нейрон
		pLayer->Insert(Object2D(pApp->rManager[res.str()], nullptr, nId.str()));
	}
}

void UI::DeleteNeuron()
{
	if (pLayer != nullptr)
	{
		ShowLayerInfo = false;

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
}

void UI::AddHiddenLayer()
{
	ShowLayerInfo = false;

	std::string baseName = "Neuron_";
	std::ostringstream oss;

	/* Настройки для перемещения слоёв */
	pos2d posSet;
	DrawLayer* pCurLayer = nullptr;
	DrawLayer* pNextLayer = nullptr;
	Cell* CellToSet = nullptr;
	/***********************************/

	DrawLayer newLayer(pApp->rManager, pApp->dList.grid);
	newLayer.type = LayerType::Hidden;
	newLayer.name = std::string("Скрытый слой ") + std::to_string(HiddenLayerCounter + InputLayerCounter);
	newLayer.Add(std::move(Object2D(pApp->rManager["res-2"], nullptr, std::string("Neuron_") + std::to_string(HiddenLayerCounter + InputLayerCounter) + std::string("_0"))));
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

		{
			auto neuronPosPrev = (pCurLayer - 1)->Get()[0].cell->x;
			auto neuronPosNext = pNextLayer->Get()[0].cell->x;

			auto dPos = neuronPosNext - neuronPosPrev;

			if (dPos <= 0)  // Если слои располагаются некорректно
			{
				// Сдвигаем последний слой вправо до расстояния между слоями 2*N_OFFSET
				ShiftLayer(pNextLayer, abs(dPos) + 2 * N_OFFSET + 2);
			}
			else
			{
				// Сдвигаем последний слой вправо до расстояния между слоями 2*N_OFFSET
				ShiftLayer(pNextLayer, 2 * N_OFFSET + 2 - dPos);
			}
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

		// Сдвигаем нейрон на N_OFFSET + 1 ячеек влево
		posSet = CellToSet->GetIdx();
		posSet.x -= (N_OFFSET + 1);

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

		// Сдвигаем нейрон на N_OFFSET + 1 ячеек вправо
		posSet = CellToSet->GetIdx();
		posSet.x += (N_OFFSET + 1);

		CellToSet = pApp->dList.grid.GetCellByPos(posSet.x, posSet.y);
		pNextLayer->dLayer[0].SetCell(CellToSet);
	}

	RenameLayer(&pApp->dList.dLayers.back(), RenameState::Up);

	selected_layers.resize(pApp->dList.dLayers.size());
	pLayer = &pApp->dList.dLayers.back() - 1;
	dragLayer = nullptr;
	pApp->dList.selected = nullptr;

	HiddenLayerCounter++;
}

void UI::DeleteHiddenLayer()
{
	if ((&pApp->dList.dLayers.back() - 1)->type == LayerType::Hidden)
	{
		ShowLayerInfo = false;

		pApp->dList.Delete(&pApp->dList.dLayers.back() - 1);

		RenameLayer(&pApp->dList.dLayers.back(), RenameState::Down);
		ShiftLayer(&pApp->dList.dLayers.back(), -N_OFFSET - 1);

		dragLayer = nullptr;
		pLayer = nullptr;
		pApp->dList.selected = nullptr;

		HiddenLayerCounter--;
	}
}

void UI::SpawnThread(void (UI::* ptr)())
{
	worker = std::async(std::launch::async, ptr, this);
}

void UI::FindLayer()
{
	for (auto& layer : pApp->dList.dLayers)
	{
		for (auto& obj : layer.dLayer)
		{
			if (obj.GetCell() == pApp->dList.grid.GetCellByHover(cMenu.pos.x, cMenu.pos.y))
			{
				if (pLayer != &layer)
				{
					ShowLayerInfo = false; // Убираем окно с информацией о слоё		

					pLayer = &layer;

					for (auto& s : selected_layers)
					{
						s = false;
					}
				}

				if (pObj != &obj)
				{
					pObj = &obj;
					break;
				}
			}
		}
	}
}

void UI::ShiftLayer(DrawLayer* l, int offset)
{
	pos2d curPos;
	for (auto& l : l->dLayer)
	{
		curPos = l.cell->GetIdx();
		curPos.x += offset;

		l.SetCell(pApp->dList.grid.GetCellByPos(curPos.x, curPos.y));
	}
}

void UI::RenameLayer(DrawLayer* l, RenameState state)
{
	assert(l->type != LayerType::Input);

	std::ostringstream oss;
	std::string baseName = "Neuron_";

	switch (l->type)
	{
	case LayerType::Hidden:
		oss << "Скрытый слой " << pApp->dList.GetIdByPtr(l);
		l->name = oss.str();

		oss.str("");
		oss.clear();

		for (size_t i = 0; i < l->dLayer.size(); i++)
		{
			oss << baseName << pApp->dList.GetIdByPtr(l) << "_" << i;
			l->dLayer[i].id = oss.str();

			oss.str("");
			oss.clear();
		}
		break;
	case LayerType::Output:
		for (size_t i = 0; i < l->dLayer.size(); i++)
		{
			oss << baseName << pApp->dList.GetIdByPtr(l) << "_" << i;
			l->dLayer[i].id = oss.str();

			oss.str("");
			oss.clear();
		}
	default:
		break;
	}
}

size_t UI::GetLayersAmount(LayerType type) const
{
	size_t counter = 0;

	// Считаем кол-во скрытых слоёв
	for (auto& l : pApp->dList.dLayers)
	{
		if (l.type == type)
		{
			counter++;
		}
	}

	return counter;
}

void UI::KeyProc(unsigned char key, bool* ctx_state, bool* query)
{
	if (pApp->wnd.kbd.KeyIsPressed(key))
	{
		*ctx_state = !(*query);
	}
	else
	{
		*ctx_state = false;
		*query     = false;
	}
}

void UI::SpawnInfoText(const std::string& str, size_t counter, size_t pass, size_t fail)
{
	ImGui::TextColored(ImVec4(counter == fail ? 0.8f : 0.0f, counter >= pass ? 0.8f : 0.0f, 0.0f, 1.0f), str.c_str());
}

void UI::TextCentered(const std::string& text)
{
	auto windowWidth = ImGui::GetWindowSize().x;
	auto windowHeight = ImGui::GetWindowSize().y;

	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
	auto textHeight = ImGui::CalcTextSize(text.c_str()).y;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::SetCursorPosY((windowHeight - textHeight) * 0.5f);
	ImGui::Text(text.c_str());
}

void UI::SaveAll()
{
	auto create_param_str = [](const std::string& key, const std::string& value)
	{
		std::ostringstream out;
		out << "\"" << key << "\"" << ":" << "\"" << value << "\"";

		return out.str();
	};

	std::string basePath = "data/Projects/Sample/";
	std::ostringstream objectName;
	std::ostringstream truePath;
	std::ostringstream prjData;

	prjData << "{";
	prjData << create_param_str("project", pApp->dList.projectName) << ",";

	for (size_t i = 0; i < pApp->dList.dLayers.size(); i++)
	{
		truePath << basePath << "layer" << i << ".json";
		prjData << "\"layer-" << i << "\":" << "\"" << truePath.str() << "\",";

		SetNewData(CreateJsonLayer(&pApp->dList.dLayers[i]), truePath.str());

		truePath.str("");
		truePath.clear();
	}

	std::string prjResult(std::move(prjData.str()));
	prjResult.back() = '}';
	
	SetNewData(prjResult, basePath + "prj.json");

	// Число файлов в папке с проектом
	auto dirIter = std::filesystem::directory_iterator(basePath);
	size_t fileCount = std::count_if(
		begin(dirIter),
		end(dirIter),
		[](auto& entry) { return entry.is_regular_file(); }
	);

	// Удаляем лишние файлы с инфо о слоях
	for (size_t i = pApp->dList.dLayers.size(); i < fileCount - 1; i++)
	{
		truePath << basePath << "layer" << i << ".json";

		std::remove(truePath.str().c_str());

		truePath.str("");
		truePath.clear();
	}
	
	isChanges = false;
}

std::string UI::CreateJsonLayer(DrawLayer* l)
{
	static size_t local_hiddenLayerCounter = 0;

	auto create_param_str = [](const std::string& key, const std::string& value)
	{
		std::ostringstream out;
		out << "\"" << key << "\"" << ":" << "\"" << value << "\"";
	
		return out.str();
	};

	auto create_param_int = [](const std::string& key, int value)
	{
		std::ostringstream out;
		out << "\"" << key << "\"" << ":" << value;

		return out.str();
	};

	std::ostringstream out;
	std::string type;

	out << "{";

	switch (l->type)
	{
	case LayerType::Input:
		type = "Входной слой";
		break;
	case LayerType::Hidden:
		local_hiddenLayerCounter++;
		type = "Скрытый слой " + std::to_string(local_hiddenLayerCounter);
		break;
	case LayerType::Output:
		type = "Выходной слой";
		local_hiddenLayerCounter = 0; // Ну такое
		break;
	default:
		break;
	}

	out << create_param_str("name", type)		  << ", ";
	out << create_param_int("type", int(l->type)) << ", ";

	out << "\"objects\" : [";
	out << "{";
	
	for (size_t i = 0; i < l->dLayer.size(); i++)
	{
		out << "\"object " << i << "\": [";
		out << "{";
		out << create_param_int("c-x",      l->dLayer[i].cell->x) << ",";
		out << create_param_int("c-y",      l->dLayer[i].cell->y) << ",";
		out << create_param_str("id",       l->dLayer[i].id)	  << ",";
		out << create_param_str("resource", pApp->rManager.GetNameBySource(l->dLayer[i].pTex->path));
		out << "}";
		out << "] ,";
	}

	std::string result(std::move(out.str()));
	result.back() = '}';
	result += "]}";
	
	return std::move(result);
}

void UI::SetNewData(const std::string& data, const std::string& path)
{
	std::ofstream ostr(path);
	ostr << data;

	ostr.close();
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

// Показать левую панель
void UI::ShowPanel()
{
	if (ImGui::Begin("MainBar", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		if (ImGui::BeginTabBar("Main bar"))
		{
			std::ostringstream prj;
			prj << "Проекты" << (isChanges ? "*" : "");

			if (ImGui::BeginTabItem(prj.str().c_str()))
			{
				//prj.str("");
				//prj.clear();

				//prj << pApp->dList.projectName.c_str() << (isChanges ? "" : " - Сохранено");
				if (ImGui::TreeNodeEx(pApp->dList.projectName.c_str(), ImGuiTreeNodeFlags_Framed))  // Имя проекта
				{
					// Не менять положение контекстного меню
					ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
					if (ImGui::BeginPopupContextItem())
					{
						ImGui::Text("Добавить слой?");

						if (ImGui::Button("OK"))
						{
							isAddLayer = true;							
						}

						ImGui::EndPopup();
					}
					ImGui::PopStyleColor();

					if (ImGui::TreeNode("Общая информация"))  // Состояние слоёв НС
					{
						std::ostringstream oss;

						oss << "Входных слоёв:  " << InputLayerCounter << "\n";
						SpawnInfoText(oss.str().c_str(), InputLayerCounter, 1, 0);

						oss.str("");
						oss.clear();

						oss << "Скрытых слоёв:  " << HiddenLayerCounter << "\n";
						SpawnInfoText(oss.str().c_str(), HiddenLayerCounter, 1, 0);

						oss.str("");
						oss.clear();

						oss << "Выходных слоёв: " << OutputLayerCounter << "\n";
						SpawnInfoText(oss.str().c_str(), OutputLayerCounter, 1, 0);

						oss.str("");
						oss.clear();

						ImGui::TreePop();
					}

					auto& layers = pApp->dList.dLayers;			// Указатель на текущий слой
					for (size_t i = 0; i < layers.size(); i++)  // Цикл по слоям 
					{
						bool curr = selected_layers[i];
						if (ImGui::Selectable(layers[i].name.c_str(), curr))  // Выбранные слои
						{
							ShowLayerInfo = false; // Убираем окно

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

						ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
						if (ImGui::BeginPopupContextItem())
						{
							if (!ImGui::GetIO().KeyCtrl)
							{
								for (auto& s : selected_layers)
								{
									s = false;
								}
							}

							pLayer = &layers[i];
							selected_layers[i] = true;
							pApp->dList.selected = pLayer;

							ImGui::Text("Удалить слой?");

							if (ImGui::Button("OK"))
							{
								isDeleteLayer = true;
							}

							ImGui::EndPopup();
						}
						ImGui::PopStyleColor();

						// Для выбранного слоя показываем содержимое
						if (pLayer == &layers[i])
						{
							selected_layers[i] = true;
							if (ImGui::TreeNode("Нейроны"))
							{
								for (size_t j = 0; j < layers[i].GetSize(); j++)
								{
									ImGui::BulletText(layers[i][j].id.c_str());
								}

								ImGui::TreePop();
							}

							if (pLayer->type == LayerType::Output)
							{
								break;
							}

							if (ImGui::TreeNode("Синапсы"))
							{
								std::ostringstream oss;

								size_t c = 0;
								for (size_t j = 0; j < pLayer->dLayer.size(); j++)
								{
									for (size_t k = 0; k < (pLayer+1)->dLayer.size(); k++)
									{
										oss << "Synapse_" << i << "_" << c;
										if (ImGui::TreeNode(oss.str().c_str()))
										{
											oss.str("");
											oss.clear();

											oss << "From: " << pLayer[0][j].id;
											ImGui::BulletText(oss.str().c_str());

											oss.str("");
											oss.clear();

											oss << "To: " << (pLayer+1)[0][k].id;
											ImGui::BulletText(oss.str().c_str());
											
											ImGui::TreePop();
										}

										oss.str("");
										oss.clear();

										c++;
									}
								}

								ImGui::TreePop();
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
		if (ImGui::VSliderFloat("", { 18, pApp->gfx.GetHeight() * 0.2f}, &appScale, 1.0f, 5.0f, "%.1f"))
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
				AddNeuron();
			}

			ImGui::SameLine();

			// Убрать нейрон / слой
			if (ImGui::Button("-", { 26, 0 }))
			{
				DeleteNeuron();
			}
	
			ImGui::PopStyleVar();
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

// Показать выходы нейронов
void UI::ShowOutputs()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
	std::string s;

	size_t layer_counter = 0;
	size_t neuron_counter = 0;
	std::ostringstream oss;

	for (auto& layer : pApp->dList.dLayers)
	{
		for (auto& obj : layer.dLayer)
		{
			s = std::string("wnd_") + obj.id;
			ImGui::SetNextWindowPos({ float(obj.cell->pos.x) + pApp->gfx.GetPanelWidth(), float(obj.cell->pos.y) });
			ImGui::SetNextWindowSize({ float(pApp->dList.grid.padding), float(pApp->dList.grid.padding) });

			if (ImGui::Begin(s.c_str(), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus))
			{	
				if (layer_counter == 0)
				{
					oss << pApp->ns.net.input_layer.get_neuron(neuron_counter)->get_output();
					TextCentered(oss.str().c_str());
				}
				else if (layer_counter == 2) // FIX
				{
					oss << pApp->ns.net.output_layer.get_neuron(neuron_counter)->get_output();
					TextCentered(oss.str().c_str());
				}
				else
				{
					for (auto& h : pApp->ns.net.hidden_layers)
					{
						oss << h.get_neuron(neuron_counter)->get_output();
						TextCentered(oss.str().c_str());
					}										
				}

				oss.str("");
				oss.clear();
			}

			ImGui::End();

			neuron_counter++;
		}
		neuron_counter = 0;
		layer_counter++;
	}
	ImGui::PopStyleColor();
}

#ifndef NDEBUG

// Simple helper function to load an image into a DX11 texture with common settings
bool UI::LoadTextureFromMemory(const void* data, size_t data_size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	pApp->gfx.pDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	pApp->gfx.pDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	stbi_image_free(image_data);

	return true;
}

// Open and read a file, then forward to LoadTextureFromMemory()
bool UI::LoadTextureFromFile(const char* file_name, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	FILE* f = fopen(file_name, "rb");
	if (f == NULL)
		return false;
	fseek(f, 0, SEEK_END);
	size_t file_size = (size_t)ftell(f);
	if (file_size == -1)
		return false;
	fseek(f, 0, SEEK_SET);
	void* file_data = IM_ALLOC(file_size);
	fread(file_data, 1, file_size, f);
	bool ret = LoadTextureFromMemory(file_data, file_size, out_srv, out_width, out_height);
	IM_FREE(file_data);
	return ret;
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

				if (pLayer != nullptr)
				{
					ImGui::Text((std::string("Layer name: ") + pLayer->name).c_str());
				}

				if (dragLayer != nullptr)
				{
					ImGui::Text((std::string("Layer on drag: ") + dragLayer->name).c_str());
				}

				ImGui::Checkbox("Camera move", &pApp->camera.isActive);
				ImGui::SliderInt("Camera x", &pApp->camera.dpos.x, -1000, 1000);
				ImGui::SliderInt("Camera y", &pApp->camera.dpos.y, -1000, 1000);

				if (ImGui::Button("Save all"))
				{
					SpawnThread(&UI::SaveAll);
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Gfx"))
			{	
				if (ImGui::Button("Test compress"))
				{
					// Тест уменьшения масштаба
					// ячейки в объектах теряют указатель
					for (auto& layer : pApp->dList.dLayers)
					{
						for (auto& obj : layer.dLayer)
						{
							if (obj.pTex->path == "input_n.bmp")
							{
								obj.pTex = pApp->rManager["res-4"];
							}
							else if (obj.pTex->path == "hidden_n.bmp")
							{
								obj.pTex = pApp->rManager["res-5"];
							}
							else if (obj.pTex->path == "output_n.bmp")
							{
								obj.pTex = pApp->rManager["res-6"];
							}
						}
					}

					pApp->dList.grid.SetPadding(25);
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Resource"))
			{
				std::ostringstream oss;
				size_t c = 0;

				for (auto& i : pApp->rManager.textures)
				{
					if (ImGui::TreeNode(i.first.c_str()))
					{
						oss << "Path: "   << i.second.path   << "\n";
						oss << "Width: "  << i.second.width  << "\n";
						oss << "Height: " << i.second.height << "\n";
						
						ImGui::Text(oss.str().c_str());

						oss.str("");
						oss.clear();

						ImGui::Image((ImTextureID)(intptr_t)pTextures[c], ImVec2(i.second.width, i.second.height));

						ImGui::TreePop();
					}

					c++;
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("NS"))
			{
				if (ImGui::Button("train"))
				{
					//worker = std::async(std::launch::async, ptr, this);
					auto thread = std::async(std::launch::async, &NetSystem::train, &pApp->ns, "sets\\train_xor");
					//pApp->ns.train("sets\\train_xor");
				}

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}

	ImGui::End();
}

#endif // NDEBUG
