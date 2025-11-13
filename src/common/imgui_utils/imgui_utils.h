#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <imgui.h>
#include <ImGuiFileDialog.h>

inline bool imgui_comboui(const std::string* caption, int32_t* current_item_index, const std::vector<std::string>* items);
inline void imgui_textrightalign(const char* text);
void imgui_open_dialog(const char* dialog_name, char* select_file_name);