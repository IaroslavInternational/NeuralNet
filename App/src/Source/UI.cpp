#include "../Include/UI.hpp"

#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/imgui_impl_win32.h"
#include "../../libs/imgui/imgui_impl_dx11.h"

UI::UI()
{
}

void UI::Render()
{
	ImGui::Begin("Test");
	ImGui::End();

}
