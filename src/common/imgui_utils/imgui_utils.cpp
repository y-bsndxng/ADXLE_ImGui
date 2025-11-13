#include <imgui_utils.h>

bool imgui_comboui(const std::string* caption, int32_t* current_item_index, const std::vector<std::string>* items)
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

inline void imgui_textrightalign(const char* text)
{
	float textWidth = ImGui::CalcTextSize(text).x;
	float availableWidth = ImGui::GetContentRegionAvail().x;
	if (availableWidth > textWidth)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availableWidth - textWidth);
	}
	ImGui::Text("%s", text);
}

void imgui_open_dialog(const char* dialog_name, char* select_file_name)
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