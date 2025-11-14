#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <imgui.h>
#include <ImGuiFileDialog.h>

namespace ImGuiUtils {
	bool Comboui(const std::string* caption, int32_t* current_item_index, const std::vector<std::string>* items);
	void Rightalign(const char* text);
	void OpenDialog(const char* dialog_name, char* select_file_name);
}
