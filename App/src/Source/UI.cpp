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
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
}

void UI::Update(float dt)
{	
	cMenu.ctrl_pressed = pApp->wnd.kbd.KeyIsPressed(VK_CONTROL);  // Если нажат Ctrl
	cMenu.shift_pressed = pApp->wnd.kbd.KeyIsPressed(VK_SHIFT);   // Если нажат Shift

	// Если нажата A
	if (pApp->wnd.kbd.KeyIsPressed('A'))
	{
		cMenu.a_pressed = !isAddNeuron;
	}
	else
	{
		cMenu.a_pressed = false;
		isAddNeuron = false;
	}
    
	// Если нажата D
	if (pApp->wnd.kbd.KeyIsPressed('D'))  
	{
		cMenu.d_pressed = !isDeleteNeuron;
	}
	else
	{
		cMenu.d_pressed = false;
		isDeleteNeuron = false;
	}

	// Если нажата H
	if (pApp->wnd.kbd.KeyIsPressed('H'))
	{
		cMenu.h_pressed = !isAddHiddenLayer;
	}
	else
	{
		cMenu.h_pressed = false;
		isAddHiddenLayer = false;
	}

	// Если нажата G
	if (pApp->wnd.kbd.KeyIsPressed('G'))
	{
		cMenu.g_pressed = !isDeleteHiddenLayer;
	}
	else
	{
		cMenu.g_pressed = false;
		isDeleteHiddenLayer = false;
	}

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
		}
		else if (cMenu.d_pressed) // Если нажата кнопка D
		{
			DeleteNeuron();
			isDeleteNeuron = true;
		}
		else if (cMenu.h_pressed)  // Если нажата кнопка H
		{
			AddHiddenLayer();
			isAddHiddenLayer = true;
		}
		else if (cMenu.g_pressed)  // Если нажата кнопка G
		{
			pApp->dList.Delete(&pApp->dList.dLayers.back() - 1);
			
			RenameLayer(&pApp->dList.dLayers.back(), RenameState::Down);
			ShiftLayer(&pApp->dList.dLayers.back(), -2);

			dragLayer = nullptr;
			pLayer = nullptr;
			pApp->dList.selected = nullptr;
			isDeleteHiddenLayer = true;
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
				if (pLayer->type == LayerType::Output)
				{
					pApp->dList.Delete(pLayer);
				}
				else if (pLayer->type == LayerType::Hidden)
				{
					pApp->dList.Delete(pLayer);
					for (size_t i = pApp->dList.GetIdByPtr(pLayer); i < pApp->dList.dLayers.size(); i++)
					{
						RenameLayer(&pApp->dList.dLayers[i], RenameState::Down);
					}
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

			LayerType type = LayerType::All;
			static bool selected_type[3] = {};
			std::string names[3] = { "Входной слой", "Скрытый слой", "Выходной слой" };

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
				int hiddenLayersCounter = 0;
				DrawLayer* pCurLayer  = nullptr;
				DrawLayer* pNextLayer = nullptr;
				Cell*	   CellToSet  = nullptr;
				/***********************************/

				// Считаем кол-во скрытых слоёв
				for (auto& l : pApp->dList.dLayers)
				{
					if (l.type == LayerType::Hidden)
					{
						hiddenLayersCounter++;
					}
				}

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
					break;
				case LayerType::Hidden: // Если скрытый слой
					AddHiddenLayer();
					break;
				case LayerType::Output:
					newLayer.name = "Выходной слой";
					newLayer.Add(std::move(Object2D(pApp->rManager["res-3"], nullptr, std::string("Neuron_") + std::to_string(hiddenLayersCounter + 1) + std::string("_0"))));
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

					// Сдвигаем нейрон на 5 ячеек вправо
					posSet = CellToSet->GetIdx();
					posSet.x += 2;

					CellToSet = pApp->dList.grid.GetCellByPos(posSet.x, posSet.y);
					pNextLayer->dLayer[0].SetCell(CellToSet);

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

	/* ==== Окно информации о слоё ==== */
	if (ShowLayerInfo)
	{
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

			if (pLayer->type != LayerType::Input)
			{
				oss << "Кол-во входящих синапсов: " << pLayer->GetSize() * (pLayer - 1)->GetSize();
				ImGui::Text(oss.str().c_str());
			}

			oss.str("");
			oss.clear();

			if (pLayer->type != LayerType::Output)
			{
				oss << "Кол-во исходящих синапсов: " << pLayer->GetSize() * (pLayer + 1)->GetSize();
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
	std::string baseName = "Neuron_";
	std::ostringstream oss;

	/* Настройки для перемещения слоёв */
	pos2d curPos;
	pos2d posSet;
	int hiddenLayersCounter = 0;
	DrawLayer* pCurLayer = nullptr;
	DrawLayer* pNextLayer = nullptr;
	Cell* CellToSet = nullptr;
	/***********************************/

	// Считаем кол-во скрытых слоёв
	for (auto& l : pApp->dList.dLayers)
	{
		if (l.type == LayerType::Hidden)
		{
			hiddenLayersCounter++;
		}
	}

	DrawLayer newLayer(pApp->rManager, pApp->dList.grid);
	newLayer.type = LayerType::Hidden;
	newLayer.name = std::string("Скрытый слой ") + std::to_string(hiddenLayersCounter + 1);
	newLayer.Add(std::move(Object2D(pApp->rManager["res-2"], nullptr, std::string("Neuron_") + std::to_string(hiddenLayersCounter + 1) + std::string("_0"))));
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

		// Сдвигаем последний слой вправо на 2 ячейки
		ShiftLayer(pNextLayer, 2);

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
		posSet.x -= 4;

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
		posSet.x += 2;

		CellToSet = pApp->dList.grid.GetCellByPos(posSet.x, posSet.y);
		pNextLayer->dLayer[0].SetCell(CellToSet);
	}

	RenameLayer(&pApp->dList.dLayers.back(), RenameState::Up);

	selected_layers.resize(pApp->dList.dLayers.size());
	pLayer = nullptr;
	dragLayer = nullptr;
	pApp->dList.selected = nullptr;
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
		throw ("Smth go wrong");
		break;
	}
}

size_t UI::GetHiddenLayersAmount() const
{
	size_t hiddenLayersCounter = 0;

	// Считаем кол-во скрытых слоёв
	for (auto& l : pApp->dList.dLayers)
	{
		if (l.type == LayerType::Hidden)
		{
			hiddenLayersCounter++;
		}
	}

	return hiddenLayersCounter;
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
			if (ImGui::BeginTabItem("Проекты"))
			{
				if (ImGui::TreeNode(pApp->dList.projectName.c_str()))  // Имя проекта
				{
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

				if (dragLayer != nullptr)
				{
					ImGui::Text((std::string("Layer on drag: ") + dragLayer->name).c_str());
				}

				ImGui::Checkbox("Camera move", &pApp->camera.isActive);
				ImGui::SliderInt("Camera x", &pApp->camera.dpos.x, -1000, 1000);
				ImGui::SliderInt("Camera y", &pApp->camera.dpos.y, -1000, 1000);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Gfx"))
			{	
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	ImGui::End();
}

#endif // NDEBUG

