#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <imgui.h>
#include <ImGuiFileDialog.h>

namespace ImGuiUtils {
	inline bool Comboui(const std::string* caption, int32_t* current_item_index, const std::vector<std::string>* items);
	inline void Rightalign(const char* text);
	inline void OpenDialog(const char* dialog_name, char* select_file_name);
	inline ImVec2 AddOffsetX(const ImVec2& v, float offset);
	inline ImVec2 AddOffsetY(const ImVec2& v, float offset);
	inline ImVec2 AddOffset(const ImVec2& v, float offset);
}

inline bool ImGuiUtils::Comboui(const std::string* caption, int32_t* current_item_index, const std::vector<std::string>* items)
{
	bool changed = false;

	if (ImGui::BeginCombo(caption->c_str(), items->at(*current_item_index).c_str())) {
		for (int n = 0; n < items->size(); n++) {
			bool is_selected = (items->at(*current_item_index).c_str()) == items->at(n).c_str();
			if (ImGui::Selectable(items->at(n).c_str(), is_selected)) {
				*current_item_index = n;
				changed = true;
			}
			if (is_selected) {
				// Set the initial focus when opening the combo (scrolling + for
				// keyboard navigation support in the upcoming navigation branch)
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	return changed;
}

inline void ImGuiUtils::Rightalign(const char* text)
{
	float text_width = ImGui::CalcTextSize(text).x;
	float available_width = ImGui::GetContentRegionAvail().x;
	if (available_width > text_width)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + available_width - text_width);
	}
	ImGui::Text("%s", text);
}

inline void ImGuiUtils::OpenDialog(const char* dialog_name, char* select_file_name)
{
	if (ImGuiFileDialog::Instance()->Display(dialog_name)) {
		if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
			std::string file_name = ImGuiFileDialog::Instance()->GetFilePathName();
			/* ファイルが選択されたらファイル名を上書き */
			std::copy(file_name.c_str(), file_name.c_str() + file_name.size() + 1, select_file_name);
		}
		/* キャンセル時は何もせず終了 */
		ImGuiFileDialog::Instance()->Close();
	}
}

inline ImVec2 ImGuiUtils::AddOffsetX(const ImVec2& v, float offset)
{
	return ImVec2{ v.x + offset, v.y };
}

inline ImVec2 ImGuiUtils::AddOffsetY(const ImVec2& v, float offset)
{
	return ImVec2{ v.x, v.y + offset };
}

inline ImVec2 ImGuiUtils::AddOffset(const ImVec2& v, float offset)
{
	return ImVec2{ ImGuiUtils::AddOffsetX(v, offset).x, ImGuiUtils::AddOffsetY(v, offset).y };
}
